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

#ifndef TwitterWidget_cpp
#define TwitterWidget_cpp

#include <QScrollBar>
#include <QDesktopServices>
#include <QPalette>
#include <QChar>

#include "TwitterWidget.h"
#include "QwitException.h"

#include <iostream>

using namespace std;

void TwitterWidgetItem::loadIcon() {
	QPixmap pixmap(iconFileName);
	if (!pixmap.isNull()) {
		icon->setPixmap(pixmap.scaled(ICON_SIZE, ICON_SIZE));
	}
	icon->resize(ICON_SIZE, ICON_SIZE);
}

TwitterWidget::TwitterWidget(): QWidget() {
	QDesktopServices::setUrlHandler("reply", this, "replyClicked");
	QDesktopServices::setUrlHandler("directMessages", this, "directMessagesClicked");
}

bool TwitterWidget::isUsernameChar(const QChar &c) const {
	return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9')) || (c == '_');
}

QString TwitterWidget::prepare(const QString &text, const int &replyStatusId, const QString &serviceBaseURL) {
	QString s = text;
	s.replace(" www.", " http://www.");
	if (s.startsWith("www.")) s = "http://" + s;
	QString t = "";
	int i = 0, j = 0;
	while ((j = s.indexOf("http://", i)) != -1) {
		t += s.mid(i, j - i);
		int k = s.indexOf(" ", j);
		if (k == -1) k = s.length();
		QString url = s.mid(j, k - j);
		t += "<a href=\"" + url + "\" style=\"text-decoration:none\">" + url + "</a>";
		i = k;
	}
	t += s.mid(i);
	if (replyStatusId && (t[0] == '@')) {
		s = t;
		int i = 1;
		while ((i < s.length()) && (QChar(s[i]).isLetterOrNumber() || (s[i] == '_'))) ++i;
		QString username = s.mid(1, i - 1);
		t = "@<a href=\"" + serviceBaseURL + "/" + username + "/statuses/" + QString::number(replyStatusId) + "\" style=\"text-decoration:none;font-weight:bold;\">" + username + "</a>" + s.mid(i);
	}
	s = t;
	t = "";
	for (int i = 0; i < s.length(); ++i) {
		t += s[i];
		if ((s[i] == '@') && (!i || !isUsernameChar(s[i - 1]))) {
			int j = i + 1;
			while ((j < s.length()) && isUsernameChar(s[j])) {
				++j;
			}
			if (j - i - 1 > 0) {
				QString username = s.mid(i + 1, j - i - 1);
				t += "<a href=\"" + serviceBaseURL + "/" + username + "\" style=\"text-decoration:none;font-weight:bold;\">" + username + "</a>";
				i = j - 1;
			}
		}
	}
	return t;
}

void TwitterWidget::clear() {
	for (int i = 0; i < items.size(); ++i) {
		delete items[i].status;
		delete items[i].icon;
		delete items[i].sign;
		delete items[i].contrl;
	}
	items.clear();
}

void TwitterWidget::addItem(const QString &userpic, const QString &username, const QString &status, const QDateTime &time, int messageId, int replyStatusId, int i, const QString &serviceBaseURL) {
	TwitterWidgetItem item = TwitterWidgetItem();

	item.time = time;
	item.username = username;
	item.messageId = messageId;

// Save raw info for caching between sessions

	item.cacheUserpic = userpic;
	item.cacheUsername = username;
	item.cacheStatus = status;
	item.cacheTime = time;
	item.cacheMessageId = messageId;
	item.cacheReplyStatusId = replyStatusId;
	item.cacheIndex = i;

        item.cleanStatus = status;

	item.status = new QTextBrowser(this);
	item.status->setHtml(prepare(status, replyStatusId, serviceBaseURL));
	item.status->setReadOnly(true);
	item.status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	item.status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	item.status->setFrameShape(QFrame::NoFrame);
	item.status->setOpenExternalLinks(true);
	QFont font = item.status->document()->defaultFont();
	font.setFamily("Verdana");
	item.status->document()->setDefaultFont(font);

	item.icon = new QLabel(this);
	item.iconFileName = userpic;
	item.loadIcon();
	item.sign = new QLabel("<a href=\"http://twitter.com/" + username + "\" style=\"font-weight:bold;text-decoration:none\">" + username + "</a> - <a href=\"http://twitter.com/" + username + "/statuses/" + QString::number(messageId) + "\" style=\"font-size:70%;text-decoration:none\">" + formatDateTime(time) + "</a> <a href=\"directMessages://" + username + ":" + QString::number(messageId) + "\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a> <a href=\"reply://tweet:" + QString::number(messageId) + "/q?user=" + username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a> <a href=\"reply://tweet:" + QString::number(messageId) + "/q?user=" + username + "&status=" + QUrl::toPercentEncoding(status) +  "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>", this);
	item.sign->setAlignment(Qt::AlignRight);
	item.sign->setOpenExternalLinks(true);
	item.contrl = new QLabel("<a href=\"http://twitter.com/" + username + "\" style=\"font-weight:bold;text-decoration:none\">" + username + "</a> - <a href=\"http://twitter.com/" + username + "/statuses/" + QString::number(messageId) + "\" style=\"font-size:70%;text-decoration:none\">" + formatDateTime(time) + "</a> <a href=\"directMessages://" + username + ":" + QString::number(messageId) + "\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a> <a href=\"reply://tweet:" + QString::number(messageId) + "/q?user=" + username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a> <a href=\"reply://tweet:" + QString::number(messageId) + "/q?user=" + username + "&status=" + QUrl::toPercentEncoding(status) +  "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>", this);
	item.contrl->setAlignment(Qt::AlignRight);
	item.contrl->setOpenExternalLinks(true);

	if (i == -1) {
		items.push_back(item);
	} else {
		if (i < 0) i = 0;
		else if (i > items.size()) i = items.size();
		items.insert(i, item);
	}

	item.status->show();
	item.icon->show();
	item.sign->show();
	item.contrl->show();

	while (items.size() > messagesPerPage) {
		TwitterWidgetItem &item = items[items.size() - 1];
		delete item.status;
		delete item.icon;
		delete item.sign;
		delete item.contrl;
		items.pop_back();
	}

	updateItems();
}

void TwitterWidget::updateItems() {
	int height = 0;
	int statusItemWidth = width() - (ICON_SIZE + 5 * MARGIN);
	for (int i = 0; i < items.size(); ++i) {
		QFontMetrics fontMetrics(items[i].status->font());
		TwitterWidgetItem &item = items[i];
		int statusItemHeight = fontMetrics.boundingRect(0, 0, statusItemWidth, 1000, Qt::AlignTop | Qt::TextWordWrap, item.status->toPlainText()).height() + 5;
		if (statusItemHeight < ICON_SIZE) {
			statusItemHeight = ICON_SIZE;
		}
		item.status->move(ICON_SIZE + 2 * MARGIN, height + MARGIN);
		item.status->resize(statusItemWidth, statusItemHeight);
		statusItemHeight += item.status->verticalScrollBar()->maximum() - item.status->verticalScrollBar()->minimum();
		item.status->resize(statusItemWidth, statusItemHeight);
		item.icon->move(MARGIN, height + MARGIN);

		if (usernameUnderAvatar) {
                QString tablew = "";
                tablew.setNum(statusItemWidth + 70);
			item.sign->setText(
			    "<table border=\"0\" width=\"" + tablew + "\" cellpadding=\"0\" cellspacing=\"0\"><tr valign=\"top\"><td width=\"50%\"><a href=\"http://twitter.com/" + item.username + "\" style=\"font-weight:bold;text-decoration:none\">" + item.username + "</a></td><td width=\"50%\"><p align=\"right\"><a href=\"http://twitter.com/" + item.username + "/statuses/" + QString::number(item.messageId) + "\" style=\"font-size:70%;text-decoration:none\">" + formatDateTime(item.time) + " </a></p></td>"  + (verticalAlignControl ? "" : "<td><p align=\"right\"><a href=\"directMessages://" + item.username + ":" + QString::number(item.messageId) + "\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a><a href=\"reply://tweet:" + QString::number(item.messageId) + "/q?user=" + item.username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a><a href=\"reply://tweet:" + QString::number(item.messageId) + "/q?user=" + item.username + "&status=" + QUrl::toPercentEncoding(item.cleanStatus) +  "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a></p></td>") + "</tr></table>");
			item.sign->resize(width()+(7 * MARGIN), 16);
			item.sign->move(MARGIN, height + statusItemHeight + MARGIN);
		} else{
			item.sign->setText("<a href=\"http://twitter.com/" + item.username + "\" style=\"font-weight:bold;text-decoration:none\">" + item.username + "</a> - <a href=\"http://twitter.com/" + item.username + "/statuses/" + QString::number(item.messageId) + "\" style=\"font-size:70%;text-decoration:none\">" + formatDateTime(item.time) + "</a> " + (verticalAlignControl ? "" : "<a href=\"directMessages://" + item.username + ":" + QString::number(item.messageId) + "\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a><a href=\"reply://tweet:" + QString::number(item.messageId) + "/q?user=" + item.username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a><a href=\"reply://tweet:" + QString::number(item.messageId) + "/q?user=" + item.username + "&status=" + QUrl::toPercentEncoding(item.cleanStatus) +  "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>"));
			item.sign->adjustSize();
			item.sign->move(width() - item.sign->width() - MARGIN, height + statusItemHeight + MARGIN);
		}
		item.contrl->setText(verticalAlignControl ? "<a href=\"directMessages://" + item.username + ":" + QString::number(item.messageId) + "\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a><br><a href=\"reply://tweet:" + QString::number(item.messageId) + "/q?user=" + item.username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a><br><a href=\"reply://tweet:" + QString::number(item.messageId) + "/q?user=" + item.username + "&status=" + QUrl::toPercentEncoding(item.cleanStatus) +  "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>" :"");
		item.contrl->adjustSize();
		item.contrl->move(width() - item.contrl->width() - MARGIN, height + MARGIN);

		if (i & 1) {
			item.color = QColor(230, 230, 230);
		} else {
			item.color = QColor("white");
		}

		int itemHeight = statusItemHeight + item.sign->height() + MARGIN;
		itemHeight = max(ICON_SIZE, item.sign->y() + item.sign->height()) + MARGIN - height;
		item.top = height;
		item.height = itemHeight;
		height += itemHeight;
	}
	resize(width(), height);
}

void TwitterWidget::resizeEvent(QResizeEvent *event) {
	if (event->oldSize() == event->size()) {
		event->ignore();
		return;
	}
	updateItems();
	event->accept();
}

void TwitterWidget::replyClicked(const QUrl &url) {
        if (url.hasQueryItem("status")) {
            emit retweet(QUrl::fromPercentEncoding((url.queryItemValue("user")).toAscii()), QUrl::fromPercentEncoding((url.queryItemValue("status")).toAscii()));
        }
        else emit reply(QUrl::fromPercentEncoding((url.queryItemValue("user")).toAscii()));
        emit replyID(QString::number(url.port()));
}

void TwitterWidget::directMessagesClicked(const QUrl &url) {
	emit directMessages(url.host());
        //emit directMessagesID(QString::number(url.port()));
}

void TwitterWidget::reloadUserpic(const QString &userpic) {
	for (int i = 0; i < items.size(); ++i) if (items[i].iconFileName == userpic) {
		TwitterWidgetItem &item = items[i];
		item.loadIcon();
//		item.icon->resize(ICON_SIZE, ICON_SIZE);
	}
}

void TwitterWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	for (int i = 0; i < items.size(); ++i) {
		TwitterWidgetItem &item = items[i];
		painter.fillRect(0, items[i].top, width(), items[i].height, QBrush(item.color));
		QPalette p = palette();
		p.setColor(QPalette::Active, QPalette::Base, item.color);
		p.setColor(QPalette::Inactive, QPalette::Base, item.color);
		item.status->setPalette(p);
	}
	event->accept();
}


QString TwitterWidget::formatDateTime(const QDateTime &time) {
	int seconds = time.secsTo(QDateTime::currentDateTime());
	if (seconds <= 15) return tr("Just now");
	if (seconds <= 45) return tr("about %n second(s) ago", "", seconds);
	int minutes = (seconds - 45 + 59) / 60;
	if (minutes <= 45) return tr("about %n minute(s) ago", "", minutes);
	int hours = (seconds - 45 * 60 + 3599) / 3600;
	if (hours <= 18) return tr("about %n hour(s) ago", "", hours);
	int days = (seconds - 18 * 3600 + 24 * 3600 - 1) / (24 * 3600);
	return tr("about %n day(s) ago", "", days);
}

const TwitterWidgetItem TwitterWidget::getItem(int index) {
// Possible fix for issue 10
	if (index >= items.size()) {
		throw QwitException("Trying to get a nonexisting item from TwitterWidget::items");
	}
	return items[index];
}

int TwitterWidget::getItemsCount() {
	return items.size();
}

void TwitterWidget::setMessagesPerPage(int value) {
	messagesPerPage = value;
}

void TwitterWidget::setUsernameUnderAvatar(bool value) {
	usernameUnderAvatar = value;
}

void TwitterWidget::setVerticalAlignControl(bool value) {
	verticalAlignControl = value;
}

#endif
