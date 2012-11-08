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

#ifndef WIDGETCONTACT_H
#define WIDGETCONTACT_H

#define MARGIN 12
#define SPACING 5

#define COLOR_BLUE QColor(

#include <QWidget>

class LineEdit;

class WidgetContact : public QWidget
{
	Q_OBJECT

	private Q_SLOTS:
		void editReturned();
		void editEscaped();

	protected:
		virtual void mousePressEvent( QMouseEvent * event );

		mutable QSize cachedSizeHint;

		QString who,
				where,
				index;

		enum Status {
			Normal,
			SettingWho,
			SettingWhere,
			SettingIndex
		} State;

		virtual const QString & whoStr() const = 0;

		virtual const QString & whereStr() const = 0;

		virtual int lineMargin() const;

		mutable int cachedLineMargin;

		QFont addressFont;

		int rowHeight() const;

		mutable int cachedRowHeight;

		LineEdit * edit;

		void editShow( int w, int m, int y );

		QStringList split( const QString & str, int w ) const;

	protected Q_SLOTS:
		void selectAddressFont();

	public:
		WidgetContact( QWidget * parent );

		void setAddressFont( const QFont & f );

		void setAll( const QString & wh, const QString & we, const QString & idx );

		const QFont & addrFont() const;

		const QString & getIndex() const;

		QStringList whoList( int w ) const;

		QStringList whereList( int w ) const;


	public Q_SLOTS:
		void setWho( const QString & text );
		void setWhere( const QString & text );
		void setIndex( const QString & text );

		void setWho();
		virtual void setWhere() = 0;
		virtual void setIndex() = 0;

	Q_SIGNALS:
		void whoChanged( const QString & text );
		void whereChanged( const QString & text );
		void indexChanged( const QString & text );

		void addressFontChanged( const QFont & font );
};

#endif

