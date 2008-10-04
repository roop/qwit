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

class TwitterTab {
public:
	bool clear;
	TwitterWidget *twitterWidget;
	int lastId;
	QScrollArea *scrollArea;
	int lastUpdateTime;
	
	TwitterTab(bool clear, QScrollArea *scrollArea, TwitterWidget *twitterWidget, int lastId) {
		this->clear = clear;
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
	UserpicsDownloader userpicsDownloader;
	StatusTextEdit *statusTextEdit;
	OptionsDialog *optionsDialog;
	LogsDialog *logsDialog;
	TwitterTab twitterTabs[TWITTER_TABS];
	Twitter twitter;
	QMap<QString, int> monthes;
	
	MainWindow(QWidget *parent = 0);
	void setupTrayIcon();
	void loadSettings();
	QDateTime dateFromString(const QString &date);
	
public slots:
	
	void sendStatus();
	void updateTimeline();
	void saveSettings();
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

protected:
	
	void closeEvent(QCloseEvent *event);
	void hideEvent(QHideEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif
