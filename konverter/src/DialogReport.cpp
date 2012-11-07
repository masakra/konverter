
#include "DialogReport.h"

#include <QtGui>

DialogReport::DialogReport( QWidget * parent )
	: QDialog( parent )
{
	setWindowTitle( "������" );
	createWidgets();
}

void
DialogReport::createWidgets()
{
	editDate = new QDateTimeEdit( QDate::currentDate(), this );
	editDate->setDisplayFormat( "dd.MM.yyyy" );

	radioWho = new QRadioButton( "�� ��������", this );
	radioWho->setChecked( true );

	radioIshod = new QRadioButton( "�� ����������", this );

	radioTime = new QRadioButton( "�� �������", this );

	QVBoxLayout * layoutOrder = new QVBoxLayout();

	layoutOrder->addWidget( radioWho );
	layoutOrder->addWidget( radioIshod );
	layoutOrder->addWidget( radioTime );

	QGroupBox * groupOrder = new QGroupBox( "�����������", this );

	groupOrder->setLayout( layoutOrder );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok |
			QDialogButtonBox::Cancel );

	connect( buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	QVBoxLayout * layout = new QVBoxLayout( this );

	layout->addWidget( editDate );
	layout->addWidget( groupOrder );
	layout->addWidget( buttonBox );
}

QDate
DialogReport::date() const
{
	return editDate->date();
}

DialogReport::OrderBy
DialogReport::orderBy() const
{
	if ( radioWho->isChecked() )
		return Who;
	else if ( radioIshod->isChecked() )
		return Number;
	else
		return Time;
}

