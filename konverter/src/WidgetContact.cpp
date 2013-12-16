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
	cachedLineMargin( 0 ),
	addressFont( QFont() ),
	cachedRowHeight( 0 )

{
	edit = new LineEdit( this );
	edit->hide();

	connect( edit, SIGNAL( returnPressed() ), SLOT( editReturned() ) );
	connect( edit, SIGNAL( escapePressed() ), SLOT( editEscaped() ) );
	connect( edit, SIGNAL( focusOut() ), SLOT( editEscaped() ) );
}

void
WidgetContact::setAddressFont( const QFont & f )
{
	addressFont = f;

	cachedRowHeight = 0;
	cachedSizeHint = QSize( 0, 0 );

	emit addressFontChanged( addressFont );

	//setMinimumSize( 350, sizeHint().height() );
}

void
WidgetContact::setAll( const QString & wh, const QString & we, const QString & idx )
{
	who = wh;
	where = we;
	index = idx;
	update();
}

void
WidgetContact::setWho( const QString & text )
{
	who = text;
	update();
}

void
WidgetContact::setWhere( const QString & text )
{
	where = text;
	update();
}

void
WidgetContact::setIndex( const QString & text )
{
	index = text;
	update();
}

int
WidgetContact::rowHeight() const
{
	if ( cachedRowHeight == 0 ) {
		const QFontMetrics fm( addressFont );

		cachedRowHeight = fm.height();
	}

	return cachedRowHeight;
}

void
WidgetContact::selectAddressFont()
{
	bool ok;

	QFont fnt = QFontDialog::getFont( &ok, addressFont, this, "Выбери шрифт" );

	if ( ok ) {
		setAddressFont( fnt );
		update();
	}
}

int
WidgetContact::lineMargin() const
{
	if ( cachedLineMargin == 0 ) {
		QFont fnt( font() );
		fnt.setItalic( true );

		const QFontMetrics fm( fnt );

		cachedLineMargin = qMax( fm.width( whoStr() ), fm.width( whereStr() ) );
		cachedLineMargin += MARGIN + 3;
	}

	return cachedLineMargin;
}

void
WidgetContact::mousePressEvent( QMouseEvent * event )
{
	if ( event->buttons() & Qt::RightButton && State == Normal ) {

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
	State = SettingWho;

	const QFontMetrics fm( addressFont );

	const int y = fm.height() + MARGIN -	// 1st row
		edit->height() + 4;

	edit->setText( who );

	newly = who.isEmpty();

	showEdit( width() - lineMargin() - MARGIN, lineMargin(), y );
}

void
WidgetContact::showEdit( int w, int m, int y )
{
	update();

	edit->resize( w, edit->height() );
	edit->move( m, y );

	edit->show();
	edit->setFocus();
}

void
WidgetContact::editReturned()
{
	switch ( State ) {
		case SettingWho:
			who = edit->text().trimmed();
			emit whoChanged( who );
			if ( newly ) {
				setWhere();
				return;
			} else
				break;

		case SettingWhere:
			where = edit->text().trimmed();
			emit whereChanged( where );
			if ( newly ) {
				setIndex();
				return;
			} else
				break;

		case SettingIndex:
			index = edit->text().trimmed();
			emit indexChanged( index );
			break;

		default:
			return;
	}

	edit->hide();
	State = Normal;

	update();
}

void
WidgetContact::editEscaped()
{
	edit->hide();
	State = Normal;
	update();
}

const QFont &
WidgetContact::addrFont() const
{
	return addressFont;
}

const QString &
WidgetContact::getIndex() const
{
	return index;
}

QStringList
WidgetContact::whoList( int w ) const
{
	return split( who, w );
}

QStringList
WidgetContact::whereList( int w ) const
{
	return split( where, w );
}

QStringList
WidgetContact::split( const QString & str, int w ) const
{
	QStringList list;

	const QFontMetrics fm( addressFont );

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


