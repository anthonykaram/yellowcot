/*
	Yellowcot 1.1.16, released 2011-01-25

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "qticonloader.h"
#include "yellowcot.h"

class QAction;
class QMenu;

class YCQuiz;

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow();
		YCQuiz *ycQuiz;
		QLabel *statusLabel;
		QLabel *filePathLbl;

	public slots:
		void open(){
			filePathLbl->setText(QFileDialog::getOpenFileName(this, tr("Open Yellowcot File"), "/home/karam", tr("Yellowcot Files (*.yc)")));
			if (!filePathLbl->text().isNull())
				statusLabel->setText(QString("Using <i>")+filePathLbl->text()+QString("</i>."));
		}
		void help() {
			QMessageBox::information(this, tr("Help"), tr("Coming soon..."));
		}
		void about() {
			QMessageBox::information(this, tr("About Yellowcot"), tr("<h3>version ")+QString(VERSION)+tr("</h3>Written by Anthony Karam. Special thanks to the GNU LGPL project, <a href=http://code.google.com/p/qticonloader>qticonloader</a> (SVN revision 897 used in Yellowcot ")+QString(VERSION)+tr(")."));
		}
		void removeTempFiles() {
			system("rm -r /var/tmp/yellowcot_* > /dev/null 2>&1");
		}
};

#endif