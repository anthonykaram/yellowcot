/*
	Yellowcot 1.1.17, released 2011-01-29

	Copyleft 2011 Anthony Karam Karam

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

	//make file object
	FILE *file;

	//quit if it is not the only Yellowcot instance
	if ((file = fopen("/var/tmp/yellowcot_quiz", "r"))) {
		QMessageBox::critical(0, QString("Fatal Error"), QString("Multiple, simultaneous instances of Yellowcot is not yet a supported feature. Sorry."));
		return 1;
		fclose (file);
	}

	//create temporary directory for yellowcot quiz
	system("mkdir /var/tmp/yellowcot_quiz");

	//grab config file, or create it if it doesn't exist
	system("cp ~/.yellowcot/config /var/tmp/yellowcot_quiz/config > /dev/null 2>&1");
if ((file = fopen("/var/tmp/yellowcot_quiz/config", "r")))
	fclose(file);
else {
	system("echo -e \"# Yellowcot configuration file\n#\n# If you'd like to use a font other than the default, specify\n# its path (e.g. /usr/share/fonts/TTF/Unifont.ttf) here.\n#font=\" > /var/tmp/yellowcot_quiz/config");
	system("mkdir ~/.yellowcot");
	system("cp /var/tmp/yellowcot_quiz/config ~/.yellowcot/config");
}





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