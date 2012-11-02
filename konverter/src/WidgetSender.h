
#ifndef WIDGETSENDER_H
#define WIDGETSENDER_H

#include "WidgetContact.h"

class WidgetSender : public WidgetContact
{
	Q_OBJECT

	private:
		//QString whereLine( int row ) const;

		static const QString fromWho,
							 fromWhere;


	private Q_SLOTS:
		virtual const QString & whoStr() const;

		virtual const QString & whereStr() const;

	protected:
		virtual QSize sizeHint() const;
		virtual void paintEvent( QPaintEvent * event );
		virtual void mouseDoubleClickEvent( QMouseEvent * event );
		virtual void resizeEvent( QResizeEvent * event );

	public:
		WidgetSender( QWidget * parent );
};

#endif

