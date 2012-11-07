
#ifndef DIALOGREPORT_H
#define DIALOGREPORT_H

#include <QDialog>

#include <QDate>

class QDateTimeEdit;
class QRadioButton;

class DialogReport : public QDialog
{
	Q_OBJECT

	public:
		enum OrderBy {
			Who,
			Number,
			Time
		};

	private:
		void createWidgets();

		QDateTimeEdit * editDate;

		QRadioButton * radioWho,
					 * radioIshod,
					 * radioTime;

	public:
		DialogReport( QWidget * parent = 0 );

		QDate date() const;

		OrderBy orderBy() const;
};

#endif

