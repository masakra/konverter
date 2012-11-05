
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
		menu.addAction( "Выбрать &шрифт", this, SLOT( selectAddressFont() ) );

		menu.exec( event->globalPos() );
	}
}

void
WidgetContact::setWho()
{
	State = SettingWho;

	const QFontMetrics fm( addressFont );

	const int y = fm.height() + MARGIN -	// 1st row
		edit->height() + 4;

	edit->setText( who );

	editShow( width() - lineMargin() - MARGIN, lineMargin(), y );
}

void
WidgetContact::editShow( int w, int m, int y )
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
			break;

		case SettingWhere:
			where = edit->text().trimmed();
			emit whereChanged( where );
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

