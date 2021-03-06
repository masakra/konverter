/***************************************************************************
 *   Copyright (C) 2012 Nordavia-RA                                        *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   ICQ: 124231040                                                        *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   All comments, if not ascii, in koi8-r                                 *
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

#ifndef WIDGETSENDER_H
#define WIDGETSENDER_H

#include <QLocale>

#include "WidgetContact.h"

class WidgetSender : public WidgetContact
{
	Q_OBJECT

	private:
		static const QString m_fromWho,
							 m_fromWhere;

		QLocale::Language m_language;

	private Q_SLOTS:
		virtual const QString & whoStr() const;
		virtual const QString & whereStr() const;

	protected:
		virtual QSize sizeHint() const;
		virtual void paintEvent( QPaintEvent * event );
		virtual void mouseDoubleClickEvent( QMouseEvent * event );
		virtual void resizeEvent( QResizeEvent * event );
		virtual void contextMenu( QMenu & menu ) const;

	public:
		WidgetSender( QWidget * parent );

		QLocale::Language language() const;

	public Q_SLOTS:
		virtual void setWhere();
		virtual void setIndex();
		void toggleLanguage( QLocale::Language lang = QLocale::AnyLanguage );

	Q_SIGNALS:
		void languageChanged( QLocale::Language lang );
};

#endif

