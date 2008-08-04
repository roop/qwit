#ifndef MainWindow_cpp
#define MainWindow_cpp

#include <iostream>

#include "MainWindow.h"

#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent): QDialog(parent) {
	setupUi(this);
	
	twitterWidget = new TwitterWidget();
	twitterWidget->setObjectName(QString::fromUtf8("twitterWidget"));
	
	scrollArea = new QScrollArea();
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	vboxLayout->insertWidget(0, scrollArea);
	
	statusLineEdit = new StatusLineEdit(this);
	statusLineEdit->setObjectName(QString::fromUtf8("statusLineEdit"));
	
	vboxLayout->insertWidget(0, statusLineEdit);
	
	lastId = 0;
	acceptClose = false;
	
	connect(statusLineEdit, SIGNAL(returnPressed()), this, SLOT(sendStatus()));
	connect(savePushButton, SIGNAL(pressed()), this, SLOT(saveSettings()));
	connect(resetPushButton, SIGNAL(pressed()), this, SLOT(resetSettings()));
	
	connect(refreshPushButton, SIGNAL(pressed()), this, SLOT(updateHome()));
	
	connect(&twitter, SIGNAL(homeUpdated(const QByteArray&)), this, SLOT(homeUpdated(const QByteArray&)));
	connect(&twitter, SIGNAL(statusUpdated()), this, SLOT(statusUpdated()));
	
	setupTrayIcon();
	
	statusLineEdit->setText("What are you doing?");
}

void MainWindow::setupTrayIcon() {
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
	trayIconMenu = new QMenu(this);
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
	
	usernameLineEdit->setText(username);
	passwordLineEdit->setText(password);
	savePasswordCheckBox->setCheckState(savePassword ? Qt::Checked : Qt::Unchecked);
	intervalLineEdit->setText(QString::number(interval));
	useProxyCheckBox->setCheckState(useProxy ? Qt::Checked : Qt::Unchecked);
	proxyAddressLineEdit->setText(proxyAddress);
	proxyPortLineEdit->setText(QString::number(proxyPort));
	proxyUsernameLineEdit->setText(proxyUsername);
	proxyPasswordLineEdit->setText(proxyPassword);
	proxySavePasswordCheckBox->setCheckState(proxySavePassword ? Qt::Checked : Qt::Unchecked);
	
	if (useProxy) {
		twitter.useProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		twitter.dontUseProxy();
	}
	
	if ((username == "") || (password == "")) {
		statusLineEdit->setDisabled(true);
	}
}
	
void MainWindow::sendStatus() {
	twitter.sendStatus(username, password, statusLineEdit->text());
	statusLineEdit->setText(WHAT_ARE_YOU_DOING);
	twitterWidget->setFocus(Qt::OtherFocusReason);
}
	
void MainWindow::updateHome() {
	if (password == "") {
		return;
	}
	twitter.updateHome(username, password);
}
	
void MainWindow::saveSettings() {
	QSettings settings("arti", "qwit");
	
	username = usernameLineEdit->text();
	password = passwordLineEdit->text();
	bool savePassword = savePasswordCheckBox->checkState() == Qt::Checked;
	interval = intervalLineEdit->text().toInt();
	
	useProxy = useProxyCheckBox->checkState() == Qt::Checked;
	proxyAddress = proxyAddressLineEdit->text();
	proxyPort = proxyPortLineEdit->text().toInt();
	proxyUsername = proxyUsernameLineEdit->text();
	proxyPassword = proxyPasswordLineEdit->text();
	bool proxySavePassword = proxySavePasswordCheckBox->checkState() == Qt::Checked;
	
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
	} else {
		twitter.dontUseProxy();
	}
	
	if ((username == "") || (password == "")) {
		statusLineEdit->setDisabled(true);
		statusLineEdit->setDisabled(true);
	} else {
		statusLineEdit->setDisabled(false);
		statusLineEdit->setDisabled(false);
	}
	updateHome();
}
	
void MainWindow::resetSettings() {
	usernameLineEdit->setText(username);
	passwordLineEdit->setText(password);
	intervalLineEdit->setText(QString::number(interval));
}
	
QString MainWindow::formatDateTime(const QDateTime &time) {
	int seconds = time.secsTo(QDateTime::currentDateTime());
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
		if (this->isVisible()) {
			this->hide();
		} else {
			this->show();
		}
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
	event->accept();
}
	
void MainWindow::resizeEvent(QResizeEvent *event) {
	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
	event->accept();
}

void MainWindow::homeUpdated(const QByteArray &buffer) {
	twitterWidget->clear();
	
	QDomDocument document;
	
	document.setContent(buffer);
	
	QDomElement root = document.documentElement();
	
	QDomNode node = root.firstChild();
	QString html = "";
	QString trayMessage = "";
	int maxId = lastId;
	while (!node.isNull()) {
		QDomNode node2 = node.firstChild();
		QString message, timeStr, user, image;
		int id = 0;
		while (!node2.isNull()) {
			if (node2.toElement().tagName() == "created_at") {
				timeStr = node2.toElement().text();
			} else if (node2.toElement().tagName() == "text") {
				message = node2.toElement().text();
			} else if (node2.toElement().tagName() == "id") {
				id = node2.toElement().text().toInt();
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
		
		twitterWidget->addItem(imageFileName, user, message, formatDateTime(time.toLocalTime()), id);
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

#endif

/*
#ifdef Q_WS_WIN
    QSettings settings;
    settings.setPath("Microsoft", "Windows", QSettings::User);
    if (appTickerConfig->getstartTicker()) {
        // Want to start on boot up
        settings.writeEntry("/CurrentVersion/Run/ticker.exe", this->argv0);
    } else {
        // Do not want to start on boot up
        settings.removeEntry("/CurrentVersion/Run/ticker.exe");
    }
#endif
*/
