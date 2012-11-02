
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
	Q_OBJECT

	protected:
		virtual void keyPressEvent( QKeyEvent * event );

	public:
		LineEdit( QWidget * parent );

	Q_SIGNALS:
		void escapePressed();
};

#endif

