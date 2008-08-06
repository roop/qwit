#ifndef MainWindow_cpp
#define MainWindow_cpp

#include <iostream>

#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent): QDialog(parent) {
	setupUi(this);
	
	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("twitterWidget"));
	
	scrollArea = new QScrollArea();
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	vboxLayout->insertWidget(1, scrollArea);
	
	statusTextEdit = new StatusTextEdit(this);
	statusTextEdit->setObjectName(QString::fromUtf8("statusTextEdit"));
	statusTextEdit->setFixedHeight(40);
	QFont font = statusTextEdit->document()->defaultFont();
	font.setFamily("Verdana");
	statusTextEdit->document()->setDefaultFont(font);
	
	charsLeftLabel->setText(QString::number(statusTextEdit->getMaxStatusCharacter()));
	charsLeftLabel->setForegroundRole(QPalette::Light);

	vboxLayout->insertWidget(1, statusTextEdit);
	
	optionsDialog = new OptionsDialog(this);
	optionsDialog->setModal(true);
	
	lastId = 0;
	acceptClose = false;
	
	connect(statusTextEdit, SIGNAL(returnPressed()), this, SLOT(sendStatus()));
	connect(optionsDialog->savePushButton, SIGNAL(pressed()), this, SLOT(saveSettings()));
	connect(optionsDialog->resetPushButton, SIGNAL(pressed()), this, SLOT(resetSettings()));
	
	connect(refreshPushButton, SIGNAL(pressed()), this, SLOT(updateHome()));
	connect(optionsPushButton, SIGNAL(pressed()), optionsDialog, SLOT(showNormal()));
	
	connect(&twitter, SIGNAL(homeUpdated(const QByteArray&)), this, SLOT(homeUpdated(const QByteArray&)));
	connect(&twitter, SIGNAL(statusUpdated()), this, SLOT(statusUpdated()));
	
	connect(twitterWidget, SIGNAL(reply(const QString &)), statusTextEdit, SLOT(reply(const QString &)));
	
	connect(statusTextEdit, SIGNAL(leftCharsNumberChanged(int)), this, SLOT(leftCharsNumberChanged(int)));
	
	setupTrayIcon();
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
	
void MainWindow::loadSettings() {
	QSettings settings("arti", "qwit");
	
	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(200, 600)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();
	settings.beginGroup("Twitter");
	username = settings.value("username", "").toString();
	bool savePassword = settings.value("savePassword", false).toBool();
	password = settings.value("password", "").toString();
	interval = settings.value("interval", "300000").toInt();
	settings.endGroup();
	settings.beginGroup("Proxy");
	useProxy = settings.value("useProxy", "").toBool();
	proxyAddress = settings.value("proxyAddress", "").toString();
	proxyPort = settings.value("proxyPort", "").toInt();
	proxyUsername = settings.value("proxyUsername", "").toString();
	bool proxySavePassword = settings.value("proxySavePassword", false).toBool();
	proxyPassword = settings.value("proxyPassword", "").toString();
	settings.endGroup();
	
	optionsDialog->usernameLineEdit->setText(username);
	optionsDialog->passwordLineEdit->setText(password);
	optionsDialog->savePasswordCheckBox->setCheckState(savePassword ? Qt::Checked : Qt::Unchecked);
	optionsDialog->intervalLineEdit->setText(QString::number(interval));
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
}
	
void MainWindow::sendStatus() {
	QString status = statusTextEdit->toPlainText().simplified();
	if (status == "") {
		return;
	}
	twitter.sendStatus(username, password, status);
	statusTextEdit->setText("");
	charsLeftLabel->setText(QString::number(statusTextEdit->getMaxStatusCharacter()));
	charsLeftLabel->setForegroundRole(QPalette::Light);
	twitterWidget->setFocus(Qt::OtherFocusReason);
}
	
void MainWindow::updateHome() {
	if (password == "") {
		return;
	}
	twitter.updateHome(username, password, lastId);
}
	
void MainWindow::saveSettings() {
	optionsDialog->hide();
	
	QSettings settings("arti", "qwit");
	
	username = optionsDialog->usernameLineEdit->text();
	password = optionsDialog->passwordLineEdit->text();
	bool savePassword = optionsDialog->savePasswordCheckBox->checkState() == Qt::Checked;
	interval = optionsDialog->intervalLineEdit->text().toInt();
	
	useProxy = optionsDialog->useProxyCheckBox->checkState() == Qt::Checked;
	proxyAddress = optionsDialog->proxyAddressLineEdit->text();
	proxyPort = optionsDialog->proxyPortLineEdit->text().toInt();
	proxyUsername = optionsDialog->proxyUsernameLineEdit->text();
	proxyPassword = optionsDialog->proxyPasswordLineEdit->text();
	bool proxySavePassword = optionsDialog->proxySavePasswordCheckBox->checkState() == Qt::Checked;
	
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
	updateHome();
}
	
void MainWindow::resetSettings() {
	optionsDialog->usernameLineEdit->setText(username);
	optionsDialog->passwordLineEdit->setText(password);
	optionsDialog->intervalLineEdit->setText(QString::number(interval));
}
	
QString MainWindow::formatDateTime(const QDateTime &time) {
	int seconds = time.secsTo(QDateTime::currentDateTime());
	if (seconds < 15) return "Just now";
	if (seconds < 45) return "about " + QString::number(seconds) + " second" + (seconds == 1 ? "" : "s") + " ago";
	int minutes = seconds / 60 + (seconds % 60 >= 30);
	if (minutes < 30) return "about " + QString::number(minutes) + " minute" + (minutes == 1 ? "" : "s") + " ago";
	int hours = seconds / 3600 + (seconds % 3600 >= 1800);
	if (hours < 24) return "about " + QString::number(hours) + " hour" + (hours == 1 ? "" : "s") + " ago";
	int days = seconds / (24 * 3600) + (seconds % (24 * 3600) >= 12 * 3600);
	return "about " + QString::number(days) + " day" + (days == 1 ? "" : "s") + " ago";
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
	QSettings settings("arti", "qwit");
	if (acceptClose) {
		twitter.abort();
		settings.beginGroup("MainWindow");
		settings.setValue("size", size());
		settings.setValue("pos", pos());
		settings.endGroup();
		event->accept();
	} else {
		hide();
		event->ignore();
	}
}

void MainWindow::hideEvent(QHideEvent *event) {
	QSettings settings("arti", "qwit");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
	event->accept();
}

void MainWindow::showEvent(QShowEvent *event) {
	static bool firstTime = true;
	if (firstTime) {
		loadSettings();
		updateHome();
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateHome()));
		timer->start(interval);
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
	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
	event->accept();
}

void MainWindow::homeUpdated(const QByteArray &buffer) {
//	twitterWidget->clear();
	
	QDomDocument document;
	
	document.setContent(buffer);
	
	QDomElement root = document.documentElement();
	
	QDomNode node = root.firstChild();
	QString html = "";
	QString trayMessage = "";
	int maxId = lastId;
	int j = 0;
	while (!node.isNull()) {
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
		if (id > maxId) maxId = id;
		QDateTime time = QDateTime::fromString(timeStr, "ddd MMM dd hh:mm:ss +0000 yyyy");
		time = QDateTime(time.date(), time.time(), Qt::UTC);
		if (id > lastId) {
			trayMessage += user + ": " + message + " " + formatDateTime(time.toLocalTime()) + "\n";
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
		
		twitterWidget->addItem(imageFileName, user, message.simplified(), formatDateTime(time.toLocalTime()), id, replyStatusId, j++);
		node = node.nextSibling();
	}
	lastId = maxId;
	if (trayMessage != "") {
		trayIcon->showMessage("Qwit updates", trayMessage);
	}
}

void MainWindow::statusUpdated() {
	updateHome();
}

void MainWindow::leftCharsNumberChanged(int count) {
	if (count >= 0) charsLeftLabel->setForegroundRole(QPalette::Light);
	else charsLeftLabel->setForegroundRole(QPalette::Dark);
	charsLeftLabel->setText(QString::number(count));
}

void MainWindow::showhide() {
	if (isVisible()) {
		hide();
	} else {
		show();
	}
}

#endif
