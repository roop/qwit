#ifndef OptionsDialog_cpp
#define OptionsDialog_cpp

#include "MainWindow.h"
#include "OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);
	connect(savePushButton, SIGNAL(pressed()), this, SLOT(saveSettings()));
	connect(resetPushButton, SIGNAL(pressed()), this, SLOT(resetSettings()));
}

void OptionsDialog::saveSettings() {
	hide();
	MainWindow::getInstance()->saveState();
	MainWindow::getInstance()->updateTimeline();
}

void OptionsDialog::resetSettings() {
	MainWindow::getInstance()->resetSettings();
}

#endif
