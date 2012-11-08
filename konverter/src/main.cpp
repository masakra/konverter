
#include <QApplication>
#include <QTextCodec>

#include "FormMain.h"

int
main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	app.setApplicationName( "konverter" );
	app.setApplicationVersion( "0.1.1" );
	app.setOrganizationName( "Nordavia-RA" );
	app.setWindowIcon( QIcon(":/main.png") );

	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "KOI8-R" ) );

	FormMain fm;

	fm.show();

	return app.exec();
}

