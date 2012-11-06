
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
class QComboBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPrinter;
class QSqlQueryModel;
class QTableView;
class QToolButton;
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

		QLabel * labelCount;

		QTableView * table;

		QLineEdit * editFilter;

		QSqlQueryModel * model;

		QComboBox * comboPaperSize;

		QToolButton * buttonAdd,
					* buttonDel;

		void setSenderData();

		void createModel();

		void refresh( int id = -1 );

		void setAddressFont( const QFont & f );

		void loadSettings();

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

	private Q_SLOTS:
		void contactChanged( const QModelIndex & current, const QModelIndex & previous );

		void print();

		void modifySenderWho( const QString & text ) const;
		void modifySenderWhere( const QString & text ) const;
		void modifySenderIndex( const QString & text ) const;

		void modifyRecipientWho( const QString & text );
		void modifyRecipientWhere( const QString & text );
		void modifyRecipientIndex( const QString & text );

		void addContact();
		void delContact();

		void filterChanged( const QString & text );

		void report();

		void about();

	public:
		FormMain( QWidget * parent = 0 );
		~FormMain();
};

#endif

