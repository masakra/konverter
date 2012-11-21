
#ifndef WIDGETADDITION_H
#define WIDGETADDITION_H

#include <QWidget>

class LineEdit;

class WidgetAddition : public QWidget
{
	Q_OBJECT

	private:
		void createWidgets();

		LineEdit * editCity;

		QString cachedCity;

	private Q_SLOTS:
		void editReturned();

		void editEscaped();

	public:
		WidgetAddition( QWidget * parent = 0 );

		void setCity( const QString & city );

	Q_SIGNALS:
		void cityChanged( const QString & text );
};

#endif
