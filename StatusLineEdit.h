#ifndef StatusLineEdit_h
#define StatusLineEdit_h

#include <QWidget>
#include <QLineEdit>
#include <QFocusEvent>

const QString WHAT_ARE_YOU_DOING = "What are you doing?";

class StatusLineEdit: public QLineEdit {
	Q_OBJECT

public:

	StatusLineEdit(QWidget *parent = 0);
	
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
};

#endif
