/*
	Yellowcot 1.2.2, released 2012-02-23

	Copyleft 2012 Anthony Karam Karam

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

#include "mainwindow.h"
#include "yellowcot.h"

MainWindow::MainWindow() {

	//create a YCQuiz and make it the central widget
	ycQuiz = new YCQuiz;
	setCentralWidget(ycQuiz);

	//create file path label
	filePathLbl = new QLabel();

	//create and populate file menu in menu bar
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QAction *newAct = new QAction(QtIconLoader::icon("document-new"), tr("&New"), this);
	QSignalMapper *signalMapper0 = new QSignalMapper(this);
	connect(newAct, SIGNAL(triggered()), signalMapper0, SLOT(map()));
	signalMapper0->setMapping(newAct, filePathLbl);
	connect(signalMapper0, SIGNAL(mapped(QWidget*)), ycQuiz, SLOT(startNewFile(QWidget*)));
	newAct->setShortcut(tr("Ctrl+N", "New"));
	fileMenu->addAction(newAct);
	fileMenu->addSeparator();
	QAction *openAct = new QAction(QtIconLoader::icon("document-open"), tr("&Open"), this);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
	QSignalMapper *signalMapper1 = new QSignalMapper(this);
	connect(openAct, SIGNAL(triggered()), signalMapper1, SLOT(map()));
	signalMapper1->setMapping(openAct, filePathLbl);
	connect(signalMapper1, SIGNAL(mapped(QWidget*)), ycQuiz, SLOT(loadFile(QWidget*)));
	openAct->setShortcut(tr("Ctrl+O", "Open"));
	fileMenu->addAction(openAct);
	fileMenu->addSeparator();
	QAction *saveAct = new QAction(QtIconLoader::icon("document-save"), tr("&Save"), this);
	QSignalMapper *signalMapper2 = new QSignalMapper(this);
	connect(saveAct, SIGNAL(triggered()), signalMapper2, SLOT(map()));
	signalMapper2->setMapping(saveAct, filePathLbl);
	connect(signalMapper2, SIGNAL(mapped(QWidget*)), ycQuiz, SLOT(saveFile(QWidget*)));
	QSignalMapper *signalMapper3 = new QSignalMapper(this);
	connect(saveAct, SIGNAL(triggered()), signalMapper3, SLOT(map()));
	signalMapper3->setMapping(saveAct, filePathLbl);
	connect(signalMapper3, SIGNAL(mapped(QWidget*)), ycQuiz, SLOT(loadFile(QWidget*)));
	saveAct->setShortcut(tr("Ctrl+S", "Save"));
	fileMenu->addAction(saveAct);
	QAction *saveAsAct = new QAction(QtIconLoader::icon("document-save-as"), tr("Save &As..."), this);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	QAction *pdfAct = new QAction(tr("&Export to PDF"), this);
	connect(pdfAct, SIGNAL(triggered()), ycQuiz, SLOT(exportToPDF()));
	fileMenu->addAction(pdfAct);
	fileMenu->addSeparator();
	QAction *exitAct = new QAction(QtIconLoader::icon("application-exit"), tr("&Quit"), this);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	exitAct->setShortcut(tr("Ctrl+Q", "Quit"));
	fileMenu->addAction(exitAct);

	//create and populate help menu in menu bar
	menuBar()->addSeparator();
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	QAction *helpAct = new QAction(QtIconLoader::icon("help-contents"), tr("&Help"), this);
	connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
	helpAct->setShortcut(tr("F1", "Help"));
	helpMenu->addAction(helpAct);
	helpMenu->addSeparator();
	QAction *aboutAct = new QAction(QIcon(QString("/usr/share/yellowcot/yellowcot.svg")), tr("&About Yellowcot"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	helpMenu->addAction(aboutAct);
	QAction *aboutQtAct = new QAction(QIcon(QString("/usr/share/icons/hicolor/64x64/apps/qtlogo.png")), tr("About &Qt"), this);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	helpMenu->addAction(aboutQtAct);

	//set the window title and icon
	setWindowTitle(tr("Yellowcot"));
	setWindowIcon(QIcon(QString("/usr/share/yellowcot/yellowcot.svg")));

	//set up the status label to show filePathLbl
	statusBar()->addPermanentWidget(filePathLbl);
}