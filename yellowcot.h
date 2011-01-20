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

#ifndef YELLOWCOT_H
#define YELLOWCOT_H

#include <QtGui>
#include <cstdlib>
#include <sys/time.h>

#define VERSION "1.1.14"
#define STRLEN 1000
#define MINWIDTH 500
#define MINHEIGHT 300
#define BUTTON_TEXT_SIZE 18
#define IMAGEBORDER 12

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
		QCheckBox *fileIsLoaded;

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
			int i = editTable->currentRow();
			editTable->insertRow(i);
			editTable->setCellWidget(i, 0, new QComboBox);
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->addItem(QString::fromUtf8("text"));
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->addItem(QString::fromUtf8("image"));
			QTableWidgetItem *cell1 = new QTableWidgetItem();
			editTable->setItem(i, 1, cell1);
			editTable->setCellWidget(i, 2, new QComboBox);
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->addItem(QString::fromUtf8("text"));
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->addItem(QString::fromUtf8("image"));
			QTableWidgetItem *cell3 = new QTableWidgetItem();
			editTable->setItem(i, 3, cell3);
			editTable->setCurrentCell(editTable->currentRow() - 1, editTable->currentColumn());
		}
		void removeARow() {
			int i=editTable->currentRow();
			if (editTable->rowCount() > 1) {
				editTable->removeRow(i);
				if (editTable->rowCount() - 1 == editTable->currentRow() || !i)
					editTable->setCurrentCell(editTable->currentRow(), editTable->currentColumn());
				else
					editTable->setCurrentCell(editTable->currentRow() + 1, editTable->currentColumn());
			}
		}
		void moveTheRowUp() {
			int i = editTable->currentRow();
			if (i != -1 && i && editTable->rowCount() > 1) {
				int col1, col3;
				char str2[STRLEN], str4[STRLEN];
				memset(str2, 0, STRLEN);
				memset(str4, 0, STRLEN);
				col1 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->currentIndex();
				sprintf(str2, editTable->item(i, 1)->text().toUtf8().data());
				col3 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->currentIndex();
				sprintf(str4, editTable->item(i, 3)->text().toUtf8().data());
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 0))->currentIndex());
				editTable->item(i, 1)->setText(editTable->item(i-1, 1)->text());
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 2))->currentIndex());
				editTable->item(i, 3)->setText(editTable->item(i-1, 3)->text());
				qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 0))->setCurrentIndex(col1);
				editTable->item(i-1, 1)->setText(QString::fromUtf8(str2));
				qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 2))->setCurrentIndex(col3);
				editTable->item(i-1, 3)->setText(QString::fromUtf8(str4));
				editTable->setCurrentCell(i-1, editTable->currentColumn());
			}
		}
		void moveTheRowDown() {
			int i = editTable->currentRow();
			if (i != -1 && i != editTable->rowCount() - 1) {
				int col1, col3;
				char str2[STRLEN], str4[STRLEN];
				memset(str2, 0, STRLEN);
				memset(str4, 0, STRLEN);
				col1 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->currentIndex();
				sprintf(str2, editTable->item(i, 1)->text().toUtf8().data());
				col3 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->currentIndex();
				sprintf(str4, editTable->item(i, 3)->text().toUtf8().data());
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 0))->currentIndex());
				editTable->item(i, 1)->setText(editTable->item(i+1, 1)->text());
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 2))->currentIndex());
				editTable->item(i, 3)->setText(editTable->item(i+1, 3)->text());
				qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 0))->setCurrentIndex(col1);
				editTable->item(i+1, 1)->setText(QString::fromUtf8(str2));
				qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 2))->setCurrentIndex(col3);
				editTable->item(i+1, 3)->setText(QString::fromUtf8(str4));
				editTable->setCurrentCell(i+1, editTable->currentColumn());
			}
		}
		void saveFile(QWidget *qLbl) {
			QLabel *theFilePath = qobject_cast<QLabel*>(qLbl);
			if (!(theFilePath->text().isNull())) {
				int i;
				char str[STRLEN];
				FILE *file;
				FILE *file2;
				system("mv /var/tmp/yellowcot_quiz/i /var/tmp/yellowcot_quiz/i_backup");
				system("mkdir /var/tmp/yellowcot_quiz/i");
				system("echo \"<?xml version=\\\"1.0\\\"?>\" > /var/tmp/yellowcot_quiz/index.xml");
				file = fopen("/var/tmp/yellowcot_quiz/index.xml", "a+");
				fprintf(file, "<quiz>\n");
				for (i = 0; i < editTable->rowCount(); i++) {
					fprintf(file, "	<qa>\n");
					if (qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->currentIndex()) {
						memset(str, 0, STRLEN);
						sprintf(str, "/var/tmp/yellowcot_quiz/i_backup/%s", (editTable->item(i, 1))->text().toUtf8().data());
						if ((file2 = fopen(str, "r"))) {
							fclose(file2);
							memset(str, 0, STRLEN);
							sprintf(str, "cp \"/var/tmp/yellowcot_quiz/i_backup/%s\" /var/tmp/yellowcot_quiz/i/.", (editTable->item(i, 1))->text().toUtf8().data());
							system(str);
							fprintf(file, "		<q type=\"image\">%s</q>\n", (editTable->item(i, 1))->text().toUtf8().data());
						}
						else if ((file2 = fopen((editTable->item(i, 1))->text().toUtf8().data(), "r"))) {
							fclose(file2);
							memset(str, 0, STRLEN);
							sprintf(str, "cp \"%s\" /var/tmp/yellowcot_quiz/i/.", (editTable->item(i, 1))->text().toUtf8().data());
							system(str);
							extractFileName((editTable->item(i, 1))->text().toUtf8().data(), str);
							fprintf(file, "		<q type=\"image\">%s</q>\n", str);
						}
						else {
							memset(str, 0, STRLEN);
							sprintf(str, "Image not saved because it could not be found: %s", (editTable->item(i, 1))->text().toUtf8().data());
							QMessageBox::warning(this, tr("Missing image"), str);
							fprintf(file, "		<q type=\"text\">(missing image)</q>\n");
						}
					}
					else
						fprintf(file, "		<q type=\"text\">%s</q>\n", (editTable->item(i, 1))->text().toUtf8().data());
					if (qobject_cast<QComboBox*>(editTable->cellWidget(i, 2))->currentIndex()) {
						memset(str, 0, STRLEN);
						sprintf(str, "/var/tmp/yellowcot_quiz/i_backup/%s", (editTable->item(i, 3))->text().toUtf8().data());
						if ((file2 = fopen(str, "r"))) {
							fclose(file2);
							memset(str, 0, STRLEN);
							sprintf(str, "cp \"/var/tmp/yellowcot_quiz/i_backup/%s\" /var/tmp/yellowcot_quiz/i/.", (editTable->item(i, 3))->text().toUtf8().data());
							system(str);
							fprintf(file, "		<a type=\"image\">%s</a>\n", (editTable->item(i, 3))->text().toUtf8().data());
						}
						else if ((file2 = fopen((editTable->item(i, 3))->text().toUtf8().data(), "r"))) {
							fclose(file2);
							memset(str, 0, STRLEN);
							sprintf(str, "cp \"%s\" /var/tmp/yellowcot_quiz/i/.", (editTable->item(i, 3))->text().toUtf8().data());
							system(str);
							extractFileName((editTable->item(i, 3))->text().toUtf8().data(), str);
							fprintf(file, "		<a type=\"image\">%s</a>\n", str);
						}
						else {
							memset(str, 0, STRLEN);
							sprintf(str, "Image not saved because it could not be found: %s", (editTable->item(i, 3))->text().toUtf8().data());
							QMessageBox::warning(this, tr("Missing image"), str);
							fprintf(file, "		<a type=\"text\">(missing image)</a>\n");
						}
					}
					else
						fprintf(file, "		<a type=\"text\">%s</a>\n", (editTable->item(i, 3))->text().toUtf8().data());
					fprintf(file, "	</qa>\n");
				}
				system("rm -r /var/tmp/yellowcot_quiz/i_backup");
				fprintf(file, "</quiz>");
				fclose(file);
				memset(str, 0, STRLEN);
				sprintf(str, "rm \"%s\" ; cd /var/tmp/yellowcot_quiz ; tar cf \"%s\" --exclude=text.png *", theFilePath->text().toUtf8().data(), theFilePath->text().toUtf8().data());
				system(str);
				memset(str, 0, STRLEN);
				sprintf(str, "File saved to %s.", theFilePath->text().toUtf8().data());
				QMessageBox::information(this, tr("Save"), str);
			}
		}
		void updateVisibleWidgets() {
			int currentTab = tabBar->currentIndex();
			if (currentTab == 0) {
				editTable->hide();
				insertRow->hide();
				removeRow->hide();
				moveRowUp->hide();
				moveRowDown->hide();
				insertImage->hide();
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
				editTable->show();
				insertRow->show();
				removeRow->show();
				moveRowUp->show();
				moveRowDown->show();
				insertImage->show();
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
				char qOrA[STRLEN], untarStr[STRLEN], croppedStr[STRLEN], qOrAType[STRLEN];
				int ctr=0;
				questionsAndAnswersList->clear();
				currQorA->setEnabled(true);
				rangeLbl->setEnabled(true);
				startBox->setEnabled(true);
				toLbl->setEnabled(true);
				endBox->setEnabled(true);
				reversedLbl->setEnabled(true);
				reversedCheckBox->setEnabled(true);
				editTable->setEnabled(true);
				insertRow->setEnabled(true);
				removeRow->setEnabled(true);
				moveRowUp->setEnabled(true);
				moveRowDown->setEnabled(true);
				insertImage->setEnabled(true);
				memset(untarStr, 0, STRLEN);
				sprintf(untarStr, "rm -r /var/tmp/yellowcot_quiz > /dev/null 2>&1 ; mkdir /var/tmp/yellowcot_quiz ; tar xf \"%s\" -C /var/tmp/yellowcot_quiz ; ", theFilePath->text().toUtf8().data());
				system(untarStr);
				file = fopen("/var/tmp/yellowcot_quiz/index.xml", "r");
				if (file != NULL) {
					memset(qOrA, 0, STRLEN);
					while (fgets(qOrA, STRLEN, file)) {
						if (strchr(qOrA, '\n') != NULL)
							if (extractQOrA(qOrA, croppedStr)) {
								extractQOrAType(qOrA, qOrAType);
								questionsAndAnswersList->addItem(QString::fromUtf8(qOrAType), ctr);
								questionsAndAnswersList->addItem(QString::fromUtf8(croppedStr), ctr);
								ctr++;
							}
						memset(qOrA, 0, STRLEN);
					}
					fclose(file);
				}
				startBox->setMinimum(1);
				startBox->setMaximum(ctr/2);
				startBox->setValue(1);
				endBox->setMinimum(1);
				endBox->setMaximum(ctr/2);
				endBox->setValue(ctr/2);
				questionsAndAnswersList->setCurrentIndex(3 - 2 * reversedCheckBox->isChecked());
				advanceQorA();
				editTable->clear();
				editTable->setRowCount(ctr / 2);
				editTable->setColumnCount(4);
				editTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Question Type")));
				editTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Question")));
				editTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Answer Type")));
				editTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Answer")));
				while (ctr > 0) {
					ctr-= 2;
					editTable->setCellWidget(ctr/2, 0, new QComboBox);
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 0))->addItem(QString::fromUtf8("text"));
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 0))->addItem(QString::fromUtf8("image"));
					if (!QString::compare(questionsAndAnswersList->itemText(ctr*2), QString("image")))
						qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 0))->setCurrentIndex(1);
					QTableWidgetItem *cell1 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*2+1).toUtf8().data()));
					editTable->setItem(ctr/2, 1, cell1);
					editTable->setCellWidget(ctr/2, 2, new QComboBox);
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 2))->addItem(QString::fromUtf8("text"));
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 2))->addItem(QString::fromUtf8("image"));
					if (!QString::compare(questionsAndAnswersList->itemText(ctr*2+2), QString("image")))
						qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 2))->setCurrentIndex(1);
					QTableWidgetItem *cell3 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*2+3).toUtf8().data()));
					editTable->setItem(ctr/2, 3, cell3);
				}
				editTable->resizeColumnsToContents();
				editTable->resizeRowsToContents();
				fileIsLoaded->setChecked(true);
			}
		}
		void updateButtonContents() {
			int buttonWidth=currQorA->width(), buttonHeight=currQorA->height();
			char str[STRLEN], newStr[STRLEN];
			if (!strncmp(questionsAndAnswersList->itemText(questionsAndAnswersList->currentIndex() - 1).toUtf8().data(), "image", STRLEN)) {
				memset(str, 0, STRLEN);
				sprintf(str, "/var/tmp/yellowcot_quiz/i/%s", questionsAndAnswersList->currentText().toUtf8().data());
				currQorA->setIcon(QIcon(QPixmap(str).scaled(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
				currQorA->setIconSize(QSize(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER));
			}
			else {
				memset(str, 0, STRLEN);
				memset(newStr, 0, STRLEN);
				addSlashes(questionsAndAnswersList->currentText().toUtf8().data(), newStr);
				sprintf(str, "convert -gravity Center -background transparent -pointsize %d -size %dx caption:\"%s\" /var/tmp/yellowcot_quiz/text.png", BUTTON_TEXT_SIZE, buttonWidth - IMAGEBORDER, newStr);
				system(str);
				currQorA->setIcon(QIcon(QPixmap("/var/tmp/yellowcot_quiz/text.png").scaled(buttonWidth - IMAGEBORDER, buttonHeight - IMAGEBORDER, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
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
				if (currentIndex%4 == 3)
					questionsAndAnswersList->setCurrentIndex(currentIndex - 2);
				else
					questionsAndAnswersList->setCurrentIndex((int)(((double)rand() * (end - start + 1)) / RAND_MAX + start - 1) * 4 + 3);
			}
			else {
				if (currentIndex%4 == 1)
					questionsAndAnswersList->setCurrentIndex(currentIndex + 2);
				else
					questionsAndAnswersList->setCurrentIndex((int)(((double)rand() * (end - start + 1)) / RAND_MAX + start - 1) * 4 + 1);
			}
			updateButtonContents();
		}
		void insertAnImage() {
			int curRow = editTable->currentRow(), curCol = editTable->currentColumn();
			if (curCol != -1 && curRow != -1) {
				QString qStr = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/karam", tr("All Images (*.*)"));
				if (!qStr.isNull()) {
					qobject_cast<QComboBox*>(editTable->cellWidget(curRow, curCol/2*2))->setCurrentIndex(1);
					QTableWidgetItem *cell2 = new QTableWidgetItem(qStr);
					editTable->setItem(curRow, curCol/2*2 + 1, cell2);
				}
			}
		}
};

#endif