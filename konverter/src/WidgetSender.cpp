
#include "WidgetSender.h"

#include <QtGui>
#include "LineEdit.h"

const QString WidgetSender::fromWho = QTextCodec::codecForName("KOI8-R")->toUnicode("�� ����");
const QString WidgetSender::fromWhere = QTextCodec::codecForName("KOI8-R")->toUnicode("������");

WidgetSender::WidgetSender( QWidget * parent )
	: WidgetContact( parent )
{
	State = Normal;

	//setVerticalStretch( 1 );
	setMinimumSize( QSize( 350, 150 ) );
}

QSize
WidgetSender::sizeHint() const
{
	if ( ! cachedSizeHint.isValid() ) {

		//const QFontMetrics fm( font() );
		const int h = rowHeight() * 4 + SPACING * 3 + MARGIN * 2;

		cachedSizeHint.setWidth( 350 );
		cachedSizeHint.setHeight( h );

		//setMinimumSize( 350, h );
	}

	return cachedSizeHint;
}

void
WidgetSender::paintEvent( QPaintEvent * event )
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

	painter.drawText( MARGIN, y1, fromWho );
	painter.drawLine( lineMargin(), y1 + 1, width() - MARGIN, y1 + 1 );

	painter.drawText( MARGIN, y2, fromWhere );
	painter.drawLine( lineMargin(), y2 + 1, width() - MARGIN, y2 + 1 );

	painter.drawLine( MARGIN, y3 + 1, width() - MARGIN, y3 + 1 );

	int boxHeight = SPACING + rowHeight(),
		boxWidth = qRound( boxHeight / 13. * 60. ),
		boxMargin = width() - MARGIN - boxWidth;

	painter.drawRect( boxMargin, y3 + 1, boxWidth, boxHeight );

	// data

	painter.setFont( addressFont );
	painter.setPen( QPen( palette().buttonText().color() ) );

	if ( State != SettingWho )
		painter.drawText( lineMargin() + 4, y1, who );

	if ( State != SettingWhere ) {
		const QStringList list = whereList( width() - lineMargin() - MARGIN );

		if ( list.size() > 0 )
			painter.drawText( lineMargin() + 4, y2, list[ 0 ] );

		if ( list.size() > 1 )
			painter.drawText( lineMargin() + 4, y3, list[ 1 ] );
	}

	if ( State != SettingIndex )
		painter.drawText( boxMargin + 4, y3 + 1, boxWidth, boxHeight,
				Qt::AlignHCenter + Qt::AlignVCenter, index );
}

/*
QString
WidgetSender::whereLine( int row ) const
{
	const int w = width() - lineMargin() - MARGIN;

	const QFontMetrics fm( addressFont );

	if ( fm.width( where ) <= w ) {
		if ( row == 1 )
			return where;
		else
			return QString();
	}

	QStringList list = where.split(" ");

	if ( list.size() == 0 )
		return QString();

	int count = 1;
	QString acc;

	for ( int i = 0; i < list.size(); ++i ) {

		if ( acc.size() != 0 )
			acc += " ";

		if ( fm.width( acc + list[ i ] ) >= w ) {
			if ( count == row )
				return acc;
			else {
				++count;
				acc = list[ i ];
			}
		} else
			acc += list[ i ];
	}

	return acc;
}
*/

void
WidgetSender::resizeEvent( QResizeEvent * event )
{
	switch ( State ) {
		case SettingWho:
		case SettingWhere:
			edit->resize( width() - lineMargin() - MARGIN, edit->height() );
			break;

		case SettingIndex: {
			const QFontMetrics fm( font() );
			const int editHeight = SPACING + fm.height(),
					  editWidth = qRound( editHeight / 13. * 60. ),
					  editMargin = width() - MARGIN - editWidth;
			edit->move( editMargin, edit->y() );
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
		const QFontMetrics fm( addressFont );

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
	return fromWho;
}

const QString &
WidgetSender::whereStr() const
{
	return fromWhere;
}

