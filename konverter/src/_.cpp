
#include "_.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

void
_yell( const QString & text )
{
	QMessageBox::critical( 0, "The message", text );
}

void
_yell( const QSqlQuery & query )
{
	_yell( query.lastError().text() );
}

