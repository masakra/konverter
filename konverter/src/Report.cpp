
#include "Report.h"

#include <QtGui>
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

	QTextEdit::show();
}
