
#include "WidgetRecipient.h"

#include <QtGui>

const QString WidgetRecipient::toWho = QTextCodec::codecForName("KOI8-R")->toUnicode("Кому");
const QString WidgetRecipient::toWhere = QTextCodec::codecForName("KOI8-R")->toUnicode("Куда");

WidgetRecipient::WidgetRecipient( QWidget * parent )
	: WidgetContact( parent )
{
	State = Normal;
}

QSize
WidgetRecipient::sizeHint() const
{
	return QSize( 400, 300 );
}

void
WidgetRecipient::paintEvent( QPaintEvent * event )
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

	painter.drawText( MARGIN, y1, toWho );
	painter.drawLine( lineMargin(), y1 + 1, width() - MARGIN, y1 + 1 );

	painter.drawLine( MARGIN, y2 + 1, width() - MARGIN, y2 + 1 );

	painter.drawText( MARGIN, y3, toWhere );
	painter.drawLine( lineMargin(), y3 + 1, width() - MARGIN, y3 + 1 );

	painter.drawLine( MARGIN, y4 + 1, width() - MARGIN, y4 + 1 );

	painter.drawLine( MARGIN, y5 + 1, width() - MARGIN, y5 + 1 );

	painter.drawLine( MARGIN, y6 + 1, width() - MARGIN, y6 + 1 );

	const int boxHeight = ( SPACING + rowHeight() ) / 6.5 * 8,
			  boxWidth = boxHeight / 8. * 35;

	painter.drawRect( MARGIN, y6 + 1, boxWidth, boxHeight );

	// data

	painter.setFont( addressFont );
	painter.setPen( QPen( palette().buttonText().color() ) );

	if ( State != SettingWho ) {
		const QStringList list = whoList( width() - lineMargin() - MARGIN );

		if ( list.size() > 0 )
			painter.drawText( lineMargin() + 4, y1, list[ 0 ] );

		if ( list.size() > 1 )
			painter.drawText( lineMargin() + 4, y2, list[ 1 ] );
	}

	if ( State != SettingWhere ) {
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

	if ( State != SettingIndex )
		painter.drawText( MARGIN, y6 + 1, boxWidth, boxHeight,
				Qt::AlignHCenter + Qt::AlignVCenter, index );
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
	return toWho;
}

const QString &
WidgetRecipient::whereStr() const
{
	return toWhere;
}

