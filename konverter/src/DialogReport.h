
#ifndef DIALOGREPORT_H
#define DIALOGREPORT_H

#include <QDialog>

class QDateTimeEdit;
class QRadioButton;

class DialogReport : public QDialog
{
	Q_OBJECT

	private:
		void createWidgets();

		QDateTimeEdit * editDate;

		QRadioButton * radioWho,
					 * radioTime;

	public:
		DialogReport( QWidget * parent = 0 );
};

#endif

