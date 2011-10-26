/*
	Yellowcot 1.2.0, released YYYY-MM-DD

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

	//make string variable
	char str[STRLEN];

	//make file object
	FILE *file;

	//quit if it is not the only Yellowcot instance
	memset(str, 0, STRLEN);
	sprintf(str, "%s", TMPDIR);
	if ((file = fopen(str, "r"))) {
		memset(str, 0, STRLEN);
		sprintf(str, "Multiple, simultaneous instances of Yellowcot is not yet a supported feature. If you are certain another instance is not running, you may execute \"sudo rm -r %s\".", TMPDIR);
		QMessageBox::critical(0, QString("Fatal Error"), QString(str));
		return 1;
		fclose (file);
	}

	//create temporary directory for yellowcot quiz
	sysprintf("mkdir %s", TMPDIR);

	//create config file if it doesn't already exist
	sysprintf("cp ~/.yellowcot/config %s/config > /dev/null 2>&1", TMPDIR);
	if ((file = fopen("/var/tmp/yellowcot_quiz/config", "r"))) {
		fclose(file);
		remove("/var/tmp/yellowcot_quiz/config");
	}
	else {
		system("echo -e \"# Yellowcot configuration file\n#\n# If you'd like to use a font other than the default, specify\n# its path (e.g. /usr/local/share/fonts/c/code2000.ttf) here.\n#font=\n# Specify the font size here.\nfontsize=20\" > /var/tmp/yellowcot_quiz/config");
		system("mkdir ~/.yellowcot");
		system("mv /var/tmp/yellowcot_quiz/config ~/.yellowcot/config");
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