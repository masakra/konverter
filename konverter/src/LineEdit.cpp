
#include "LineEdit.h"

#include <QtGui>

LineEdit::LineEdit( QWidget * parent )
	: QLineEdit( parent )
{
}

void
LineEdit::keyPressEvent( QKeyEvent * event )
{
	if ( event->key() == Qt::Key_Escape )
		emit escapePressed();

	QLineEdit::keyPressEvent( event );
}

void
LineEdit::focusOutEvent( QFocusEvent * event )
{
	emit focusOut();

	QLineEdit::focusOutEvent( event );
}

