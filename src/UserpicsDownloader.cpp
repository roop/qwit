#ifndef UserpicsDownloader_cpp
#define UserpicsDownloader_cpp

#include <iostream>

#include "UserpicsDownloader.h"

using namespace std;

UserpicsDownloader::UserpicsDownloader() {
	connect(&http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
}
	
void UserpicsDownloader::startDownload() {
	QPair<QString, QString> item = queue.front();
	file.setFileName(item.second);
	if (!file.open(QIODevice::WriteOnly)) {
		return;
	}
	QUrl url(item.first);
	if (proxyAddress != "") {
		http.setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		http.setProxy("", 0);
	}
	http.setHost(url.host(), url.port(80));
	http.get(QUrl::toPercentEncoding(url.path(), "/"), &file);
	emit stateChanged("Downloading " + item.first);
}
	
void UserpicsDownloader::download(const QString &url, const QString &filename) {
	QFileInfo fi = QFileInfo(filename);
	if (fi.exists() && (fi.size() > 0)) return;
	queue.enqueue(qMakePair(url, filename));
	if (queue.size() == 1) {
		startDownload();
	}
}

void UserpicsDownloader::httpDone(bool error) {
	if (error) {
		emit stateChanged("Error while downloading userpic: " + http.errorString());
	}
	QPair<QString, QString> item = queue.dequeue();
	file.close();
	emit userpicDownloaded(item.second);
	emit stateChanged("Downloaded " + item.first);
	if (queue.size()) {
		startDownload();
	}
}

void UserpicsDownloader::useProxy(const QString &address, int port, const QString &username, const QString &password) {
	proxyAddress = address;
	proxyPort = port;
	proxyUsername = username;
	proxyPassword = password;
}

void UserpicsDownloader::dontUseProxy() {
	proxyAddress = "";
}

#endif
