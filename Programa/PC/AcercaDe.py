# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'AcercaDe.ui'
#
# Created: Fri Jan 10 12:53:30 2014
#      by: PyQt4 UI code generator 4.10.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_AcercaWindow(object):
    def setupUi(self, AcercaWindow):
        AcercaWindow.setObjectName(_fromUtf8("AcercaWindow"))
        AcercaWindow.resize(160, 210)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(AcercaWindow.sizePolicy().hasHeightForWidth())
        AcercaWindow.setSizePolicy(sizePolicy)
        AcercaWindow.setMinimumSize(QtCore.QSize(160, 210))
        AcercaWindow.setMaximumSize(QtCore.QSize(160, 210))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8("icono.jpg")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        AcercaWindow.setWindowIcon(icon)
        self.bSalir = QtGui.QPushButton(AcercaWindow)
        self.bSalir.setGeometry(QtCore.QRect(45, 185, 75, 23))
        self.bSalir.setObjectName(_fromUtf8("bSalir"))
        self.plainTextEdit = QtGui.QPlainTextEdit(AcercaWindow)
        self.plainTextEdit.setEnabled(False)
        self.plainTextEdit.setGeometry(QtCore.QRect(5, 5, 161, 181))
        self.plainTextEdit.setAutoFillBackground(False)
        self.plainTextEdit.setFrameShape(QtGui.QFrame.NoFrame)
        self.plainTextEdit.setFrameShadow(QtGui.QFrame.Plain)
        self.plainTextEdit.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.plainTextEdit.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.plainTextEdit.setUndoRedoEnabled(False)
        self.plainTextEdit.setLineWrapMode(QtGui.QPlainTextEdit.WidgetWidth)
        self.plainTextEdit.setTextInteractionFlags(QtCore.Qt.NoTextInteraction)
        self.plainTextEdit.setBackgroundVisible(False)
        self.plainTextEdit.setObjectName(_fromUtf8("plainTextEdit"))

        self.retranslateUi(AcercaWindow)
        QtCore.QMetaObject.connectSlotsByName(AcercaWindow)

    def retranslateUi(self, AcercaWindow):
        AcercaWindow.setWindowTitle(_translate("AcercaWindow", "Acerca de...", None))
        self.bSalir.setText(_translate("AcercaWindow", "Salir", None))
        self.plainTextEdit.setPlainText(_translate("AcercaWindow", "              UTN - FRVM\n"
"\n"
"Generador de señales por USB\n"
"\n"
"Grupo: \n"
"- Griffa, Joaquín Daniel\n"
"- Martino, Ignacio\n"
"- Vico, Matías Daniel\n"
"\n"
"Técnicas Digitales III\n"
"Medidas electrónicas II \n"
"\n"
"Año 2013", None))

