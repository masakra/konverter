
#include "WidgetAddition.h"

#include <QtGui>
#include "LineEdit.h"

WidgetAddition::WidgetAddition( QWidget * parent )
	: QWidget( parent )
{
	createWidgets();
}

void
WidgetAddition::createWidgets()
{
	editCity = new LineEdit( this );
	editCity->setToolTip( "Измени и нажми Enter" );

	connect( editCity, SIGNAL( returnPressed() ), SLOT( editReturned() ) );
	connect( editCity, SIGNAL( escapePressed() ), SLOT( editEscaped() ) );

	QLabel * labelCity = new QLabel( "&Город", this );

	labelCity->setBuddy( editCity );

	QGridLayout * layout = new QGridLayout( this );

	layout->addWidget( labelCity, 0, 0, Qt::AlignRight );
	layout->addWidget( editCity, 0, 1 );
}

void
WidgetAddition::setCity( const QString & city )
{
	cachedCity = city;

	editCity->setText( city );
}

void
WidgetAddition::editReturned()
{
	if ( editCity->text() == cachedCity )
		return;

	cachedCity = editCity->text().trimmed();

	emit cityChanged( cachedCity );
}

void
WidgetAddition::editEscaped()
{
	editCity->setText( cachedCity );
}

