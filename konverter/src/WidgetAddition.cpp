
#include "WidgetAddition.h"

#include <QtGui>
#include <QtSql>
#include <NaraPg>
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
	m_editCity = new LineEdit( this );
	m_editCity->setToolTip( "Измени и нажми Enter" );

	connect( m_editCity, SIGNAL( returnPressed() ), SLOT( editReturned() ) );
	connect( m_editCity, SIGNAL( escapePressed() ), SLOT( editEscaped() ) );

	m_modelCompleter = new QStringListModel( this );

	m_editCity->setCompleter( new QCompleter( m_modelCompleter, m_editCity ) );

	QLabel * labelCity = new QLabel( "&Город", this );

	labelCity->setBuddy( m_editCity );

	QGridLayout * layout = new QGridLayout( this );

	layout->addWidget( labelCity, 0, 0, Qt::AlignRight );
	layout->addWidget( m_editCity, 0, 1 );
}

void
WidgetAddition::setCity( const QString & city )
{
	m_cachedCity = city;

	m_editCity->setText( city );
}

void
WidgetAddition::editReturned()
{
	if ( m_editCity->text() == m_cachedCity )
		return;

	m_cachedCity = m_editCity->text().trimmed();

	emit cityChanged( m_cachedCity );

	updateModelCompleter();
}

void
WidgetAddition::editEscaped()
{
	m_editCity->setText( m_cachedCity );
}

void
WidgetAddition::updateModelCompleter()
{
	PgQuery q;

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

		m_modelCompleter->setStringList( list );

	} else
		_yell( q );
}

