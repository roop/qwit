#ifndef TwitterWidget_cpp
#define TwitterWidget_cpp

#include "TwitterWidget.h"

#include <iostream>

using namespace std;

QString TwitterWidget::convertUrlsToLinks(const QString &text) {
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
		t += "<a href=\"" + url + "\">" + url + "</a>";
		i = k;
	}
	t += s.mid(i);
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
	
void TwitterWidget::addItem(const QString &userpic, const QString &username, const QString &status, const QString &time, const int &messageId) {
	TwitterWidgetItem item = TwitterWidgetItem();
	item.status = new QTextBrowser(this);
	item.status->setHtml(convertUrlsToLinks(status));
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
	
	item.sign = new QLabel("<a href=\"http://twitter.com/" + username + "\" style=\"font-weight:bold;text-decoration:none;color:#00f\">" + username + "</a> - <a href=\"http://twitter.com/" + username + "/statuses/" + QString::number(messageId) + "\" style=\"font-size:70%;text-decoration:none;color:#00f\">" + time + "</a>", this);
	item.sign->setOpenExternalLinks(true);
	items.push_back(item);
	
	item.status->show();
	item.icon->show();
	item.sign->show();
	
	updateItems();
}
	
void TwitterWidget::updateItems() {
	int height = 0;
	int statusItemWidth = width() - (ICON_SIZE + 3 * MARGIN);
	for (int i = 0; i < items.size(); ++i) {
		QFontMetrics fontMetrics(items[i].status->font());
		TwitterWidgetItem &item = items[i];
		int statusItemHeight = fontMetrics.boundingRect(0, 0, statusItemWidth, 1000, Qt::AlignTop | Qt::TextWordWrap, item.status->toPlainText()).height() + 5;
		item.status->move(ICON_SIZE + 2 * MARGIN, height + MARGIN);
		item.status->resize(statusItemWidth, statusItemHeight);
		item.icon->move(MARGIN, height + MARGIN);
		
		item.sign->move(width() - item.sign->width() - MARGIN, height + statusItemHeight + 2 * MARGIN);
		int itemHeight = statusItemHeight + item.sign->height() + MARGIN;
		height += max(ICON_SIZE, itemHeight) + 2 * MARGIN;
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

#endif
