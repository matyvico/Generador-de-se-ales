# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Configuracion.ui'
#
# Created: Fri Jan 10 12:53:40 2014
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

class Ui_ConfWindow(object):
    def setupUi(self, ConfWindow):
        ConfWindow.setObjectName(_fromUtf8("ConfWindow"))
        ConfWindow.resize(100, 100)
        ConfWindow.setMinimumSize(QtCore.QSize(100, 100))
        ConfWindow.setMaximumSize(QtCore.QSize(100, 100))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8("icono.jpg")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        ConfWindow.setWindowIcon(icon)

        self.retranslateUi(ConfWindow)
        QtCore.QMetaObject.connectSlotsByName(ConfWindow)

    def retranslateUi(self, ConfWindow):
        ConfWindow.setWindowTitle(_translate("ConfWindow", "Configuración", None))

