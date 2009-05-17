#include "TwitPicDialog.h"
#include <QGridLayout>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImageReader>
#include <QFile>
#include <QRegExp>

TwitPicDialog::TwitPicDialog(const QString &fileName, QWidget *parent)
		: QDialog(parent),
		m_fileName(fileName),
		m_pixmapLabel(0),
		m_postButton(0),
		m_progressBar(0)
{
	setWindowTitle("TwitPic");
	QVBoxLayout *layout = new QVBoxLayout;
	m_pixmapLabel = new QLabel(this);
	layout->addWidget(m_pixmapLabel);
	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	m_postButton = buttonBox->addButton("Post image", QDialogButtonBox::AcceptRole);
	m_cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
	layout->addWidget(buttonBox, 0, Qt::AlignCenter);
	m_progressBar = new QProgressBar(this);
	m_progressBar->setMinimum(0);
	m_progressBar->setMaximum(100);
	m_progressBar->setTextVisible(false);
	m_progressBar->hide();
	layout->addWidget(m_progressBar);
	m_errorMsgLabel = new QLabel(this);
	m_errorMsgLabel->hide();
	layout->addWidget(m_errorMsgLabel);
	setLayout(layout);
	m_postButton->setDefault(true);
	m_postButton->setEnabled(false);
	setTwitPixmap(QPixmap(m_fileName));
	connect(m_postButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void TwitPicDialog::setTwitPixmap(const QPixmap &pixmap) {
	QSize size(600, 200);
	if (pixmap.width() > size.width() || pixmap.height() > size.height()) {
		m_pixmapLabel->setPixmap(pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	} else {
		m_pixmapLabel->setPixmap(pixmap);
	}
}

void TwitPicDialog::setUser(const QString &username, const QString &password) {
	m_username = username;
	m_password = password;
	m_postButton->setEnabled(true);
}

void TwitPicDialog::accept() {
	m_progressBar->reset();
	m_errorMsgLabel->hide();
	m_progressBar->show();
	QUrl url("http://twitpic.com/api/upload");
	QNetworkRequest post(url);
	QByteArray boundary = "------------------------------7251e57fe7ed"; // "--------dlf-maximum";
	post.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
	post.setRawHeader("Host", "twitpic.com");
	QByteArray ba;
	ba.append("--" + boundary + "\r\n");
	ba.append("Content-Disposition: form-data; name=\"media\"; filename=\"1.jpg\"\r\n");
	ba.append("Content-Type: image/" + QImageReader::imageFormat(m_fileName) + "\r\n\r\n");
	QFile file(m_fileName);
	file.open(QIODevice::ReadOnly);
	ba.append(file.readAll());
	ba.append("\r\n");
	ba.append("--" + boundary + "\r\n");
	ba.append("Content-Disposition: form-data; name=\"username\"\r\n\r\n");
	ba.append(m_username + "\r\n");
	ba.append("--" + boundary + "\r\n");
	ba.append("Content-Disposition: form-data; name=\"password\"\r\n\r\n");
	ba.append(m_password + "\r\n");
	ba.append("--" + boundary + "--" + "\r\n");
	post.setHeader(QNetworkRequest::ContentLengthHeader, ba.count());

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	m_reply = manager->post(post, ba);
	connect(m_reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(updateProgressBar(qint64,qint64)));
	connect(m_reply, SIGNAL(finished()), this, SLOT(uploadFinished()));
}

void TwitPicDialog::updateProgressBar(qint64 sent, qint64 total) {
	m_progressBar->setValue(sent * m_progressBar->maximum() / total);
}

void TwitPicDialog::uploadFinished() {
	// The xml module is too heavy for this (and a pain to use). Managing with QRegExp for now.
	QString xmlReply = m_reply->readAll();
	xmlReply.replace("\r\n", "");
	xmlReply.replace("\n", "");
	QString errorMsg = "";
	QRegExp rx("<rsp stat=\"(\\S+)\">");
	if (rx.indexIn(xmlReply) >= 0) {
		if (rx.capturedTexts().at(1) == "ok") {
			QRegExp rx2("<mediaurl>(\\S+)<\\/mediaurl>");
			if (rx2.indexIn(xmlReply) >= 0) {
				m_twitPickedUrlString = rx2.capturedTexts().at(1);
			}
		} else {
			QRegExp rx("<err .* msg=\"([^\"]*)\"");
			if (rx.indexIn(xmlReply) >= 0) {
				errorMsg = rx.capturedTexts().at(1);
			}
		}
	}
	if (!m_twitPickedUrlString.isEmpty()) {
		QDialog::accept();
	} else if (!errorMsg.isEmpty()) {
		m_errorMsgLabel->setText("TwitPic said: " + errorMsg);
		m_progressBar->hide();
		m_errorMsgLabel->show();
	}
}

QString TwitPicDialog::twitPickedUrlString() const {
	return m_twitPickedUrlString;
}
