/***************************************************************************
 *   Copyright (C) 2012 Nordavia-RA                                        *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   ICQ: 124231040                                                        *
 *   jabber: masakra@jabber.ru                                             *
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

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QtGlobal>
#include <QString>

class Envelope
{
	public:
		struct Contact {
			qreal x,
				  y,
				  width;
			int whoRowCount,
				whereRowCount;

			qreal indexMargin,
				  indexTopOffset;
		};

	private:

		bool m_valid;

		int m_e_id;

		QString m_e_name;

		qreal m_e_rowHeight;

		Contact m_sender,
				m_recipient;

		bool e_doubleSide;

		bool e_log;

	public:
		Envelope();
		Envelope( const QString & line );

		bool isValid() const;

		int id() const;

		const QString & name() const;

		qreal rowHeight() const;

		const Contact & sender() const;

		const Contact & recipient() const;

		bool doubleSide() const;

		bool log() const;
};

#endif

