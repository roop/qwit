#ifndef Twitter_h
#define Twitter_h

#include <QObject>
#include <QHttp>
#include <QString>
#include <QBuffer>

#define HOME_XML_URL "http://twitter.com/statuses/friends_timeline.xml"
#define PUBLIC_XML_URL "http://twitter.com/statuses/public_timeline.xml"
#define REPLIES_XML_URL "http://twitter.com/statuses/replies.xml"
#define CUSTOM_XML_URL "http://twitter.com/statuses/user_timeline/"

#define STATUS_UPDATE_URL "http://twitter.com/statuses/update.xml"

//#define HOME_XML_URL "http://twitter.com/statuses/user_timeline/freetonik.xml"
//#define HOME_XML_URL "http://twitter.lo/друзья.php"
//#define STATUS_UPDATE_URL "http://twitter.lo/update.php"

class Twitter: public QObject {
	Q_OBJECT

	QHttp statusHttp;
	QHttp timelineHttp;
	QBuffer buffer;
	QString proxyAddress;
	int proxyPort;
	QString proxyUsername;
	QString proxyPassword;
	int currentType;
	QString urls[4];
	
public:

	Twitter();
	void useProxy(const QString &address, int port, const QString &username, const QString &password);
	void dontUseProxy();
	void sendStatus(QString username, QString password, QString status);
	void update(QString username, QString password, int lastStatusId, int type, int count);
	void setUrl(int index, const QString &url);
	void abort();

public slots:

	void statusHttpDone(bool error);
	void timelineHttpDone(bool error);

signals:
	
	void updated(const QByteArray &, int);
	void statusUpdated();
	void stateChanged(const QString &);
};

#endif
