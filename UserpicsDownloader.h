#ifndef UserpicsDownloader_h
#define UserpicsDownloader_h

#include <QObject>
#include <QQueue>
#include <QPair>
#include <QString>
#include <QFile>
#include <QHttp>
#include <QUrl>
#include <QFileInfo>

class UserpicsDownloader: QObject {
	Q_OBJECT
	
	QQueue<QPair<QString, QString> > queue;
	QFile file;
	QHttp http;
	QString proxyAddress;
	int proxyPort;
	QString proxyUsername;
	QString proxyPassword;
	
public:

	UserpicsDownloader();	
	void startDownload();
	void download(const QString &url, const QString &filename);
	void useProxy(const QString &address, int port, const QString &username, const QString &password);
	void dontUseProxy();

public slots:

	void httpDone(bool error);
};

#endif
