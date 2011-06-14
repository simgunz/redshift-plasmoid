# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'config.ui'
#
# Created: Tue Jun 14 22:45:42 2011
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_cfgDialog(object):
	def setupUi(self, cfgDialog):
		cfgDialog.setObjectName("cfgDialog")
		cfgDialog.setWindowModality(QtCore.Qt.NonModal)
		cfgDialog.resize(150, 80)
		cfgDialog.setMinimumSize(QtCore.QSize(150, 80))
		cfgDialog.setMaximumSize(QtCore.QSize(150, 80))
		self.formLayoutWidget = QtGui.QWidget(cfgDialog)
		self.formLayoutWidget.setGeometry(QtCore.QRect(0, 0, 151, 81))
		self.formLayoutWidget.setObjectName("formLayoutWidget")
		self.layout = QtGui.QFormLayout(self.formLayoutWidget)
		self.layout.setFieldGrowthPolicy(QtGui.QFormLayout.ExpandingFieldsGrow)
		self.layout.setLabelAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
		self.layout.setFormAlignment(QtCore.Qt.AlignJustify|QtCore.Qt.AlignVCenter)
		self.layout.setObjectName("layout")
		self.latLabel = QtGui.QLabel(self.formLayoutWidget)
		self.latLabel.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
		self.latLabel.setObjectName("latLabel")
		self.layout.setWidget(0, QtGui.QFormLayout.LabelRole, self.latLabel)
		self.latBox = QtGui.QDoubleSpinBox(self.formLayoutWidget)
		self.latBox.setButtonSymbols(QtGui.QAbstractSpinBox.UpDownArrows)
		self.latBox.setDecimals(1)
		self.latBox.setMinimum(-190.0)
		self.latBox.setMaximum(190.0)
		self.latBox.setSingleStep(0.1)
		self.latBox.setObjectName("latBox")
		self.layout.setWidget(0, QtGui.QFormLayout.FieldRole, self.latBox)
		self.lonLabel = QtGui.QLabel(self.formLayoutWidget)
		self.lonLabel.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
		self.lonLabel.setObjectName("lonLabel")
		self.layout.setWidget(1, QtGui.QFormLayout.LabelRole, self.lonLabel)
		self.lonBox = QtGui.QDoubleSpinBox(self.formLayoutWidget)
		self.lonBox.setDecimals(1)
		self.lonBox.setMinimum(-180.0)
		self.lonBox.setMaximum(180.0)
		self.lonBox.setSingleStep(0.1)
		self.lonBox.setObjectName("lonBox")
		self.layout.setWidget(1, QtGui.QFormLayout.FieldRole, self.lonBox)
		self.tmpLabel = QtGui.QLabel(self.formLayoutWidget)
		self.tmpLabel.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
		self.tmpLabel.setObjectName("tmpLabel")
		self.layout.setWidget(2, QtGui.QFormLayout.LabelRole, self.tmpLabel)
		self.tmpBox = QtGui.QSpinBox(self.formLayoutWidget)
		self.tmpBox.setButtonSymbols(QtGui.QAbstractSpinBox.PlusMinus)
		self.tmpBox.setPrefix("")
		self.tmpBox.setMaximum(9999)
		self.tmpBox.setObjectName("tmpBox")
		self.layout.setWidget(2, QtGui.QFormLayout.FieldRole, self.tmpBox)

		self.retranslateUi(cfgDialog)
		QtCore.QMetaObject.connectSlotsByName(cfgDialog)

	def retranslateUi(self, cfgDialog):
		cfgDialog.setWindowTitle(QtGui.QApplication.translate("cfgDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
		self.latLabel.setText(QtGui.QApplication.translate("cfgDialog", "Latitude", None, QtGui.QApplication.UnicodeUTF8))
		self.latBox.setSuffix(QtGui.QApplication.translate("cfgDialog", "º", None, QtGui.QApplication.UnicodeUTF8))
		self.lonLabel.setText(QtGui.QApplication.translate("cfgDialog", "Longitude", None, QtGui.QApplication.UnicodeUTF8))
		self.lonBox.setSuffix(QtGui.QApplication.translate("cfgDialog", "º", None, QtGui.QApplication.UnicodeUTF8))
		self.tmpLabel.setText(QtGui.QApplication.translate("cfgDialog", "Temperature", None, QtGui.QApplication.UnicodeUTF8))
		self.tmpBox.setSuffix(QtGui.QApplication.translate("cfgDialog", "ºC", None, QtGui.QApplication.UnicodeUTF8))

