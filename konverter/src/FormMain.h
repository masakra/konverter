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

#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QMainWindow>

#include <QHash>
#include "Envelope.h"

#define FONT_FAMILY "font_family"
#define FONT_POINT_SIZE "font_point_size"
#define FONT_WEIGHT "font_weight"
#define FONT_ITALIC "font_italic"

class QAction;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPrinter;
class QSettings;
class QTableView;
class QToolButton;
class SqlQueryModel;
class WidgetAddition;
class WidgetContact;
class WidgetRecipient;
class WidgetSender;

class FormMain : public QMainWindow
{
	Q_OBJECT

	private:
		enum Status {
			Normal,
			NewContact
		} State;

		void setStatus( Status s );

		void createWidgets();

		WidgetSender * sender;

		WidgetRecipient * recipient;

		WidgetAddition * addition;

		QLabel * labelCount;

		QTableView * table;

		QLineEdit * editIshod,
				  * editFilter;

		QCheckBox * checkZakaz;

		SqlQueryModel * model;

		QComboBox * comboPaperSize,
				  * comboIshod;

		QToolButton * buttonAdd,
					* buttonDel;

		void setSenderData();

		void createModel();

		void refresh( int id = -1 );

		void loadSettings();

		void loadFont( QSettings & settings, WidgetContact * contact, const QString & prefix );

		void createActions();

		QAction * actionSelectFont;

		void createToolBar();

		void createPrinter();

		QPrinter * printer;

		void modifyContact( const QString & field, const QString & text, int id = 0 ) const;

		void modifyRecipient( const QString & field, const QString & text );

		int insertContact( const QString & field, const QString & text ) const;

		int currentId() const;

		void writeLog() const;

		void loadEnvelopes();

		QHash< int, Envelope > envelopes;

		bool checkSize( const QStringList & list, int count, const QString & text );

		bool printSide( const Envelope & e, int side );

		QString dataPath() const;

		void saveAddressFont( const QFont & font, const QString & prefix ) const;

		void saveEnvelopeType() const;

		//bool dbPg;

		//QString tableName( const QString & table ) const;

	private Q_SLOTS:
		void contactChanged( const QModelIndex & current, const QModelIndex & previous );

		void setCount( int rows );

		void print();

		void modifySenderWho( const QString & text ) const;
		void modifySenderWhere( const QString & text ) const;
		void modifySenderIndex( const QString & text ) const;

		void modifyRecipientWho( const QString & text );
		void modifyRecipientWhere( const QString & text );
		void modifyRecipientIndex( const QString & text );

		void modifyRecipientCity( const QString & text );

		void addContact();
		void delContact();

		void saveSenderAddressFont( const QFont & font ) const;
		void saveRecipientAddressFont( const QFont & font ) const;

		void filterChanged( const QString & text );

		void report();

		void about();

	public:
		FormMain( QWidget * parent = 0 );
		~FormMain();

		bool dbConnect();
};

#endif

