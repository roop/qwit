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
	setAcceptRichText(false);
	emit leftCharsNumberChanged(MAX_STATUS_CHARACTERS);
	connect(this, SIGNAL(textChanged()), this, SLOT(textChangedToCharsNumberChanged()));
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
	QTextEdit::keyPressEvent(e);
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

void StatusTextEdit::setRetweetTag(const QString &rtTag, const bool &rtPlace) {
        retweetTag = rtTag;
        retweetTagPlace = rtPlace;
}

void StatusTextEdit::retweet(const QString &username, const QString &text) {
    //need to add customizeable retweet message
    emit getRetweetTag();
    int i = retweetTag.indexOf("@");
    if (i != -1) {
        QString firsthalf = retweetTag.left(i+1);
        QString secondhalf = retweetTag.right(retweetTag.size()-(i+1));
        retweetTag = firsthalf + username + secondhalf;
    }
    if (retweetTagPlace) {
        setText(text + " " + retweetTag);
    }
    else {
        setText(retweetTag + " " + text);
    }

    setFocus(Qt::OtherFocusReason);
    moveCursor(QTextCursor::NextWord);
}

void StatusTextEdit::directMessages(const QString &username) {
	QString text = toPlainText().simplified();
	if (text[0] == 'd') {
		int i = text.indexOf(" ");
		if (i == -1) {
			i = text.length();
		}
		text = text.mid(i + 1).simplified();
	}
	setText("d " + username + " " + text);
	setFocus(Qt::OtherFocusReason);
	moveCursor(QTextCursor::NextWord);
	moveCursor(QTextCursor::NextWord);
}

void StatusTextEdit::textChangedToCharsNumberChanged() {
	emit leftCharsNumberChanged(MAX_STATUS_CHARACTERS - toPlainText().length());
	updateSize();
}

void StatusTextEdit::updateSize() {
	if (verticalScrollBar()->maximum() - verticalScrollBar()->minimum()) {
		setFixedHeight(height() + verticalScrollBar()->maximum() - verticalScrollBar()->minimum());
	}
}

#endif
