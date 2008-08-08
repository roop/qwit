#ifndef TwitterWidget_cpp
#define TwitterWidget_cpp

#include <QScrollBar>
#include <QDesktopServices>
#include <QPalette>

#include "TwitterWidget.h"

#include <iostream>

using namespace std;

TwitterWidget::TwitterWidget(): QWidget() {
	QDesktopServices::setUrlHandler("reply", this, "replyClicked");
}

QString TwitterWidget::prepare(const QString &text, const int &replyStatusId) {
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
		t = "@<a href=\"http://twitter.com/" + username + "/statuses/" + QString::number(replyStatusId) + "\" style=\"text-decoration:none\">" + username + "</a>" + s.mid(i);
	}
	return t;
}
	
void TwitterWidget::clear() {
	for (int i = 0; i < items.size(); ++i) {
		delete items[i].status;
		delete items[i].icon;
		delete items[i].sign;
	}
	items.clear();
}
	
void TwitterWidget::addItem(const QString &userpic, const QString &username, const QString &status, const QString &time, int messageId, int replyStatusId, int i) {
	TwitterWidgetItem item = TwitterWidgetItem();
	item.status = new QTextBrowser(this);
	item.status->setHtml(prepare(status, replyStatusId));
	item.status->setReadOnly(true);
	item.status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	item.status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	item.status->setFrameShape(QFrame::NoFrame);
	item.status->setOpenExternalLinks(true);
	QFont font = item.status->document()->defaultFont();
	font.setFamily("Verdana");
	item.status->document()->setDefaultFont(font);
	
	item.icon = new QLabel(this);
	item.icon->setPixmap(QPixmap(userpic));
	item.iconFileName = userpic;
	
	item.sign = new QLabel("<a href=\"http://twitter.com/" + username + "\" style=\"font-weight:bold;text-decoration:none\">" + username + "</a> - <a href=\"http://twitter.com/" + username + "/statuses/" + QString::number(messageId) + "\" style=\"font-size:70%;text-decoration:none\">" + time + "</a> <a href=\"reply://" + username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a>", this);
	item.sign->setAlignment(Qt::AlignRight);
	item.sign->setOpenExternalLinks(true);
	if (i == -1) i = items.size();
	items.insert(i, item);
	
	item.status->show();
	item.icon->show();
	item.sign->show();
	
	while (items.size() > MAX_ITEMS_SIZE) {
		items.pop_back();
	}
	
	updateItems();
}
	
void TwitterWidget::updateItems() {
	int height = 0;
	int statusItemWidth = width() - (ICON_SIZE + 3 * MARGIN);
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
		
		item.sign->move(width() - item.sign->width() - MARGIN, height + statusItemHeight + MARGIN);
		
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
	emit reply(url.host());
}

void TwitterWidget::reloadUserpic(const QString &userpic) {
	for (int i = 0; i < items.size(); ++i) if (items[i].iconFileName == userpic) {
		TwitterWidgetItem &item = items[i];
		item.icon->setPixmap(QPixmap(item.iconFileName));
		item.icon->resize(ICON_SIZE, ICON_SIZE);
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

#endif
