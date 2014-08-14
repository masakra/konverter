
#ifndef WIDGETADDITION_H
#define WIDGETADDITION_H

#include <QWidget>

class QStringListModel;
class LineEdit;

class WidgetAddition : public QWidget
{
	Q_OBJECT

	private:
		void createWidgets();

		LineEdit * m_editCity;

		QString m_cachedCity;

		QStringListModel * m_modelCompleter;

	private Q_SLOTS:
		void editReturned();

		void editEscaped();

	public:
		WidgetAddition( QWidget * parent = 0 );

		void setCity( const QString & city );

		void updateModelCompleter();

	Q_SIGNALS:
		void cityChanged( const QString & text );
};

#endif
