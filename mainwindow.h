/*
 * This file is part of Manual Matrix Editor.
 *
 * Manual Matrix Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Manual Matrix Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Manual Matrix Editor.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QWidget>
#include <QTableView>
#include <QItemDelegate>
#include <QLinkedList>
#include <QDebug>
#include <QMediaPlayer>
#include <QStringBuilder>
#include "settings.h"
#include "ui_main.h"
#include "matrix.h"
#include "floatdelegate.h"
#include "buttonwithnum.h"

#define MANUALFILE "MANUAL"

namespace UI {
class MainWindow;
}

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow() {}

private:
	std::unique_ptr<QMediaPlayer> electricPlayer;
	std::shared_ptr<Ui::MME> ui;
	std::unique_ptr<MME::Matrix> matrix;
	std::unique_ptr<MME::Settings> settings;
	QTableView *matrixWidget;
	std::unique_ptr<FloatDelegate> delegate;
	QIntValidator *sizeValidator;
	QLinkedList<ButtonWithNum*> rowsButtons;

	QString logText;

	void decRowsB();
	void incRowsB();
	void setRowsB(int n);

	void log(QString str);
	void checkMatrixSize();

	void printDet3x3();

private slots:
	void rowSelected(int r);
	void on_rowEdit_textChanged(const QString &arg1);
	void on_colEdit_textChanged(const QString &arg1);
	void on_goButton_clicked();
	void on_swapButton_clicked();
	void on_plusButton_clicked();
	void on_mulButton_clicked();
	void on_minusButton_clicked();
	void on_divButton_clicked();
	void on_clearButton_clicked();
	void on_oneButton_clicked();
	void on_twoButton_clicked();
	void on_threeButton_clicked();
	void on_fourButton_clicked();
	void on_identityButton_clicked();
	void on_undoButton_clicked();
	void on_nextLayerButton_clicked();
	void on_submitTextMatrixButton_clicked();
	void on_detButton_clicked();
	void on_electricButton_clicked();
	void on_bareissButton_clicked();
	void on_transposeButton_clicked();
	void on_mulLastColButton_clicked();
};

#endif // MAINWINDOW_H
