#ifndef LogsDialog_h
#define LogsDialog_h

#include "ui_LogsDialog.h"

class LogsDialog: public QDialog, public Ui::LogsDialog {
	Q_OBJECT

public:

	LogsDialog(QWidget *parent);
	
};

#endif
