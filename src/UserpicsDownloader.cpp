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
