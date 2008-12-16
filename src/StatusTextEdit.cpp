#ifndef StatusTextEdit_cpp
#define StatusTextEdit_cpp

#include <QToolTip>
#include <QScrollBar>

#include "StatusTextEdit.h"

#include <iostream>

using namespace std;

const int MAX_STATUS_CHARACTERS = 140;
const int STANDARD_HEIGHT = 45;

StatusTextEdit::StatusTextEdit(QWidget *parent): QTextEdit(parent) {
	setFixedHeight(STANDARD_HEIGHT);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
		if (height() != STANDARD_HEIGHT) {
			setFixedHeight(STANDARD_HEIGHT);
		}
		e->accept();
		return;
	}
	int a = toPlainText().length();
	QTextEdit::keyPressEvent(e);
	int b = toPlainText().length();
	if (a != b) emit leftCharsNumberChanged(MAX_STATUS_CHARACTERS - b);
	if (verticalScrollBar()->maximum() - verticalScrollBar()->minimum()) {
		setFixedHeight(height() + verticalScrollBar()->maximum() - verticalScrollBar()->minimum());
	}
}

void StatusTextEdit::reply(const QString &username) {
	QString text = toPlainText().simplified();
	if (text[0] == '@') {
		int i = text.indexOf(" ");
		if (i == -1) {
			i = text.length();
		}
		text = text.mid(i + 1).simplified();
	}
	setText("@" + username + " " + text);
	setFocus(Qt::OtherFocusReason);
	moveCursor(QTextCursor::NextWord);
}

#endif
