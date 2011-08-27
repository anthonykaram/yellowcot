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

#ifndef YELLOWCOT_H
#define YELLOWCOT_H

#include <QtGui>
#include <cstdlib>
#include <sys/time.h>

#define VERSION "1.2.0"
#define STRLEN 1000
#define MINWIDTH 500
#define MINHEIGHT 300
#define IMAGEBORDER 12

class YCQuiz : public QWidget {
	Q_OBJECT

	public:
		YCQuiz (QWidget *parent = 0);
		QTabBar *tabBar;
		QComboBox *questionsAndAnswersList;
		QCheckBox *fileIsLoaded;

		//quiz tab widgets
		QPushButton *currQorA;
		QLabel *rangeLbl;
		QSpinBox *startBox;
		QLabel *toLbl;
		QSpinBox *endBox;
		QLabel *reversedLbl;
		QCheckBox *reversedCheckBox;

		//media tab widgets
		QTableWidget *mediaTable;

		//questions/answers tab widgets
		QTableWidget *editTable;
		QPushButton *insertRow;
		QPushButton *removeRow;
		QPushButton *moveRowUp;
		QPushButton *moveRowDown;
		QPushButton *insertImage;

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

			//add question cells
			editTable->setCellWidget(i, 0, new QComboBox);
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->addItem(QString::fromUtf8("text"));
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->addItem(QString::fromUtf8("image"));
			QTableWidgetItem *cell1 = new QTableWidgetItem();
			editTable->setItem(i, 1, cell1);
			QTableWidgetItem *cell2 = new QTableWidgetItem();
			editTable->setItem(i, 2, cell2);
			QTableWidgetItem *cell3 = new QTableWidgetItem();
			editTable->setItem(i, 3, cell3);

			//add answer cells
			editTable->setCellWidget(i, 4, new QComboBox);
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->addItem(QString::fromUtf8("text"));
			qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->addItem(QString::fromUtf8("image"));
			QTableWidgetItem *cell5 = new QTableWidgetItem();
			editTable->setItem(i, 5, cell5);
			QTableWidgetItem *cell6 = new QTableWidgetItem();
			editTable->setItem(i, 6, cell6);
			QTableWidgetItem *cell7 = new QTableWidgetItem();
			editTable->setItem(i, 7, cell7);

			//set current cell
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

				//make/prepare variables
				int col1, col5;
				char str2[STRLEN], str3[STRLEN], str4[STRLEN], str6[STRLEN], str7[STRLEN], str8[STRLEN];
				memset(str2, 0, STRLEN);
				memset(str3, 0, STRLEN);
				memset(str4, 0, STRLEN);
				memset(str6, 0, STRLEN);
				memset(str7, 0, STRLEN);
				memset(str8, 0, STRLEN);

				//record data in row
				col1 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->currentIndex();
				sprintf(str2, editTable->item(i, 1)->text().toUtf8().data());
				sprintf(str3, editTable->item(i, 2)->text().toUtf8().data());
				sprintf(str4, editTable->item(i, 3)->text().toUtf8().data());
				col5 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->currentIndex();
				sprintf(str6, editTable->item(i, 5)->text().toUtf8().data());
				sprintf(str7, editTable->item(i, 6)->text().toUtf8().data());
				sprintf(str8, editTable->item(i, 7)->text().toUtf8().data());

				//overwrite row with data from row above
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 0))->currentIndex());
				editTable->item(i, 1)->setText(editTable->item(i-1, 1)->text());
				editTable->item(i, 2)->setText(editTable->item(i-1, 2)->text());
				editTable->item(i, 3)->setText(editTable->item(i-1, 3)->text());
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 4))->currentIndex());
				editTable->item(i, 5)->setText(editTable->item(i-1, 5)->text());
				editTable->item(i, 6)->setText(editTable->item(i-1, 6)->text());
				editTable->item(i, 7)->setText(editTable->item(i-1, 7)->text());

				//write data from variables into row above
				qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 0))->setCurrentIndex(col1);
				editTable->item(i-1, 1)->setText(QString::fromUtf8(str2));
				editTable->item(i-1, 2)->setText(QString::fromUtf8(str3));
				editTable->item(i-1, 3)->setText(QString::fromUtf8(str4));
				qobject_cast<QComboBox*>(editTable->cellWidget(i-1, 4))->setCurrentIndex(col5);
				editTable->item(i-1, 5)->setText(QString::fromUtf8(str6));
				editTable->item(i-1, 6)->setText(QString::fromUtf8(str7));
				editTable->item(i-1, 7)->setText(QString::fromUtf8(str8));

				//set current cell to row above
				editTable->setCurrentCell(i-1, editTable->currentColumn());
			}
		}
		void moveTheRowDown() {
			int i = editTable->currentRow();
			if (i != -1 && i != editTable->rowCount() - 1) {

				//make/prepare variables
				int col1, col5;
				char str2[STRLEN], str3[STRLEN], str4[STRLEN], str6[STRLEN], str7[STRLEN], str8[STRLEN];
				memset(str2, 0, STRLEN);
				memset(str3, 0, STRLEN);
				memset(str4, 0, STRLEN);
				memset(str6, 0, STRLEN);
				memset(str7, 0, STRLEN);
				memset(str8, 0, STRLEN);

				//record data in row
				col1 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->currentIndex();
				sprintf(str2, editTable->item(i, 1)->text().toUtf8().data());
				sprintf(str3, editTable->item(i, 2)->text().toUtf8().data());
				sprintf(str4, editTable->item(i, 3)->text().toUtf8().data());
				col5 = qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->currentIndex();
				sprintf(str6, editTable->item(i, 5)->text().toUtf8().data());
				sprintf(str7, editTable->item(i, 6)->text().toUtf8().data());
				sprintf(str8, editTable->item(i, 7)->text().toUtf8().data());

				//overwrite row with data from row below
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 0))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 0))->currentIndex());
				editTable->item(i, 1)->setText(editTable->item(i+1, 1)->text());
				editTable->item(i, 2)->setText(editTable->item(i+1, 2)->text());
				editTable->item(i, 3)->setText(editTable->item(i+1, 3)->text());
				qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->setCurrentIndex(qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 4))->currentIndex());
				editTable->item(i, 5)->setText(editTable->item(i+1, 5)->text());
				editTable->item(i, 6)->setText(editTable->item(i+1, 6)->text());
				editTable->item(i, 7)->setText(editTable->item(i+1, 7)->text());

				//write data from variables into row below
				qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 0))->setCurrentIndex(col1);
				editTable->item(i+1, 1)->setText(QString::fromUtf8(str2));
				editTable->item(i+1, 2)->setText(QString::fromUtf8(str3));
				editTable->item(i+1, 3)->setText(QString::fromUtf8(str4));
				qobject_cast<QComboBox*>(editTable->cellWidget(i+1, 4))->setCurrentIndex(col5);
				editTable->item(i+1, 5)->setText(QString::fromUtf8(str6));
				editTable->item(i+1, 6)->setText(QString::fromUtf8(str7));
				editTable->item(i+1, 7)->setText(QString::fromUtf8(str8));

				//set current cell to row below
				editTable->setCurrentCell(i+1, editTable->currentColumn());
			}
		}
		void exportMP3() {

			//declare variables
			timeval tv;
			int x;
			int numqs = questionsAndAnswersList->count() / 8;
			char str[STRLEN];
			int numinstances = 3;

			//generate new random seed based on microseconds since UNIX epoch
			gettimeofday(&tv, NULL);
			srand((int)tv.tv_usec);

			//create the mp3
			system("sox -r 16k -n /var/tmp/yellowcot_quiz/silence.wav trim 0 5");
			system("cp /var/tmp/yellowcot_quiz/silence.wav /var/tmp/yellowcot_quiz/out.wav");
			for (int i = 0 ; i < numqs * numinstances ; i++) {
				x = (int)((double)rand() * numqs / RAND_MAX + 1);
				memset(str, 0, STRLEN);
				sprintf(str, "echo \"%s\" | text2wave -o /var/tmp/yellowcot_quiz/question.wav", questionsAndAnswersList->itemText(x * 8 - 7).toUtf8().data());
				system(str);
				system("sox /var/tmp/yellowcot_quiz/out.wav /var/tmp/yellowcot_quiz/question.wav /var/tmp/yellowcot_quiz/silence.wav /var/tmp/yellowcot_quiz/out2.wav");
				system("mv /var/tmp/yellowcot_quiz/out2.wav /var/tmp/yellowcot_quiz/out.wav");
				memset(str, 0, STRLEN);
				sprintf(str, "echo \"%s\" | text2wave -o /var/tmp/yellowcot_quiz/answer.wav", questionsAndAnswersList->itemText(x * 8 - 3).toUtf8().data());
				system(str);
				system("sox /var/tmp/yellowcot_quiz/out.wav /var/tmp/yellowcot_quiz/answer.wav /var/tmp/yellowcot_quiz/silence.wav /var/tmp/yellowcot_quiz/out2.wav");
				system("mv /var/tmp/yellowcot_quiz/out2.wav /var/tmp/yellowcot_quiz/out.wav");
			}
			system("lame /var/tmp/yellowcot_quiz/out.wav ~/ycquiz.mp3 ; rm /var/tmp/yellowcot_quiz/*.wav");

			//tell the user of the success
			QMessageBox::information(this, tr("Export to MP3"), tr("Yellowcot quiz successfully exported to: ~/ycquiz.mp3"));
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

					//begin qa section
					fprintf(file, "	<qa>\n");

					//store question type and question content
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

					//store question source
					fprintf(file, "		<qsrc>%s</qsrc>\n", (editTable->item(i, 2))->text().toUtf8().data());

					//store question licence
					fprintf(file, "		<qlic>%s</qlic>\n", (editTable->item(i, 3))->text().toUtf8().data());

					//store answer type and answer content
					if (qobject_cast<QComboBox*>(editTable->cellWidget(i, 4))->currentIndex()) {
						memset(str, 0, STRLEN);
						sprintf(str, "/var/tmp/yellowcot_quiz/i_backup/%s", (editTable->item(i, 5))->text().toUtf8().data());
						if ((file2 = fopen(str, "r"))) {
							fclose(file2);
							memset(str, 0, STRLEN);
							sprintf(str, "cp \"/var/tmp/yellowcot_quiz/i_backup/%s\" /var/tmp/yellowcot_quiz/i/.", (editTable->item(i, 5))->text().toUtf8().data());
							system(str);
							fprintf(file, "		<a type=\"image\">%s</a>\n", (editTable->item(i, 5))->text().toUtf8().data());
						}
						else if ((file2 = fopen((editTable->item(i, 5))->text().toUtf8().data(), "r"))) {
							fclose(file2);
							memset(str, 0, STRLEN);
							sprintf(str, "cp \"%s\" /var/tmp/yellowcot_quiz/i/.", (editTable->item(i, 5))->text().toUtf8().data());
							system(str);
							extractFileName((editTable->item(i, 5))->text().toUtf8().data(), str);
							fprintf(file, "		<a type=\"image\">%s</a>\n", str);
						}
						else {
							memset(str, 0, STRLEN);
							sprintf(str, "Image not saved because it could not be found: %s", (editTable->item(i, 5))->text().toUtf8().data());
							QMessageBox::warning(this, tr("Missing image"), str);
							fprintf(file, "		<a type=\"text\">(missing image)</a>\n");
						}
					}
					else
						fprintf(file, "		<a type=\"text\">%s</a>\n", (editTable->item(i, 5))->text().toUtf8().data());

					//store answer source
					fprintf(file, "		<asrc>%s</asrc>\n", (editTable->item(i, 6))->text().toUtf8().data());

					//store answer licence
					fprintf(file, "		<alic>%s</alic>\n", (editTable->item(i, 7))->text().toUtf8().data());

					//end qa section
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

				//hide media tab widgets
				mediaTable->hide();

				//hide questions/answers tab widgets
				editTable->hide();
				insertRow->hide();
				removeRow->hide();
				moveRowUp->hide();
				moveRowDown->hide();
				insertImage->hide();

				//show quiz tab widgets
				currQorA->show();
				rangeLbl->show();
				startBox->show();
				toLbl->show();
				endBox->show();
				reversedLbl->show();
				reversedCheckBox->show();
			}
			else if (currentTab == 1) {

				//hide quiz tab widgets
				currQorA->hide();
				rangeLbl->hide();
				startBox->hide();
				toLbl->hide();
				endBox->hide();
				reversedLbl->hide();
				reversedCheckBox->hide();

				//hide questions/answers tab widgets
				editTable->hide();
				insertRow->hide();
				removeRow->hide();
				moveRowUp->hide();
				moveRowDown->hide();
				insertImage->hide();

				//show media tab widgets
				mediaTable->show();
			}
			else {

				//hide quiz tab widgets
				currQorA->hide();
				rangeLbl->hide();
				startBox->hide();
				toLbl->hide();
				endBox->hide();
				reversedLbl->hide();
				reversedCheckBox->hide();

				//hide media tab widgets
				mediaTable->hide();

				//show questions/answers tab widgets
				editTable->show();
				insertRow->show();
				removeRow->show();
				moveRowUp->show();
				moveRowDown->show();
				insertImage->show();
			}
			updateButtonContents();
		}
		int extractXMLContent(char * in, const char * tag, char * out) {

			//prepare variables
			int i=0, j=0, k=0, startTagLength=0, endTagLength=0, tagFound=0;
			char startTag[STRLEN], endTag[STRLEN];
			memset(out, 0, STRLEN);
			memset(startTag, 0, STRLEN);
			memset(endTag, 0, STRLEN);

			//create start tag and store its length
			startTag[0] = '<';
			while (tag[i] != '\0') {
				startTag[i+1] = tag[i];
				i++;
			}
			startTag[i+1] = '>';
			startTagLength = i + 2;

			//create end tag and store its length
			i = 0;
			endTag[0] = '<';
			endTag[1] = '/';
			while (tag[i] != '\0') {
				endTag[i+2] = tag[i];
				i++;
			}
			endTag[i+2] = '>';
			endTagLength = i + 3;

			//read through completion of start tag
			i = 0;
			while (!tagFound && in[j] != '\0') {
				if (in[j] == startTag[i]) {
					i++;
					if (i == startTagLength)
						tagFound = 1;
				}
				else
					i = 0;
				j++;
			}
			if (!tagFound)
				return 0;

			//read through completion of end tag
			tagFound = 0;
			i = 0;
			while (!tagFound && in[j] != '\0') {
				out[k++] = in[j];
				if (in[j] == endTag[i]) {
					i++;
					if (i == endTagLength)
						tagFound = 1;
				}
				else
					i = 0;
				j++;
			}
			if (!tagFound)
				return 0;

			//remove the end tag from the end of the data
			out[k-i] = '\0';
			return 1;
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
				char indexXMLChunk[STRLEN], qOrA[STRLEN], untarStr[STRLEN], croppedStr[STRLEN], qOrAType[STRLEN], content[STRLEN];
				int ctr=0, currCol=0, mediaRows;
				questionsAndAnswersList->clear();
				mediaTable->setEnabled(true);
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
				sprintf(untarStr, "rm -r /var/tmp/yellowcot_quiz/* > /dev/null 2>&1 ; tar xf \"%s\" -C /var/tmp/yellowcot_quiz ; ", theFilePath->text().toUtf8().data());
				system(untarStr);

				//read in data (old code)
				file = fopen("/var/tmp/yellowcot_quiz/index.xml", "r");
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
				editTable->setColumnCount(8);
				editTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Question Type")));
				editTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Question")));
				editTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Question Source")));
				editTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Question Licence")));
				editTable->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Answer Type")));
				editTable->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Answer")));
				editTable->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Answer Source")));
				editTable->setHorizontalHeaderItem(7, new QTableWidgetItem(tr("Answer Licence")));
				while (ctr > 0) {
					ctr -= 2;

					//populate cell 0 (question type)
					editTable->setCellWidget(ctr/2, 0, new QComboBox);
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 0))->addItem(QString::fromUtf8("text"));
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 0))->addItem(QString::fromUtf8("image"));
					if (!QString::compare(questionsAndAnswersList->itemText(ctr*4), QString("image")))
						qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 0))->setCurrentIndex(1);

					//populate cell 1 (question)
					QTableWidgetItem *cell1 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+1).toUtf8().data()));
					editTable->setItem(ctr/2, 1, cell1);

					//populate cell 2 (question source)
					QTableWidgetItem *cell2 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+2).toUtf8().data()));
					editTable->setItem(ctr/2, 2, cell2);

					//populate cell 3 (question licence)
					QTableWidgetItem *cell3 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+3).toUtf8().data()));
					editTable->setItem(ctr/2, 3, cell3);

					//populate cell 4 (answer type)
					editTable->setCellWidget(ctr/2, 4, new QComboBox);
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 4))->addItem(QString::fromUtf8("text"));
					qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 4))->addItem(QString::fromUtf8("image"));
					if (!QString::compare(questionsAndAnswersList->itemText(ctr*4+4), QString("image")))
						qobject_cast<QComboBox*>(editTable->cellWidget(ctr/2, 4))->setCurrentIndex(1);

					//populate cell 5 (answer)
					QTableWidgetItem *cell5 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+5).toUtf8().data()));
					editTable->setItem(ctr/2, 5, cell5);

					//populate cell 6 (answer source)
					QTableWidgetItem *cell6 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+6).toUtf8().data()));
					editTable->setItem(ctr/2, 6, cell6);

					//populate cell 7 (answer licence)
					QTableWidgetItem *cell7 = new QTableWidgetItem(QString::fromUtf8(questionsAndAnswersList->itemText(ctr*4+7).toUtf8().data()));
					editTable->setItem(ctr/2, 7, cell7);
				}
				editTable->resizeColumnsToContents();
				editTable->resizeRowsToContents();

				//populate media table based on index.xml
				file = fopen("/var/tmp/yellowcot_quiz/index.xml", "r");
				mediaTable->clear();
				mediaTable->setColumnCount(4);
				mediaTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Extension")));
				mediaTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Source")));
				mediaTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Licence")));
				mediaTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Used?")));
				if (file != NULL) {
					memset(indexXMLChunk, 0, STRLEN);
					while (fgets(indexXMLChunk, STRLEN, file)) {
						if (strchr(indexXMLChunk, '\n') != NULL)
							if (extractXMLContent(indexXMLChunk, "id", content))
								mediaRows++;
						memset(indexXMLChunk, 0, STRLEN);
					}
					fclose(file);
				}
				mediaTable->setRowCount(mediaRows);
				ctr = 0;
				file = fopen("/var/tmp/yellowcot_quiz/index.xml", "r");
				if (file != NULL) {
					memset(indexXMLChunk, 0, STRLEN);
					while (ctr < mediaRows && fgets(indexXMLChunk, STRLEN, file)) {
						if (strchr(indexXMLChunk, '\n') != NULL)
							if (!currCol && extractXMLContent(indexXMLChunk, "extension", content)) {
								QTableWidgetItem *mediaCell = new QTableWidgetItem(QString::fromUtf8(content));
								mediaTable->setItem(ctr, currCol++, mediaCell);
							}
							else if (currCol == 1 && extractXMLContent(indexXMLChunk, "src", content)) {
								QTableWidgetItem *mediaCell = new QTableWidgetItem(QString::fromUtf8(content));
								mediaTable->setItem(ctr, currCol++, mediaCell);
							}
							else if (currCol == 2 && extractXMLContent(indexXMLChunk, "licence", content)) {
								QTableWidgetItem *mediaCell = new QTableWidgetItem(QString::fromUtf8(content));
								mediaTable->setItem(ctr, currCol++, mediaCell);
								QTableWidgetItem *mediaCell2 = new QTableWidgetItem(QString::fromUtf8("N"));
								mediaTable->setItem(ctr++, currCol, mediaCell2);
								currCol = 0;
							}
						memset(indexXMLChunk, 0, STRLEN);
					}
					fclose(file);
				}
				mediaTable->resizeColumnsToContents();
				mediaTable->resizeRowsToContents();

				//note that file has been loaded
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
				sprintf(str, "font=$(more ~/.yellowcot/config | grep \"font=\") ; fontsize=$(more ~/.yellowcot/config | grep \"fontsize=\") ; if echo $font | grep -q -v \"^[#]\" ; then convert -font ${font:5} -gravity Center -background transparent -pointsize ${fontsize:9} -size %dx caption:\"%s\" /var/tmp/yellowcot_quiz/text.png ; else convert -gravity Center -background transparent -pointsize ${fontsize:9} -size %dx caption:\"%s\" /var/tmp/yellowcot_quiz/text.png ; fi", buttonWidth - IMAGEBORDER, newStr, buttonWidth - IMAGEBORDER, newStr);
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