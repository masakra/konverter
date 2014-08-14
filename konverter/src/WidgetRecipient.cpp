/***************************************************************************
 *   Copyright (C) 2012 Nordavia-RA                                        *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   ICQ: 124231040                                                        *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   All comments, if not ascii, in koi8-r                                 *
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

#include "WidgetRecipient.h"

#include <QtGui>
#include "LineEdit.h"

const QString WidgetRecipient::m_toWho = QTextCodec::codecForName("UTF-8")->toUnicode("Кому");
const QString WidgetRecipient::m_toWhere = QTextCodec::codecForName("UTF-8")->toUnicode("Куда");

WidgetRecipient::WidgetRecipient( QWidget * parent )
	: WidgetContact( parent )
{
	m_state = Normal;
}

QSize
WidgetRecipient::sizeHint() const
{
	return QSize( 400, 220 );
}

void
WidgetRecipient::paintEvent( QPaintEvent * /*event*/ )
{
	QPainter painter( this );

	QFont fnt( font() );
	fnt.setItalic( true );

	painter.setPen( QPen( QColor( "lightskyblue" ) ) );

	painter.setFont( fnt );

	int y1 = MARGIN + rowHeight(),
		y2 = y1 + SPACING + rowHeight(),
		y3 = y2 + SPACING + rowHeight(),
		y4 = y3 + SPACING + rowHeight(),
		y5 = y4 + SPACING + rowHeight(),
		y6 = y5 + SPACING + rowHeight();

	painter.drawText( MARGIN, y1, m_toWho );
	painter.drawLine( lineMargin(), y1 + 1, width() - MARGIN, y1 + 1 );

	painter.drawLine( MARGIN, y2 + 1, width() - MARGIN, y2 + 1 );

	painter.drawText( MARGIN, y3, m_toWhere );
	painter.drawLine( lineMargin(), y3 + 1, width() - MARGIN, y3 + 1 );

	painter.drawLine( MARGIN, y4 + 1, width() - MARGIN, y4 + 1 );

	painter.drawLine( MARGIN, y5 + 1, width() - MARGIN, y5 + 1 );

	painter.drawLine( MARGIN, y6 + 1, width() - MARGIN, y6 + 1 );

	const int boxHeight = ( SPACING + rowHeight() ) / 6.5 * 8,
			  boxWidth = boxHeight / 8. * 35;

	painter.drawRect( MARGIN, y6 + 1, boxWidth, boxHeight );

	// data

	painter.setFont( m_addressFont );
	painter.setPen( QPen( palette().buttonText().color() ) );

	if ( m_state != SettingWho ) {
		const QStringList list = whoList( width() - lineMargin() - MARGIN );

		if ( list.size() > 0 )
			painter.drawText( lineMargin() + 4, y1, list[ 0 ] );

		if ( list.size() > 1 )
			painter.drawText( lineMargin() + 4, y2, list[ 1 ] );
	}

	if ( m_state != SettingWhere ) {
		const QStringList list = whereList( width() - lineMargin() - MARGIN );

		if ( list.size() > 0 )
			painter.drawText( lineMargin() + 4, y3, list[ 0 ] );

		if ( list.size() > 1 )
			painter.drawText( lineMargin() + 4, y4, list[ 1 ] );

		if ( list.size() > 2 )
			painter.drawText( lineMargin() + 4, y5, list[ 2 ] );

		if ( list.size() > 3 )
			painter.drawText( lineMargin() + 4, y6, list[ 3 ] );
	}

	if ( m_state != SettingIndex )
		painter.drawText( MARGIN, y6 + 1, boxWidth, boxHeight,
				Qt::AlignHCenter + Qt::AlignVCenter, m_index );
}


/*
int
WidgetRecipient::lineMargin() const
{
	if ( cachedLineMargin == 0 ) {
		QFont fnt( font() );
		fnt.setItalic( true );

		const QFontMetrics fm( fnt );

		cachedLineMargin = qMax( fm.width( toWho ), fm.width( toWhere ) );
		cachedLineMargin += MARGIN + 3;
	}

	return cachedLineMargin;
}
*/

const QString &
WidgetRecipient::whoStr() const
{
	return m_toWho;
}

const QString &
WidgetRecipient::whereStr() const
{
	return m_toWhere;
}

void
WidgetRecipient::setWhere()
{
	m_state = SettingWhere;

	const QFontMetrics fm( m_addressFont );

	const int y = fm.height() + MARGIN +	// 1st row
		SPACING + fm.height() +				// 2nd row
		SPACING + fm.height() -				// 3rd row
		m_edit->height() + 4;

	m_edit->setText( m_where );

	m_newly = m_where.isEmpty();

	showEdit( width() - lineMargin() - MARGIN, lineMargin(), y );
}

void
WidgetRecipient::setIndex()
{
	m_state = SettingIndex;

	const QFontMetrics fm( m_addressFont );

	const int boxHeight = qRound( SPACING + fm.height() ) / 6.5 * 8,
			  y = fm.height() + MARGIN +		// 1st row
					SPACING + fm.height() +		// 2nd row
					SPACING + fm.height() +		// 3rd row
					SPACING + fm.height() +		// 4th row
					SPACING + fm.height() +		// 5th row
					SPACING + fm.height() +		// 6th row
					boxHeight -
					m_edit->height() + 4,

			  editWidth = qRound( boxHeight / 8. * 35. );

	m_edit->setText( m_index );

	m_newly = m_index.isEmpty();

	showEdit( editWidth, MARGIN, y );
}

void
WidgetRecipient::mouseDoubleClickEvent( QMouseEvent * event )
{
	const int x = event->x(),
			  y = event->y();

	if ( x > MARGIN && x <= width() - MARGIN &&  y > MARGIN  ) {
		const QFontMetrics fm( m_addressFont );

		const int boxHeight = qRound( SPACING + fm.height() ) / 6.5 * 8,
				  y2 = MARGIN + fm.height() + SPACING + fm.height() + 2,
				  y6 = y2 +
					  SPACING + fm.height() +
					  SPACING + fm.height() +
					  SPACING + fm.height() +
					  SPACING + fm.height(),
				  y7 = y6 + boxHeight;

		if ( y <= y2 )
			setWho();
		else if ( y <= y6 )
			setWhere();
		else if ( y <= y7 ) {
			const int boxWidth = qRound( boxHeight / 8. * 35 );

			if ( x > MARGIN && x <= MARGIN + boxWidth )
				setIndex();
		}
	}
}

