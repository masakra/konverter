
#include "Report.h"

#include <QtGui>

Report::Report( QWidget * parent )
	: QTextEdit( parent )
{
	setWindowIcon( QIcon(":/report.png") );
}

void
Report::show( const DialogReport & dialog )
{

	QTextEdit::show();
}
