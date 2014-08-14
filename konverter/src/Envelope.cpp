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
	: m_valid( false )
{
}

Envelope::Envelope( const QString & line )
	: m_valid( false )
{
	QStringList list = line.split( "|", QString::SkipEmptyParts );

	if ( list.size() != 19 ) {
		_yell( "Количество параметров конверта должно быть 19" );
		return;
	}

	m_e_id = list[ 0 ].trimmed().toInt();
	m_e_name = list[ 1 ].trimmed();
	m_e_rowHeight = list[ 2 ].trimmed().toDouble();

	m_sender.x = list[ 3 ].trimmed().toDouble();
	m_sender.y = list[ 4 ].trimmed().toDouble();
	m_sender.width = list[ 5 ].trimmed().toDouble();
	m_sender.whoRowCount = list[ 6 ].trimmed().toInt();
	m_sender.whereRowCount = list[ 7 ].trimmed().toInt();
	m_sender.indexMargin = list[ 8 ].trimmed().toDouble();
	m_sender.indexTopOffset = list[ 9 ].trimmed().toDouble();

	m_recipient.x = list[ 10 ].trimmed().toDouble();
	m_recipient.y = list[ 11 ].trimmed().toDouble();
	m_recipient.width = list[ 12 ].trimmed().toDouble();
	m_recipient.whoRowCount = list[ 13 ].trimmed().toInt();
	m_recipient.whereRowCount = list[ 14 ].trimmed().toInt();
	m_recipient.indexMargin = list[ 15 ].trimmed().toDouble();
	m_recipient.indexTopOffset = list[ 16 ].trimmed().toDouble();

	e_doubleSide = ( list[ 17 ].trimmed().toInt() == 1 );

	e_log = ( list[ 18 ].trimmed().toInt() == 1 );

	m_valid = true;
}

bool
Envelope::isValid() const
{
	return m_valid;
}

int
Envelope::id() const
{
	return m_e_id;
}

const QString &
Envelope::name() const
{
	return m_e_name;
}

qreal
Envelope::rowHeight() const
{
	return m_e_rowHeight;
}

const Envelope::Contact &
Envelope::sender() const
{
	return m_sender;
}

const Envelope::Contact &
Envelope::recipient() const
{
	return m_recipient;
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

