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
#include <NaraPg>
#include "_.h"

ReportEdit::ReportEdit( const QDate & d, QWidget * parent )
	: Grid( parent ), m_date( d )
{
	setAttribute( Qt::WA_DeleteOnClose );

	setWindowTitle( QString("Данные за %1").arg( m_date.toString("dd.MM.yyyy") ) );
	setWindowIcon( QIcon(":/report.png") );

	horizontalHeader()->hide();
	verticalHeader()->hide();

	setModel( m_model = new QSqlQueryModel( this ) );

	connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
			SLOT( doubleClicked( const QModelIndex & ) ) );

	refresh();
}

void
ReportEdit::refresh( const QDateTime & key )
{
	m_model->setQuery( QString("SELECT "
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
		.arg( _dbPg ? QString("date( l.drec ) = '%1'").arg( m_date.toString("yyyy-MM-dd") ) :
				QString("substr( l.timestamp, 1, 10 ) = '%1'").arg( m_date.toString("yyyy-MM-dd") ) ) );

	hideColumn( 0 );	// datetime
	hideColumn( 5 );	// time

	resizeColumnsToContents();

	if ( key.isValid() ) {
		for ( int i = 0; i < m_model->rowCount(); ++i ) {
			if ( m_model->index( i, 0 ).data().toDateTime() == key ) {
				const QModelIndex index = m_model->index( i, 1 );
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

	const QDateTime key = m_model->index( row, 0 ).data().toDateTime();

	switch ( index.column() ) {
		case 1:
			if ( QMessageBox::question( this, "Подтверджение", "Удалить запись?",
						QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes ) {
				deleteRecord( key, row );
			}
			break;

		case 2:
			modifyNumText( key, row );
			break;

		case 3:
			modifyNum( key, row );
			break;

		case 4: {
			toggleZakaz( key );
			break;
		}

		default:
			;
	}
}

void
ReportEdit::deleteRecord( const QDateTime & key, int row )
{
	QDateTime prevKey;

	if ( row > 0 )
		m_model->index( row - 1, 0 ).data().toDateTime();

	PgQuery q;

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
	PgQuery q;

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

void
ReportEdit::modifyNumText( const QDateTime & key, int row )
{
	const QString oldText = m_model->index( row, 2 ).data().toString();

	bool ok;

	const QString newText = QInputDialog::getText( this, "Изменить", "Исх. / с/ф текст", QLineEdit::Normal,
			oldText, &ok );

	if ( ! ok || oldText == newText )
		return;

	PgQuery q;

	q.prepare( QString("UPDATE %1 SET "
				"num_text = :numt "
			"WHERE "
				"%2")
		.arg( _tableName( "log" ) )
		.arg( _dbPg ? "date_trunc( 'second', drec ) = :dt" :
				QString("timestamp = '%1'").arg( key.toString("yyyy-MM-dd hh:mm:ss") ) ) );

	q.bindValue(":numt", newText );

	if ( _dbPg )
		q.bindValue(":dt", key );

	if ( q.exec() )
		refresh( key );
	else
		_yell( q );
}

void
ReportEdit::modifyNum( const QDateTime & key, int row )
{
	const QString oldText = m_model->index( row, 3 ).data().toString();

	bool ok;

	const QString newText = QInputDialog::getText( this, "Изменить", "Исх. / с/ф", QLineEdit::Normal,
			oldText, &ok );

	if ( ! ok || oldText == newText )
		return;

	PgQuery q;

	q.prepare( QString("UPDATE %1 SET "
				"num = :num "
			"WHERE "
				"%2")
		.arg( _tableName( "log" ) )
		.arg( _dbPg ? "date_trunc( 'second', drec ) = :dt" :
				QString("timestamp = '%1'").arg( key.toString("yyyy-MM-dd hh:mm:ss") ) ) );

	q.bindValue(":num", newText );

	if ( _dbPg )
		q.bindValue(":dt", key );

	if ( q.exec() )
		refresh( key );
	else
		_yell( q );
}

