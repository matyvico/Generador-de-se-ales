from MainWindow import Ui_MainWindow        #Se importa la configuracion de la ventana principal
from Configuracion import Ui_ConfWindow     #Se importa la configuracion de la ventana de configuracion
from AcercaDe import Ui_AcercaWindow        #Se importa la configuracion de la ventana de informacion
####from PyQt4 import QtCore, QtGui             #Se importa la libreria de GUI
##from PyQt4.QtCore import SIGNAL, QObject    
import PyQt4
import sys                                  #Se importa la libreria del sistema
import array                                #Se importa la libreria de manejo de cadenas
import os                                   #Se importa la libreria del SO
import usb.core                             #Se importa la libreria de manejo del  BUS USB del sistema
import usb.util                             
import time                                 #Se importa la libreria de tiempo del sistema

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s


class MainWindow(QtGui.QMainWindow):        #Configuracion basica, de SEÑALES y de SLOTS de la GUI (ventana principal)

    def __init__(self, parent=None):
        
        super(MainWindow, self).__init__(parent)
        QtGui.QWidget.__init__(self, parent)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.actionSalir.triggered.connect(self.close)
        self.ui.actionConfiguracion.triggered.connect(self.Configuracion)
        self.ui.actionAcerca_de.triggered.connect(self.Acerca)
        
        self.ui.generar.clicked.connect(self.Generar)
        self.ui.pausar.clicked.connect(self.Pausar)
        self.ui.borrar.clicked.connect(self.Borrar)
        self.ui.borrar.clicked.connect(self.ui.senoidal.click)
        
        self.ui.offset.valueChanged.connect(self.Off)
        self.ui.frecuencia.valueChanged.connect(self.Frec)
        self.ui.senoidal.clicked.connect(self.Senoidal)
        self.ui.triangular.clicked.connect(self.Triangular)
        self.ui.rampa.clicked.connect(self.Rampa)
        self.ui.cuadrada.clicked.connect(self.Cuadrada)

    def Generar(self):                      #Funcion del boton "Generar"
        global Control
        Control = '1'
        
        try:
            Config = self.Configurar()
            signal = dev.write(1, Config, 0, 100)
            if signal:
                print('Se generó la señal')
                print('Se enviaron: ' + str(signal) + ' bytes: \'' + str(Config) + '\'')
                print('')
            else:
                print('Se produjo un error en el envio, revise la configuración')
        except:
            print ('No se pudo configurar el dispositivo')

    def Pausar(self):                       #Funcion del boton "Pausar"
        global Control
        global Config

        if Control == '1' and Control != '0':
            Control = '0'
            Config = self.Configurar()
            signal = dev.write(1, Config, 0, 100)
            print('Se pausó la señal')
            print('Se enviaron: ' + str(signal) + ' bytes: \'' + str(Config) + '\'')
            print('')
        else:
            print('La señal ya se encuentra pausada')

    def Borrar(self):                       #Funcion del boton "Borrar"
        global Tipo
        global Frecuencia
        global Offset
        global Control
        global Config

        Tipo = '0'                          #Seno (tipo por defecto)
        Frecuencia = '5000'                 #5000 Hz (frecuencia por defecto)
        Offset = '0'                        #0V de offset (offset por defecto)
        Control = '0'                       #Desactivo la generación de la señal
        
        self.ui.frecuencia.setValue(5000)
        self.ui.offset.setValue(0)

        Config = self.Configurar()
        try:
            signal = dev.write(1, Config, 0, 100)
            if signal:
                print('Se configuró la señal por defecto')
                print('Se enviaron: ' + str(signal) + ' bytes: \'' + str(Config) + '\'')
                print('')
            else:
                print('Se produjo un error en el envio, revise la configuración')
        except:
            print ('No se pudo configurar el dispositivo')
            
    def Senoidal(self):                     #Funcion del radio boton "Senoidal"
        global Tipo
        
        Tipo = '0'
        print('La señal es de tipo senoidal')
        
    def Triangular(self):                   #Funcion del radio boton "Triangular"
        global Tipo
    
        Tipo = '1'
        print('La señal es de tipo triangular')
    
    def Rampa(self):                        #Funcion del radio boton "Rampa"
        global Tipo
        
        Tipo = '2'
        print('La señal es de tipo rampa')
        
    def Cuadrada(self):                     #Funcion del radio boton "Cuadrada"
        global Tipo
        
        Tipo = '3'
        print('La señal es de tipo cuadrada')
        
    def Frec(self, Frec):                   #Funcion del spinbox de "Frecuencia"
        global Frecuencia
        
        Frecuencia = str(Frec)
        print('Frecuencia: ' + Frecuencia)
            
    def Off (self, Off):                    #Funcion del slider de "Offset"
        global Offset
        
        self.ui.offset.setToolTip(_fromUtf8(str(-self.ui.offset.value())))
        self.ui.lOffset.setText(_fromUtf8(str(-self.ui.offset.value()) + ' %'))
        Offset = str(-Off)
        print('Offset: ' + Offset)
        
    def Configurar(self):                   #Funcion que genera el vector de configuracion de la señal a enviar por USB
        global Tipo
        global Frecuencia
        global Offset
        global Control
        
        """
        ORDEN DE LOS BYTES A ENVIAR:
        1er BYTE --> TIPO DE SEÑAL [0 SENOIDAL, 
                                    1 TRIANGULAR, 
                                    2 RAMPA, 
                                    3 CUADRADA] --> N° en ASCII
        2do BYTE --> FRECUENCIA [4] --------------> N° en ASCII                                   
        3er BYTE --> FRECUENCIA [3] --------------> N° en ASCII
        4to BYTE --> FRECUENCIA [2] --------------> N° en ASCII
        5to BYTE --> FRECUENCIA [1] --------------> N° en ASCII
        6to BYTE --> FRECUENCIA [0] --------------> N° en ASCII
        7mo BYTE --> OFFSET [SIGNO] --------------> Signo (+ o -) en ASCII
        8vo BYTE --> OFFSET [2] ------------------> N° en ASCII
        9no BYTE --> OFFSET [1] ------------------> N° en ASCII
        10mo BYTE --> OFFSET [0] -----------------> N° en ASCII
        11vo BYTE --> CONTROL [0 PARA, 1 INICIA] -> N° en ASCII
        """
        
        if len(str(Frecuencia)) < 5 and len(str(Frecuencia)) > 3:
            Frecuencia = '0' + Frecuencia
        elif len(str(Frecuencia)) < 4 and len(str(Frecuencia)) > 2:
            Frecuencia = '00' + Frecuencia
        elif len(str(Frecuencia)) < 3 and len(str(Frecuencia)) > 1:
            Frecuencia = '000' + Frecuencia
        elif len(str(Frecuencia)) < 2:
            Frecuencia = '0000' + Frecuencia
            
        if int(Offset) >= 0:
            if len(Offset) == 3:
                Offset = '+' + Offset
            elif len(Offset) < 3 and len(Offset) > 1:
                Offset = '+0' + Offset
            elif len(Offset) < 2:
                Offset = '+00' + Offset
             
        elif int(Offset) < 0:
            if len(Offset) < 4 and len(Offset) > 2:
                Offset = '-0' + str(-int(Offset))
            elif len(Offset) < 3:
                Offset = '-00' + str(-int(Offset))
            
        Config = Tipo + str(Frecuencia) + str(Offset) + Control
        print(Config)
        
        return Config
            
    def Configuracion(self):                #Funcion de llamada a la ventana de "Configuracion"
        self.w = ConfWindow()
        #self.w.show()
        self.w.exec_()                      #Una vez abierta solo se puede interactuar con esta ventana
        
    def Acerca(self):                       #Funcion de llamada a la ventana de informacion "Acerca de..."
        self.w = AcercaWindow()
        #self.w.show()
        self.w.exec_()                      #Una vez abierta solo se puede interactuar con esta ventana

class ConfWindow(QtGui.QDialog):            #Configuracion basica, de SEÑALES y de SLOTS de la GUI (ventana de configuracion)

    def __init__(self, parent=None):
        super(ConfWindow, self).__init__(parent)
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_ConfWindow()
        self.ui.setupUi(self)
        
class AcercaWindow(QtGui.QDialog):          #Configuracion basica, de SEÑALES y de SLOTS de la GUI (ventana de informacion)
    
    def __init__(self, parent=None):
        super(AcercaWindow, self).__init__(parent)
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_AcercaWindow()
        self.ui.setupUi(self)

        self.ui.bSalir.clicked.connect(self.close)

#VALORES INICIALES DE LA SEÑAL
Tipo = '0'
Frecuencia = '5000'
Offset = '0'
Control = '1'

try:                                        #Busqueda del dispositivo por VID y PID
    dev = usb.core.find(idVendor=0x04D8, idProduct=0x000B)
    print('Dispositivo encontrado')
except:
    print('Dispositivo no encontrado')

if dev is None:
    MainWindow().ui.setStatusTip('Dispositivo no encontrado')
    raise ValueError('Dispositivo no encontrado')
else:
    dev.set_configuration()
    cfg = dev.get_active_configuration()
    cfg = dev[0]
    intf = cfg[(0,0)]
    ep = usb.util.find_descriptor(
    intf,
    #Se asocia al primer ENDPOINT de salida
    custom_match = \
    lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_OUT
)
    ep = intf[0]
    interface = 0
    endpoint = dev[0][(0,0)][0]
    assert ep is not None

if __name__ == '__main__':                  #Se inicia el bucle de ejecucion del programa
    app = QtGui.QApplication(sys.argv)
    myapp = MainWindow()
    myapp.show()
    sys.exit(app.exec_())
