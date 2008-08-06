#ifndef Twitter_cpp
#define Twitter_cpp

#include <QUrl>
#include <QHttpRequestHeader>
#include <QByteArray>

#include "Twitter.h"

#include <iostream>

using namespace std;

Twitter::Twitter() {
	proxyAddress = "";
	connect(&statusHttp, SIGNAL(done(bool)), this, SLOT(statusHttpDone(bool)));
	connect(&homeHttp, SIGNAL(done(bool)), this, SLOT(homeHttpDone(bool)));
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
	cout << "Sending status: " << qPrintable(status) << endl;
	
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
	statusHttp.request(header, data);
}

void Twitter::updateHome(QString username, QString password, int lastStatusId) {
	cout << "Updating" << endl;
	
	QUrl url(HOME_XML_URL);
	
	if (proxyAddress != "") {
		homeHttp.setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		homeHttp.setProxy("", 0);
	}
	
	homeHttp.setHost(url.host(), url.port(80));
	homeHttp.setUser(username, password);
	
	buffer.open(QIODevice::WriteOnly);
	homeHttp.get(url.path() + (lastStatusId ? "?since_id=" + QString::number(lastStatusId) : ""), &buffer);
}

void Twitter::statusHttpDone(bool error) {
	if (error) {
		cerr << "Error occured: " << qPrintable(statusHttp.errorString()) << endl;
		return;
	}
	emit statusUpdated();
}

void Twitter::homeHttpDone(bool error) {
	if (error) {
		cerr << "Error occured: " << qPrintable(homeHttp.errorString()) << endl;
		return;
	}
	buffer.close();
	emit homeUpdated(buffer.data());
}

void Twitter::abort() {
	homeHttp.abort();
	statusHttp.abort();
}

#endif
