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

#include "yellowcot.h"

YCQuiz::YCQuiz(QWidget *parent) : QWidget(parent) {

	//create widgets
	fileIsLoaded = new QCheckBox();
	tabBar = new QTabBar;
	tabBar->addTab(tr("Quiz"));
	tabBar->addTab(tr("Edit"));
	questionsAndAnswersList = new QComboBox();
	currQorA = new QPushButton();
	currQorA->setLayout(new QHBoxLayout());
	currQorA->setEnabled(false);
	currQorA->setMinimumWidth(MINWIDTH);
	currQorA->setMinimumHeight(MINHEIGHT);
	currQorA->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	rangeLbl = new QLabel(tr("Range:"));
	rangeLbl->setEnabled(false);
	startBox = new QSpinBox();
	startBox->setEnabled(false);
	toLbl = new QLabel(tr("to"));
	toLbl->setAlignment(Qt::AlignCenter);
	toLbl->setEnabled(false);
	endBox = new QSpinBox();
	endBox->setEnabled(false);
	reversedLbl = new QLabel(tr("Reversed Mode:"));
	reversedLbl->setEnabled(false);
	reversedCheckBox = new QCheckBox(tr("Swap Questions and Answers"));
	reversedCheckBox->setEnabled(false);
	editTable = new QTableWidget(this);
	editTable->setRowCount(0);
	editTable->setColumnCount(8);
	editTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	editTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	QTableWidgetItem *header0 = new QTableWidgetItem(tr("Question Type"));
	editTable->setHorizontalHeaderItem(0, header0);
	QTableWidgetItem *header1 = new QTableWidgetItem(tr("Question"));
	editTable->setHorizontalHeaderItem(1, header1);
	QTableWidgetItem *header2 = new QTableWidgetItem(tr("Question Source"));
	editTable->setHorizontalHeaderItem(2, header2);
	QTableWidgetItem *header3 = new QTableWidgetItem(tr("Question Licence"));
	editTable->setHorizontalHeaderItem(3, header3);
	QTableWidgetItem *header4 = new QTableWidgetItem(tr("Answer Type"));
	editTable->setHorizontalHeaderItem(4, header4);
	QTableWidgetItem *header5 = new QTableWidgetItem(tr("Answer"));
	editTable->setHorizontalHeaderItem(5, header5);
	QTableWidgetItem *header6 = new QTableWidgetItem(tr("Answer Source"));
	editTable->setHorizontalHeaderItem(6, header6);
	QTableWidgetItem *header7 = new QTableWidgetItem(tr("Answer Licence"));
	editTable->setHorizontalHeaderItem(7, header7);
	editTable->resizeColumnsToContents();
	editTable->resizeRowsToContents();
	editTable->setEnabled(false);
	insertRow = new QPushButton(tr("Insert Row"));
	removeRow = new QPushButton(tr("Remove Row"));
	moveRowUp = new QPushButton(tr("Move Row Up"));
	moveRowDown = new QPushButton(tr("Move Row Down"));
	insertImage = new QPushButton(tr("Insert Image"));
	insertRow->setEnabled(false);
	removeRow->setEnabled(false);
	moveRowUp->setEnabled(false);
	moveRowDown->setEnabled(false);
	insertImage->setEnabled(false);

	//give the tab bar control over which widgets are visible at any given time
	connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(updateVisibleWidgets()));
	updateVisibleWidgets();

	//respond appropriately if either the start field or end field is changed
	connect(startBox, SIGNAL(valueChanged(int)), this, SLOT(respondToStartChange()));
	connect(endBox, SIGNAL(valueChanged(int)), this, SLOT(respondToEndChange()));

	//advance the current question/answer when it is clicked
	connect(currQorA, SIGNAL(clicked()), this, SLOT(advanceQorA()));

	//insert, remove, or move a row depending on which button is clicked
	connect(insertRow, SIGNAL(clicked()), this, SLOT(insertARow()));
	connect(removeRow, SIGNAL(clicked()), this, SLOT(removeARow()));
	connect(moveRowUp, SIGNAL(clicked()), this, SLOT(moveTheRowUp()));
	connect(moveRowDown, SIGNAL(clicked()), this, SLOT(moveTheRowDown()));

	//insert an image if the button is clicked
	connect(insertImage, SIGNAL(clicked()), this, SLOT(insertAnImage()));

	//organise widgets on a layout
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(tabBar, 0, 0, 1, 4);
	mainLayout->addWidget(currQorA, 1, 0, 1, 4);
	mainLayout->addWidget(rangeLbl, 2, 0);
	mainLayout->addWidget(startBox, 2, 1);
	mainLayout->addWidget(toLbl, 2, 2);
	mainLayout->addWidget(endBox, 2, 3);
	mainLayout->addWidget(reversedLbl, 3, 0);
	mainLayout->addWidget(reversedCheckBox, 3, 1, 1, 3);
	mainLayout->addWidget(editTable, 4, 0, 1, 4);
	mainLayout->addWidget(insertRow, 5, 0, 1, 2);
	mainLayout->addWidget(removeRow, 5, 2, 1, 2);
	mainLayout->addWidget(moveRowUp, 6, 0, 1, 2);
	mainLayout->addWidget(moveRowDown, 6, 2, 1, 2);
	mainLayout->addWidget(insertImage, 7, 0, 1, 4);
	setLayout(mainLayout);
}