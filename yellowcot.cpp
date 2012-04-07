/*
	Yellowcot 1.2.5, released 2012-04-07

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
	currQorA->setMinimumWidth(MINWIDTH);
	currQorA->setMinimumHeight(MINHEIGHT);
	currQorA->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	rangeLbl = new QLabel(tr("Range:"));
	startBox = new QSpinBox();
	toLbl = new QLabel(tr("to"));
	toLbl->setAlignment(Qt::AlignCenter);
	endBox = new QSpinBox();
	reversedLbl = new QLabel(tr("Reversed Mode:"));
	reversedCheckBox = new QCheckBox(tr("Swap Questions and Answers"));
	editTable = new QTableWidget(this);
	editTable->setRowCount(0);
	editTable->setColumnCount(6);
	editTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	editTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	QTableWidgetItem *header0 = new QTableWidgetItem(tr("Question"));
	editTable->setHorizontalHeaderItem(0, header0);
	QTableWidgetItem *header1 = new QTableWidgetItem(tr("Question Source"));
	editTable->setHorizontalHeaderItem(1, header1);
	QTableWidgetItem *header2 = new QTableWidgetItem(tr("Question Licence"));
	editTable->setHorizontalHeaderItem(2, header2);
	QTableWidgetItem *header3 = new QTableWidgetItem(tr("Answer"));
	editTable->setHorizontalHeaderItem(3, header3);
	QTableWidgetItem *header4 = new QTableWidgetItem(tr("Answer Source"));
	editTable->setHorizontalHeaderItem(4, header4);
	QTableWidgetItem *header5 = new QTableWidgetItem(tr("Answer Licence"));
	editTable->setHorizontalHeaderItem(5, header5);
	editTable->resizeColumnsToContents();
	editTable->resizeRowsToContents();
	insertRow = new QPushButton(tr("Insert Row"));
	removeRow = new QPushButton(tr("Remove Row"));
	moveRowUp = new QPushButton(tr("Move Row Up"));
	moveRowDown = new QPushButton(tr("Move Row Down"));
	insertImage = new QPushButton(tr("Insert Image"));
	insertText = new QPushButton(tr("Insert Text"));
	questionMediaSources = new QComboBox();
	answerMediaSources = new QComboBox();
	showLicencingData = new QCheckBox(tr("Show Licencing Data"));

	//hide licencing columns
	editTable->setColumnHidden(1, true);
	editTable->setColumnHidden(2, true);
	editTable->setColumnHidden(4, true);
	editTable->setColumnHidden(5, true);

	//respond appropriately if the showLicencingData checkbox is checked/unchecked
	connect(showLicencingData, SIGNAL(stateChanged(int)), this, SLOT(toggleLicencing()));

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

	//insert a text if the button is clicked
	connect(insertText, SIGNAL(clicked()), this, SLOT(insertAText()));

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
	mainLayout->addWidget(showLicencingData, 4, 0, 1, 4);
	mainLayout->addWidget(editTable, 5, 0, 1, 4);
	mainLayout->addWidget(insertRow, 6, 0, 1, 2);
	mainLayout->addWidget(removeRow, 6, 2, 1, 2);
	mainLayout->addWidget(moveRowUp, 7, 0, 1, 2);
	mainLayout->addWidget(moveRowDown, 7, 2, 1, 2);
	mainLayout->addWidget(insertImage, 8, 0, 1, 2);
	mainLayout->addWidget(insertText, 8, 2, 1, 2);

	//use the layout
	setLayout(mainLayout);

	//clear everything
	clearEverything();
}