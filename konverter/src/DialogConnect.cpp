/***************************************************************************
 *   Copyright (C) 2008 by Chursanov Sergey Nikolaevich                    *
 *                                                                         *
 *   masakra@mail.ru                                                       *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include "DialogConnect.h"

#include <QtGui>
#include <QtSql>

DialogConnect::DialogConnect( QString title, QWidget * parent )
	: QDialog( parent )
{
	setWindowTitle( title );
	setWindowIcon( QIcon(":/connecting.png") );
	createWidgets();
	loadSettings();
	check();
}

void
DialogConnect::createWidgets()
{
	// картинка

	QLabel * labelImage = new QLabel( this );
	labelImage->setPixmap( QPixmap(":/nordavia_konverter.png") );
	// группа User

	QGroupBox * groupUser = new QGroupBox( "Пользователь" );
	QVBoxLayout * layoutUser = new QVBoxLayout( groupUser );
	layoutUser->addLayout( createLabeledEdit( &editUsername, "&Пользователь" ) );
	layoutUser->addLayout( createLabeledEdit( &editPassword, "&Пароль" ) );
	editPassword->setEchoMode( QLineEdit::Password );

	connect( editUsername, SIGNAL( textChanged( const QString ) ), SLOT( check() ) );

	// группа Server

	labelHint = new QLabel( this );
	labelHint->setEnabled( false );

	groupServer = new QGroupBox( "Сервер" );
	QVBoxLayout * layoutServer = new QVBoxLayout( groupServer );
	layoutServer->addLayout( createLabeledEdit( &editHost, "&Сервер" ) );
	layoutServer->addLayout( createLabeledEdit( &editPort, "&Порт" ) );
	layoutServer->addLayout( createLabeledEdit( &editDatabase, "База &данных" ) );
	groupServer->hide(); // при запуске не показывать

	connect( editHost, SIGNAL( textChanged( const QString & ) ), SLOT( check() ) );
	connect( editPort, SIGNAL( textChanged( const QString & ) ), SLOT( check() ) );
	connect( editDatabase, SIGNAL( textChanged( const QString & ) ), SLOT( check() ) );

	QVBoxLayout * layoutButtons = new QVBoxLayout();
	buttonOk = new QPushButton( tr("Ok") );
	connect( buttonOk, SIGNAL( clicked() ), SLOT( accept() ) );

	QPushButton * buttonClose = new QPushButton( "Закрыть" );
	connect( buttonClose, SIGNAL( clicked() ), SLOT( close() ) );

	buttonServer = new QPushButton( "Сервер" );
	buttonServer->setCheckable( true );
	connect( buttonServer, SIGNAL( toggled( bool ) ), groupServer,
			SLOT( setVisible( bool ) ) );
	connect( buttonServer, SIGNAL( toggled( bool ) ), labelHint,
			SLOT( setHidden( bool ) ) );

	layoutButtons->addWidget( buttonOk );
	layoutButtons->addWidget( buttonClose );
	layoutButtons->addWidget( buttonServer );

	QGridLayout * layoutMain = new QGridLayout( this );
	layoutMain->setSizeConstraint( QLayout::SetFixedSize );
	layoutMain->setContentsMargins( 5, 3, 5, 5 );
	layoutMain->setSpacing( 5 );

	layoutMain->addWidget( labelImage, 0, 0, 1, 2 );
	layoutMain->addWidget( groupUser, 1, 0 );
	layoutMain->addWidget( groupServer, 2, 0 );
	layoutMain->addLayout( layoutButtons, 1, 1 );
	layoutMain->addWidget( labelHint, 3, 0 );
}

void
DialogConnect::loadSettings()
{
	QSettings ini;
	ini.beginGroup( "login" );
		editUsername->setText( ini.value("user").toString() );
	ini.endGroup();
	ini.beginGroup( "server" );
		editHost->setText( ini.value( "host", "localhost" ).toString() );
		editPort->setText( ini.value( "port", "5432" ).toString() );
		editDatabase->setText( ini.value("database").toString() );
	ini.endGroup();

	setLabelHint();

	// если имя не пустое - фокус ввода на пароль
	if ( ! editUsername->text().isEmpty() ) editPassword->setFocus();

	// если хотя бы один параметр соединения пустой, показать группу, спрятать метку
	if ( editHost->text().isEmpty() || editPort->text().isEmpty() || editDatabase->text().isEmpty() ) {
		buttonServer->setChecked( true );
	}
}

void
DialogConnect::saveSettings() const
{
	QSettings	ini;
	ini.beginGroup( "login" );
		ini.setValue( "user", editUsername->text() );
	ini.endGroup();
	ini.beginGroup( "server" );
		ini.setValue( "host", editHost->text() );
		ini.setValue( "port", editPort->text() );
		ini.setValue( "database", editDatabase->text() );
	ini.endGroup();
}

/*
void
DialogConnect::doConnect()
{
	QSqlDatabase db = QSqlDatabase::addDatabase( "QPSQL" );

	db.setHostName( editHost->text() );
	db.setPort( editPort->text().toInt() );
	db.setDatabaseName( editDatabase->text() );
	db.setUserName( editUsername->text() );
	db.setPassword( editPassword->text() );

	if ( db.open() ) {
		saveSettings();
		accept();
	} else {
		QMessageBox::critical( this, "Error", db.lastError().text() );
	}
}
*/

QHBoxLayout *
DialogConnect::createLabeledEdit( QLineEdit ** edit, const QString labelText )
{
	( *edit ) = new QLineEdit( this );

	QLabel * label = new QLabel( labelText, this );

	label->setBuddy( *edit );

	QHBoxLayout * layout = new QHBoxLayout();

	layout->addWidget( label );
	layout->addStretch();
	layout->addWidget( *edit );

	return layout;
}

void
DialogConnect::check()
{
	buttonOk->setEnabled( ( ! editUsername->text().isEmpty() ) &&
			( ! editHost->text().isEmpty() ) &&
			( ! editPort->text().isEmpty() ) &&
			( ! editDatabase->text().isEmpty() ) );

	setLabelHint();
}

void
DialogConnect::setLabelHint()
{
	labelHint->setText( editHost->text() + " " +
						editPort->text() + " " +
						editDatabase->text() );
}

QString
DialogConnect::hostName() const
{
	return editHost->text();
}

QString
DialogConnect::port() const
{
	return editPort->text();
}

QString
DialogConnect::databaseName() const
{
	return editDatabase->text();
}

QString
DialogConnect::userName() const
{
	return editUsername->text();
}

QString
DialogConnect::password() const
{
	return editPassword->text();
}

