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

#include "WidgetSender.h"

#include <QtGui>
#include "LineEdit.h"

const QString WidgetSender::m_fromWho = QTextCodec::codecForName("UTF-8")->toUnicode("От кого");
const QString WidgetSender::m_fromWhere = QTextCodec::codecForName("UTF-8")->toUnicode("Откуда");

WidgetSender::WidgetSender( QWidget * parent )
	: WidgetContact( parent ), m_language( QLocale::Russian )
{
	m_state = Normal;

	setMinimumSize( QSize( 350, 150 ) );
}

QSize
WidgetSender::sizeHint() const
{
	if ( ! m_cachedSizeHint.isValid() ) {

		const int h = rowHeight() * 4 + SPACING * 3 + MARGIN * 2;

		m_cachedSizeHint.setWidth( 350 );
		m_cachedSizeHint.setHeight( h );
	}

	return m_cachedSizeHint;
}

void
WidgetSender::paintEvent( QPaintEvent * /*event*/ )
{
	QPainter painter( this );

	QFont fnt( font() );
	fnt.setItalic( true );

	//painter.setPen( QPen( palette().buttonText().color() ) );
	painter.setPen( QPen( QColor( "lightskyblue" ) ) );

	//const QFontMetrics fm( fnt );

	painter.setFont( fnt );

	int y1 = MARGIN + rowHeight(),
		y2 = y1 + SPACING + rowHeight(),
		y3 = y2 + SPACING + rowHeight();
		//y4 = y3 + SPACING + fm.height();

	painter.drawText( MARGIN, y1, m_fromWho );
	painter.drawLine( lineMargin(), y1 + 1, width() - MARGIN, y1 + 1 );

	painter.drawText( MARGIN, y2, m_fromWhere );
	painter.drawLine( lineMargin(), y2 + 1, width() - MARGIN, y2 + 1 );

	painter.drawLine( MARGIN, y3 + 1, width() - MARGIN, y3 + 1 );

	int boxHeight = SPACING + rowHeight(),
		boxWidth = qRound( boxHeight / 13. * 60. ),
		boxMargin = width() - MARGIN - boxWidth;

	painter.drawRect( boxMargin, y3 + 1, boxWidth, boxHeight );

	// data

	painter.setFont( m_addressFont );
	painter.setPen( QPen( palette().buttonText().color() ) );

	if ( m_state != SettingWho )
		painter.drawText( lineMargin() + 4, y1, m_who );

	if ( m_state != SettingWhere ) {
		const QStringList list = whereList( width() - lineMargin() - MARGIN );

		if ( list.size() > 0 )
			painter.drawText( lineMargin() + 4, y2, list[ 0 ] );

		if ( list.size() > 1 )
			painter.drawText( lineMargin() + 4, y3, list[ 1 ] );
	}

	if ( m_state != SettingIndex )
		painter.drawText( boxMargin + 4, y3 + 1, boxWidth, boxHeight,
				Qt::AlignHCenter + Qt::AlignVCenter, m_index );
}

void
WidgetSender::resizeEvent( QResizeEvent * event )
{
	switch ( m_state ) {
		case SettingWho:
		case SettingWhere:
			m_edit->resize( width() - lineMargin() - MARGIN, m_edit->height() );
			break;

		case SettingIndex: {
			const QFontMetrics fm( font() );
			const int editHeight = SPACING + fm.height(),
					  editWidth = qRound( editHeight / 13. * 60. ),
					  editMargin = width() - MARGIN - editWidth;
			m_edit->move( editMargin, m_edit->y() );
			break;
		}

		default:
			;
	}

	QWidget::resizeEvent( event );
}

void
WidgetSender::mouseDoubleClickEvent( QMouseEvent * event )
{
	const int x = event->x(),
			  y = event->y();

	if ( x > MARGIN && x <= width() - MARGIN &&  y > MARGIN  ) {
		const QFontMetrics fm( m_addressFont );

		const int y1 = MARGIN + fm.height() + 2,
				  y3 = y1 + SPACING + fm.height() + SPACING + fm.height(),
				  y4 = y3 + SPACING + fm.height();

		if ( y <= y1 )
			setWho();
		else if ( y <= y3 )
			setWhere();
		else if ( y <= y4 ) {
			const int boxHeight = SPACING + fm.height(),
					  boxWidth = qRound( boxHeight / 13. * 60. ),
					  boxMargin = width() - MARGIN - boxWidth;

			if ( x > boxMargin )
				setIndex();
		}
	}
}

const QString &
WidgetSender::whoStr() const
{
	return m_fromWho;
}

const QString &
WidgetSender::whereStr() const
{
	return m_fromWhere;
}

void
WidgetSender::setWhere()
{
	m_state = SettingWhere;

	const QFontMetrics fm( m_addressFont );

	const int y = fm.height() + MARGIN +	// 1st row
		SPACING + fm.height() -				// 2nd row
		m_edit->height() + 4;

	m_edit->setText( m_where );

	m_newly = m_where.isEmpty();

	showEdit( width() - lineMargin() - MARGIN, lineMargin(), y );
}

void
WidgetSender::setIndex()
{
	m_state = SettingIndex;

	const QFontMetrics fm( m_addressFont );

	const int y = fm.height() + MARGIN +	// 1st row
		SPACING + fm.height() +				// 2nd row
		SPACING + fm.height() +				// 3rd row
		SPACING + fm.height() -				// 4th row
		m_edit->height() + 4,

			  editHeight = SPACING + fm.height(),
			  editWidth = qRound( editHeight / 13. * 60. ),
			  editMargin = width() - MARGIN - editWidth;

	m_edit->setText( m_index );

	m_newly = m_index.isEmpty();

	showEdit( editWidth, editMargin, y );
}

void
WidgetSender::contextMenu( QMenu & menu ) const
{
	menu.addSeparator();
	if ( m_language == QLocale::Russian )
		menu.addAction( QIcon::fromTheme( "flag-en", QIcon(":/flag-en.png") ),
				"&Английский язык", this, SLOT( toggleLanguage() ), Qt::CTRL + Qt::Key_L );
	else
		menu.addAction( QIcon::fromTheme( "flag-ru", QIcon(":/flag-ru.png") ),
				"&Русский язык", this, SLOT( toggleLanguage() ), Qt::CTRL + Qt::Key_L );
}

void
WidgetSender::toggleLanguage( QLocale::Language lang )
{
	if ( lang == QLocale::AnyLanguage ) {
		if ( m_language == QLocale::English )
			m_language = QLocale::Russian;
		else
			m_language = QLocale::English;

		emit languageChanged( m_language );

	} else if ( m_language != lang ) {
		m_language = lang;

		emit languageChanged( m_language );
	}
}

QLocale::Language
WidgetSender::language() const
{
	return m_language;
}

