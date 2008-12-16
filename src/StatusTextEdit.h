#ifndef StatusTextEdit_h
#define StatusTextEdit_h

#include <QWidget>
#include <QTextEdit>
#include <QFocusEvent>

class StatusTextEdit: public QTextEdit {
	Q_OBJECT

public:

	StatusTextEdit(QWidget *parent = 0);
	
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
	int getMaxStatusCharacter();

protected:
	
	void keyPressEvent(QKeyEvent *e);

signals:
	
	void returnPressed();
	void leftCharsNumberChanged(int);
	
public slots:
	
	void reply(const QString &username);
};

#endif
