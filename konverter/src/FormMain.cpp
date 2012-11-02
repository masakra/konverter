
#include "FormMain.h"

#include <QtGui>
#include <QtSql>
#include "_.h"
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

	model->setQuery("SELECT "
			"id, "
			"who, "
			"whe, "
			"ind "
		"FROM "
			"contact "
		"WHERE "
			"id != 0 "
		"ORDER BY "
			"touch DESC ");

	table->setModel( model );
	table->hideColumn( 0 );
	table->hideColumn( 2 );
	table->hideColumn( 3 );

	connect( table->selectionModel(), SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ),
				SLOT( contactChanged( const QModelIndex &, const QModelIndex & ) ) );
}

void
FormMain::createActions()
{
	actionSelectFont = new QAction( QIcon(":/font.png"), "Выбрать шрифт", this );
}

void
FormMain::createToolBar()
{
	QToolBar * toolBar = new QToolBar( "Действия", this );
	toolBar->addAction( actionSelectFont );
}

void
FormMain::createWidgets()
{
	QWidget * centralWidget = new QWidget( this );

	sender = new WidgetSender( centralWidget );

	table = new QTableView( centralWidget );
	table->horizontalHeader()->hide();
	table->verticalHeader()->hide();
	table->horizontalHeader()->setStretchLastSection( true );

	QToolButton * buttonPrint = new QToolButton( centralWidget );
	buttonPrint->setIconSize( QSize( 97, 61 ) );
	buttonPrint->setIcon( QIcon(":/postmark.png") );

	connect( buttonPrint, SIGNAL( clicked() ), SLOT( print() ) );


	QHBoxLayout * layoutControls = new QHBoxLayout();
	layoutControls->setMargin( 15 );

	layoutControls->addStretch();
	layoutControls->addWidget( buttonPrint );

	QGridLayout * gridLayout = new QGridLayout( centralWidget );

	recipient = new WidgetRecipient( centralWidget );

	gridLayout->addWidget( sender, 0, 0 );
	gridLayout->addLayout( layoutControls, 0, 1, 1, 1, Qt::AlignTop );

	gridLayout->addWidget( table, 1, 0 );
	gridLayout->addWidget( recipient, 1, 1 );

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
			_yell("Запись отправителя (id=0) не найдена");

	} else
		_yell( q );

}

void
FormMain::contactChanged( const QModelIndex & current, const QModelIndex & previous )
{
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
		QMessageBox::information( this, "К сведению", "Поддерживаемые форматы:<BR><BR>"
				"1. <B>C5E</B> - 163 x 229 мм.<BR>"
				"2. <B>DLE</B> - 110 x 220 мм.<BR><BR>"
				"Для реализации поддержки других форматов<BR>"
				"обратитесь к разработчику:<BR><BR>"
				"email: <A HREF='mailto:masakra@mail.ru'>masakra@mail.ru</A><BR>"
				"ICQ: 124321040<BR>"
				"jabber: masakra@jabber.ru");

		return;
	}
	*/

	/// printing ))))

	printer->setOrientation( QPrinter::Portrait );
	printer->setPaperSize( QPrinter::C5E );
	printer->setResolution( 300 );	// dpi

	QPainter painter;

	painter.begin( printer );

	// sender data
	painter.setFont( sender->addrFont() );
	painter.setPen( QPen( Qt::black ) );

	painter.save();			// sender from
	painter.translate( 465, 1720 );
	painter.rotate( -90 );

	QStringList whoList = sender->whoList( 400 );

	for ( int i = 0; i < whoList.size(); ++i ) {
		painter.drawText( 0, 0, whoList[ i ] );
	}

	painter.restore();



}

void
FormMain::createPrinter()
{
	printer = new QPrinter( QPrinter::HighResolution );
}

