
#include "Envelope.h"

#include <QtGui>
#include "_.h"

Envelope::Envelope()
	: valid( false )
{
}

Envelope::Envelope( const QString & line )
	: valid( false )
{
	QStringList list = line.split( "|", QString::SkipEmptyParts );

	if ( list.size() != 17 ) {
		_yell( "Количество параметров конверта должно быть 17" );
		return;
	}

	e_id = list[ 0 ].trimmed().toInt();
	e_name = list[ 1 ].trimmed();
	e_rowHeight = list[ 2 ].trimmed().toDouble();

	sender.x = list[ 3 ].trimmed().toDouble();
	sender.y = list[ 4 ].trimmed().toDouble();
	sender.width = list[ 5 ].trimmed().toDouble();
	sender.whoRowCount = list[ 6 ].trimmed().toInt();
	sender.whereRowCount = list[ 7 ].trimmed().toInt();
	sender.indexMargin = list[ 8 ].trimmed().toDouble();

	recipient.x = list[ 9 ].trimmed().toDouble();
	recipient.y = list[ 10 ].trimmed().toDouble();
	recipient.width = list[ 11 ].trimmed().toDouble();
	recipient.whoRowCount = list[ 12 ].trimmed().toInt();
	recipient.whereRowCount = list[ 13 ].trimmed().toInt();
	recipient.indexMargin = list[ 14 ].trimmed().toDouble();
	recipient.indexTopOffset = list[ 15 ].trimmed().toDouble();

	e_doubleSide = ( list[ 16 ].trimmed().toInt() == 1 );

	valid = true;
}

bool
Envelope::isValid() const
{
	return valid;
}

int
Envelope::id() const
{
	return e_id;
}

const QString &
Envelope::name() const
{
	return e_name;
}

qreal
Envelope::rowHeight() const
{
	return e_rowHeight;
}

qreal
Envelope::senderX() const
{
	return sender.x;
}

qreal
Envelope::senderY() const
{
	return sender.y;
}

qreal
Envelope::senderWidth() const
{
	return sender.width;
}

int
Envelope::senderWhoRowCount() const
{
	return sender.whoRowCount;
}

int
Envelope::senderWhereRowCount() const
{
	return sender.whereRowCount;
}

qreal
Envelope::senderIndexMargin() const
{
	return sender.indexMargin;
}

qreal
Envelope::senderIndexTopOffset() const
{
	return sender.indexTopOffset;
}

qreal
Envelope::recipientX() const
{
	return recipient.x;
}

qreal
Envelope::recipientY() const
{
	return recipient.y;
}

qreal
Envelope::recipientWidth() const
{
	return recipient.width;
}

int
Envelope::recipientWhoRowCount() const
{
	return recipient.whoRowCount;
}

int
Envelope::recipientWhereRowCount() const
{
	return recipient.whereRowCount;
}

qreal
Envelope::recipientIndexMargin() const
{
	return recipient.indexMargin;
}

qreal
Envelope::recipientIndexTopOffset() const
{
	return recipient.indexTopOffset;
}
