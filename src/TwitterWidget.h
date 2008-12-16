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
	QString iconFileName;
	int top;
	int height;
	QColor color;
	QDateTime time;
	QString username;
	int messageId;
	
	QString cacheUserpic;
	QString cacheUsername;
	QString cacheStatus;
	QDateTime cacheTime;
	int cacheMessageId;
	int cacheReplyStatusId;
	int cacheIndex;
};

const int ICON_SIZE = 48;
const int MARGIN = 5;
const int MAX_ITEMS_SIZE = 20;

class TwitterWidget: public QWidget {
	Q_OBJECT

private:

	QVector<TwitterWidgetItem> items;
	
	QString prepare(const QString &text, const int &replyStatusId);
	
public:

	TwitterWidget();
	void clear();
	void addItem(const QString &userpic, const QString &username, const QString &status, const QDateTime &time, int messageId, int replyStatusId, int i = -1);
	void updateItems();
	const TwitterWidgetItem getItem(int index);
	int getItemsCount();
	static QString formatDateTime(const QDateTime &time);
	
protected:

	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);
	
public slots:

	void replyClicked(const QUrl &url);
	void reloadUserpic(const QString &userpic);
	
signals:
	
	void reply(const QString &);
};

#endif
