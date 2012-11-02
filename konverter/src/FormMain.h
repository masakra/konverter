
#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QMainWindow>

#define FONT_FAMILY "font_family"
#define FONT_POINT_SIZE "font_point_size"
#define FONT_WEIGHT "font_weight"
#define FONT_ITALIC "font_italic"

class QAction;
class QModelIndex;
class QPrinter;
class QSqlQueryModel;
class QTableView;
class WidgetRecipient;
class WidgetSender;

class FormMain : public QMainWindow
{
	Q_OBJECT

	private:
		void createWidgets();

		WidgetSender * sender;

		WidgetRecipient * recipient;

		QTableView * table;

		QSqlQueryModel * model;

		void setSenderData();

		void createModel();

		void setAddressFont( const QFont & f );

		void loadSettings();

		void createActions();

		QAction * actionSelectFont;

		void createToolBar();

		void createPrinter();

		QPrinter * printer;

	private Q_SLOTS:
		void contactChanged( const QModelIndex & current, const QModelIndex & previous );

		void print();

	public:
		FormMain( QWidget * parent = 0 );
		~FormMain();
};

#endif

