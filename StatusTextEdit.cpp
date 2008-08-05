#ifndef StatusTextEdit_cpp
#define StatusTextEdit_cpp

#include <QToolTip>

#include "StatusTextEdit.h"

#include <iostream>

using namespace std;

const int MAX_STATUS_CHARACTERS = 140;

StatusTextEdit::StatusTextEdit(QWidget *parent): QTextEdit(parent) {
	emit leftCharsNumberChanged(MAX_STATUS_CHARACTERS);
}
	
int StatusTextEdit::getMaxStatusCharacter() {
	return MAX_STATUS_CHARACTERS;
}

void StatusTextEdit::focusInEvent(QFocusEvent *event) {
	QTextEdit::focusInEvent(event);
}
	
void StatusTextEdit::focusOutEvent(QFocusEvent *event) {
	QTextEdit::focusOutEvent(event);
}

void StatusTextEdit::keyPressEvent(QKeyEvent *e) {
	if ((e->key() == Qt::Key_Return) || (e->key() == Qt::Key_Enter)) {
		emit returnPressed();
		e->accept();
		return;
	}
	int a = toPlainText().length();
	QTextEdit::keyPressEvent(e);
	int b = toPlainText().length();
	if (a != b) emit leftCharsNumberChanged(MAX_STATUS_CHARACTERS - b);
}

#endif