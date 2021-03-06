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

#ifndef Twitter_cpp
#define Twitter_cpp

#include <QUrl>
#include <QHttpRequestHeader>
#include <QByteArray>
#include <QDateTime>
#include <QNetworkProxy>

#include "Twitter.h"

#include <iostream>

using namespace std;

Twitter::Twitter() {
	urls[0] = HOME_XML_URL;
	urls[1] = PUBLIC_XML_URL;
	urls[2] = REPLIES_XML_URL;
	urls[3] = "";
	urls[4] = INPUT_DIRECT_XML_URL;
	urls[5] = OUTPUT_DIRECT_XML_URL;
	urls[6] = SEARCH_ATOM_URL;
	proxyAddress = "";
	connect(&statusHttp, SIGNAL(done(bool)), this, SLOT(statusHttpDone(bool)));
	connect(&timelineHttp, SIGNAL(done(bool)), this, SLOT(timelineHttpDone(bool)));
	connect(&timelineHttp, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(httpsError(const QList<QSslError> &)));
	connect(&statusHttp, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(httpsError(const QList<QSslError> &)));
}

void Twitter::setServiceBaseURL(const QString &url) {
	serviceBaseURL = url;
}

void Twitter::setServiceAPIURL(const QString &url) {
	serviceAPIURL = url;
}

QString Twitter::getServiceBaseURL() {
	return serviceBaseURL;
}

QString Twitter::getServiceAPIURL() {
	return serviceAPIURL;
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

void Twitter::sendStatus(QString username, QString password, QString status, QString replyID) {
	QUrl url(serviceAPIURL + STATUS_UPDATE_URL);

	QHttpRequestHeader header;
	header.setRequest("POST", url.path());
	header.setValue("Host", url.host());
	header.setContentType("application/x-www-form-urlencoded");

	if (proxyAddress != "") {
		statusHttp.setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		timelineHttp.setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}
    /* FIXME
     * https (ssl) mode is not working if setProxy is called with ("", 0)
     *   } else {
     *	  timelineHttp.setProxy("", 0);
     *   }
     */
    
    if(url.toString().indexOf("https") == 0) {
	    statusHttp.setHost(url.host(), QHttp::ConnectionModeHttps, 443);
    }
    else {
        statusHttp.setHost(url.host(), url.port(80));
    }

	statusHttp.setUser(username, password);

	QByteArray data = "status=";
	data += QUrl::toPercentEncoding(status);
        if (replyID != "") {
                data += "&in_reply_to_status_id=";
                data += replyID;
        }
	data += "&source=qwit";
	statusHttp.request(header, data);

	emit stateChanged(tr("Sending status: %1").arg(serviceAPIURL + STATUS_UPDATE_URL));
}

void Twitter::update(QString username, QString password, int lastStatusId, int type, int count) {
	if (urls[type] == "") {
		cerr << "No url defined" << endl;
		return;
	}

	if (timelineHttp.state() != QHttp::Unconnected) {
		timelineHttp.abort();
	}

	currentType = type;
	QUrl url;
	if (type == 6) {
		url=("http://search.twitter.com" + urls[type]);
	} else {
		url=(serviceAPIURL + urls[type]);
	}

	if (proxyAddress != "") {
		timelineHttp.setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
    } else {
		timelineHttp.setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}
    /* FIXME
     * https (ssl) mode is not working if setProxy is called with ("", 0)
     *   } else {
     *	  timelineHttp.setProxy("", 0);
     *   }
     */

    if(url.toString().indexOf("https") == 0) {
	    timelineHttp.setHost(url.host(), QHttp::ConnectionModeHttps, 443);
    }
    else {
        timelineHttp.setHost(url.host(), url.port(80));
    }

	timelineHttp.setUser(username, password);

	buffer.open(QIODevice::WriteOnly);

	if (type == 6) {
		timelineHttp.get(urls[type] + "&rpp=" + QString::number(count), &buffer);

		emit stateChanged(tr("Updating timeline: %1").arg("http://search.twitter.com" + urls[type] + "&rpp=" + QString::number(count)));
	} else {
		timelineHttp.get(url.path() + "?count=" + QString::number(count) + (lastStatusId ? "&since_id=" + QString::number(lastStatusId) : ""), &buffer);

		emit stateChanged(tr("Updating timeline: %1").arg(serviceAPIURL + urls[type]));
	}
}

void Twitter::statusHttpDone(bool error) {
	if (error) {
		emit stateChanged(tr("Error sending status: %1").arg(statusHttp.errorString()));
		return;
	}
	emit stateChanged(tr("Status sent: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
	emit statusUpdated();
}

void Twitter::timelineHttpDone(bool error) {
	if (error) {
		emit stateChanged(tr("Error while updating timeline: %1").arg(timelineHttp.errorString()));
		return;
	}
	buffer.close();
	emit stateChanged(tr("Timeline updated: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
	emit updated(buffer.data(), currentType);
}

void Twitter::httpsError(const QList<QSslError> & errors) {
    /*
     * maybe here should be implemtend some error handling
     * to just ignore all ssl errors (not recommended):
     *  timelineHttp.ignoreSslErrors();
     */
    emit stateChanged(tr("SSL Error while updating/sending: %1").arg(errors[0].errorString()));
}


void Twitter::abort() {
	timelineHttp.abort();
	statusHttp.abort();
}

void Twitter::setUrl(int index, const QString &url) {
	urls[index] = url;
	emit stateChanged(url);
}

#endif
