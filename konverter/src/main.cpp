
#include <QApplication>
#include <QTextCodec>

#include "FormMain.h"

//#include "WidgetSender.h"

int
main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	app.setApplicationName( "konverter" );
	app.setApplicationVersion( "0.0.1" );
	app.setOrganizationName( "Nordavia-RA" );

	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "KOI8-R" ) );

	FormMain fm;
	//WidgetSender fm( 0 );
	//fm.setAll("ЗАО \"Нордавиа-РА\"", "г. Архангельск, аэропорт Архангельск д.4, корп.1", "163053");

	fm.show();

	return app.exec();
}

