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

#include "DialogReport.h"

#include <QtGui>

DialogReport::DialogReport( QWidget * parent )
	: QDialog( parent )
{
	setWindowTitle("Журнал");
	setWindowIcon( QIcon(":/report.png") );
	createWidgets();
}

void
DialogReport::createWidgets()
{
	m_editDate = new QDateTimeEdit( QDate::currentDate(), this );
	m_editDate->setDisplayFormat("dd.MM.yyyy");

	m_radioWho = new QRadioButton("по адресату", this );
	m_radioWho->setChecked( true );

	m_radioIshod = new QRadioButton("по исходящему", this );

	m_radioTime = new QRadioButton("по времени", this );

	m_radioCity = new QRadioButton("по городу", this );

	QVBoxLayout * layoutOrder = new QVBoxLayout();

	layoutOrder->addWidget( m_radioWho );
	layoutOrder->addWidget( m_radioIshod );
	layoutOrder->addWidget( m_radioTime );
	layoutOrder->addWidget( m_radioCity );

	QGroupBox * groupOrder = new QGroupBox("Сортировать", this );

	groupOrder->setLayout( layoutOrder );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok |
			QDialogButtonBox::Cancel );

	connect( buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	QVBoxLayout * layout = new QVBoxLayout( this );

	layout->addWidget( m_editDate );
	layout->addWidget( groupOrder );
	layout->addWidget( buttonBox );
}

QDate
DialogReport::date() const
{
	return m_editDate->date();
}

DialogReport::OrderBy
DialogReport::orderBy() const
{
	if ( m_radioWho->isChecked() )
		return Who;

	else if ( m_radioIshod->isChecked() )
		return Number;

	else if ( m_radioTime->isChecked() )
		return Time;

	else
		return City;
}

