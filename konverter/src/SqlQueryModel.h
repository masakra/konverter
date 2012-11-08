
#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>

class SqlQueryModel : public QSqlQueryModel
{
	Q_OBJECT

	protected:
		virtual void fetchMore( const QModelIndex & parent );

	public:
		SqlQueryModel( QObject * parent );

	Q_SIGNALS:
		void fetchedMore( int rows );
};

#endif

