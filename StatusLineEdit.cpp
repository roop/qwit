#ifndef StatusLineEdit_cpp
#define StatusLineEdit_cpp

#include "StatusLineEdit.h"

StatusLineEdit::StatusLineEdit(QWidget *parent): QLineEdit(parent) {
}
	
void StatusLineEdit::focusInEvent(QFocusEvent *event) {
	if (text() == WHAT_ARE_YOU_DOING) setText("");
	QLineEdit::focusInEvent(event);
}
	
void StatusLineEdit::focusOutEvent(QFocusEvent *event) {
	if (text() == "") setText(WHAT_ARE_YOU_DOING);
	QLineEdit::focusOutEvent(event);
}

#endif
