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

#ifndef TwitterWidget_h
#define TwitterWidget_h

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBrowser>
#include <QLabel>
#include <QDateTime>

class TwitterWidgetItem {
public:
	QTextBrowser *status;
	QLabel *icon;
	QLabel *sign;
	QLabel *contrl;
	QString iconFileName;
	int top;
	int height;
	QColor color;
	QDateTime time;
	QString username;
	int messageId;

        QString cleanStatus;
	QString cacheUserpic;
	QString cacheUsername;
	QString cacheStatus;
	QDateTime cacheTime;
	int cacheMessageId;
	int cacheReplyStatusId;
	int cacheIndex;

	void loadIcon();
};

const int ICON_SIZE = 48;
const int MARGIN = 5;

class TwitterWidget: public QWidget {
	Q_OBJECT

private:
	int messagesPerPage;
	bool usernameUnderAvatar;
	bool verticalAlignControl;

	QVector<TwitterWidgetItem> items;
	QString prepare(const QString &text, const int &replyStatusId, const QString &serviceBaseURL);
	bool isUsernameChar(const QChar &c) const;
	QString serviceBaseURL;

public:

	TwitterWidget();
	void clear();
	void addItem(const QString &userpic, const QString &username, const QString &status, const QDateTime &time, int messageId, int replyStatusId, int i, const QString &serviceBaseURL);
	void updateItems();
	const TwitterWidgetItem getItem(int index);
	int getItemsCount();
	void setMessagesPerPage(int value);
	void setUsernameUnderAvatar(bool value);
	void setVerticalAlignControl(bool value);

	static QString formatDateTime(const QDateTime &time);

protected:

	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);

public slots:

	void replyClicked(const QUrl &url);
	void directMessagesClicked(const QUrl &url);
	void reloadUserpic(const QString &userpic);

signals:

        void reply(const QString &);
        void retweet(const QString &, const QString &);
	void directMessages(const QString &);
        void replyID(const QString &);
        void directMessagesID(const QString &);
};

#endif
