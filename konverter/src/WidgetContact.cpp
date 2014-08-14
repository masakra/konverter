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

#include "WidgetContact.h"

#include <QtGui>
#include <QDebug>
#include "LineEdit.h"

WidgetContact::WidgetContact( QWidget * parent )
	: QWidget( parent ),
	m_cachedLineMargin( 0 ),
	m_addressFont( QFont() ),
	m_cachedRowHeight( 0 )

{
	m_edit = new LineEdit( this );
	m_edit->hide();

	connect( m_edit, SIGNAL( returnPressed() ), SLOT( editReturned() ) );
	connect( m_edit, SIGNAL( escapePressed() ), SLOT( editEscaped() ) );
	connect( m_edit, SIGNAL( focusOut() ), SLOT( editEscaped() ) );
}

void
WidgetContact::setAddressFont( const QFont & f )
{
	m_addressFont = f;

	m_cachedRowHeight = 0;
	m_cachedSizeHint = QSize( 0, 0 );

	emit addressFontChanged( m_addressFont );

	//setMinimumSize( 350, sizeHint().height() );
}

void
WidgetContact::setAll( const QString & wh, const QString & we, const QString & idx )
{
	m_who = wh;
	m_where = we;
	m_index = idx;
	update();
}

void
WidgetContact::setWho( const QString & text )
{
	m_who = text;
	update();
}

void
WidgetContact::setWhere( const QString & text )
{
	m_where = text;
	update();
}

void
WidgetContact::setIndex( const QString & text )
{
	m_index = text;
	update();
}

int
WidgetContact::rowHeight() const
{
	if ( m_cachedRowHeight == 0 ) {
		const QFontMetrics fm( m_addressFont );

		m_cachedRowHeight = fm.height();
	}

	return m_cachedRowHeight;
}

void
WidgetContact::selectAddressFont()
{
	bool ok;

	QFont fnt = QFontDialog::getFont( &ok, m_addressFont, this, "Выбери шрифт" );

	if ( ok ) {
		setAddressFont( fnt );
		update();
	}
}

int
WidgetContact::lineMargin() const
{
	if ( m_cachedLineMargin == 0 ) {
		QFont fnt( font() );
		fnt.setItalic( true );

		const QFontMetrics fm( fnt );

		m_cachedLineMargin = qMax( fm.width( whoStr() ), fm.width( whereStr() ) );
		m_cachedLineMargin += MARGIN + 3;
	}

	return m_cachedLineMargin;
}

void
WidgetContact::mousePressEvent( QMouseEvent * event )
{
	if ( event->buttons() & Qt::RightButton && m_state == Normal ) {

		QMenu menu;

		menu.addAction( "Установить " + whoStr(), this, SLOT( setWho() ) );
		menu.addAction( "Установить " + whereStr(), this, SLOT( setWhere() ) );
		menu.addAction( "Установить Индекс", this, SLOT( setIndex() ) );
		menu.addSeparator();
		menu.addAction( QIcon::fromTheme("preferences-desktop-font", QIcon(":/font.png") ),
				"Выбрать &шрифт", this, SLOT( selectAddressFont() ) );

		contextMenu( menu );

		menu.exec( event->globalPos() );
	}
}

void
WidgetContact::contextMenu( QMenu & /*menu*/ ) const
{
}

void
WidgetContact::setWho()
{
	m_state = SettingWho;

	const QFontMetrics fm( m_addressFont );

	const int y = fm.height() + MARGIN -	// 1st row
		m_edit->height() + 4;

	m_edit->setText( m_who );

	m_newly = m_who.isEmpty();

	showEdit( width() - lineMargin() - MARGIN, lineMargin(), y );
}

void
WidgetContact::showEdit( int w, int m, int y )
{
	update();

	m_edit->resize( w, m_edit->height() );
	m_edit->move( m, y );

	m_edit->show();
	m_edit->setFocus();
}

void
WidgetContact::editReturned()
{
	switch ( m_state ) {
		case SettingWho:
			m_who = m_edit->text().trimmed();
			emit whoChanged( m_who );
			if ( m_newly ) {
				setWhere();
				return;
			} else
				break;

		case SettingWhere:
			m_where = m_edit->text().trimmed();
			emit whereChanged( m_where );
			if ( m_newly ) {
				setIndex();
				return;
			} else
				break;

		case SettingIndex:
			m_index = m_edit->text().trimmed();
			emit indexChanged( m_index );
			break;

		default:
			return;
	}

	m_edit->hide();
	m_state = Normal;

	update();
}

void
WidgetContact::editEscaped()
{
	m_edit->hide();
	m_state = Normal;
	update();
}

const QFont &
WidgetContact::addrFont() const
{
	return m_addressFont;
}

const QString &
WidgetContact::getIndex() const
{
	return m_index;
}

QStringList
WidgetContact::whoList( int w ) const
{
	return split( m_who, w );
}

QStringList
WidgetContact::whereList( int w ) const
{
	return split( m_where, w );
}

QStringList
WidgetContact::split( const QString & str, int w ) const
{
	QStringList list;

	const QFontMetrics fm( m_addressFont );

	if ( fm.width( str ) <= w ) {
		list << str;
		return list;
	}

	QStringList words = str.trimmed().split(" ", QString::SkipEmptyParts );

	if ( words.size() == 0 )
		return list;

	QString acc;

	for ( int i = 0; i < words.size(); ++i ) {

		if ( fm.width( acc + " " + words[ i ] ) >= w ) {
			list << acc;
			acc = words[ i ];
		} else {
			acc += " " + words[ i ];
		}
	}

	if ( acc.isEmpty() )
		return list;
	else
		return list << acc;
}


