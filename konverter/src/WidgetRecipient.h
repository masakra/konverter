
#ifndef WIDGETRECIPIENT_H
#define WIDGETRECIPIENT_H

#include "WidgetContact.h"

class WidgetRecipient : public WidgetContact
{
	private:
		static const QString toWho,
							 toWhere;


	protected:
		virtual QSize sizeHint() const;
		virtual void paintEvent( QPaintEvent * event );

		virtual const QString & whoStr() const;

		virtual const QString & whereStr() const;

	public:
		WidgetRecipient( QWidget * parent );
};

#endif

