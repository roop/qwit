#ifndef Twitter_h
#define Twitter_h

#include <QObject>
#include <QHttp>
#include <QString>
#include <QBuffer>

#define HOME_XML_URL "http://twitter.com/statuses/friends_timeline.xml"

#define STATUS_UPDATE_URL "http://twitter.com/statuses/update.xml"

//#define HOME_XML_URL "http://twitter.com/statuses/user_timeline/freetonik.xml"
//#define HOME_XML_URL "http://twitter.lo/друзья.php"
//#define STATUS_UPDATE_URL "http://twitter.lo/update.php"

class Twitter: public QObject {
	Q_OBJECT

	QHttp statusHttp;
	QHttp homeHttp;
	QBuffer buffer;
	QString proxyAddress;
	int proxyPort;
	QString proxyUsername;
	QString proxyPassword;
	
public:

	Twitter();
	void useProxy(const QString &address, int port, const QString &username, const QString &password);
	void dontUseProxy();
	void sendStatus(QString username, QString password, QString status);
	void updateHome(QString username, QString password, int lastStatusId);
	void abort();

public slots:

	void statusHttpDone(bool error);
	void homeHttpDone(bool error);

signals:
	
	void homeUpdated(const QByteArray &buffer);
	void statusUpdated();
};

#endif
