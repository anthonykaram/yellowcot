/*
	Yellowcot 1.1.14, released 2010-08-09

	Copyleft 2010 Anthony Karam Karam

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui>

#include "mainwindow.h"
#include "yellowcot.h"

int main(int argc, char *argv[]) {

	//define qapplication class
	QApplication app(argc, argv);

	//quit if it is not the only Yellowcot instance
	FILE *file;
	if ((file = fopen("/var/tmp/yellowcot_quiz", "r"))) {
		QMessageBox::critical(0, QString("Fatal Error"), QString("Multiple, simultaneous instances of Yellowcot is not yet a supported feature. Sorry."));
		return 1;
	}

	//create temporary directory for yellowcot quiz
	system("mkdir /var/tmp/yellowcot_quiz");

	//start translator
	QTranslator translator;
	translator.load(QString("yellowcot_") + QLocale::system().name());
	app.installTranslator(&translator);

	//create and show main window
	MainWindow mainWindow;
	mainWindow.show();

	//delete temporary file when yellowcot is about to quit
	QObject::connect(&app, SIGNAL(aboutToQuit()), &mainWindow, SLOT(removeTempFiles()));

	//pass control to qt
	return app.exec();
}