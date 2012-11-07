
#ifndef REPORT_H
#define REPORT_H

#include <QTextEdit>

class DialogReport;

class Report : public QTextEdit
{
	private:
		void makeReport( const DialogReport & dialog );

	public:
		Report( QWidget * parent = 0 );

		void show( const DialogReport & dialog );
};

#endif

