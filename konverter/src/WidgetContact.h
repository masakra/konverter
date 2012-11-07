
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

