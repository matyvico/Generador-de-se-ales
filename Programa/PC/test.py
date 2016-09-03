 ###########################################  
 # PyUSB and PIC18F2550 demo  (first revision)    #  
 # Julius 'yus' Constante    Nov. 21, 2009         #  
 ###########################################  
import sys, usb  
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  
   
class UsbPic:  
    def __init__(self, vendor_id, product_id):  
        busses = usb.busses() # enumerate busses  
        self.handle = None  
        for bus in busses:  
            devices = bus.devices  
        for dev in devices:  
            if dev.idVendor==vendor_id and dev.idProduct==product_id: # device matches  
                self.dev = dev  
                self.conf = self.dev.configurations[0]  
                self.intf = self.conf.interfaces[0][0]  
                self.endpoints = []  
            for endpoint in self.intf.endpoints:  
                self.endpoints.append(endpoint)  
            return  
       
    def open(self):  
        if self.handle:  
            self.handle = None  
        try:  
            self.handle = self.dev.open()  
            self.handle.detachKernelDriver(0)  
            self.handle.detachKernelDriver(1)  
            self.handle.setConfiguration(self.conf)  
            self.handle.claimInterface(self.intf)  
            self.handle.setAltInterface(self.intf)  
            return True  
        except:  
            return False  
   
    def write(self, ep, buff, timeout = 100):  
        try:  
            return self.handle.interruptWrite(ep, buff, timeout) #return bytes written  
        except:  
            return 0  
    def read(self, ep, size, timeout = 100):  
        try:  
            return self.handle.interruptRead(ep, size, timeout) # return data read  
        except:  
            return []  
    def getDeviceName(self):  
        return self.handle.getString(2, 40)  
       
class SimpleForm(QDialog):  
    def __init__(self, parent = None):  
        super(SimpleForm, self).__init__(parent)  
        self.setWindowTitle('PIC18F and PyUSB')  
        self.setMinimumSize(270, 180)  
        # create widgets/controls  
        self.connect_btn = QPushButton('Connect')  
        self.toggle1_btn = QPushButton('Toggle LED1')  
        self.toggle2_btn = QPushButton('Toggle LED2')  
        self.pot_position = QDial()  
        self.pot_label = QLabel('Potentiometer Position')  
        self.status_label = QLabel('   press "Connect" button')  
        self.update_timer = QTimer()  
        # form layout and widgets location  
        layout = QGridLayout()  
        layout.addWidget(self.connect_btn, 9, 0, 2, 1)  
        layout.addWidget(self.pot_position, 0, 0, 8, 7)  
        layout.addWidget(self.pot_label, 8, 0, 1, 7)  
        layout.addWidget(self.toggle1_btn, 4, 7)  
        layout.addWidget(self.toggle2_btn, 5, 7)  
        layout.addWidget(self.status_label, 9, 1, 2, 7)  
        self.setLayout(layout)  
        # widgets initial condition  
        self.toggle1_btn.setEnabled(False)  
        self.toggle2_btn.setEnabled(False)  
        self.pot_position.setEnabled(False) #read only  
        self.pot_position.setNotchesVisible(True)  
        self.pot_position.setRange(0, 255) # for PIC 8-bit ADC value  
        self.pot_label.setAlignment(Qt.AlignHCenter)  
        # signals  
        self.connect(self.connect_btn, SIGNAL("clicked()"), self.DeviceConnect)  
        self.connect(self.toggle1_btn, SIGNAL("clicked()"), self.toggleLED1)  
        self.connect(self.toggle2_btn, SIGNAL("clicked()"), self.toggleLED2)  
        self.connect(self.update_timer, SIGNAL("timeout()"), self.updatePotPosition)  

    def DeviceConnect(self):  
        self.device = UsbPic(0x04d8, 0x000b) # Microchip Vendor ID and Product ID  
        if self.device.open():  
            self.toggle1_btn.setEnabled(True)  
            self.toggle2_btn.setEnabled(True)  
            self.update_timer.start(200) # update every 200ms  
            self.status_label.setText('Connected to:\n %s' %self.device.getDeviceName())  
        else:  
            self.toggle1_btn.setEnabled(False)  
            self.toggle2_btn.setEnabled(False)  
            self.update_timer.stop()  
            self.status_label.setText('Warning:\n No Device Found!')  
        
    def toggleLED1(self):  
        self.device.write(1, [0x80], 1000)  
       
    def toggleLED2(self):  
        self.device.write(1, [0x82], 1000)  
     
    def updatePotPosition(self):  
        self.device.write(1, [0x81])  
        byteread = self.device.read(0x81, 64)  
        if len(byteread)>1:  
            self.pot_position.setValue(byteread[1])  

if __name__ == '__main__':  
    app = QApplication(sys.argv)  
    form = SimpleForm()  
    form.show()  
    sys.exit(app.exec_())  