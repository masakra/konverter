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

	printer.setPageMargins( 5, 5, 5, 5, QPrinter::Millimeter );
}

void
Report::show( const DialogReport & dialog )
{
	setWindowTitle( dialog.date().toString( "dd MMM yyyy" ) );

	defaultFileName = QString("konverter_%1_report").arg( dialog.date().toString("dd_MM_yyyy" ) );

	makeReport( dialog );

	QTextEdit::show();
}

void
Report::makeReport( const DialogReport & dialog )
{

	QString text = QString("<TABLE BORDER=0 BGCOLOR=white CELLSPACING=0 WIDTH=700>"
		"<TR>"
		  "<TD ALIGN=left>"
		    "<FONT SIZE=1 COLOR=lightgray>ЗАО %1Нордавиа-РА%2</FONT>"
		  "</TD>"
		  "<TD ALIGN=right>"
		    "<FONT SIZE=1 COLOR=lightgray>%3 v.%4 %5</FONT>"
		  "</TD>"
		"</TR>")
		.arg( QChar( 0xAB ) )
		.arg( QChar( 0xBB ) )
		.arg( qApp->applicationName(), qApp->applicationVersion() )
		.arg( QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") );


	text += QString("<TR>"
			  "<TD COLSPAN=2 ALIGN=center>"
			    "<H3><I>Реестр за %1</I></H3><BR>"
			  "</TD>"
			"</TR>")
		.arg( dialog.date().toString("dd.MM.yyyy") );

	text += QString(
			"<TR>"
			"<TD COLSPAN=2>"
			"<TABLE CELLSPACING=1 CELLPADDING=7 BGCOLOR=%1 WIDTH=100%>"
			  "<TR>"
			    "<TH BGCOLOR=%2>%3</TH>"
			    "<TH BGCOLOR=%2>Город</TH>"
			    "<TH BGCOLOR=%2>Адресат</TH>"
				"<TH BGCOLOR=%2>Исх. / с/ф</TH>"
				//"<TH BGCOLOR=%2>Время</TH>"
				"<TH BGCOLOR=%2>Прим.</TH>"
			  "</TR>")
		.arg( "lightskyblue", "paleturquoise" )
		.arg( QChar( 0x2116 ) );

	QString orderColumn;

	switch ( dialog.orderBy() ) {
		case DialogReport::Who:
			orderColumn = "1";
			break;

		case DialogReport::City:
			orderColumn = "2";
			break;

		case DialogReport::Number:
			orderColumn = "4";
			break;

		case DialogReport::Time:
		default:
			orderColumn = "5";	// по-умолчанию по городу
	}

	QSqlQuery q;

	q.prepare( QString("SELECT "
			"c.who, "
			"c.city, "
			"l.num_text, "
			"l.num, "
			"%1, "
			"zakaz "
		"FROM "
			"%2 l "
		"INNER JOIN "
			"%3 c ON c.id = l.contact_id "
		"WHERE "
			"%4 "//"substr( l.timestamp, 1, 10 ) = :date "
		"ORDER BY "
			"%5 ")
			.arg( _dbPg ? "\"time\"( drec )" : "substr( l.timestamp, 12 )" )
			.arg( tableName( "log" ) )
			.arg( tableName( "contact" ) )
			.arg( _dbPg ? "date( drec ) = :date" : "substr( l.timestamp, 1, 10 ) = :date " )
			.arg( orderColumn ) );

	q.bindValue(":date", dialog.date().toString("yyyy-MM-dd") );

	if ( q.exec() ) {
		int row = 0;

		while ( q.next() ) {

			const QString bgcolor = row++ % 2 ? "white" : "azure";

			text += "<TR>";
			  text += QString("<TD BGCOLOR=%1 ALIGN=center>").arg( bgcolor );
			  text += QString::number( row );
			  text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  text += q.value( 1 ).toString();
			  text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  text += q.value( 0 ).toString();
			  text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  if ( ! q.value( 3 ).toString().trimmed().isEmpty() )
				  text += q.value( 2 ).toString() + " " + q.value( 3 ).toString();
			  text += "</TD>";

			  //text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  //text += q.value( 3 ).toString();
			  //text += "</TD>";

			  text += QString("<TD BGCOLOR=%1>").arg( bgcolor );
			  text += ( q.value( 5 ).toInt() == 1 ? "заказное" : "" );
			  text += "</TD>";

			text += "</TR>";
		}
	} else
		_yell( q );

	text += "</TABLE>"
		    "</TD>"
		  "</TR>"
		"</TABLE>";

	document()->setHtml( text );
}

void
Report::contextMenuEvent( QContextMenuEvent * event )
{
	QMenu * menu = createStandardContextMenu();

	QAction * actionPrint = menu->addAction( "Печатать..." );
	actionPrint->setShortcut( Qt::CTRL + Qt::Key_P );
	connect( actionPrint, SIGNAL( triggered() ), SLOT( printToPrinter() ) );

	QAction * actionToPdf = menu->addAction( "Сохранить в PDF..." );
	actionToPdf->setShortcut( Qt::CTRL + Qt::Key_E );
	connect( actionToPdf, SIGNAL( triggered() ), SLOT( saveToPdf() ) );

	menu->exec( event->globalPos() );

	delete actionToPdf;
	delete actionPrint;
	delete menu;
}

void
Report::printToPrinter()
{
	QPrintDialog pd( &printer );

	if ( pd.exec() == QDialog::Accepted ) {

		printer.setOutputFormat( QPrinter::NativeFormat );
		print( &printer );
	}
}

void
Report::saveToPdf()
{
	QSettings settings;

	QString fileName = QFileDialog::getSaveFileName( 0, "Сохранит в PDF",
			settings.value( DIR_PATH, ".").toString() +
			QDir::separator() + defaultFileName,
			tr("PDF files (*.pdf)"));

	if ( ! fileName.isEmpty() ) {
		// save last saving dir
		QFileInfo fileInfo( fileName );

		settings.setValue( DIR_PATH, fileInfo.dir().path() );

		printer.setOutputFormat( QPrinter::PdfFormat );
		printer.setOutputFileName( fileName );
		print( &printer );
	}
}




