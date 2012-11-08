/***************************************************************************
 *   Copyright (C) 2012 Nordavia-RA                                        *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   ICQ: 124231040                                                        *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   All comments, if not ascii, in koi8-r                                 *
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

#include "Report.h"

#include <QtGui>
#include <QtSql>
#include "_.h"
#include "DialogReport.h"

Report::Report( QWidget * parent )
	: QTextEdit( parent )
{
	setWindowIcon( QIcon(":/report.png") );

}

void
Report::show( const DialogReport & dialog )
{
	setWindowTitle( dialog.date().toString( "dd MMM yyyy" ) );

	makeReport( dialog );

	QTextEdit::show();
}

void
Report::makeReport( const DialogReport & dialog )
{
	QString text = QString("<H3><I>Реестр за %1</I></H3>")
		.arg( dialog.date().toString("dd.MM.yyyy") );

	text += QString("<TABLE CELLSPACING=1 CELLPADDING=7 BGCOLOR=%1>"
			  "<TR>"
			    "<TH BGCOLOR=%2>Адресат</TH>"
				"<TH BGCOLOR=%2>Исх. / с/ф</TH>"
				"<TH BGCOLOR=%2>Время</TH>"
				"<TH BGCOLOR=%2>Прим.</TH>"
			  "</TR>").arg( "lightskyblue", "paleturquoise" );

	QString orderColumn;

	switch ( dialog.orderBy() ) {
		case DialogReport::Who:
			orderColumn = "1";
			break;

		case DialogReport::Number:
			orderColumn = "3";
			break;

		default:
			orderColumn = "4";
	}

	QSqlQuery q;

	q.prepare( QString("SELECT "
			"c.who, "
			"l.num_text, "
			"l.num, "
			"substr( l.timestamp, 12 ), "
			"zakaz "
		"FROM "
			"log l "
		"INNER JOIN "
			"contact c ON c.id = l.contact_id "
		"WHERE "
			"substr( l.timestamp, 1, 10 ) = :date "
		"ORDER BY "
			"%1 ").arg( orderColumn ) );

	q.bindValue(":date", dialog.date().toString("yyyy-MM-dd") );

	if ( q.exec() ) {
		int row = 0;

		while ( q.next() ) {

			const QString bgcolor = row++ % 2 ? "white" : "azure";

			text += "<TR>";
			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  text += q.value( 0 ).toString();
			  text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  if ( ! q.value( 2 ).toString().trimmed().isEmpty() )
				  text += q.value( 1 ).toString() + " " + q.value( 2 ).toString();
			  text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  text += q.value( 3 ).toString();
			  text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  text += ( q.value( 4 ).toInt() == 1 ? "заказное" : "" );
			  text += "</TD>";

			text += "</TR>";
		}
	} else
		_yell( q );

	text += "</TABLE>";

	document()->setHtml( text );
}

