/*
	Yellowcot 1.2.3, released 2012-02-24

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

#ifndef YELLOWCOT_H
#define YELLOWCOT_H

#include <QtGui>
#include <cstdlib>
#include <sys/time.h>
#include "pound_defs_and_sysprintf.h"

class YCQuiz : public QWidget {
	Q_OBJECT

	public:
		YCQuiz (QWidget *parent = 0);
		QTabBar *tabBar;
		QComboBox *questionsAndAnswersList;
		QPushButton *currQorA;
		QLabel *rangeLbl;
		QSpinBox *startBox;
		QLabel *toLbl;
		QSpinBox *endBox;
		QLabel *reversedLbl;
		QCheckBox *reversedCheckBox;
		QTableWidget *editTable;
		QPushButton *insertRow;
		QPushButton *removeRow;
		QPushButton *moveRowUp;
		QPushButton *moveRowDown;
		QPushButton *insertImage;
		QPushButton *insertText;
		QCheckBox *fileIsLoaded;
		QComboBox *questionMediaSources;
		QComboBox *answerMediaSources;
		QCheckBox *showLicencingData;

	public slots:
		void resizeEvent(QResizeEvent *event) {
			updateButtonContents();
			QWidget::resizeEvent(event);
		}
		void respondToStartChange() {
			if (!fileIsLoaded->isChecked())
				return;
			int start = startBox->value(), end = endBox->value(), currentIndex = questionsAndAnswersList->currentIndex() / 4 + 1;
			if (start > end) {
				startBox->setValue(end);
				start = end;
			}
			if (currentIndex < start || currentIndex > end) {
				questionsAndAnswersList->setCurrentIndex(3 - 2 * reversedCheckBox->isChecked());
				advanceQorA();
			}
		}
		void respondToEndChange() {
			if (!fileIsLoaded->isChecked())
				return;
			int start = startBox->value(), end = endBox->value(), currentIndex = questionsAndAnswersList->currentIndex() / 4 + 1;
			if (end < start) {
				endBox->setValue(start);
				end = start;
			}
			if (currentIndex < start || currentIndex > end) {
				questionsAndAnswersList->setCurrentIndex(3 - 2 * reversedCheckBox->isChecked());
				advanceQorA();
			}
		}
		void insertARow() {

			//create variables
			int i = editTable->currentRow(), j, numQAPairs = questionMediaSources->count();

			//insert the row
			editTable->insertRow(i);

			//add the cells
			for (j = 0 ; j < 6 ; j++) {
				QTableWidgetItem *cell = new QTableWidgetItem();
				editTable->setItem(i, j, cell);
			}

			//update the media source lists
			questionMediaSources->addItem("");
			for (j = numQAPairs ; j > i ; j--)
				questionMediaSources->setItemText(j, questionMediaSources->itemText(j-1));
			questionMediaSources->setItemText(i, "");
			answerMediaSources->addItem("");
			for (j = numQAPairs ; j > i ; j--)
				answerMediaSources->setItemText(j, answerMediaSources->itemText(j-1));
			answerMediaSources->setItemText(i, "");

			//set current cell
			editTable->setCurrentCell(i, editTable->currentColumn());
		}
		void removeARow() {
			int i=editTable->currentRow();
			if (editTable->rowCount() > 1) {
				editTable->removeRow(i);
				questionMediaSources->removeItem(i);
				answerMediaSources->removeItem(i);
				if (editTable->rowCount() - 1 == editTable->currentRow() || !i)
					editTable->setCurrentCell(editTable->currentRow(), editTable->currentColumn());
				else
					editTable->setCurrentCell(editTable->currentRow() + 1, editTable->currentColumn());
			}
		}
		void moveTheRowUp() {
			int i = editTable->currentRow();
			if (i > 0) {

				//make/prepare variables
				int qtype = 0, atype = 0;
				char str1[STRLEN], str2[STRLEN], str3[STRLEN], str4[STRLEN], str5[STRLEN], str6[STRLEN], str1_top[STRLEN], str4_top[STRLEN];
				memset(str1, 0, STRLEN);
				memset(str2, 0, STRLEN);
				memset(str3, 0, STRLEN);
				memset(str4, 0, STRLEN);
				memset(str5, 0, STRLEN);
				memset(str6, 0, STRLEN);

				//record data in current row (including media source info)
				sprintf(str1, editTable->item(i, 0)->text().toUtf8().data());
				if (str1[0] == '\0') {
					sprintf(str1, questionMediaSources->itemText(i).toUtf8().data());
					if (str1[0] != '\0')
						qtype = 1;
				}
				sprintf(str2, editTable->item(i, 1)->text().toUtf8().data());
				sprintf(str3, editTable->item(i, 2)->text().toUtf8().data());
				sprintf(str4, editTable->item(i, 3)->text().toUtf8().data());
				if (str4[0] == '\0') {
					sprintf(str4, answerMediaSources->itemText(i).toUtf8().data());
					if (str4[0] != '\0')
						atype = 1;
				}
				sprintf(str5, editTable->item(i, 4)->text().toUtf8().data());
				sprintf(str6, editTable->item(i, 5)->text().toUtf8().data());

				//overwrite current row with data from row above
				sprintf(str1_top, editTable->item(i-1, 0)->text().toUtf8().data());
				if (str1_top[0] != '\0') {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str1_top));
					editTable->setItem(i, 0, cell);
				}
				else {
					sprintf(str1_top, questionMediaSources->itemText(i-1).toUtf8().data());
					if (str1_top[0] == '\0') {
						QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(editTable->item(i-1, 0)->text().toUtf8().data()));
						editTable->setItem(i, 0, cell);
					}
					else {
						QTableWidgetItem *cell = new QTableWidgetItem();
						cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str1_top)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
						cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
						editTable->setItem(i, 0, cell);
					}
				}
				questionMediaSources->setItemText(i, questionMediaSources->itemText(i-1));
				editTable->item(i, 1)->setText(editTable->item(i-1, 1)->text());
				editTable->item(i, 2)->setText(editTable->item(i-1, 2)->text());
				sprintf(str4_top, editTable->item(i-1, 3)->text().toUtf8().data());
				if (str4_top[0] != '\0') {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str4_top));
					editTable->setItem(i, 3, cell);
				}
				else {
					sprintf(str4_top, answerMediaSources->itemText(i-1).toUtf8().data());
					if (str4_top[0] == '\0') {
						QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(editTable->item(i-1, 3)->text().toUtf8().data()));
						editTable->setItem(i, 3, cell);
					}
					else {
						QTableWidgetItem *cell = new QTableWidgetItem();
						cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str4_top)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
						cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
						editTable->setItem(i, 3, cell);
					}
				}
				answerMediaSources->setItemText(i, answerMediaSources->itemText(i-1));
				editTable->item(i, 4)->setText(editTable->item(i-1, 4)->text());
				editTable->item(i, 5)->setText(editTable->item(i-1, 5)->text());

				//write data from variables into row above
				if (qtype) {
					QTableWidgetItem *cell = new QTableWidgetItem();
					cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str1)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
					cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
					editTable->setItem(i-1, 0, cell);
					questionMediaSources->setItemText(i-1, QString::fromUtf8(str1));
				}
				else {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str1));
					editTable->setItem(i-1, 0, cell);
					questionMediaSources->setItemText(i-1, "");
				}
				editTable->item(i-1, 1)->setText(QString::fromUtf8(str2));
				editTable->item(i-1, 2)->setText(QString::fromUtf8(str3));
				if (atype) {
					QTableWidgetItem *cell = new QTableWidgetItem();
					cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str4)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
					cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
					editTable->setItem(i-1, 3, cell);
					answerMediaSources->setItemText(i-1, QString::fromUtf8(str4));
				}
				else {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str4));
					editTable->setItem(i-1, 3, cell);
					answerMediaSources->setItemText(i-1, "");
				}
				editTable->item(i-1, 4)->setText(QString::fromUtf8(str5));
				editTable->item(i-1, 5)->setText(QString::fromUtf8(str6));

				//set current cell to row above
				editTable->setCurrentCell(i-1, editTable->currentColumn());
			}
		}
		void moveTheRowDown() {
			int i = editTable->currentRow();
			if (i != -1 && i != editTable->rowCount() - 1) {

				//make/prepare variables
				int qtype = 0, atype = 0;
				char str1[STRLEN], str2[STRLEN], str3[STRLEN], str4[STRLEN], str5[STRLEN], str6[STRLEN], str1_top[STRLEN], str4_top[STRLEN];
				memset(str1, 0, STRLEN);
				memset(str2, 0, STRLEN);
				memset(str3, 0, STRLEN);
				memset(str4, 0, STRLEN);
				memset(str5, 0, STRLEN);
				memset(str6, 0, STRLEN);

				//record data in current row (including media source info)
				sprintf(str1, editTable->item(i, 0)->text().toUtf8().data());
				if (str1[0] == '\0') {
					sprintf(str1, questionMediaSources->itemText(i).toUtf8().data());
					if (str1[0] != '\0')
						qtype = 1;
				}
				sprintf(str2, editTable->item(i, 1)->text().toUtf8().data());
				sprintf(str3, editTable->item(i, 2)->text().toUtf8().data());
				sprintf(str4, editTable->item(i, 3)->text().toUtf8().data());
				if (str4[0] == '\0') {
					sprintf(str4, answerMediaSources->itemText(i).toUtf8().data());
					if (str4[0] != '\0')
						atype = 1;
				}
				sprintf(str5, editTable->item(i, 4)->text().toUtf8().data());
				sprintf(str6, editTable->item(i, 5)->text().toUtf8().data());

				//overwrite current row with data from row below
				sprintf(str1_top, editTable->item(i+1, 0)->text().toUtf8().data());
				if (str1_top[0] != '\0') {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str1_top));
					editTable->setItem(i, 0, cell);
				}
				else {
					sprintf(str1_top, questionMediaSources->itemText(i+1).toUtf8().data());
					if (str1_top[0] == '\0') {
						QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(editTable->item(i+1, 0)->text().toUtf8().data()));
						editTable->setItem(i, 0, cell);
					}
					else {
						QTableWidgetItem *cell = new QTableWidgetItem();
						cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str1_top)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
						cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
						editTable->setItem(i, 0, cell);
					}
				}
				questionMediaSources->setItemText(i, questionMediaSources->itemText(i+1));
				editTable->item(i, 1)->setText(editTable->item(i+1, 1)->text());
				editTable->item(i, 2)->setText(editTable->item(i+1, 2)->text());
				sprintf(str4_top, editTable->item(i+1, 3)->text().toUtf8().data());
				if (str4_top[0] != '\0') {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str4_top));
					editTable->setItem(i, 3, cell);
				}
				else {
					sprintf(str4_top, answerMediaSources->itemText(i+1).toUtf8().data());
					if (str4_top[0] == '\0') {
						QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(editTable->item(i+1, 3)->text().toUtf8().data()));
						editTable->setItem(i, 3, cell);
					}
					else {
						QTableWidgetItem *cell = new QTableWidgetItem();
						cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str4_top)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
						cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
						editTable->setItem(i, 3, cell);
					}
				}
				answerMediaSources->setItemText(i, answerMediaSources->itemText(i+1));
				editTable->item(i, 4)->setText(editTable->item(i+1, 4)->text());
				editTable->item(i, 5)->setText(editTable->item(i+1, 5)->text());

				//write data from variables into row below
				if (qtype) {
					QTableWidgetItem *cell = new QTableWidgetItem();
					cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str1)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
					cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
					editTable->setItem(i+1, 0, cell);
					questionMediaSources->setItemText(i+1, QString::fromUtf8(str1));
				}
				else {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str1));
					editTable->setItem(i+1, 0, cell);
					questionMediaSources->setItemText(i+1, "");
				}
				editTable->item(i+1, 1)->setText(QString::fromUtf8(str2));
				editTable->item(i+1, 2)->setText(QString::fromUtf8(str3));
				if (atype) {
					QTableWidgetItem *cell = new QTableWidgetItem();
					cell->setData(Qt::DecorationRole, QPixmap(QString::fromUtf8(str4)).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
					cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
					editTable->setItem(i+1, 3, cell);
					answerMediaSources->setItemText(i+1, QString::fromUtf8(str4));
				}
				else {
					QTableWidgetItem *cell = new QTableWidgetItem(QString::fromUtf8(str4));
					editTable->setItem(i+1, 3, cell);
					answerMediaSources->setItemText(i+1, "");
				}
				editTable->item(i+1, 4)->setText(QString::fromUtf8(str5));
				editTable->item(i+1, 5)->setText(QString::fromUtf8(str6));

				//set current cell to row below
				editTable->setCurrentCell(i+1, editTable->currentColumn());
			}
		}
		void saveFile(QWidget *qLbl) {
			QLabel *theFilePath = qobject_cast<QLabel*>(qLbl);
			if (theFilePath->text().isNull()) {
				QString qStr = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Yellowcot Files (*.yc)"));
				if (!(qStr.isNull())) {
					theFilePath->setText(qStr);
					fileIsLoaded->setChecked(true);
				}
			}
			if (!(theFilePath->text().isNull())) {
				int i, mediaCtr = 1;
				char str[STRLEN];
				FILE *file;
				FILE *file2;
				sysprintf("mkdir %s/new ; mkdir %s/new/media", TMPDIR, TMPDIR);
				sysprintf("echo \"<?xml version=\\\"1.0\\\"?>\" > %s/new/index.xml", TMPDIR);
				memset(str, 0, STRLEN);
				sprintf(str, "%s/new/index.xml", TMPDIR);
				file = fopen(str, "a+");
				fprintf(file, "<quiz>\n");
				for (i = 0; i < editTable->rowCount(); i++) {

					//begin qa section
					fprintf(file, "	<qa>\n");

					//store question type and question content
					memset(str, 0, STRLEN);
					sprintf(str, questionMediaSources->itemText(i).toUtf8().data());
					if (str[0] == '\0')
						fprintf(file, "		<q type=\"text\">%s</q>\n", (editTable->item(i, 0))->text().toUtf8().data());
					else {
						sysprintf("ext=$(echo \"%s\" | awk -F . '{print tolower($NF)}') ; cp \"%s\" %s/new/media/%d.$ext ; cd %s/new/media ; echo -n %s/media/ > %s/media.txt ; echo %d.* >> %s/media.txt", str, str, TMPDIR, mediaCtr, TMPDIR, TMPDIR, TMPDIR, mediaCtr, TMPDIR);
						fprintf(file, "		<q type=\"media\">%d</q>\n", mediaCtr++);
						memset(str, 0, STRLEN);
						sprintf(str, "%s/media.txt", TMPDIR);
						file2 = fopen(str, "r");
						memset(str, 0, STRLEN);
						fgets(str, STRLEN, file2);
						str[strlen(str) - 1] = '\0';
						fclose(file2);
						sysprintf("rm %s/media.txt", TMPDIR);
						questionMediaSources->setItemText(i, str);
					}

					//store question source
					fprintf(file, "		<qsrc>%s</qsrc>\n", (editTable->item(i, 1))->text().toUtf8().data());

					//store question licence
					fprintf(file, "		<qlic>%s</qlic>\n", (editTable->item(i, 2))->text().toUtf8().data());

					//store answer type and answer content
					memset(str, 0, STRLEN);
					sprintf(str, answerMediaSources->itemText(i).toUtf8().data());
					if (str[0] == '\0')
						fprintf(file, "		<a type=\"text\">%s</a>\n", (editTable->item(i, 3))->text().toUtf8().data());
					else {
						sysprintf("ext=$(echo \"%s\" | awk -F . '{print tolower($NF)}') ; cp \"%s\" %s/new/media/%d.$ext ; cd %s/new/media ; echo -n %s/media/ > %s/media.txt ; echo %d.* >> %s/media.txt", str, str, TMPDIR, mediaCtr, TMPDIR, TMPDIR, TMPDIR, mediaCtr, TMPDIR);
						fprintf(file, "		<a type=\"media\">%d</a>\n", mediaCtr++);
						memset(str, 0, STRLEN);
						sprintf(str, "%s/media.txt", TMPDIR);
						file2 = fopen(str, "r");
						memset(str, 0, STRLEN);
						fgets(str, STRLEN, file2);
						str[strlen(str) - 1] = '\0';
						fclose(file2);
						sysprintf("rm %s/media.txt", TMPDIR);
						answerMediaSources->setItemText(i, str);
					}

					//store answer source
					fprintf(file, "		<asrc>%s</asrc>\n", (editTable->item(i, 4))->text().toUtf8().data());

					//store answer licence
					fprintf(file, "		<alic>%s</alic>\n", (editTable->item(i, 5))->text().toUtf8().data());

					//end qa section
					fprintf(file, "	</qa>\n");
				}
				fprintf(file, "</quiz>");
				fclose(file);
				sysprintf("rm \"%s\" ; cd %s/new ; tar cf \"%s\" * ; cd .. ; rm -r media ; mv new/* . ; rm -r new", theFilePath->text().toUtf8().data(), TMPDIR, theFilePath->text().toUtf8().data());
				memset(str, 0, STRLEN);
				sprintf(str, "File saved to %s.", theFilePath->text().toUtf8().data());
				QMessageBox::information(this, tr("Save"), str);
			}
		}
		void toggleLicencing() {
			if (showLicencingData->isChecked()) {
				editTable->setColumnHidden(1, false);
				editTable->setColumnHidden(2, false);
				editTable->setColumnHidden(4, false);
				editTable->setColumnHidden(5, false);
			}
			else {
				editTable->setColumnHidden(1, true);
				editTable->setColumnHidden(2, true);
				editTable->setColumnHidden(4, true);
				editTable->setColumnHidden(5, true);
			}
		}
		void updateVisibleWidgets() {
			int currentTab = tabBar->currentIndex();
			if (currentTab == 0) {
				showLicencingData->hide();
				editTable->hide();
				insertRow->hide();
				removeRow->hide();
				moveRowUp->hide();
				moveRowDown->hide();
				insertImage->hide();
				insertText->hide();
				currQorA->show();
				rangeLbl->show();
				startBox->show();
				toLbl->show();
				endBox->show();
				reversedLbl->show();
				reversedCheckBox->show();
			}
			else {
				currQorA->hide();
				rangeLbl->hide();
				startBox->hide();
				toLbl->hide();
				endBox->hide();
				reversedLbl->hide();
				reversedCheckBox->hide();
				showLicencingData->show();
				editTable->show();
				insertRow->show();
				removeRow->show();
				moveRowUp->show();
				moveRowDown->show();
				insertImage->show();
				insertText->show();
			}
			updateButtonContents();
		}
		int extractQOrA(char * in, char * out) {
			int i=0, j=0, endCarrotFound=0, startCarrotFound=0;
			memset(out, 0, STRLEN);
			while (in[i] != '\0' && !endCarrotFound) {
				if (in[i] == '>')
					endCarrotFound = 1;
				else
					i++;
			}
			i++;
			if (endCarrotFound) {
				while (in[i] != '\0' && !startCarrotFound) {
					if (in[i] == '<')
						startCarrotFound = 1;
					else {
						out[j] = in[i];
						j++;
						i++;
					}
				}
				if (startCarrotFound)
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
		void extractFileName(char * in, char * out) {
			memset(out, 0, STRLEN);
			int i=0, j=0;
			while (in[i] != '\0') {
				if (in[i] == '/') {
					memset(out, 0, STRLEN);
					j = 0;
				}
				else {
					out[j] = in[i];
					j++;
				}
				i++;
			}
		}
		void extractQOrAType(char * in, char * out) {
			memset(out, 0, STRLEN);
			int i=0, j=0, firstQuotesFound=0, secondQuotesFound=0;
			while (in[i] != '\0' && !firstQuotesFound) {
				if (in[i] == '"')
					firstQuotesFound = 1;
				i++;
			}
			if (firstQuotesFound) {
				while (in[i] != '\0' && !secondQuotesFound)
					if (in[i] == '"')
						secondQuotesFound = 1;
					else {
						out[j] = in[i];
						j++;
						i++;
					}
				if (!secondQuotesFound) {
					memset(out, 0, STRLEN);
					sprintf(out, "text");
				}
			}
			else
				sprintf(out, "text");
		}
		void loadFile (QWidget *qLbl) {
			QLabel *theFilePath = qobject_cast<QLabel*>(qLbl);
			fileIsLoaded->setChecked(false);
			if (!(theFilePath->text().isNull())) {
				FILE *file;
				char str[STRLEN], qOrA[STRLEN], croppedStr[STRLEN], qOrAType[STRLEN];
				int ctr=0;
				questionsAndAnswersList->clear();
				currQorA->setEnabled(true);
				rangeLbl->setEnabled(true);
				startBox->setEnabled(true);
				toLbl->setEnabled(true);
				endBox->setEnabled(true);
				reversedLbl->setEnabled(true);
				reversedCheckBox->setEnabled(true);
				showLicencingData->setEnabled(true);
				editTable->setEnabled(true);
				insertRow->setEnabled(true);
				removeRow->setEnabled(true);
				moveRowUp->setEnabled(true);
				moveRowDown->setEnabled(true);
				insertImage->setEnabled(true);
				insertText->setEnabled(true);
				sysprintf("rm -r %s/* > /dev/null 2>&1 ; tar xf \"%s\" -C %s ; ", TMPDIR, theFilePath->text().toUtf8().data(), TMPDIR);
				memset(str, 0, STRLEN);
				sprintf(str, "%s/index.xml", TMPDIR);
				file = fopen(str, "r");
				if (file != NULL) {
					memset(qOrA, 0, STRLEN);
					while (fgets(qOrA, STRLEN, file)) {
						if (strchr(qOrA, '\n') != NULL)
							if (extractQOrA(qOrA, croppedStr)) {

								//get q/a type and actual content
								extractQOrAType(qOrA, qOrAType);
								questionsAndAnswersList->addItem(QString::fromUtf8(qOrAType), ctr);
								questionsAndAnswersList->addItem(QString::fromUtf8(croppedStr), ctr);

								//get q/a source
								fgets(qOrA, STRLEN, file);
								extractQOrA(qOrA, croppedStr);
								questionsAndAnswersList->addItem(QString::fromUtf8(croppedStr), ctr);

								//get q/a licence
								fgets(qOrA, STRLEN, file);
								extractQOrA(qOrA, croppedStr);
								questionsAndAnswersList->addItem(QString::fromUtf8(croppedStr), ctr);

								//update q/a counter
								ctr++;
							}
						memset(qOrA, 0, STRLEN);
					}
					fclose(file);
				}

				//clear media source lists and fill them with blank spaces
				questionMediaSources->clear();
				answerMediaSources->clear();
				while (ctr > 0) {
					questionMediaSources->addItem("");
					answerMediaSources->addItem("");
					ctr-=2;
				}

				//populate the edit table and media source lists
				ctr = questionsAndAnswersList->count() /4;
				editTable->clear();
				editTable->setRowCount(ctr / 2);
				editTable->setColumnCount(6);
				editTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Question")));
				editTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Question Source")));
				editTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Question Licence")));
				editTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Answer")));
				editTable->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Answer Source")));
				editTable->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Answer Licence")));
				while (ctr > 0) {
					ctr -= 2;

					//populate cell 0 (question)
					if (!QString::compare(questionsAndAnswersList->itemText(ctr*4), QString("media"))) {
						sysprintf("echo %s/media/%s.* > %s/media.txt", TMPDIR, questionsAndAnswersList->itemText(ctr*4+1).toUtf8().data(), TMPDIR);
						memset(str, 0, STRLEN);
						sprintf(str, "%s/media.txt", TMPDIR);
						file = fopen(str, "r");
						memset(str, 0, STRLEN);
						fgets(str, STRLEN, file);
						str[strlen(str) - 1] = '\0';
						fclose(file);
						sysprintf("rm %s/media.txt", TMPDIR);
						QTableWidgetItem *cell0a = new QTableWidgetItem();
						cell0a->setData(Qt::DecorationRole, QPixmap(str).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
						cell0a->setFlags(cell0a->flags() & ~Qt::ItemIsEditable);
						editTable->setItem(ctr/2, 0, cell0a);
						questionMediaSources->setItemText(ctr/2, str);
					}
					else {
						QTableWidgetItem *cell0b = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+1).toUtf8().data()));
						editTable->setItem(ctr/2, 0, cell0b);
					}

					//populate cell 1 (question source)
					QTableWidgetItem *cell1 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+2).toUtf8().data()));
					editTable->setItem(ctr/2, 1, cell1);

					//populate cell 2 (question licence)
					QTableWidgetItem *cell2 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+3).toUtf8().data()));
					editTable->setItem(ctr/2, 2, cell2);

					//populate cell 3 (answer)
					if (!QString::compare(questionsAndAnswersList->itemText(ctr*4+4), QString("media"))) {
						sysprintf("echo %s/media/%s.* > %s/media.txt", TMPDIR, questionsAndAnswersList->itemText(ctr*4+5).toUtf8().data(), TMPDIR);
						memset(str, 0, STRLEN);
						sprintf(str, "%s/media.txt", TMPDIR);
						file = fopen(str, "r");
						memset(str, 0, STRLEN);
						fgets(str, STRLEN, file);
						str[strlen(str) - 1] = '\0';
						fclose(file);
						sysprintf("rm %s/media.txt", TMPDIR);
						QTableWidgetItem *cell3a = new QTableWidgetItem();
						cell3a->setData(Qt::DecorationRole, QPixmap(str).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
						cell3a->setFlags(cell3a->flags() & ~Qt::ItemIsEditable);
						editTable->setItem(ctr/2, 3, cell3a);
						answerMediaSources->setItemText(ctr/2, str);
					}
					else {
						QTableWidgetItem *cell3b = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+5).toUtf8().data()));
						editTable->setItem(ctr/2, 3, cell3b);
					}

					//populate cell 4 (answer source)
					QTableWidgetItem *cell4 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+6).toUtf8().data()));
					editTable->setItem(ctr/2, 4, cell4);

					//populate cell 5 (answer licence)
					QTableWidgetItem *cell5 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+7).toUtf8().data()));
					editTable->setItem(ctr/2, 5, cell5);
				}
				editTable->resizeColumnsToContents();
				editTable->resizeRowsToContents();

				//remove (from play) qa pairs which do not have both question and answer content
				ctr = questionsAndAnswersList->count();
				while (ctr) {
					memset(str, 0, STRLEN);
					sprintf(str, questionsAndAnswersList->itemText(ctr - 7).toUtf8().data());
					if (str[0] != '\0') {
						memset(str, 0, STRLEN);
						sprintf(str, questionsAndAnswersList->itemText(ctr - 3).toUtf8().data());
						if (str[0] == '\0') {
							questionsAndAnswersList->removeItem(ctr - 1);
							questionsAndAnswersList->removeItem(ctr - 2);
							questionsAndAnswersList->removeItem(ctr - 3);
							questionsAndAnswersList->removeItem(ctr - 4);
							questionsAndAnswersList->removeItem(ctr - 5);
							questionsAndAnswersList->removeItem(ctr - 6);
							questionsAndAnswersList->removeItem(ctr - 7);
							questionsAndAnswersList->removeItem(ctr - 8);
						}
					}
					else {
						questionsAndAnswersList->removeItem(ctr - 1);
						questionsAndAnswersList->removeItem(ctr - 2);
						questionsAndAnswersList->removeItem(ctr - 3);
						questionsAndAnswersList->removeItem(ctr - 4);
						questionsAndAnswersList->removeItem(ctr - 5);
						questionsAndAnswersList->removeItem(ctr - 6);
						questionsAndAnswersList->removeItem(ctr - 7);
						questionsAndAnswersList->removeItem(ctr - 8);
					}
					ctr -= 8;
				}

				//set up the start/end spinners, and flip to a random question/answer
				ctr = questionsAndAnswersList->count() / 8;
				startBox->setMinimum(1);
				startBox->setMaximum(ctr);
				startBox->setValue(1);
				endBox->setMinimum(1);
				endBox->setMaximum(ctr);
				endBox->setValue(ctr);
				questionsAndAnswersList->setCurrentIndex(5 - 4 * reversedCheckBox->isChecked());
				advanceQorA();

				//flag the file as loaded
				fileIsLoaded->setChecked(true);
			}
		}
		void updateButtonContents() {
			int buttonWidth=currQorA->width(), buttonHeight=currQorA->height();
			char str[STRLEN], newStr[STRLEN];
			FILE *file;
			if (!strncmp(questionsAndAnswersList->itemText(questionsAndAnswersList->currentIndex() - 1).toUtf8().data(), "media", STRLEN)) {
				sysprintf("echo %s/media/%s.* > %s/media.txt", TMPDIR, questionsAndAnswersList->currentText().toUtf8().data(), TMPDIR);
				memset(str, 0, STRLEN);
				sprintf(str, "%s/media.txt", TMPDIR);
				file = fopen(str, "r");
				memset(str, 0, STRLEN);
				fgets(str, STRLEN, file);
				str[strlen(str) - 1] = '\0';
				fclose(file);
				sysprintf("rm %s/media.txt", TMPDIR);
				currQorA->setIcon(QIcon(QPixmap(str).scaled(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
				currQorA->setIconSize(QSize(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER));
			}
			else {
				memset(newStr, 0, STRLEN);
				addSlashes(questionsAndAnswersList->currentText().toUtf8().data(), newStr);
				sysprintf("font=$(more ~/.yellowcot/config | grep \"font=\") ; fontsize=$(more ~/.yellowcot/config | grep \"fontsize=\") ; if echo $font | grep -q -v \"^[#]\" ; then convert -font ${font:5} -gravity Center -background transparent -pointsize ${fontsize:9} -size %dx caption:\"%s\" %s/text.png ; else convert -gravity Center -background transparent -pointsize ${fontsize:9} -size %dx caption:\"%s\" %s/text.png ; fi", buttonWidth - IMAGEBORDER, newStr, TMPDIR, buttonWidth - IMAGEBORDER, newStr, TMPDIR);
				memset(newStr, 0, STRLEN);
				sprintf(newStr, "%s/text.png", TMPDIR);
				currQorA->setIcon(QIcon(QPixmap(newStr).scaled(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
				currQorA->setIconSize(QSize(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER));
			}
		}
		void addSlashes(char * initStr, char * newStr) {
			int i, j=0;
			int keepGoing = 1;
			for (i = 0 ; i < STRLEN ; i++)
				if (keepGoing == 1) {
					if (initStr[i] == '\0') {
						newStr[j] = '\0';
						keepGoing = 0;
					}
					if (initStr[i] == '\"') {
						if (j > (STRLEN - 3))
							keepGoing = 0;
						else {
							newStr[j++] = '\\';
							newStr[j++] = initStr[i];
						}
					}
					else {
						if (j > (STRLEN - 2))
							keepGoing = 0;
						else
							newStr[j++] = initStr[i];
					}
				}
			newStr[STRLEN - 1] = '\0';
		}
		void advanceQorA () {

			//declare variables
			int start = atoi(startBox->text().toUtf8().data()), end = atoi(endBox->text().toUtf8().data()), currentIndex = questionsAndAnswersList->currentIndex();
			timeval tv;

			//generate new random seed based on microseconds since UNIX epoch
			gettimeofday(&tv, NULL);
			srand((int)tv.tv_usec);

			//advance the question/answer
			if (reversedCheckBox->isChecked()) {
				if (currentIndex%8 == 5)
					questionsAndAnswersList->setCurrentIndex(currentIndex - 4);
				else
					questionsAndAnswersList->setCurrentIndex((int)(((double)rand() * (end - start + 1)) / RAND_MAX + start - 1) * 8 + 5);
			}
			else {
				if (currentIndex%8 == 1)
					questionsAndAnswersList->setCurrentIndex(currentIndex + 4);
				else
					questionsAndAnswersList->setCurrentIndex((int)(((double)rand() * (end - start + 1)) / RAND_MAX + start - 1) * 8 + 1);
			}
			updateButtonContents();
		}
		void insertAnImage() {
			int curRow = editTable->currentRow(), curCol = editTable->currentColumn();
			if ((curCol == 0 || curCol == 3) && curRow != -1) {
				QString qStr = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("All Images (*.*)"));
				if (!qStr.isNull()) {
					QTableWidgetItem *cell = new QTableWidgetItem();
					cell->setData(Qt::DecorationRole, QPixmap(qStr).scaledToHeight(EDIT_TABLE_IMG_HEIGHT, Qt::SmoothTransformation));
					cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
					editTable->setItem(curRow, curCol, cell);
					if (curCol)
						answerMediaSources->setItemText(curRow, qStr);
					else
						questionMediaSources->setItemText(curRow, qStr);
				}
			}
		}
		void insertAText() {
			int curRow = editTable->currentRow(), curCol = editTable->currentColumn();
			char str[STRLEN];

			if ((curCol == 0 || curCol == 3) && curRow != -1) {
				memset(str, 0, STRLEN);
				if (curCol) {
					sprintf(str, answerMediaSources->itemText(curRow).toUtf8().data());
					if (str[0] != '\0') {
						QTableWidgetItem *cell = new QTableWidgetItem();
						editTable->setItem(curRow, curCol, cell);
						answerMediaSources->setItemText(curRow, "");
					}
				}
				else {
					sprintf(str, questionMediaSources->itemText(curRow).toUtf8().data());
					if (str[0] != '\0') {
						QTableWidgetItem *cell = new QTableWidgetItem();
						editTable->setItem(curRow, curCol, cell);
						questionMediaSources->setItemText(curRow, "");
					}
				}
			}
		}
		void startNewFile(QWidget *qLbl) {

			//clear theFilePath
			QLabel *theFilePath = qobject_cast<QLabel*>(qLbl);
			theFilePath->setText(QString());

			//flag the file as not-loaded
			fileIsLoaded->setChecked(false);

			//clear everything
			clearEverything();

		}
		void clearEverything() {

			//clear widgets
			questionsAndAnswersList->clear();
			questionMediaSources->clear();
			answerMediaSources->clear();
			questionMediaSources->addItem("");
			answerMediaSources->addItem("");
			editTable->clear();
			editTable->setRowCount(1);

			//set up the start/end spinners just as if a file with no questions/answers had been loaded
			startBox->setMinimum(1);
			startBox->setMaximum(0);
			startBox->setValue(0);
			endBox->setMinimum(1);
			endBox->setMaximum(0);
			endBox->setValue(0);

			//create variable
			int j;

			//add cells to edit table row
			for (j = 0 ; j < 6 ; j++) {
				QTableWidgetItem *cell = new QTableWidgetItem();
				editTable->setItem(0, j, cell);
			}

			//clear big question/answer button
			advanceQorA();

			//set edit table column headers
			editTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Question")));
			editTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Question Source")));
			editTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Question Licence")));
			editTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Answer")));
			editTable->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Answer Source")));
			editTable->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Answer Licence")));
		}
		void exportToPDF() {

			//get ready
			char str[STRLEN];

			//determine number of qa pairs
			int num = endBox->maximum();

			//determine card width and height
			int card_w = EXPORT_PAGE_W * EXPORT_DPI / 2 - EXPORT_PAGE_BORDER;
			int card_h = (EXPORT_PAGE_H * EXPORT_DPI - 2 * EXPORT_PAGE_BORDER) / EXPORT_PAGE_ROWS;

			//make white card image
			sysprintf("convert -size %dx%d canvas:white %s/white_card.png", card_w - EXPORT_BLACK_BORDER_W * 2 - EXPORT_WHITE_BORDER_W * 2, card_h - EXPORT_BLACK_BORDER_W * 2 - EXPORT_WHITE_BORDER_W * 2, TMPDIR);

			//loop through cards, creating an image for each
			int i;
			for (i = 0; i < num ; i++) {

				//create image for question card
				if (!QString::compare(questionsAndAnswersList->itemText(i*8), QString("media")))
					sysprintf("convert %s/media/%s.* -resize %dx%d %s/%d_a.png", TMPDIR, questionsAndAnswersList->itemText(i*8 + 1).toUtf8().data(), card_w - EXPORT_BLACK_BORDER_W * 2 - EXPORT_WHITE_BORDER_W * 2, card_h - EXPORT_BLACK_BORDER_W * 2 - EXPORT_WHITE_BORDER_W * 2, TMPDIR, i);
				else {
					memset(str, 0, STRLEN);
					addSlashes(questionsAndAnswersList->itemText(i * 8 + 1).toUtf8().data(), str);
					sysprintf("font=$(more ~/.yellowcot/config | grep \"font=\") ; if echo $font | grep -q -v \"^[#]\" ; then convert -font ${font:5} -gravity Center -background transparent -pointsize %d -size %dx caption:\"%s\" %s/%d_a.png ; else convert -gravity Center -background transparent -pointsize %d -size %dx caption:\"%s\" %s/%d_a.png ; fi", EXPORT_FONT_SIZE, card_w - 2 * EXPORT_BLACK_BORDER_W - 2 * EXPORT_WHITE_BORDER_W, str, TMPDIR, i, EXPORT_FONT_SIZE, card_w - 2 * EXPORT_BLACK_BORDER_W - 2 * EXPORT_WHITE_BORDER_W, str, TMPDIR, i);
				}
				sysprintf("composite -gravity center %s/%d_a.png %s/white_card.png %s/%d_a.png", TMPDIR, i, TMPDIR, TMPDIR, i);

				//create image for answer card
				if (!QString::compare(questionsAndAnswersList->itemText(i*8 + 4), QString("media")))
					sysprintf("convert %s/media/%s.* -resize %dx%d %s/%d_b.png", TMPDIR, questionsAndAnswersList->itemText(i*8 + 5).toUtf8().data(), card_w - EXPORT_BLACK_BORDER_W * 2 - EXPORT_WHITE_BORDER_W * 2, card_h - EXPORT_BLACK_BORDER_W * 2 - EXPORT_WHITE_BORDER_W * 2, TMPDIR, i);
				else {
					memset(str, 0, STRLEN);
					addSlashes(questionsAndAnswersList->itemText(i * 8 + 5).toUtf8().data(), str);
					sysprintf("font=$(more ~/.yellowcot/config | grep \"font=\") ; if echo $font | grep -q -v \"^[#]\" ; then convert -font ${font:5} -gravity Center -background transparent -pointsize %d -size %dx caption:\"%s\" %s/%d_b.png ; else convert -gravity Center -background transparent -pointsize %d -size %dx caption:\"%s\" %s/%d_b.png ; fi", EXPORT_FONT_SIZE, card_w - 2 * EXPORT_BLACK_BORDER_W - 2 * EXPORT_WHITE_BORDER_W, str, TMPDIR, i, EXPORT_FONT_SIZE, card_w - 2 * EXPORT_BLACK_BORDER_W - 2 * EXPORT_WHITE_BORDER_W, str, TMPDIR, i);
				}
				sysprintf("composite -gravity center %s/%d_b.png %s/white_card.png %s/%d_b.png", TMPDIR, i, TMPDIR, TMPDIR, i);

			}

			//add black border and white border to all card images
			sysprintf("mogrify -bordercolor black -border %d %s/*_a.png", EXPORT_BLACK_BORDER_W, TMPDIR);
			sysprintf("mogrify -bordercolor white -border %d %s/*_a.png", EXPORT_WHITE_BORDER_W, TMPDIR);
			sysprintf("mogrify -bordercolor black -border %d %s/*_b.png", EXPORT_BLACK_BORDER_W, TMPDIR);
			sysprintf("mogrify -bordercolor white -border %d %s/*_b.png", EXPORT_WHITE_BORDER_W, TMPDIR);

			//loop through cards, placing their images on white sheets
			int pagenum = 0;
			for (i = 0; i < num; i++) {
				if (!(i%EXPORT_PAGE_ROWS))
					sysprintf("convert -size %dx%d canvas:white %s/%d_c.png", (int)(EXPORT_PAGE_W * EXPORT_DPI), (int)(EXPORT_PAGE_H * EXPORT_DPI), TMPDIR, pagenum++);
				sysprintf("convert %s/%d_c.png %s/%d_a.png -geometry +%d+%d -composite %s/%d_c.png", TMPDIR, pagenum - 1, TMPDIR, i, EXPORT_PAGE_BORDER, EXPORT_PAGE_BORDER + card_h * (i%EXPORT_PAGE_ROWS), TMPDIR, pagenum - 1);
				sysprintf("convert %s/%d_c.png %s/%d_b.png -geometry +%d+%d -composite %s/%d_c.png", TMPDIR, pagenum - 1, TMPDIR, i, EXPORT_PAGE_BORDER + card_w, EXPORT_PAGE_BORDER + card_h * (i%EXPORT_PAGE_ROWS), TMPDIR, pagenum - 1);
			}

			//create jpeg-compressed PDF file from PNG images of pages
			sysprintf("echo -n \"convert -density %d -compress jpeg\" > %s/export.sh", EXPORT_DPI, TMPDIR);
			for (i = 0 ; i < (num - 1) / EXPORT_PAGE_ROWS + 1; i++)
				sysprintf("echo -n \" %d_c.png\" >> %s/export.sh", i, TMPDIR);
			sysprintf("echo \" ~/flash_cards.pdf\" >> %s/export.sh", TMPDIR);
			sysprintf("cd %s ; sh export.sh", TMPDIR);

			//clean up tmp dir
			sysprintf("cd %s ; rm *_a.png *_b.png *_c.png white_card.png export.sh", TMPDIR);

			//inform user that export is complete
			QMessageBox::information(this, tr("Export"), tr("Flash cards exported to ~/flash_cards.pdf"));
		}
};

#endif