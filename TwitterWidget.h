#ifndef TwitterWidget_h
#define TwitterWidget_h

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBrowser>
#include <QLabel>

class TwitterWidgetItem {
public:
	QTextBrowser *status;
	QLabel *icon;
	QLabel *sign;
};

const int ICON_SIZE = 48;
const int MARGIN = 5;

class TwitterWidget: public QWidget {
	Q_OBJECT

private:

	QVector<TwitterWidgetItem> items;
	
	QString prepare(const QString &text, const int &replyStatusId);
	
public:

	TwitterWidget();
	void clear();
	void addItem(const QString &userpic, const QString &username, const QString &status, const QString &time, int messageId, int replyStatusId, int i = -1);
	void updateItems();
	
protected:

	void resizeEvent(QResizeEvent *event);
	
public slots:

	void replyClicked(const QUrl &url);
	
signals:
	
	void reply(const QString &);
};

#endif
