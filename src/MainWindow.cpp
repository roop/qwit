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

#ifndef MainWindow_cpp
#define MainWindow_cpp

#include "MainWindow.h"

#include "QwitException.h"

#include <iostream>

using namespace std;

MainWindow* MainWindow::instance;

MainWindow* MainWindow::getInstance() {
	if (!instance) instance = new MainWindow();
	return instance;
}

MainWindow::MainWindow(QWidget *parent): QDialog(parent) {
	setupUi(this);

	monthes["Jan"] = 1;
	monthes["Feb"] = 2;
	monthes["Mar"] = 3;
	monthes["Apr"] = 4;
	monthes["May"] = 5;
	monthes["Jun"] = 6;
	monthes["Jul"] = 7;
	monthes["Aug"] = 8;
	monthes["Sep"] = 9;
	monthes["Oct"] = 10;
	monthes["Nov"] = 11;
	monthes["Dec"] = 12;

	TwitterWidget *twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("homeTwitterWidget"));
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	QGridLayout *gridLayout = new QGridLayout(homeTab);
	gridLayout->setObjectName(QString::fromUtf8("homeGridLayout"));

	QScrollArea *scrollArea = new QScrollArea(homeTab);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	twitterTabs[HOME_TWITTER_TAB] = TwitterTab(scrollArea, twitterWidget, 0);


	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("publicTwitterWidget"));
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	gridLayout = new QGridLayout(publicTab);
	gridLayout->setObjectName(QString::fromUtf8("publicGridLayout"));

	scrollArea = new QScrollArea(publicTab);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	twitterTabs[PUBLIC_TWITTER_TAB] = TwitterTab(scrollArea, twitterWidget, 0);


	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("repliesTwitterWidget"));
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	gridLayout = new QGridLayout(repliesTab);
	gridLayout->setObjectName(QString::fromUtf8("repliesGridLayout"));

	scrollArea = new QScrollArea(repliesTab);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	twitterTabs[REPLIES_TWITTER_TAB] = TwitterTab(scrollArea, twitterWidget, 0);


	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("customTwitterWidget"));
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	scrollArea = new QScrollArea(customTab);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	vboxLayout1->addWidget(scrollArea);

	twitterTabs[CUSTOM_TWITTER_TAB] = TwitterTab(scrollArea, twitterWidget, 0);

	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("inboxTwitterWidget"));
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	gridLayout = new QGridLayout(inboxTab);
	gridLayout->setObjectName(QString::fromUtf8("inboxGridLayout"));

	scrollArea = new QScrollArea(inboxTab);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	twitterTabs[INBOX_TWITTER_TAB] = TwitterTab(scrollArea, twitterWidget, 0);

	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("outboxTwitterWidget"));
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	gridLayout = new QGridLayout(outboxTab);
	gridLayout->setObjectName(QString::fromUtf8("outboxGridLayout"));

	scrollArea = new QScrollArea(outboxTab);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	twitterTabs[OUTBOX_TWITTER_TAB] = TwitterTab(scrollArea, twitterWidget, 0);

	statusTextEdit = new StatusTextEdit(this);
	statusTextEdit->setObjectName(QString::fromUtf8("statusTextEdit"));
	QFont font = statusTextEdit->document()->defaultFont();
	font.setFamily("Verdana");
	statusTextEdit->document()->setDefaultFont(font);

	charsLeftLabel->setText(QString::number(statusTextEdit->getMaxStatusCharacter()));
	charsLeftLabel->setForegroundRole(QPalette::Light);

	vboxLayout->insertWidget(1, statusTextEdit);

	optionsDialog = new OptionsDialog(this);
	optionsDialog->setModal(true);

	logsDialog = new LogsDialog(this);

	acceptClose = false;

	connect(statusTextEdit, SIGNAL(returnPressed()), this, SLOT(sendStatus()));
	connect(customUsernameLineEdit, SIGNAL(returnPressed()), this, SLOT(customUsernameChanged()));

	connect(refreshPushButton, SIGNAL(pressed()), this, SLOT(updateTimeline()));
	connect(optionsPushButton, SIGNAL(pressed()), optionsDialog, SLOT(showNormal()));
	connect(logsPushButton, SIGNAL(pressed()), logsDialog, SLOT(showNormal()));
	connect(exitPushButton, SIGNAL(pressed()), this, SLOT(quit()));

	connect(&twitter, SIGNAL(updated(const QByteArray&, int)), this, SLOT(updated(const QByteArray&, int)));
	connect(&twitter, SIGNAL(statusUpdated()), this, SLOT(statusUpdated()));

	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	for (int i = 0; i < TWITTER_TABS; ++i) {
		connect(twitterTabs[i].twitterWidget, SIGNAL(reply(const QString &)), statusTextEdit, SLOT(reply(const QString &)));
                connect(twitterTabs[i].twitterWidget, SIGNAL(replyID(const QString &)), this, SLOT(setReplyID(const QString &)));
//-----
        connect(twitterTabs[i].twitterWidget, SIGNAL(directMessages(const QString &)), statusTextEdit, SLOT(directMessages(const QString &)));
//   		connect(twitterTabs[i].twitterWidget, SIGNAL(reply(const QString &)), statusTextEdit, SLOT(reply(const QString &)));
                connect(twitterTabs[i].twitterWidget, SIGNAL(directMessagesID(const QString &)), this, SLOT(setDirectMessagesID(const QString &)));
	}

	connect(statusTextEdit, SIGNAL(leftCharsNumberChanged(int)), this, SLOT(leftCharsNumberChanged(int)));

	connect(&twitter, SIGNAL(stateChanged(const QString&)), this, SLOT(updateState(const QString&)));
	connect(&userpicsDownloader, SIGNAL(stateChanged(const QString&)), this, SLOT(updateState(const QString&)));

	for (int i = 0; i < TWITTER_TABS; ++i) {
		connect(&userpicsDownloader, SIGNAL(userpicDownloaded(const QString&)), twitterTabs[i].twitterWidget, SLOT(reloadUserpic(const QString&)));
	}

	setupTrayIcon();

	timer2 = new QTimer(this);
	connect(timer2, SIGNAL(timeout()), this, SLOT(updateItems()));
	timer2->start(10000);
}

void MainWindow::setupTrayIcon() {
	showhideAction = new QAction(tr("&Show / Hide"), this);
	connect(showhideAction, SIGNAL(triggered()), this, SLOT(showhide()));
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(showhideAction);
	trayIconMenu->addAction(quitAction);
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(QIcon(":/images/qwit.png"));
	trayIcon->show();
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::loadState() {
	QSettings settings("arti", "qwit");

	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)).toSize());
	move(settings.value("pos", QPoint(DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y)).toPoint());
	settings.endGroup();
	settings.beginGroup("Twitter");
	username = settings.value("username", "").toString();
	bool savePassword = settings.value("savePassword", false).toBool();
	password = settings.value("password", "").toString();
	interval = settings.value("interval", DEFAULT_UPDATE_INTERVAL).toInt();
	messagesPerPage = settings.value("messagesPerPage", DEFAULT_MESSAGES_PER_PAGE).toInt();
	messagesPerTray = settings.value("messagesPerTray", DEFAULT_MESSAGES_PER_TRAY).toInt();
	updatesNotification = settings.value("updatesNotification", true).toBool();
	usernameUnderAvatar = settings.value("usernameUnderAvatar", true).toBool();
	twitter.setServiceAPIURL(settings.value("serviceAPIURL", "http://twitter.com").toString());
	twitter.setServiceBaseURL(settings.value("serviceBaseURL", "http://twitter.com").toString());
	greetingMessageLabel->setText(settings.value("greetingMessage", "What are you doing?").toString());
	settings.endGroup();
	settings.beginGroup("Proxy");
	useProxy = settings.value("useProxy", "").toBool();
	proxyAddress = settings.value("proxyAddress", "").toString();
	proxyPort = settings.value("proxyPort", "").toInt();
	proxyUsername = settings.value("proxyUsername", "").toString();
	bool proxySavePassword = settings.value("proxySavePassword", false).toBool();
	proxyPassword = settings.value("proxyPassword", "").toString();
	settings.endGroup();
	settings.beginGroup("Other");
	customUsernameLineEdit->setText(settings.value("customUsername").toString());
	settings.endGroup();

	twitter.setUrl(CUSTOM_TWITTER_TAB, QString(CUSTOM_XML_URL) + customUsernameLineEdit->text() + ".xml");

	optionsDialog->messagesPerPageLineEdit->setText(QString::number(messagesPerPage));
	optionsDialog->messagesPerTrayLineEdit->setText(QString::number(messagesPerTray));
	optionsDialog->updatesNotificationCheckBox->setCheckState(updatesNotification ? Qt::Checked : Qt::Unchecked);
	optionsDialog->usernameUnderAvatarCheckBox->setCheckState(usernameUnderAvatar ? Qt::Checked : Qt::Unchecked);
	optionsDialog->serviceBaseURLLineEdit->setText(twitter.getServiceBaseURL());
	optionsDialog->serviceAPIURLLineEdit->setText(twitter.getServiceAPIURL());
	optionsDialog->usernameLineEdit->setText(username);
	optionsDialog->passwordLineEdit->setText(password);
	optionsDialog->savePasswordCheckBox->setCheckState(savePassword ? Qt::Checked : Qt::Unchecked);
	optionsDialog->intervalLineEdit->setText(QString::number(interval));
	optionsDialog->greetingMessageLineEdit->setText(greetingMessageLabel->text());
	optionsDialog->useProxyCheckBox->setCheckState(useProxy ? Qt::Checked : Qt::Unchecked);
	optionsDialog->proxyAddressLineEdit->setText(proxyAddress);
	optionsDialog->proxyPortLineEdit->setText(QString::number(proxyPort));
	optionsDialog->proxyUsernameLineEdit->setText(proxyUsername);
	optionsDialog->proxyPasswordLineEdit->setText(proxyPassword);
	optionsDialog->proxySavePasswordCheckBox->setCheckState(proxySavePassword ? Qt::Checked : Qt::Unchecked);

	if (useProxy) {
		twitter.useProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
		userpicsDownloader.useProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		twitter.dontUseProxy();
		userpicsDownloader.dontUseProxy();
	}

	if ((username == "") || (password == "")) {
		statusTextEdit->setDisabled(true);
	}

	for (int tab = 0; tab < TWITTER_TABS; ++tab) {
		twitterTabs[tab].twitterWidget->setMessagesPerPage(messagesPerPage);
		twitterTabs[tab].twitterWidget->setUsernameUnderAvatar(usernameUnderAvatar);
	}
	settings.beginGroup("Twits");
	for (int tab = 0; tab < TWITTER_TABS; ++tab) {
		int size = settings.beginReadArray("Twits" + QString::number(tab));
		for (int i = 0; i < size; ++i) {
			settings.setArrayIndex(i);
			if (i == 0) {
				twitterTabs[tab].lastId = settings.value("messageId").toInt();
			}
			twitterTabs[tab].twitterWidget->addItem(
				settings.value("userpic").toString(),
				settings.value("username").toString(),
				settings.value("status").toString(),
				QDateTime::fromString(settings.value("time").toString(), "yyyy-MM-dd hh:mm:ss"),
				settings.value("messageId").toInt(),
				settings.value("replyStatusId").toInt(),
				-1,
				twitter.getServiceBaseURL()
			);
		}
		settings.endArray();
	}
	settings.endGroup();
}

void MainWindow::setReplyID(const QString &replyID) {
        replyStatusID = replyID;
        isReply = true;
}

void MainWindow::setDirectMessagesID(const QString &replyID) {
        replyStatusID = replyID;
        isReply = true;
}

void MainWindow::sendStatus() {
	QString status = statusTextEdit->toPlainText().simplified();
	if (status == "") {
		return;
	}
        if (isReply) {
                twitter.sendStatus(username, password, status, replyStatusID);
                isReply = false;
                replyStatusID = "";
        }
        else twitter.sendStatus(username, password, status, "");
	statusTextEdit->setText("");
	charsLeftLabel->setText(QString::number(statusTextEdit->getMaxStatusCharacter()));
	charsLeftLabel->setForegroundRole(QPalette::Light);
	twitterTabs[tabWidget->currentIndex()].twitterWidget->setFocus(Qt::OtherFocusReason);
}

void MainWindow::updateTimeline() {
	if (password == "") {
		optionsDialog->showNormal();
		return;
	}
	twitter.update(username, password, twitterTabs[tabWidget->currentIndex()].lastId, tabWidget->currentIndex(), messagesPerPage);
}

void MainWindow::saveState() {
	QSettings settings("arti", "qwit");

	for (int tab = 0; tab < TWITTER_TABS; ++tab) {
		twitterTabs[tab].twitterWidget->setMessagesPerPage(messagesPerPage);
		twitterTabs[tab].twitterWidget->setUsernameUnderAvatar(usernameUnderAvatar);
	}

	username = optionsDialog->usernameLineEdit->text();
	password = optionsDialog->passwordLineEdit->text();
	bool savePassword = optionsDialog->savePasswordCheckBox->checkState() == Qt::Checked;
	interval = optionsDialog->intervalLineEdit->text().toInt();
	twitter.setServiceBaseURL(optionsDialog->serviceBaseURLLineEdit->text());
	twitter.setServiceAPIURL(optionsDialog->serviceAPIURLLineEdit->text());

	useProxy = optionsDialog->useProxyCheckBox->checkState() == Qt::Checked;
	proxyAddress = optionsDialog->proxyAddressLineEdit->text();
	proxyPort = optionsDialog->proxyPortLineEdit->text().toInt();
	proxyUsername = optionsDialog->proxyUsernameLineEdit->text();
	proxyPassword = optionsDialog->proxyPasswordLineEdit->text();
	messagesPerPage = optionsDialog->messagesPerPageLineEdit->text().toInt();
	messagesPerTray = optionsDialog->messagesPerTrayLineEdit->text().toInt();
	bool proxySavePassword = optionsDialog->proxySavePasswordCheckBox->checkState() == Qt::Checked;
	updatesNotification = optionsDialog->updatesNotificationCheckBox->checkState() == Qt::Checked;
	usernameUnderAvatar = optionsDialog->usernameUnderAvatarCheckBox->checkState() == Qt::Checked;
	greetingMessageLabel->setText(optionsDialog->greetingMessageLineEdit->text());

	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
	settings.beginGroup("Twitter");
	settings.setValue("username", username);
	settings.setValue("savePassword", savePassword);
	if (savePassword) {
		settings.setValue("password", password);
	} else {
		settings.setValue("password", "");
	}
	settings.setValue("interval", interval);
	settings.setValue("messagesPerPage", messagesPerPage);
	settings.setValue("messagesPerTray", messagesPerTray);
	settings.setValue("updatesNotification", updatesNotification);
	settings.setValue("usernameUnderAvatar", usernameUnderAvatar);
	settings.setValue("serviceBaseURL", twitter.getServiceBaseURL());
	settings.setValue("serviceAPIURL", twitter.getServiceAPIURL());
	settings.setValue("greetingMessage", greetingMessageLabel->text());
	settings.endGroup();
	settings.beginGroup("Proxy");
	settings.setValue("useProxy", useProxy);
	settings.setValue("proxyAddress", proxyAddress);
	settings.setValue("proxyPort", proxyPort);
	settings.setValue("proxyUsername", proxyUsername);
	settings.setValue("proxySavePassword", proxySavePassword);
	if (proxySavePassword) {
		settings.setValue("proxyPassword", proxyPassword);
	} else {
		settings.setValue("proxyPassword", "");
	}
	settings.endGroup();

	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();

	settings.beginGroup("Twits");
	for (int tab = 0; tab < TWITTER_TABS; ++tab) {
		settings.beginWriteArray("Twits" + QString::number(tab));
		for (int i = 0; i < twitterTabs[tab].twitterWidget->getItemsCount(); ++i) {
			try {
				const TwitterWidgetItem &item = twitterTabs[tab].twitterWidget->getItem(i);
				settings.setArrayIndex(i);
				settings.setValue("userpic", item.cacheUserpic);
				settings.setValue("username", item.cacheUsername);
				settings.setValue("status", item.cacheStatus);
				settings.setValue("time", item.cacheTime.toString("yyyy-MM-dd hh:mm:ss"));
				settings.setValue("messageId", item.cacheMessageId);
				settings.setValue("replyStatusId", item.cacheReplyStatusId);
			} catch (const QwitException &exception) {
				cerr << "Exception catched: " << qPrintable(exception.message) << endl;
			}
		}
		settings.endArray();
	}
	settings.endGroup();

	settings.beginGroup("Other");
	settings.setValue("customUsername", customUsernameLineEdit->text());
	settings.endGroup();

	if (useProxy) {
		twitter.useProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
		userpicsDownloader.useProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		twitter.dontUseProxy();
		userpicsDownloader.dontUseProxy();
	}

	if ((username == "") || (password == "")) {
		statusTextEdit->setDisabled(true);
	} else {
		statusTextEdit->setDisabled(false);
	}
}

void MainWindow::resetSettings() {
	optionsDialog->usernameLineEdit->setText(username);
	optionsDialog->passwordLineEdit->setText(password);
	optionsDialog->intervalLineEdit->setText(QString::number(interval));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		showhide();
	}
}

void MainWindow::quit() {
	acceptClose = true;
	close();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (acceptClose) {
		logsDialog->close();
		twitter.abort();
		saveState();
		event->accept();
	} else {
		hide();
		event->ignore();
	}
}

void MainWindow::hideEvent(QHideEvent *event) {
	saveState();
	event->accept();
}

void MainWindow::showEvent(QShowEvent *event) {
	static bool firstTime = true;
	if (firstTime) {
		loadState();
		updateTimeline();
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeline()));
		timer->start(interval * 1000);
		firstTime = false;
	}

	QSettings settings("arti", "qwit");
	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(200, 600)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();

	statusTextEdit->setFocus(Qt::OtherFocusReason);

	event->accept();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	for (int i = 0; i < TWITTER_TABS; ++i) {
		twitterTabs[i].twitterWidget->resize(twitterTabs[i].scrollArea->width() - twitterTabs[i].scrollArea->verticalScrollBar()->width() - 5, 500);
	}
	event->accept();
}

QDateTime MainWindow::dateFromString(const QString &date) {
	char s[10];
	int year, day, hours, minutes, seconds;
	sscanf(qPrintable(date), "%*s %s %d %d:%d:%d %*s %d", s, &day, &hours, &minutes, &seconds, &year);
	int month = monthes[s];
	return QDateTime(QDate(year, month, day), QTime(hours, minutes, seconds));
}

void MainWindow::updated(const QByteArray &buffer, int type) {
	QDomDocument document;

	document.setContent(buffer);

	QDomElement root = document.documentElement();
	if (root.tagName() == "statuses") {
	QDomNode node = root.firstChild();
	QString html = "";
	QString trayMessage = "";
	int maxId = twitterTabs[type].lastId;
	int j = 0;
	while (!node.isNull()) {
		if (node.toElement().tagName() != "status") {
			return;
		}
		QDomNode node2 = node.firstChild();
		QString message = "", timeStr = "", user = "", image = "";
		int id = 0, replyUserID = 0, replyStatusId = 0;
		while (!node2.isNull()) {
			if (node2.toElement().tagName() == "created_at") {
				timeStr = node2.toElement().text();
			} else if (node2.toElement().tagName() == "text") {
				message = node2.toElement().text();
			} else if (node2.toElement().tagName() == "id") {
				id = node2.toElement().text().toInt();
			} else if (node2.toElement().tagName() == "in_reply_to_status_id") {
				replyStatusId = node2.toElement().text().toInt();
			} else if (node2.toElement().tagName() == "in_reply_to_user_id") {
				replyUserID = node2.toElement().text().toInt();
			} else if (node2.toElement().tagName() == "user") {
				QDomNode node3 = node2.firstChild();
				while (!node3.isNull()) {
					if (node3.toElement().tagName() == "screen_name") {
						user = node3.toElement().text();
					} else if (node3.toElement().tagName() == "profile_image_url") {
						image = node3.toElement().text();
					}
					node3 = node3.nextSibling();
				}
			}
			node2 = node2.nextSibling();
		}
		if (id) {
			if (id > maxId) maxId = id;
			QDateTime time = dateFromString(timeStr);
			time = QDateTime(time.date(), time.time(), Qt::UTC);
			if ((id > twitterTabs[type].lastId)  && j < messagesPerTray) {
				trayMessage += user + ": " + message + " /" + TwitterWidget::formatDateTime(time.toLocalTime()) + "\n----------------------------\n";
			}
			QByteArray hash = QCryptographicHash::hash(image.toAscii(), QCryptographicHash::Md5);
			QString imageFileName = "";
			for (int i = 0; i < hash.size(); ++i) {
				unsigned char c = hash[i];
				c >>= 4;
				if (c < 10) {
					c += '0';
				} else {
					c += 'A' - 10;
				}
				imageFileName += (char)c;
				c = hash[i];
				c &= 15;
				if (c < 10) {
					c += '0';
				} else {
					c += 'A' - 10;
				}
				imageFileName += (char)c;
			}
			imageFileName += "." + QFileInfo(QUrl(image).path()).suffix();
			QDir dir(QDir::homePath());
			dir.mkdir(".qwit");
			imageFileName = dir.absolutePath() + "/.qwit/" + imageFileName;
			userpicsDownloader.download(image, imageFileName);
			twitterTabs[type].twitterWidget->addItem(imageFileName, user, message.simplified(), time.toLocalTime(), id, replyStatusId, j++, twitter.getServiceBaseURL());
		}
		node = node.nextSibling();
	}
	twitterTabs[type].lastId = maxId;
	twitterTabs[type].lastUpdateTime = QDateTime::currentDateTime().toTime_t();
	if ((trayMessage != "") && updatesNotification) {
		trayIcon->showMessage(tr("Qwit updates"), trayMessage);
	}
	saveState();
	return;
	}

//inbox DirectMessages
	if (root.tagName() == "direct-messages") {
	QDomNode node = root.firstChild();
	QString html = "";
	QString trayMessage = "";
	int maxId = twitterTabs[type].lastId;
	int j = 0;
	while (!node.isNull()) {
		if (node.toElement().tagName() != "direct_message") {
			return;
		}
		QDomNode node2 = node.firstChild();
		QString message = "", timeStr = "", user = "", image = "", imageRecipient = "",recipientUser = "";
		int id = 0, replyUserID = 0, replyStatusId = 0;
		while (!node2.isNull()) {
			if (node2.toElement().tagName() == "created_at") {
				timeStr = node2.toElement().text();
			} else if (node2.toElement().tagName() == "text") {
				message = node2.toElement().text();
			} else if (node2.toElement().tagName() == "id") {
				id = node2.toElement().text().toInt();
			} else if (node2.toElement().tagName() == "sender_screen_name") {
				user = node2.toElement().text();
			} else if (node2.toElement().tagName() == "recipient_screen_name") {
				recipientUser = node2.toElement().text();
			} else if (node2.toElement().tagName() == "sender") {
				QDomNode node3 = node2.firstChild();
				while (!node3.isNull()) {
					if (node3.toElement().tagName() == "profile_image_url") {
						image = node3.toElement().text();
					}
					node3 = node3.nextSibling();
				}
			} else if (node2.toElement().tagName() == "recipient") {
				QDomNode node3 = node2.firstChild();
				while (!node3.isNull()) {
					if (node3.toElement().tagName() == "profile_image_url") {
						imageRecipient = node3.toElement().text();
					}
					node3 = node3.nextSibling();
				}
			}
			node2 = node2.nextSibling();
		}
		if (id) {
			if (id > maxId) maxId = id;
			if (type==5) {
				user=recipientUser;
				image=imageRecipient;
			}
			QDateTime time = dateFromString(timeStr);
			time = QDateTime(time.date(), time.time(), Qt::UTC);
			if ((id > twitterTabs[type].lastId)  && j < messagesPerTray) {
				trayMessage += user + ": " + message + " /" + TwitterWidget::formatDateTime(time.toLocalTime()) + "\n----------------------------\n";
			}
			QByteArray hash = QCryptographicHash::hash(image.toAscii(), QCryptographicHash::Md5);
			QString imageFileName = "";
			for (int i = 0; i < hash.size(); ++i) {
				unsigned char c = hash[i];
				c >>= 4;
				if (c < 10) {
					c += '0';
				} else {
					c += 'A' - 10;
				}
				imageFileName += (char)c;
				c = hash[i];
				c &= 15;
				if (c < 10) {
					c += '0';
				} else {
					c += 'A' - 10;
				}
				imageFileName += (char)c;
			}
			imageFileName += "." + QFileInfo(QUrl(image).path()).suffix();
			QDir dir(QDir::homePath());
			dir.mkdir(".qwit");
			imageFileName = dir.absolutePath() + "/.qwit/" + imageFileName;
			userpicsDownloader.download(image, imageFileName);
//			twitterTabs[type].twitterWidget->addItem(imageFileName, user, message.simplified(), time.toLocalTime(), id, replyStatusId, j++, twitter.getServiceBaseURL());
			twitterTabs[type].twitterWidget->addItem(imageFileName, user, message.simplified(), time.toLocalTime(), id, id, j++, twitter.getServiceBaseURL());
		}
		node = node.nextSibling();
	}
	twitterTabs[type].lastId = maxId;
	twitterTabs[type].lastUpdateTime = QDateTime::currentDateTime().toTime_t();
	if ((trayMessage != "") && updatesNotification) {
		trayIcon->showMessage(tr("Qwit updates"), trayMessage);
	}
	saveState();
//	return;
	}

}

void MainWindow::statusUpdated() {
	updateTimeline();
}

void MainWindow::leftCharsNumberChanged(int count) {
	if (count >= 0) charsLeftLabel->setForegroundRole(QPalette::Light);
	else charsLeftLabel->setForegroundRole(QPalette::Dark);
	charsLeftLabel->setText(QString::number(count));
}

void MainWindow::showhide() {
	if (isVisible()) {
		logsDialog->hide();
		hide();
	} else {
		show();
		for (int i = 0; i < TWITTER_TABS; ++i) {
			twitterTabs[i].twitterWidget->updateItems();
		}
	}
}

void MainWindow::updateState(const QString &state) {
	stateLabel->setText(state);
	stateLabel->setToolTip(state);
	logsDialog->logsTextEdit->setPlainText(logsDialog->logsTextEdit->toPlainText() + QDateTime::currentDateTime().toString("hh:mm:ss") + " - " + state + "\n");
}

void MainWindow::tabChanged(int index) {
	if (index < TWITTER_TABS) {
		int height = twitterTabs[index].scrollArea->width() - twitterTabs[index].scrollArea->verticalScrollBar()->width() - 5;
		if (twitterTabs[index].twitterWidget->height() != height) {
			twitterTabs[index].twitterWidget->resize(height, 500);
		}
		int time = QDateTime::currentDateTime().toTime_t();
		if (twitterTabs[index].lastUpdateTime + interval <= time) {
			twitter.update(username, password, twitterTabs[index].lastId, index, messagesPerPage);
		}
	}
}

void MainWindow::updateItems() {
	for (int i = 0; i < TWITTER_TABS; ++i) {
		twitterTabs[i].twitterWidget->updateItems();
	}
}

void MainWindow::customUsernameChanged() {
	twitter.setUrl(CUSTOM_TWITTER_TAB, QString(CUSTOM_XML_URL) + customUsernameLineEdit->text() + ".xml");

	twitterTabs[CUSTOM_TWITTER_TAB].twitterWidget->clear();
	twitterTabs[CUSTOM_TWITTER_TAB].lastId = 0;

	saveState();

	updateTimeline();
}

#endif
