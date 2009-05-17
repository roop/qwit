#include "TwitPicDialog.h"
#include <QGridLayout>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QNetworkRequest>
#include <QImageReader>

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
	setLayout(layout);
	m_postButton->setDefault(true);
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

void TwitPicDialog::accept() {
	m_progressBar->reset();
	m_progressBar->show();
}
