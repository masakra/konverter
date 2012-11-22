/***************************************************************************
 *   Copyright (C) 2012 Nordavia-RA                                        *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   ICQ: 124231040                                                        *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

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

	if ( list.size() != 19 ) {
		_yell( "Количество параметров конверта должно быть 19" );
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
	sender.indexTopOffset = list[ 9 ].trimmed().toDouble();

	recipient.x = list[ 10 ].trimmed().toDouble();
	recipient.y = list[ 11 ].trimmed().toDouble();
	recipient.width = list[ 12 ].trimmed().toDouble();
	recipient.whoRowCount = list[ 13 ].trimmed().toInt();
	recipient.whereRowCount = list[ 14 ].trimmed().toInt();
	recipient.indexMargin = list[ 15 ].trimmed().toDouble();
	recipient.indexTopOffset = list[ 16 ].trimmed().toDouble();

	e_doubleSide = ( list[ 17 ].trimmed().toInt() == 1 );

	e_log = ( list[ 18 ].trimmed().toInt() == 1 );

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

bool
Envelope::doubleSide() const
{
	return e_doubleSide;
}

bool
Envelope::log() const
{
	return e_log;
}

