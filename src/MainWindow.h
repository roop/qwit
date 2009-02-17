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

#ifndef MainWindow_h
#define MainWindow_h

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include <QBuffer>
#include <QtNetwork/QHttp>
#include <QUrl>
#include <QtXml/QDomDocument>
#include <QString>
#include <QFile>
#include <QTime>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <QMenu>
#include <QCloseEvent>
#include <QSettings>
#include <QScrollArea>
#include <QScrollBar>
#include <QQueue>
#include <QPair>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDir>

#include "UserpicsDownloader.h"
#include "StatusTextEdit.h"
#include "TwitterWidget.h"
#include "Twitter.h"
#include "OptionsDialog.h"
#include "LogsDialog.h"

#include "ui_MainWindow.h"

const int TWITTER_TABS = 4;
const int HOME_TWITTER_TAB = 0;
const int PUBLIC_TWITTER_TAB = 1;
const int REPLIES_TWITTER_TAB = 2;
const int CUSTOM_TWITTER_TAB = 3;

const int DEFAULT_WINDOW_WIDTH = 200;
const int DEFAULT_WINDOW_HEIGHT = 600;
const int DEFAULT_WINDOW_X = 200;
const int DEFAULT_WINDOW_Y = 200;
const int DEFAULT_UPDATE_INTERVAL = 300;
const int DEFAULT_MESSAGES_PER_PAGE = 20;

class TwitterTab {
public:
	TwitterWidget *twitterWidget;
	int lastId;
	QScrollArea *scrollArea;
	int lastUpdateTime;
	
	TwitterTab(QScrollArea *scrollArea, TwitterWidget *twitterWidget, int lastId) {
		this->scrollArea = scrollArea;
		this->twitterWidget = twitterWidget;
		this->lastId = lastId;
		this->lastUpdateTime = 0;
	}
	
	TwitterTab() {
	}
};

class MainWindow: public QDialog, public Ui::MainWindow {
	Q_OBJECT
	
private:
	static MainWindow* instance;
	MainWindow(QWidget *parent = 0);

public:
	
	QTimer *timer;
	QTimer *timer2;
	QString username;
	QString password;
	int interval;
	QAction *quitAction;
	QAction *showhideAction;
	QMenu *trayIconMenu;
	QSystemTrayIcon *trayIcon;
	bool acceptClose;
	bool useProxy;
	QString proxyUsername;
	QString proxyPassword;
	QString proxyAddress;
	int proxyPort;
	int messagesPerPage;
	UserpicsDownloader userpicsDownloader;
	StatusTextEdit *statusTextEdit;
	OptionsDialog *optionsDialog;
	LogsDialog *logsDialog;
	TwitterTab twitterTabs[TWITTER_TABS];
	Twitter twitter;
	QMap<QString, int> monthes;
	bool updatesNotification;

        QString replyStatusID;
        bool isReply;
	
	void setupTrayIcon();
	void loadState();
	QDateTime dateFromString(const QString &date);
	static MainWindow* getInstance();
	
public slots:
	
	void sendStatus();
	void updateTimeline();
	void saveState();
	void resetSettings();
	void updated(const QByteArray &buffer, int type);
	void statusUpdated();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void leftCharsNumberChanged(int length);
	void showhide();
	void quit();
	void updateState(const QString &state);
	void tabChanged(int index);
	void updateItems();
	void customUsernameChanged();
        void setReplyID(const QString &replyID);

protected:
	
	void closeEvent(QCloseEvent *event);
	void hideEvent(QHideEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif
