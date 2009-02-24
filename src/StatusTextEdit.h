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

#ifndef StatusTextEdit_h
#define StatusTextEdit_h

#include <QWidget>
#include <QTextEdit>
#include <QFocusEvent>

class StatusTextEdit: public QTextEdit {
	Q_OBJECT

public:

	StatusTextEdit(QWidget *parent = 0);

	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
	int getMaxStatusCharacter();

protected:

	void keyPressEvent(QKeyEvent *e);

signals:

	void returnPressed();
	void leftCharsNumberChanged(int);

public slots:

	void reply(const QString &username);
	void directMessages(const QString &username);
	void textChangedToCharsNumberChanged();
	void updateSize();
};

#endif
