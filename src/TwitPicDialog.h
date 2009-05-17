#ifndef TWITPICDIALOG_H
#define TWITPICDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>

class TwitPicDialog : public QDialog
{
public:
	TwitPicDialog(const QString &fileName, QWidget *parent = 0);
	void setTwitPixmap(const QPixmap &pixmap);
	QProgressBar *progressBar() const;
	void setUser(const QString &username, const QString &password);

public slots:
	void accept();

private:
	QString m_fileName;
	QLabel *m_pixmapLabel;
	QPushButton *m_postButton, *m_cancelButton;
	QProgressBar *m_progressBar;
};

#endif // TWITPICDIALOG_H
