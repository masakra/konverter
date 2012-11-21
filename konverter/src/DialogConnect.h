/***************************************************************************
 *   Copyright (C) 2008 by Chursanov Sergey Nikolaevich                    *
 *                                                                         *
 *   masakra@mail.ru                                                       *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

 /** \class DialogConnect
 //
 // \brief Диалог соединения с базой данных
 */

#ifndef DIALOGCONNECT_H
#define DIALOGCONNECT_H

#include <QDialog>

class QGroupBox;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlDatabase;

class DialogConnect : public QDialog
{
	Q_OBJECT

	private:
		QGroupBox * groupServer;

		QLineEdit * editUsername,
				  * editPassword,
				  * editHost,
				  * editPort,
				  * editDatabase;

		QPushButton	* buttonOk,
					* buttonServer;

		QLabel * labelHint;

		void setLabelHint();

		void createWidgets();

		QHBoxLayout * createLabeledEdit( QLineEdit ** edit, const QString labelText );

		void loadSettings();

	private slots:
		//void doConnect();
		void check();

	public:
		DialogConnect( QWidget * parent = 0 );

		void saveSettings() const;

		QString hostName() const;

		QString port() const;

		QString databaseName() const;

		QString userName() const;

		QString password() const;
};

#endif

