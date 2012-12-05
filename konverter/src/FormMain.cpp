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

#include "FormMain.h"

#include <QtGui>
#include <QtSql>
#include "_.h"
#include "DialogConnect.h"
#include "DialogReport.h"
#include "DialogReportEdit.h"
#include "Report.h"
#include "ReportEdit.h"
#include "SqlQueryModel.h"
#include "WidgetAddition.h"
#include "WidgetRecipient.h"
#include "WidgetSender.h"

#ifdef Q_OS_WIN
#include "qt_windows.h"
#endif

FormMain::FormMain( QWidget * parent )
	: QMainWindow( parent )//, dbPg( false )
{
	setWindowTitle( qApp->applicationName() + " - no connection" );

	createWidgets();

	createPrinter();

	loadEnvelopes();

	loadSettings();
}

FormMain::~FormMain()
{
	delete printer;

	saveEnvelopeType();
}

void
FormMain::createModel()
{
	model = new SqlQueryModel( this );

	connect( model, SIGNAL( fetchedMore( int ) ), SLOT( setCount( int ) ) );

	refresh();
}

void
FormMain::refresh( int id )
{
	setStatus( Normal );

	model->setQuery( QString("SELECT "
			"id, "
			"who, "
			"whe, "
			"ind, "
			"city "
		"FROM "
			"%1 "
		"WHERE "
			"id != 0 "	// 0 is a sender
			"%2 "
		"ORDER BY "
			"touch DESC ")
			.arg( _tableName( "contact" ) )
			.arg( editFilter->text().isEmpty() ? "":
				QString("AND ( UPPER( who ) like '%%1%' "
						   "OR UPPER( ind ) like '%%1%' )").arg( editFilter->text().toUpper() ) ) );

	table->setModel( model );
	table->hideColumn( 0 );
	table->hideColumn( 2 );
	table->hideColumn( 3 );
	table->hideColumn( 4 );

	connect( table->selectionModel(), SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ),
				SLOT( contactChanged( const QModelIndex &, const QModelIndex & ) ) );

	if ( id != -1 ) {
		for ( int i = 0; i < model->rowCount(); ++i ) {
			if ( model->index( i, 0 ).data().toInt() == id ) {
				const QModelIndex index = model->index( i, 1 );
				table->setCurrentIndex( index );
				table->scrollTo( index );
				break;
			}
		}
	}
}

void
FormMain::createWidgets()
{
	QWidget * centralWidget = new QWidget( this );

	sender = new WidgetSender( centralWidget );

	connect( sender, SIGNAL( whoChanged( const QString & ) ),
			SLOT( modifySenderWho( const QString & ) ) );
	connect( sender, SIGNAL( whereChanged( const QString & ) ),
			SLOT( modifySenderWhere( const QString & ) ) );
	connect( sender, SIGNAL( indexChanged( const QString & ) ),
			SLOT( modifySenderIndex( const QString & ) ) );
	connect( sender, SIGNAL( addressFontChanged( const QFont & ) ),
			SLOT( saveSenderAddressFont( const QFont & ) ) );

	labelCount = new QLabel( centralWidget );

	table = new QTableView( centralWidget );
	table->horizontalHeader()->hide();
	table->verticalHeader()->hide();
	table->horizontalHeader()->setStretchLastSection( true );
	table->setSelectionBehavior( QAbstractItemView::SelectRows );
	table->setSelectionMode( QAbstractItemView::SingleSelection );
	table->setAlternatingRowColors( true );
	table->resize( 300, 100 );

	editFilter = new QLineEdit( centralWidget );
	editFilter->setToolTip( "Фильтр по имени или почтовому индексу" );

	connect( editFilter, SIGNAL( textChanged( const QString & ) ),
			SLOT( filterChanged( const QString & ) ) );

	QVBoxLayout * layoutTable = new QVBoxLayout();

	layoutTable->addWidget( labelCount );
	layoutTable->addWidget( table );
	layoutTable->addWidget( editFilter );

	buttonAdd = new QToolButton( centralWidget );
	buttonDel = new QToolButton( centralWidget );

	QToolButton * buttonPrint = new QToolButton( centralWidget ),
				* buttonReport = new QToolButton( centralWidget ),
				* buttonAbout = new QToolButton( centralWidget );

	buttonPrint->setIconSize( QSize( 97, 61 ) );
	buttonPrint->setIcon( QIcon(":/postmark.png") );
	buttonPrint->setToolTip( "Печать, c Ctrl - выбор принтера" );

	buttonAdd->setIconSize( QSize( 48, 48 ) );
	buttonAdd->setIcon( QIcon(":/add.png") );
	buttonAdd->setToolTip( "Добавить контакт" );

	buttonDel->setIconSize( QSize( 48, 48 ) );
	buttonDel->setIcon( QIcon(":/delete.png") );
	buttonDel->setToolTip( "Удалить контакт" );

	buttonReport->setIconSize( QSize( 48, 48 ) );
	buttonReport->setIcon( QIcon( ":/report.png") );
	buttonReport->setToolTip( "Сформировать реестр за дату, с Ctrl - редактировать реестр на дату" );

	buttonAbout->setIconSize( QSize( 48, 48 ) );
	buttonAbout->setIcon( qApp->windowIcon() );
	buttonAbout->setToolTip( "О программе" );

	connect( buttonAdd, SIGNAL( clicked() ), SLOT( addContact() ) );

	connect( buttonDel, SIGNAL( clicked() ), SLOT( delContact() ) );

	connect( buttonReport, SIGNAL( clicked() ), SLOT( report() ) );

	connect( buttonAbout, SIGNAL( clicked() ), SLOT( about() ) );

	connect( buttonPrint, SIGNAL( clicked() ), SLOT( print() ) );

	comboPaperSize = new QComboBox( centralWidget );

	comboIshod = new QComboBox( centralWidget );
	comboIshod->addItem( "Исходящий", QString("Исх. %1").arg( QChar( 0x2116 ) ) );
	comboIshod->addItem( "Счёт-фактура", "с/ф" );
	comboIshod->addItem( "Счёт", "cчёт" );

	editIshod = new QLineEdit( centralWidget );

	checkZakaz = new QCheckBox( "&заказное", centralWidget );

	QHBoxLayout * layoutControls = new QHBoxLayout(),
				* layoutIshod = new QHBoxLayout(),
				* layoutButtons = new QHBoxLayout();

	layoutControls->setMargin( 15 );
	layoutButtons->setMargin( 15 );

	layoutControls->addStretch();
	layoutControls->addWidget( comboPaperSize );
	layoutControls->addStretch();
	layoutControls->addWidget( buttonPrint );

	layoutIshod->addStretch();
	layoutIshod->addWidget( comboIshod );
	layoutIshod->addWidget( editIshod );
	layoutIshod->addWidget( checkZakaz );
	layoutIshod->addStretch();

	recipient = new WidgetRecipient( centralWidget );

	connect( recipient, SIGNAL( whoChanged( const QString & ) ),
			SLOT( modifyRecipientWho( const QString & ) ) );
	connect( recipient, SIGNAL( whereChanged( const QString & ) ),
			SLOT( modifyRecipientWhere( const QString & ) ) );
	connect( recipient, SIGNAL( indexChanged( const QString & ) ),
			SLOT( modifyRecipientIndex( const QString & ) ) );
	connect( recipient, SIGNAL( addressFontChanged( const QFont & ) ),
			SLOT( saveRecipientAddressFont( const QFont & ) ) );

	addition = new WidgetAddition( this );

	connect( addition, SIGNAL( cityChanged( const QString & ) ),
			SLOT( modifyRecipientCity( const QString & ) ) );

	layoutButtons->addStretch();
	layoutButtons->addWidget( buttonAdd );
	layoutButtons->addWidget( buttonDel );
	layoutButtons->addWidget( buttonReport );
	layoutButtons->addStretch();
	layoutButtons->addWidget( buttonAbout );
	layoutButtons->addStretch();

	QGridLayout * gridLayout = new QGridLayout( centralWidget );

	gridLayout->addWidget( sender, 0, 0, 2, 1 );
	gridLayout->addLayout( layoutControls, 0, 1, 1, 1, Qt::AlignTop );

	gridLayout->addLayout( layoutIshod, 1, 1 );

	gridLayout->addLayout( layoutTable, 2, 0, 3, 1 );
	gridLayout->addWidget( recipient, 2, 1 );

	gridLayout->addWidget( addition, 3, 1 );

	gridLayout->addLayout( layoutButtons, 4, 1 );

	setCentralWidget( centralWidget );
}

void
FormMain::setSenderData()
{
	QSqlQuery q;

	q.prepare( QString("SELECT "
			"who, "
			"whe, "
			"ind "
		"FROM "
			"%1 "
		"WHERE "
			"id = 0")	// not id as variant )))
			.arg( _tableName( "contact" ) ) );

	if ( q.exec() ) {
		if ( q.first() )
			sender->setAll( q.value( 0 ).toString(),
					q.value( 1 ).toString(),
					q.value( 2 ).toString() );
		else
			_yell("Запись отправителя (id=0) не найдена");

	} else
		_yell( q );

}

void
FormMain::contactChanged( const QModelIndex & current, const QModelIndex & )
{
	setStatus( Normal );

	const int row = current.row();

	const QString who = model->index( row, 1 ).data().toString(),
				  where = model->index( row, 2 ).data().toString(),
				  index = model->index( row, 3 ).data().toString(),
				  city = model->index( row, 4 ).data().toString();

	recipient->setAll( who, where, index );

	addition->setCity( city );
}

void
FormMain::setCount( int rows )
{
	QString s( "записей: " );

	if ( model->canFetchMore() )
		s += "более ";

	labelCount->setText( s += QString::number( rows ) );
}

void
FormMain::loadSettings()
{
	QSettings s;

	loadFont( s, sender, "sender" );
	loadFont( s, recipient, "recipient" );

	const int e_type = s.value( "envelope_type", 0 ).toInt();

	for ( int i = 0; comboPaperSize->count(); ++i )
		if ( comboPaperSize->itemData( i ) == e_type ) {
			comboPaperSize->setCurrentIndex( i );
			break;
		}
}

void
FormMain::loadFont( QSettings & settings, WidgetContact * contact, const QString & prefix )
{
	const QString key = prefix + "_font_";

	QFont font( settings.value( key + "family",
#ifdef Q_OS_WIN32
			"Times"
#else
			"Droid Serif"
#endif
				).toString() );

	font.setPointSize( settings.value( key + "size", 12 ).toInt() );
	font.setBold( settings.value( key + "bold", false ).toBool() );
	font.setItalic( settings.value( key + "italic", true ).toBool() );

	contact->setAddressFont( font );
}

void
FormMain::print()
{
	Qt::KeyboardModifiers modifiers = qApp->keyboardModifiers();

	if ( modifiers & Qt::ControlModifier ) {

		QPrintDialog d( printer, this );

		if ( d.exec() != QDialog::Accepted )
			return;
	}

	if ( editIshod->text().trimmed().isEmpty() ) {
		if ( QMessageBox::warning( this, "Предупреждение",
				"Не заполнено поле " + comboIshod->currentText() +
				".\nПродолжить?",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::No )
			return;
	}

	if ( comboPaperSize->count() == 0 )
		return;

	int e_id = comboPaperSize->itemData( comboPaperSize->currentIndex() ).toInt();

	const Envelope & e = envelopes[ e_id ];

	if ( e.log() )
		writeLog();

	if ( printSide( e, 1 ) ) {

		if ( QMessageBox::information( this, "Вторая сторона",
				"Печать второй стороны.",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
			printSide( e, 2 );
	}
}

bool
FormMain::printSide( const Envelope & e, int side )
{
	// sizes checking
	QStringList senderWho = sender->whoList( e.senderWidth() ),
				senderWhere = sender->whereList( e.senderWidth() ),
				recipientWho = recipient->whoList( e.recipientWidth() ),
				recipientWhere = recipient->whereList( e.recipientWidth() );

	if ( ! checkSize( senderWho, e.senderWhoRowCount(), "От кого отправителя" ) ||
			! checkSize( senderWhere, e.senderWhereRowCount(), "Откуда отправителя" ) ||
			! checkSize( recipientWho, e.recipientWhoRowCount(), "Кому адресата" ) ||
			! checkSize( recipientWhere, e.recipientWhereRowCount(), "Куда адресата" ) )
		return false;

	/// printing

	QPainter painter;

	painter.begin( printer );

	if ( ! e.doubleSide() || side == 1 ) {

		// sender data
		painter.setFont( sender->addrFont() );
		painter.setPen( QPen( Qt::black ) );

		painter.save();			// sender from
		painter.translate( e.senderX(), e.senderY() );
		painter.rotate( -90 );

		// who
		for ( int i = 0; i < senderWho.size() && i < e.senderWhoRowCount(); ++i )
			painter.drawText( 0, qRound( i * e.rowHeight() ), senderWho[ i ] );

		// where
		for ( int i = 0; i < senderWhere.size() && i < e.senderWhereRowCount(); ++i )
			painter.drawText( 0, ( i + e.senderWhoRowCount() ) * e.rowHeight(), senderWhere[ i ] );

		// исходящий номер
		QString ishod = editIshod->text().trimmed();

		if ( ! ishod.isEmpty() ) {
			ishod.prepend(" ").prepend( comboIshod->itemData( comboIshod->currentIndex() ).toString() );

			painter.drawText( 0,
					( e.senderWhoRowCount() + e.senderWhereRowCount() ) * e.rowHeight(),
					ishod );

			editIshod->clear();
			checkZakaz->setChecked( false );
		}

		// index
		painter.drawText( e.senderIndexMargin(),
				( e.senderWhoRowCount() + e.senderWhereRowCount() ) * e.rowHeight() +
					e.senderIndexTopOffset(),
				sender->getIndex() );

		painter.restore();
	}

	if ( ! e.doubleSide() || side == 2 ) {
		// recipient data
		painter.setFont( recipient->addrFont() );
		painter.setPen( QPen( Qt::black ) );

		painter.save();
		painter.translate( e.recipientX(), e.recipientY() );
		painter.rotate( -90 );

		// who
		for ( int i = 0; i < recipientWho.size() && i < e.recipientWhoRowCount(); ++i )
			painter.drawText( 0, i * e.rowHeight(), recipientWho[ i ] );

		// where
		for ( int i = 0; i < recipientWhere.size() && i < e.recipientWhereRowCount(); ++i )
			painter.drawText( 0, ( i + e.recipientWhoRowCount() ) * e.rowHeight(),
					recipientWhere[ i ] );

		// index
		painter.drawText( e.recipientIndexMargin(),
				( e.recipientWhoRowCount() + e.recipientWhereRowCount() ) * e.rowHeight() +
					e.recipientIndexTopOffset(),
				recipient->getIndex() );

		painter.restore();
	}

	if ( e.doubleSide() && side == 1 )
		return true;
	else
		return false;
}

void
FormMain::createPrinter()
{
	printer = new QPrinter( QPrinter::HighResolution );

	printer->setOrientation( QPrinter::Portrait );
	printer->setPaperSize( QPrinter::A4 );
	printer->setResolution( 300 );	// dpi

}

void
FormMain::modifyContact( const QString & field, const QString & text, int id ) const
{
	QSqlQuery q;

	q.prepare( QString("UPDATE "
			"%1 "
		"SET "
			"%2 = :text "
		"WHERE "
			"id = :id")
			.arg( _tableName( "contact" ) )
			.arg( field ) );

	q.bindValue(":text", text );
	q.bindValue(":id", id );

	if ( ! q.exec() )
		_yell( q );
}

void
FormMain::modifySenderWho( const QString & text ) const
{
	modifyContact( "who", text );
}

void
FormMain::modifySenderWhere( const QString & text ) const
{
	modifyContact( "whe", text );
}

void
FormMain::modifySenderIndex( const QString & text ) const
{
	modifyContact( "ind", text );
}

void
FormMain::modifyRecipient( const QString & field, const QString & text )
{
	if ( State == NewContact ) {
		const int newId = insertContact( field, text );
		refresh( newId );
	} else {
		const int id = currentId();
		if ( id != -1 ) {
			modifyContact( field, text, id );
			refresh( id );
		} else {
			QMessageBox::warning( this, "Предупреждение",
					"Не выбрана ни одна запись.\n"
					"Изменения будут проигнорированы." );
		}
	}
}

void
FormMain::modifyRecipientWho( const QString & text )
{
	modifyRecipient( "who", text );
}

void
FormMain::modifyRecipientWhere( const QString & text )
{
	modifyRecipient( "whe", text );
}

void
FormMain::modifyRecipientIndex( const QString & text )
{
	modifyRecipient( "ind", text );
}

void
FormMain::modifyRecipientCity( const QString & text )
{
	modifyRecipient( "city", text );
}

int
FormMain::insertContact( const QString & field, const QString & text ) const
{
	QString sql( QString("INSERT INTO %1 ( who, whe, ind, city, touch ) VALUES (" )
			.arg( _tableName( "contact" ) ) );

	QSqlQuery q;

	if ( field == "who" )
		sql += ":text, '', '', ''";

	else if ( field == "whe" )
		sql += "'', :text, '', ''";

	else if ( field == "ind" )
		sql += "'', '', :text, ''";

	else if ( field == "city" )
		sql += "'', '', '', :text";

	else {
		_yell( QString("Неизвестное поле: %1").arg( field ) );
		return -1;
	}

	if ( _dbPg )
		q.prepare( sql + ", \"now\"() ) RETURNING id " );
	else
		q.prepare( sql + ", \"current_timestamp\"() )" );

	q.bindValue(":text", text );

	if ( q.exec() ) {
		if ( _dbPg ) {
			if ( q.first() )
				return q.value( 0 ).toInt();
			else
				_yell( "No returning id" );

		} else {
			q.prepare("SELECT last_insert_rowid()");

			if ( q.exec() && q.first() )
				return q.value( 0 ).toInt();
			else
				_yell( q );
		}

	} else
		_yell( q );

	return -1;
}

void
FormMain::addContact()
{
	setStatus( NewContact );

	recipient->setAll( "", "", "" );
	recipient->setWho();
}

void
FormMain::delContact()
{
	if ( ! table->selectionModel() )
		return;

	if ( QMessageBox::question( this, "Подтвердите", "Действительно удалить контакт?",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		return;

	QSqlQuery q;

	// TODO удалить все записи из журнала, или не надо, ON DELETE CASCADE, проверить

	q.prepare( QString("DELETE FROM "
				"%1 "
			"WHERE "
				"id = :id")
			.arg( _tableName( "contact" ) ) );

	q.bindValue(":id", currentId() );

	if ( q.exec() )
		refresh();
	else
		_yell( q );

}

int
FormMain::currentId() const
{
	if ( table->selectionModel() ) {

		const int row = table->selectionModel()->currentIndex().row();

		if ( row != -1 )
			return model->index( row, 0 ).data().toInt();
	}

	return -1;
}

void
FormMain::about()
{
	QMessageBox::about( this, "О программе",
			QString("<H3>%1 v.%2</H3>").arg( qApp->applicationName(), qApp->applicationVersion() ) +
				"2012 г. ЗАО " + QChar(0xAB) + QString("Нордавиа-РА") + QChar(0xBB) + "<BR><BR>" +
				"e-mail: <A HREF='mailto:masakra@mail.ru'>masakra@mail.ru</A><BR>"
				"ICQ: 124231040<BR>"
				"jabber: masakra@jabber.ru" );
}

void
FormMain::filterChanged( const QString & /*text*/ )
{
	refresh( currentId() );
}

void
FormMain::report()
{
	Qt::KeyboardModifiers modifiers = qApp->keyboardModifiers();

	if ( modifiers & Qt::ControlModifier ) {
		DialogReportEdit d;

		if ( d.exec() ) {
			ReportEdit * repEdit = new ReportEdit( d.date() );
			repEdit->resize( 600, 450 );
			repEdit->show();
		}

	} else {

		DialogReport d;

		if ( d.exec() ) {
			Report * rep = new Report();
			rep->resize( 600, 450 );
			rep->show( d );
		}
	}
}

void
FormMain::writeLog() const
{
	const int id = currentId();

	if ( id == -1 )
		return;

	QSqlQuery q;

	q.prepare( QString("INSERT INTO %1 ("
			"contact_id, "
			"num_text, "
			"num, "
			"zakaz "
		") VALUES ("
			":id, "
			":numt, "
			":num, "
			":zak )")
			.arg( _tableName( "log" ) ) );

	q.bindValue(":id", id );
	q.bindValue(":numt", comboIshod->itemData( comboIshod->currentIndex() ).toString() );
	q.bindValue(":num", editIshod->text().trimmed() );
	q.bindValue(":zak", checkZakaz->isChecked() ? 1 : 0 );

	if ( ! q.exec() )
		_yell( q );
}

void
FormMain::setStatus( Status s )
{
	if ( State == s )
		return;

	State = s;

	switch ( s ) {
		case Normal:
			buttonAdd->setEnabled( true );
			buttonDel->setEnabled( true );
			break;

		case NewContact:
			buttonAdd->setEnabled( false );
			buttonDel->setEnabled( false );
			break;
	}
}

void
FormMain::loadEnvelopes()
{
	QFile file( dataPath() + "envelope.conf");

	if ( ! file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		_yell( file.errorString() );
		return;
	}

	QTextStream in( &file );
	in.setCodec( "UTF-8" );

	while ( ! in.atEnd() ) {
		const QString line = in.readLine().trimmed();

		if ( line[ 0 ] == '#' )
			continue;

		if ( line.size() == 0 )
			continue;

		Envelope e( line );

		if ( e.isValid() ) {
			envelopes[ e.id() ] = e;
			comboPaperSize->addItem( e.name(), e.id() );
		}
	}
}

bool
FormMain::checkSize( const QStringList & list, int count, const QString & text )
{
	if ( list.size() <= count )
		return true;

	QStringList l;

	for ( int i = 0; i < list.size() && i < count; ++i )
		l << list[ i ];

	if ( QMessageBox::warning( this, "Предупреждение",
				QString("%1 не помещается!\n\n"
					"< %2 >.\n\n"
					"Полный текст:\n\n"
					"< %3 >.\n\n"
					"Продолжить печать?")
						.arg( text )
						.arg( l.join("\n") )
						.arg( list.join("\n") ),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
		return true;

	return false;
}

void
FormMain::saveSenderAddressFont( const QFont & font ) const
{
	saveAddressFont( font, "sender" );
}

void
FormMain::saveRecipientAddressFont( const QFont & font ) const
{
	saveAddressFont( font, "recipient" );
}

void
FormMain::saveAddressFont( const QFont & font, const QString & prefix ) const
{
	const QString key = prefix + "_font_";

	QSettings s;

	s.setValue( key + "family", font.family() );
	s.setValue( key + "size", font.pointSize() );
	s.setValue( key + "bold", font.bold() );
	s.setValue( key + "italic", font.italic() );
}

void
FormMain::saveEnvelopeType() const
{
	QSettings s;

	s.setValue( "envelope_type", comboPaperSize->itemData( comboPaperSize->currentIndex() ).toInt() );
}

bool
FormMain::dbConnect()
{
	QStringList args = qApp->arguments();

	QString host( "localhost" ),
			database,
			port( "5432" ),
			user( getenv("USER") ),
			pass;

	for ( int i = 0; i < args.size(); ++i ) {

		if ( args[ i ] == "-p" ) {
			_dbPg = true;
			continue;
		}

		if ( i > 0 && args[ i ][ 0 ] != '-' ) {
			const QStringList dbList = args[ i ].split(":");

			switch ( dbList.size() ) {
				case 1:
					database = dbList[ 0 ];
					break;

				case 2:
					database = dbList[ 0 ];
					user = dbList[ 1 ];
					break;

				case 3:
					host = dbList[ 0 ];
					database = dbList[ 1 ];
					user =dbList[ 2 ];
					break;

				case 4:
					host = dbList[ 0 ];
					database = dbList[ 1 ];
					user = dbList[ 2 ];
					pass = dbList[ 3 ];
					break;

				case 5:
					host = dbList[ 0 ];
					port = dbList[ 1 ];
					database = dbList[ 2 ];
					user = dbList[ 3 ];
					pass = dbList[ 4 ];
					break;

				default:
					;
			}
			continue;
		}
	}

	QSqlDatabase db = QSqlDatabase::addDatabase( _dbPg ? "QPSQL" : "QSQLITE" );

	DialogConnect * dc = 0;

	if ( _dbPg ) {

		if ( database.isEmpty() ) {
			dc = new DialogConnect( this );

			if ( ! dc->exec() )
				return false;

			host = dc->hostName();
			port = dc->port();
			database = dc->databaseName();
			user = dc->userName();
			pass = dc->password();
		}

		db.setHostName( host );
		db.setPort( port.toInt() );
		db.setDatabaseName( database );
		db.setUserName( user );
		db.setPassword( pass );

	} else {
		db.setDatabaseName( dataPath() + qApp->applicationName() + ".db" );
	}

	if ( db.open() ) {
		setSenderData();
		createModel();

		if ( _dbPg )
			setWindowTitle( qApp->applicationName().append(" - %1@%2").arg( user, host ) );
		else
			setWindowTitle( qApp->applicationName() );

		if ( dc )
			dc->saveSettings();

		return true;

	} else
		_yell( db.lastError().text() );

	return false;
}

QString
FormMain::dataPath() const
{
#ifdef Q_WS_WIN
	// находит SysDrive:\Document And Settings\All Users\Application Data

	QString winMapPath;

	QLibrary library( QLatin1String( "shell32" ) );

	QT_WA ( {
			typedef BOOL ( WINAPI * GetSpecialFolderPath )( HWND, LPTSTR, int, BOOL );
			GetSpecialFolderPath SHGetSpecialFolderPath = ( GetSpecialFolderPath )library.resolve("SHGetSpecialFolderPathW");
			if ( SHGetSpecialFolderPath ) {
				TCHAR path[ MAX_PATH ];
				SHGetSpecialFolderPath( 0, path, 0x0023, FALSE );
				winMapPath = QString::fromUtf16( ( ushort* )path );
			}
	} , {
			typedef BOOL ( WINAPI * GetSpecialFolderPath)( HWND, char*, int, BOOL );
			GetSpecialFolderPath SHGetSpecialFolderPath = ( GetSpecialFolderPath )library.resolve("SHGetSpecialFolderPathA");
			if ( SHGetSpecialFolderPath ) {
				char path[ MAX_PATH ];
				SHGetSpecialFolderPath( 0, path 0x0023, FALSE );
				winMapPath = QString::fromLocal8Bit( path );
			}
	} );

	return winMapPath + QDir::separator() +
		qApp->applicationName() + QDir::separator();
#else

	return qApp->applicationDirPath() + QDir::separator() + ".." +
		QDir::separator();
#endif
}

