#ifndef TWITPICDIALOG_H
#define TWITPICDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QNetworkReply>

class TwitPicDialog : public QDialog
{
	Q_OBJECT
public:
	TwitPicDialog(const QString &fileName, QWidget *parent = 0);
	void setTwitPixmap(const QPixmap &pixmap);
	QProgressBar *progressBar() const;
	void setUser(const QString &username, const QString &password);
	QString twitPickedUrlString() const;

public slots:
	void accept();

private slots:
	void updateProgressBar(qint64, qint64);
	void uploadFinished();

private:
	QString m_fileName;
	QLabel *m_pixmapLabel, *m_errorMsgLabel;
	QPushButton *m_postButton, *m_cancelButton;
	QProgressBar *m_progressBar;
	QString m_username, m_password;
	QNetworkReply *m_reply;
	QString m_twitPickedUrlString;
};

#endif // TWITPICDIALOG_H
