/*  This file is part of Qwit.

    Qwit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Qwit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Qwit.  If not, see <http://www.gnu.org/licenses/>. */

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
