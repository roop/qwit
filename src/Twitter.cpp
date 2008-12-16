#ifndef Twitter_cpp
#define Twitter_cpp

#include <QUrl>
#include <QHttpRequestHeader>
#include <QByteArray>
#include <QDateTime>

#include "Twitter.h"

#include <iostream>

using namespace std;

Twitter::Twitter() {
	urls[0] = HOME_XML_URL;
	urls[1] = PUBLIC_XML_URL;
	urls[2] = REPLIES_XML_URL;
	urls[3] = "";
	proxyAddress = "";
	connect(&statusHttp, SIGNAL(done(bool)), this, SLOT(statusHttpDone(bool)));
	connect(&timelineHttp, SIGNAL(done(bool)), this, SLOT(timelineHttpDone(bool)));
}

void Twitter::useProxy(const QString &address, int port, const QString &username, const QString &password) {
	proxyAddress = address;
	proxyPort = port;
	proxyUsername = username;
	proxyPassword = password;
}

void Twitter::dontUseProxy() {
	proxyAddress = "";
}

void Twitter::sendStatus(QString username, QString password, QString status) {
	QUrl url(STATUS_UPDATE_URL);

	QHttpRequestHeader header;
	header.setRequest("POST", url.path());
	header.setValue("Host", url.host());
	header.setContentType("application/x-www-form-urlencoded");
	
	if (proxyAddress != "") {
		statusHttp.setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		statusHttp.setProxy("", 0);
	}
	statusHttp.setHost(url.host(), url.port(80));
	statusHttp.setUser(username, password);
	
	QByteArray data = "status=";
	data += QUrl::toPercentEncoding(status);
	data += "&source=qwit";
	statusHttp.request(header, data);

	emit stateChanged("Sending status...");
}

void Twitter::update(QString username, QString password, int lastStatusId, int type) {
	if (urls[type] == "") return;
	
	if (timelineHttp.state() != QHttp::Unconnected) {
		cout << timelineHttp.state() <<endl;
		timelineHttp.abort();
		cout << timelineHttp.state() <<endl;
	}
	
	currentType = type;
	
	QUrl url(urls[type]);
	
	if (proxyAddress != "") {
		timelineHttp.setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		timelineHttp.setProxy("", 0);
	}
	
	timelineHttp.setHost(url.host(), url.port(80));
	timelineHttp.setUser(username, password);
	
	buffer.open(QIODevice::WriteOnly);
	timelineHttp.get(url.path() + (lastStatusId ? "?since_id=" + QString::number(lastStatusId) : ""), &buffer);
	
	emit stateChanged("Updating timeline...");
}

void Twitter::statusHttpDone(bool error) {
	if (error) {
		emit stateChanged("Error sending status: " + statusHttp.errorString());
		return;
	}
	emit stateChanged("Status send at " + QDateTime::currentDateTime().toString("hh:mm:ss"));
	emit statusUpdated();
}

void Twitter::timelineHttpDone(bool error) {
	if (error) {
		emit stateChanged("Error while updating timeline: " + timelineHttp.errorString());
		return;
	}
	buffer.close();
	emit stateChanged("Timeline updated at " + QDateTime::currentDateTime().toString("hh:mm:ss"));
	emit updated(buffer.data(), currentType);
}

void Twitter::abort() {
	timelineHttp.abort();
	statusHttp.abort();
}

void Twitter::setUrl(int index, const QString &url) {
	urls[index] = url;
}

#endif
