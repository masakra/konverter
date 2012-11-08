
#include "SqlQueryModel.h"

SqlQueryModel::SqlQueryModel( QObject * parent )
	: QSqlQueryModel( parent )
{
}

void
SqlQueryModel::fetchMore( const QModelIndex & parent )
{
	QSqlQueryModel::fetchMore( parent );

	emit fetchedMore( rowCount() );
}

