
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

