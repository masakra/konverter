
#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QtGlobal>
#include <QString>

class Envelope
{
	private:

		struct Contact {
			qreal x,
				  y,
				  width;
			int whoRowCount,
				whereRowCount;

			qreal indexMargin,
				  indexTopOffset;
		};

		bool valid;

		int e_id;

		QString e_name;

		qreal e_rowHeight;

		Contact sender,
				recipient;

		bool e_doubleSide;

	public:
		Envelope();
		Envelope( const QString & line );

		bool isValid() const;

		int id() const;

		const QString & name() const;

		qreal rowHeight() const;

		qreal senderX() const;

		qreal senderY() const;

		qreal senderWidth() const;

		int senderWhoRowCount() const;

		int senderWhereRowCount() const;

		qreal senderIndexMargin() const;

		qreal senderIndexTopOffset() const;

		qreal recipientX() const;

		qreal recipientY() const;

		qreal recipientWidth() const;

		int recipientWhoRowCount() const;

		int recipientWhereRowCount() const;

		qreal recipientIndexMargin() const;

		qreal recipientIndexTopOffset() const;

		bool doubleSide() const;
};

#endif

