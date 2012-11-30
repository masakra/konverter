/***************************************************************************
 *   Copyright (C) 2012 Nordavia-RA                                        *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   ICQ: 124231040                                                        *
 *   jabber: masakra@jabber.ru                                             *
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

#include "ReportEdit.h"

#include <QtGui>
#include <QtSql>
#include "_.h"

#include <QDebug>

ReportEdit::ReportEdit( const QDate & d, QWidget * parent )
	: QTableView( parent ), date( d )
{
	setAttribute( Qt::WA_DeleteOnClose );

	setWindowTitle( QString("Данные за %1").arg( date.toString("dd.MM.yyyy") ) );

	horizontalHeader()->hide();
	verticalHeader()->hide();

	setSelectionBehavior( QAbstractItemView::SelectRows );
	setSelectionMode( QAbstractItemView::SingleSelection );
	setAlternatingRowColors( true );

	setModel( model = new QSqlQueryModel( this ) );

	connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
			SLOT( doubleClicked( const QModelIndex & ) ) );

	refresh();
}

void
ReportEdit::refresh( const QDateTime & key )
{
	model->setQuery( QString("SELECT "
			"%1, "			// datetime is a unique attribute
			"c.who, "
			"l.num_text, "
			"l.num, "
			"CASE WHEN l.zakaz = 0 THEN '' ELSE 'заказное' END, "
			"%2 "
		"FROM "
			"%3 l "
		"INNER JOIN "
			"%4 c ON l.contact_id = c.id "
		"WHERE "
			"%5 "
		"ORDER BY "
			"6 "	// by time
			)
		.arg( _dbPg ? "date_trunc( 'second', l.drec )" : "l.timestamp" )
		.arg( _dbPg ? "\"time\"( l.drec )" : "substr( l.timestamp, 12 )" )
		.arg( _tableName( "log" ) )
		.arg( _tableName( "contact" ) )
		.arg( _dbPg ? QString("date( l.drec ) = '%1'").arg( date.toString("yyyy-MM-dd") ) :
				QString("substr( l.timestamp, 1, 10 ) = '%1'").arg( date.toString("yyyy-MM-dd") ) ) );

	hideColumn( 0 );	// datetime
	hideColumn( 5 );	// time

	resizeColumnsToContents();

	if ( key.isValid() ) {
		for ( int i = 0; i < model->rowCount(); ++i ) {
			if ( model->index( i, 0 ).data().toDateTime() == key ) {
				const QModelIndex index = model->index( i, 1 );
				setCurrentIndex( index );
				scrollTo( index );
				break;
			}
		}
	}
}

void
ReportEdit::doubleClicked( const QModelIndex & index )
{
	const int row = index.row();

	const QDateTime key = model->index( row, 0 ).data().toDateTime();

	//qDebug() << key.toString("yyyy-MM-dd hh:mm:ss:zzz");

	if ( index.column() == 1 &&
			QMessageBox::question( this, "Подтверджение", "Удалить запись?",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes ) {

		deleteRecord( key, row );
		return;
	}

	if ( index.column() == 4 ) {
		toggleZakaz( key );
		return;
	}
}

void
ReportEdit::deleteRecord( const QDateTime & key, int row )
{
	QDateTime prevKey;

	if ( row > 0 )
		model->index( row - 1, 0 ).data().toDateTime();

	QSqlQuery q;

	q.prepare( QString("DELETE FROM "
			"%1 "
		"WHERE "
			"%2")
		.arg( _tableName( "log" ) )
		.arg( _dbPg ? "date_trunc( 'second', drec ) = :dt" :
				QString("timestamp = '%1'").arg( key.toString("yyyy-MM-dd hh:mm:ss") ) ) );

	if ( _dbPg )
		q.bindValue(":dt", key );

	if ( q.exec() )
		refresh( prevKey );
	else
		_yell( q );
}

void
ReportEdit::toggleZakaz( const QDateTime & key )
{
	QSqlQuery q;

	q.prepare( QString("UPDATE %1 SET "
			"zakaz = ABS( zakaz - 1 ) "
		"WHERE "
			"%2")
		.arg( _tableName( "log" ) )
		.arg( _dbPg ? "date_trunc( 'second', drec ) = :dt" :
				QString("timestamp = '%1'").arg( key.toString("yyyy-MM-dd hh:mm:ss") ) ) );

	if ( _dbPg )
		q.bindValue(":dt", key );

	if ( q.exec() )
		refresh( key );
	else
		_yell( q );
}

