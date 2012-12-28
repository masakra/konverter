
#include "WidgetAddition.h"

#include <QtGui>
#include <QtSql>
#include "LineEdit.h"
#include "_.h"

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

	modelCompleter = new QStringListModel( this );

	editCity->setCompleter( new QCompleter( modelCompleter, editCity ) );

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

	updateModelCompleter();
}

void
WidgetAddition::editEscaped()
{
	editCity->setText( cachedCity );
}

void
WidgetAddition::updateModelCompleter()
{
	QSqlQuery q;

	q.prepare( QString("SELECT DISTINCT "
			"trim( %1( city ) ) "
		"FROM "
			"%2 "
		"ORDER BY 1")
			.arg( _dbPg ? "initcap": "" )
			.arg( _tableName( "contact" ) ) );

	if ( q.exec() ) {

		QStringList list;

		while ( q.next() )
			list << q.value( 0 ).toString();

		modelCompleter->setStringList( list );

	} else
		_yell( q );
}

