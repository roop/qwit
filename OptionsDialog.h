#ifndef OptionsDialog_h
#define OptionsDialog_h

#include "ui_OptionsDialog.h"

class OptionsDialog: public QDialog, public Ui::OptionsDialog {
	Q_OBJECT

public:

	OptionsDialog(QWidget *parent);
	
};

#endif
