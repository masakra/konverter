
#include "FormMain.h"

#include <QtGui>
#include <QtSql>
#include "_.h"
#include "DialogReport.h"
#include "Report.h"
#include "WidgetRecipient.h"
#include "WidgetSender.h"

FormMain::FormMain( QWidget * parent )
	: QMainWindow( parent )
{
	setWindowTitle( qApp->applicationName() + " " + qApp->applicationVersion() );

	createWidgets();

	createActions();

	//createToolBar();
	createPrinter();

	loadSettings();

	loadEnvelopes();

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");	// sqlite3 database

	db.setDatabaseName( qApp->applicationName() + ".db" );

	if ( db.open() ) {
		setSenderData();
		createModel();
	} else
		_yell( db.lastError().text() );
}

FormMain::~FormMain()
{
	delete printer;
}

void
FormMain::createModel()
{
	model = new QSqlQueryModel( this );

	refresh();
}

void
FormMain::refresh( int id )
{
	//State = Normal;
	setStatus( Normal );

	model->setQuery( QString("SELECT "
			"id, "
			"who, "
			"whe, "
			"ind "
		"FROM "
			"contact "
		"WHERE "
			"id != 0 "
			"%1 "
		"ORDER BY "
			"touch DESC ")
			.arg( editFilter->text().isEmpty() ? "": QString("AND who like '%%1%'").arg( editFilter->text() ) ) );

	table->setModel( model );
	table->hideColumn( 0 );
	table->hideColumn( 2 );
	table->hideColumn( 3 );

	connect( table->selectionModel(), SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ),
				SLOT( contactChanged( const QModelIndex &, const QModelIndex & ) ) );

	labelCount->setText( QString("�������: %1").arg( model->rowCount() ) );

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
FormMain::createActions()
{
	actionSelectFont = new QAction( QIcon(":/font.png"), "������� �����", this );
}

void
FormMain::createToolBar()
{
	QToolBar * toolBar = new QToolBar( "��������", this );
	toolBar->addAction( actionSelectFont );
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

	labelCount = new QLabel( centralWidget );

	table = new QTableView( centralWidget );
	table->horizontalHeader()->hide();
	table->verticalHeader()->hide();
	table->horizontalHeader()->setStretchLastSection( true );
	table->setSelectionBehavior( QAbstractItemView::SelectRows );
	table->setSelectionMode( QAbstractItemView::SingleSelection );
	table->setAlternatingRowColors( true );

	editFilter = new QLineEdit( centralWidget );

	connect( editFilter, SIGNAL( textChanged( const QString & ) ),
			SLOT( filterChanged( const QString & ) ) );

	QVBoxLayout * layoutTable = new QVBoxLayout();

	layoutTable->addWidget( labelCount );
	layoutTable->addWidget( table );
	layoutTable->addWidget( editFilter );

	buttonAdd = new QToolButton( centralWidget );
	buttonDel = new QToolButton( centralWidget );

	QToolButton * buttonPrint = new QToolButton( centralWidget ),
				//* buttonAdd = new QToolButton( centralWidget ),
				//* buttonDel = new QToolButton( centralWidget ),
				* buttonReport = new QToolButton( centralWidget ),
				* buttonAbout = new QToolButton( centralWidget );

	buttonPrint->setIconSize( QSize( 97, 61 ) );
	buttonPrint->setIcon( QIcon(":/postmark.png") );

	buttonAdd->setIconSize( QSize( 48, 48 ) );
	buttonAdd->setIcon( QIcon(":/add.png") );

	buttonDel->setIconSize( QSize( 48, 48 ) );
	buttonDel->setIcon( QIcon(":/delete.png") );

	buttonReport->setIconSize( QSize( 48, 48 ) );
	buttonReport->setIcon( QIcon( ":/report.png") );

	buttonAbout->setIconSize( QSize( 48, 48 ) );
	buttonAbout->setIcon( qApp->windowIcon() );

	connect( buttonAdd, SIGNAL( clicked() ), SLOT( addContact() ) );

	connect( buttonDel, SIGNAL( clicked() ), SLOT( delContact() ) );

	connect( buttonReport, SIGNAL( clicked() ), SLOT( report() ) );

	connect( buttonAbout, SIGNAL( clicked() ), SLOT( about() ) );

	connect( buttonPrint, SIGNAL( clicked() ), SLOT( print() ) );

	comboPaperSize = new QComboBox( this );
	//comboPaperSize->addItem( "C5E - 163 x 229 ��.", QPrinter::C5E );
	//comboPaperSize->addItem( "DLE - 110 x 220 ��.", QPrinter::DLE );
	//comboPaperSize->addItem( "����������� - 100 x 145 ��.", QPrinter::A6 );

	QHBoxLayout * layoutControls = new QHBoxLayout(),
				* layoutButtons = new QHBoxLayout();

	layoutControls->setMargin( 15 );
	layoutButtons->setMargin( 15 );

	layoutControls->addStretch();
	layoutControls->addWidget( comboPaperSize );
	layoutControls->addStretch();
	layoutControls->addWidget( buttonPrint );

	QGridLayout * gridLayout = new QGridLayout( centralWidget );

	recipient = new WidgetRecipient( centralWidget );

	connect( recipient, SIGNAL( whoChanged( const QString & ) ),
			SLOT( modifyRecipientWho( const QString & ) ) );

	connect( recipient, SIGNAL( whereChanged( const QString & ) ),
			SLOT( modifyRecipientWhere( const QString & ) ) );

	connect( recipient, SIGNAL( indexChanged( const QString & ) ),
			SLOT( modifyRecipientIndex( const QString & ) ) );

	layoutButtons->addStretch();
	layoutButtons->addWidget( buttonAdd );
	layoutButtons->addWidget( buttonDel );
	layoutButtons->addWidget( buttonReport );
	layoutButtons->addStretch();
	layoutButtons->addWidget( buttonAbout );
	layoutButtons->addStretch();

	gridLayout->addWidget( sender, 0, 0 );
	gridLayout->addLayout( layoutControls, 0, 1, 1, 1, Qt::AlignTop );

	gridLayout->addLayout( layoutTable, 1, 0, 2, 1 );
	gridLayout->addWidget( recipient, 1, 1 );

	gridLayout->addLayout( layoutButtons, 2, 1 );

	setCentralWidget( centralWidget );
}

void
FormMain::setSenderData()
{
	QSqlQuery q;

	q.prepare("SELECT "
			"who, "
			"whe, "
			"ind "
		"FROM "
			"contact "
		"WHERE "
			"id = 0");	// not id as variant )))

	if ( q.exec() ) {
		if ( q.first() )
			sender->setAll( q.value( 0 ).toString(),
					q.value( 1 ).toString(),
					q.value( 2 ).toString() );
		else
			_yell("������ ����������� (id=0) �� �������");

	} else
		_yell( q );

}

void
FormMain::contactChanged( const QModelIndex & current, const QModelIndex & previous )
{
	setStatus( Normal );

	const int row = current.row();

	const int id = model->index( row, 0 ).data().toInt();

	const QString who = model->index( row, 1 ).data().toString(),
				  where = model->index( row, 2 ).data().toString(),
				  index = model->index( row, 3 ).data().toString();

	recipient->setAll( who, where, index );
}

void
FormMain::setAddressFont( const QFont & f )
{
	sender->setAddressFont( f );
	recipient->setAddressFont( f );
}

void
FormMain::loadSettings()
{
	QSettings s;

	const QString font_family = s.value( FONT_FAMILY,
#ifdef Q_OS_WIN32
			"Times"
#else
			"Droid Serif"
#endif
			).toString();

	const int font_point_size = s.value( FONT_POINT_SIZE, 10 ).toInt(),
		      font_weight = s.value( FONT_WEIGHT, QFont::Normal ).toInt();

	const bool font_italic = s.value( FONT_ITALIC, true ).toBool();

	setAddressFont( QFont( font_family, font_point_size, font_weight, font_italic ) );
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

	/// printing

	const QPrinter::PaperSize paperSize = printer->paperSize();

	qDebug() << printer->paperSize() << QPrinter::C5E << QPrinter::DLE;

	/*
	if ( paperSize != QPrinter::C5E || paperSize != QPrinter::DLE ) {
		QMessageBox::information( this, "� ��������", "�������������� �������:<BR><BR>"
				"1. <B>C5E</B> - 163 x 229 ��.<BR>"
				"2. <B>DLE</B> - 110 x 220 ��.<BR><BR>"
				"��� ���������� ��������� ������ ��������<BR>"
				"���������� � ������������:<BR><BR>"
				"email: <A HREF='mailto:masakra@mail.ru'>masakra@mail.ru</A><BR>"
				"ICQ: 124321040<BR>"
				"jabber: masakra@jabber.ru");

		return;
	}
	*/

	if ( comboPaperSize->count() == 0 )
		return;

	int e_id = comboPaperSize->itemData( comboPaperSize->currentIndex() ).toInt();

	Envelope & e = envelopes[ e_id ];

	// sizes checking
	QStringList senderWho = sender->whoList( e.senderWidth() ),
				senderWhere = sender->whereList( e.senderWidth() ),
				recipientWho = recipient->whoList( e.recipientWidth() ),
				recipientWhere = recipient->whereList( e.recipientWidth() );

	if ( ! checkSize( senderWho, e.senderWhoRowCount(), "�� ���� �����������" ) ||
			! checkSize( senderWhere, e.senderWhereRowCount(), "������ �����������" ) ||
			! checkSize( recipientWho, e.recipientWhoRowCount(), "���� ��������" ) ||
			! checkSize( recipientWhere, e.recipientWhereRowCount(), "���� ��������" ) )
		return;

	qDebug() << "senderX" << e.senderX() << '\n'
			<< "senderY" << e.senderY() << '\n'
			<< "id" << e.id() << '\n'
			<< "name" << e.name() << '\n'
			<< "rowHeight" << e.rowHeight() << '\n'
			<< "senderWidth" << e.senderWidth() << '\n'
			<< "recipientWidth" << e.recipientWidth() << '\n';

	/// printing ))))

	printer->setOrientation( QPrinter::Portrait );
	printer->setPaperSize( QPrinter::A4 );
	printer->setResolution( 300 );	// dpi

	QPainter painter;

	painter.begin( printer );

	// sender data
	painter.setFont( sender->addrFont() );
	painter.setPen( QPen( Qt::black ) );

	painter.save();			// sender from
	//painter.translate( 470, 1720 );
	painter.translate( e.senderX(), e.senderY() );
	painter.rotate( -90 );

	// who
	for ( int i = 0; i < senderWho.size() && i < e.senderWhoRowCount(); ++i )
		painter.drawText( 0, qRound( i * e.rowHeight() ), senderWho[ i ] );

	// where
	for ( int i = 0; i < senderWhere.size() && i < e.senderWhereRowCount(); ++i )
		painter.drawText( 0, ( i + e.senderWhoRowCount() ) * e.rowHeight(), senderWhere[ i ] );

	// index
	painter.drawText( e.senderIndexMargin(),
			( e.senderWhoRowCount() + e.senderWhereRowCount() ) * e.rowHeight() +
				e.senderIndexTopOffset(),
			sender->getIndex() );

	painter.restore();

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

void
FormMain::createPrinter()
{
	printer = new QPrinter( QPrinter::HighResolution );
}

void
FormMain::modifyContact( const QString & field, const QString & text, int id ) const
{
	QSqlQuery q;

	q.prepare( QString("UPDATE "
			"contact "
		"SET "
			"%1 = :text "
		"WHERE "
			"id = :id").arg( field ) );

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
		modifyContact( field, text, id );
		refresh( id );
	}
}

void
FormMain::modifyRecipientWho( const QString & text )
{
	modifyRecipient( "who", text );
	/*
	if ( State == NewContact ) {
		const int newId = insertContact( "who", text );
		refresh( newId );
	} else {
		const int id = currentId();
		modifyContact( "who", text, id );
		refresh( id );
	}
	*/
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

int
FormMain::insertContact( const QString & field, const QString & text ) const
{
	QString sql( "INSERT INTO contact ( who, whe, ind ) VALUES (" );

	QSqlQuery q;

	if ( field == "who" )
		sql += ":text, '', ''";
	else if ( field == "whe" )
		sql += "'', :text, ''";
	else if ( field == "ind" )
		sql += "'', '', :text";
	else {
		_yell( QString("����������� ����: %1").arg( field ) );
		return;
	}

	q.prepare( sql + ")" );

	q.bindValue(":text", text );

	if ( q.exec() ) {
		q.prepare("SELECT last_insert_rowid()");

		if ( q.exec() && q.first() ) {
			return q.value( 0 ).toInt();
		} else
			_yell( q );
	} else
		_yell( q );

	return -1;
}

void
FormMain::addContact()
{
	//State = NewContact;
	setStatus( NewContact );

	recipient->setAll( "", "", "" );
	recipient->setWho();
}

void
FormMain::delContact()
{
	if ( ! table->selectionModel() )
		return;

	if ( QMessageBox::question( this, "�����������", "������������� ������� �������?",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		return;

	QSqlQuery q;

	// TODO ������� ��� ������ �� �������

	q.prepare("DELETE FROM "
				"contact "
			"WHERE "
				"id = :id");

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
		return model->index( row, 0 ).data().toInt();
	} else
		return -1;
}

void
FormMain::about()
{
	QMessageBox::about( this, "� ���������",
			QString("<H3>%1</H3>").arg( qApp->applicationName() ) +
				"2012 �. ��� " + QChar(0xAB) + QString("��������-��") + QChar(0xBB) + "<BR><BR>" +
				"e-mail: <A HREF='mailto:masakra@mail.ru'>masakra@mail.ru</A><BR>"
				"ICQ: 124231040<BR>"
				"jabber: masakra@jabber.ru" );
}

void
FormMain::filterChanged( const QString & text )
{
	refresh( currentId() );
}

void
FormMain::report()
{
	DialogReport d;

	if ( d.exec() ) {
		Report * rep = new Report();
		rep->show( d );
	}
}

void
FormMain::writeLog() const
{

	const int id = currentId();


	if ( id == -1 )
		return;

	QSqlQuery q;

	q.prepare("INSERT INTO log ("
			"contact_id "
		") VALUES ("
			":id )");

	q.bindValue(":id", id );

	if ( ! q.exec() )
		_yell( q );
}

void
FormMain::setStatus( Status s )
{
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
	QFile file("envelope.conf");

	if ( ! file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		_yell( file.errorString() );
		return;
	}

	QTextStream in( &file );

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

	if ( QMessageBox::warning( this, "��������������",
				QString("%1 �� ����������!\n���������� ������?").arg( text ),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
		return true;

	return false;
}

