from MainWindow import Ui_MainWindow        #Se importa la configuracion de la ventana principal
from Configuracion import Ui_ConfWindow     #Se importa la configuracion de la ventana de configuracion
from AcercaDe import Ui_AcercaWindow        #Se importa la configuracion de la ventana de informacion
from PyQt4 import QtCore, QtGui             #Se importa la libreria de GUI
from PyQt4.QtCore import SIGNAL, QObject
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


class MainWindow(QtGui.QMainWindow):

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


    def Generar(self):
        global Control

        # ORDEN DE LOS BYTES A ENVIAR:
        # 1er BYTE --> TIPO DE SEÑAL [0 SENOIDAL, 1 TRIANGULAR, 2 RAMPA, 3 CUADRADA]
        # 2do BYTE --> FRECUENCIA [4]
        # 3er BYTE --> FRECUENCIA [3]
        # 4to BYTE --> FRECUENCIA [2]
        # 5to BYTE --> FRECUENCIA [1]
        # 6to BYTE --> FRECUENCIA [0]
        # 7mo BYTE --> OFFSET [SIGNO]
        # 8vo BYTE --> OFFSET [2]
        # 9no BYTE --> OFFSET [1]
        # 10mo BYTE --> OFFSET [0]
        # 11vo BYTE --> CONTROL [0 PARA, 1 INICIA]

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

    def Pausar(self):
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

    def Borrar(self):
        #ScribbleArea().clearImage(self.scribbleArea)
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

    def Senoidal(self):
        global Tipo

        Tipo = '0'
        print('La señal es de tipo senoidal')

    def Triangular(self):
        global Tipo

        Tipo = '1'
        print('La señal es de tipo triangular')

    def Rampa(self):
        global Tipo

        Tipo = '2'
        print('La señal es de tipo rampa')

    def Cuadrada(self):
        global Tipo

        Tipo = '3'
        print('La señal es de tipo cuadrada')

    def Frec(self, Frec):
        global Frecuencia

        Frecuencia = str(Frec)
        print('Frecuencia: ' + Frecuencia)

    def Off(self, Off):
        global Offset

        self.ui.offset.setToolTip(_fromUtf8(str(-self.ui.offset.value())))
        self.ui.lOffset.setText(_fromUtf8(str(-self.ui.offset.value()) + ' %'))
        Offset = str(-Off)
        print('Offset: ' + Offset)

    def Configurar(self):
        global Tipo
        global Frecuencia
        global Offset
        global Control

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

    def Configuracion(self):
        self.w = ConfWindow()
        #self.w.show()
        self.w.exec_()                      #Una vez abierta solo se puede interactuar con esta ventana

    def Acerca(self):
        self.w = AcercaWindow()
        #self.w.show()
        self.w.exec_()                      #Una vez abierta solo se puede interactuar con esta ventana

class ConfWindow(QtGui.QDialog):

    def __init__(self, parent=None):
        super(ConfWindow, self).__init__(parent)
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_ConfWindow()
        self.ui.setupUi(self)

class AcercaWindow(QtGui.QDialog):

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

try:
    dev = usb.core.find(idVendor=0x04D8, idProduct=0x000B)
    print('Dispositivo encontrado')
except:
    print('Dispositivo no encontrado') #No es redundante?

if dev is None:
    MainWindow().ui.setStatusTip('Dispositivo no encontrado')
    #MainWindow.Ui_MainWindow().setStatusTip('Dispositivo no encontrado')
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

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    myapp = MainWindow()
    myapp.show()
    sys.exit(app.exec_())

################################################################################
"""class ScribbleArea(QtGui.QWidget):
    def __init__(self, parent=None):
        super(ScribbleArea, self).__init__(parent)

        self.setAttribute(QtCore.Qt.WA_StaticContents)
        self.modified = False
        self.scribbling = False
        self.myPenWidth = 5
        self.myPenColor = QtCore.Qt.black
        imageSize = QtCore.QSize(300, 256)
#       self.image = QtGui.QImage()
        self.image = QtGui.QImage(imageSize, QtGui.QImage.Format_RGB32)
        self.lastPoint = QtCore.QPoint()

    def setPenColor(self, newColor):
        self.myPenColor = newColor

    def setPenWidth(self, newWidth):
        self.myPenWidth = newWidth

    def clearImage(self):
        self.image.fill(QtGui.qRgb(255, 255, 255))
        self.modified = True
        self.update()

    def mousePressEvent(self, event):
#       print "self.image.width() = %d" % self.image.width()
#       print "self.image.height() = %d" % self.image.height()
#       print "self.image.size() = %s" % self.image.size()
#       print "self.size() = %s" % self.size()
#       print "event.pos() = %s" % event.pos()
        if event.button() == QtCore.Qt.LeftButton:
            self.lastPoint = event.pos()
            self.scribbling = True

    def mouseMoveEvent(self, event):
        if (event.buttons() & QtCore.Qt.LeftButton) and self.scribbling:
            self.drawLineTo(event.pos())

    def mouseReleaseEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton and self.scribbling:
            self.drawLineTo(event.pos())
            self.scribbling = False

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.drawImage(event.rect(), self.image)

    def resizeEvent(self, event):
#       print "resize event"
#       print "event = %s" % event
#       print "event.oldSize() = %s" % event.oldSize()
#       print "event.size() = %s" % event.size()

        self.resizeImage(self.image, event.size())

#       if self.width() > self.image.width() or self.height() > self.image.height():
#           newWidth = max(self.width() + 128, self.image.width())
#           newHeight = max(self.height() + 128, self.image.height())
#           print "newWidth = %d, newHeight = %d" % (newWidth, newHeight)
#           self.resizeImage(self.image, QtCore.QSize(newWidth, newHeight))
#           self.update()

        super(ScribbleArea, self).resizeEvent(event)

    def drawLineTo(self, endPoint):
        painter = QtGui.QPainter(self.image)
        painter.setPen(QtGui.QPen(self.myPenColor, self.myPenWidth,
            QtCore.Qt.SolidLine, QtCore.Qt.RoundCap, QtCore.Qt.RoundJoin))
        painter.drawLine(self.lastPoint, endPoint)
        self.modified = True

        # rad = self.myPenWidth / 2 + 2
        # self.update(QtCore.QRect(self.lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad))
        self.update()
        self.lastPoint = QtCore.QPoint(endPoint)

    def resizeImage(self, image, newSize):
        if image.size() == newSize:
            return

#       print "image.size() = %s" % repr(image.size())
#       print "newSize = %s" % newSize

# this resizes the canvas without resampling the image
        newImage = QtGui.QImage(newSize, QtGui.QImage.Format_RGB32)
        newImage.fill(QtGui.qRgb(255, 255, 255))
        painter = QtGui.QPainter(newImage)
        painter.drawImage(QtCore.QPoint(0, 0), image)


##  this resampled the image but it gets messed up with so many events...
##      painter.setRenderHint(QtGui.QPainter.SmoothPixmapTransform, True)
##      painter.setRenderHint(QtGui.QPainter.HighQualityAntialiasing, True)
#
#       newImage = QtGui.QImage(newSize, QtGui.QImage.Format_RGB32)
#       newImage.fill(QtGui.qRgb(255, 255, 255))
#       painter = QtGui.QPainter(newImage)
#       srcRect = QtCore.QRect(QtCore.QPoint(0,0), image.size())
#       dstRect = QtCore.QRect(QtCore.QPoint(0,0), newSize)
##      print "srcRect = %s" % srcRect
##      print "dstRect = %s" % dstRect
#       painter.drawImage(dstRect, image, srcRect)


        self.image = newImage

    def print_(self):
        printer = QtGui.QPrinter(QtGui.QPrinter.HighResolution)

        printDialog = QtGui.QPrintDialog(printer, self)
        if printDialog.exec_() == QtGui.QDialog.Accepted:
            painter = QtGui.QPainter(printer)
            rect = painter.viewport()
            size = self.image.size()
            size.scale(rect.size(), QtCore.Qt.KeepAspectRatio)
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height())
            painter.setWindow(self.image.rect())
            painter.drawImage(0, 0, self.image)
            painter.end()

    def isModified(self):
        return self.modified

    def penColor(self):
        return self.myPenColor

    def penWidth(self):
        return self.myPenWidth"""
################################################################################
