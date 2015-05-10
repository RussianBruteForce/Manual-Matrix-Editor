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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent),
        ui(std::make_shared<Ui::MME>()),
        matrix(std::make_unique<MME::Matrix>(3,3)),
        settings(std::make_unique<MME::Settings>(ui)),
        matrixWidget(new QTableView(this)),
        delegate(std::make_unique<FloatDelegate>()),
        sizeValidator(new QIntValidator(1,9,this))
{
	ui->setupUi(this);
	setRowsB(3);
	ui->rowLayout->setDirection(QBoxLayout::TopToBottom);
	ui->matrixLayout->setDirection(QBoxLayout::BottomToTop);
	ui->matrixLayout->addWidget(matrixWidget);

	ui->rowEdit->setValidator(sizeValidator);
	ui->colEdit->setValidator(sizeValidator);

	matrixWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	matrixWidget->setModel(matrix.get());
	matrixWidget->setItemDelegate(delegate.get());
	matrixWidget->horizontalHeader()->hide();
	matrixWidget->verticalHeader()->hide();
	connect(matrix.get(), &MME::Matrix::dataChanged,
	        matrixWidget, &QTableView::resizeColumnsToContents);
	connect(matrix.get(), &MME::Matrix::dataChanged,
	        matrixWidget, &QTableView::resizeRowsToContents);
	matrixWidget->resizeColumnsToContents();
	matrixWidget->resizeRowsToContents();

	ui->helpLabel->setText(tr(
                               #include MANUALFILE
	                               ));

	settings->connections();
	settings->restore();
	checkMatrixSize();
}

void MainWindow::decRowsB()
{
	rowsButtons.last()->disconnect();
	delete rowsButtons.takeLast();
}

void MainWindow::incRowsB()
{
	rowsButtons.append(new ButtonWithNum(rowsButtons.size()+1, this));
	ui->rowLayout->addWidget(rowsButtons.last());
	connect(rowsButtons.last(), &ButtonWithNum::selected,
	        this, &MainWindow::rowSelected);
}

void MainWindow::setRowsB(int n)
{
	if (n < 1)
		return;
	while (n != rowsButtons.size()) {
		if (n < rowsButtons.size())
			decRowsB();
		else
			incRowsB();
	}
}

void MainWindow::log(QString str)
{
	logText.prepend(str + "<br>");
	ui->log->setText(logText);
}

void MainWindow::checkMatrixSize()
{
	if (matrix->isSquare()) {
		ui->detButton->setEnabled(true);
		ui->bareissButton->setEnabled(true);
		ui->mulLastColButton->setEnabled(false);
	} else {
		ui->detButton->setEnabled(false);
		ui->bareissButton->setEnabled(false);

		if (
		                matrix->rowCount() + 1
		                ==
		                matrix->columnCount()
		                )
			ui->mulLastColButton->setEnabled(true);
		else
			ui->mulLastColButton->setEnabled(false);
	}
}

void MainWindow::printDet3x3()
{
	auto s = [this](int x, int y) {
		return QString::number(matrix->data(matrix->index(x,y)).toFloat());
	};
	auto f = [this](int x, int y) {
		return matrix->data(matrix->index(x,y)).toFloat();
	};

	log ("= " %
	     QString::number(
	             f(0,0)
	             *
	             f(1,1)
	             *
	             f(2,2)) %
	     " + " %
	     QString::number(
	             f(0,1)
	             *
	             f(1,2)
	             *
	             f(2,0)) %
	     " + " %
	     QString::number(
	             f(0,2)
	             *
	             f(1,0)
	             *
	             f(2,1)) %
	     " - " %
	     QString::number(
	             f(0,2)
	             *
	             f(1,1)
	             *
	             f(2,0)) %
	     " - " %
	     QString::number(
	             f(0,1)
	             *
	             f(1,0)
	             *
	             f(2,2)) %
	     " - " %
	     QString::number(
	             f(0,0)
	             *
	             f(1,2)
	             *
	             f(2,1))
	     );
	log (
	                        s(0,0) %
	                        "•" %
	                        s(1,1) %
	                        "•" %
	                        s(2,2) %
	                        " + " % // +
	                        s(0,1) %
	                        "•" %
	                        s(1,2) %
	                        "•" %
	                        s(2,0) %
	                        " + " % // +
	                        s(0,2) %
	                        "•" %
	                        s(1,0) %
	                        "•" %
	                        s(2,1) %
	                        " - " % // -
	                        s(0,2) %
	                        "•" %
	                        s(1,1) %
	                        "•" %
	                        s(2,0) %
	                        " - " % // -
	                        s(0,1) %
	                        "•" %
	                        s(1,0) %
	                        "•" %
	                        s(2,2) %
	                        " - " % // -
	                        s(0,0) %
	                        "•" %
	                        s(1,2) %
	                        "•" %
	                        s(2,1) % " ="
	                        );
}

void MainWindow::on_rowEdit_textChanged(const QString &arg1)
{
	auto n = arg1.toInt();

	if (n < 1)
		return;
	matrix->setRows(n);
	matrixWidget->resizeRowsToContents();

	setRowsB(arg1.toInt());

	checkMatrixSize();
}

void MainWindow::on_colEdit_textChanged(const QString &arg1)
{
	auto m = arg1.toInt();

	if (m < 1)
		return;
	matrix->setCols(m);
	matrixWidget->resizeColumnsToContents();

	checkMatrixSize();
}

void MainWindow::on_goButton_clicked()
{
	QString c = ui->command->text().trimmed();
	if (c.contains("swap")) {
		//swap Ra Rb
		if (c[5] == 'R' && c[8] == 'R') {
			int a = c[6].digitValue(), b = c[9].digitValue();
			matrix->swapRows(--a,--b);
		}
	} else if (c.indexOf('+') == 3) {
		//Ra + Rb * x
		if (c.contains('*') && c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			float x = c.mid(10).toFloat();
			qDebug() << a << b << x;
			matrix->plusRows(--a,--b,x);
		}
		//Ra + Rb
		else if (c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			qDebug() << a << b;
			matrix->plusRows(--a,--b);
		}
		//Ra + x
		else if (c[0] == 'R') {
			int a = c[1].digitValue();
			float x = c.mid(4).toFloat();
			qDebug() << a << "x:" << x;
			matrix->plusRow(--a, x);
		}
	} else if (c.indexOf('-') == 3) {
		qDebug()<< '-';
		//Ra - Rb * x
		if (c.contains('*') && c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			float x = c.mid(10).toFloat();
			qDebug() << a << b << x;
			matrix->minusRows(--a,--b,x);
		}
		//Ra - Rb
		else if (c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			qDebug() << a << b;
			matrix->minusRows(--a,--b);
		}
		//Ra - x
		else if (c[0] == 'R') {
			int a = c[1].digitValue();
			float x = c.mid(4).toFloat();
			qDebug() << a << "x:" << x;
			matrix->minusRow(--a, x);
		}
	} else if (c.indexOf('*') == 3) {
		//Ra * Rb * x
		if (c.lastIndexOf('*') == 8 && c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			float x = c.mid(10).toFloat();
			qDebug() << a << b << x;
			matrix->mulRows(--a,--b,x);
		}
		//Ra * Rb
		else if (c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			qDebug() << a << b;
			matrix->mulRows(--a,--b);
		}
		//Ra * x
		else if (c[0] == 'R') {
			int a = c[1].digitValue();
			float x = c.mid(4).toFloat();
			qDebug() << a << "x:" << x;
			matrix->mulRow(--a, x);
		}
	} else if (c.indexOf('/') == 3) {
		//Ra / Rb * x
		if (c.lastIndexOf('*') == 8 && c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			float x = c.mid(10).toFloat();
			qDebug() << a << b << x;
			matrix->divRows(--a,--b,x);
		}
		//Ra / Rb
		else if (c[0] == 'R' && c[5] == 'R') {
			int a = c[1].digitValue(), b = c[6].digitValue();
			qDebug() << a << b;
			matrix->divRows(--a,--b);
		}
		//Ra / x
		else if (c[0] == 'R') {
			int a = c[1].digitValue();
			float x = c.mid(4).toFloat();
			qDebug() << a << "x:" << x;
			matrix->divRow(--a, x);
		}
	}

	ui->command->clear();
	log(c);
}

void MainWindow::on_swapButton_clicked()
{
	ui->command->setText("swap ");
}

void MainWindow::on_plusButton_clicked()
{
	ui->command->setText(ui->command->text() + "+ ");
}

void MainWindow::on_mulButton_clicked()
{
	ui->command->setText(ui->command->text() + "* ");
}

void MainWindow::on_minusButton_clicked()
{
	ui->command->setText(ui->command->text() + "- ");
}

void MainWindow::on_divButton_clicked()
{
	ui->command->setText(ui->command->text() + "/ ");
}

void MainWindow::on_clearButton_clicked()
{
	float x = ui->command->text().toFloat();
	matrix->clear(x);

	log(tr("Cleaning..."));

	matrixWidget->resizeColumnsToContents();
}

void MainWindow::rowSelected(int r)
{
	ui->command->setText(ui->command->text() + "R" + QString::number(r) + " ");
}

void MainWindow::on_oneButton_clicked()
{
	ui->command->setText(ui->command->text() + "1 ");
}

void MainWindow::on_twoButton_clicked()
{
	ui->command->setText(ui->command->text() + "2 ");
}

void MainWindow::on_threeButton_clicked()
{
	ui->command->setText(ui->command->text() + "3 ");
}

void MainWindow::on_fourButton_clicked()
{
	ui->command->setText(ui->command->text() + "4 ");
}

void MainWindow::on_identityButton_clicked()
{
	ui->detButton->setEnabled(false);
	ui->bareissButton->setEnabled(false);
	matrix->addIdentity();

	log(tr("Identity matrix appended"));

	ui->colEdit->setText(QString::number(matrix->columnCount()));
	matrixWidget->resizeColumnsToContents();
}

void MainWindow::on_undoButton_clicked()
{
	matrix->undo();

	checkMatrixSize();

	log(tr("Undo"));
}

void MainWindow::on_nextLayerButton_clicked()
{
	ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_submitTextMatrixButton_clicked()
{
	auto s = ui->textMatrixEdit->toPlainText();
	if(s.isEmpty()) {
		ui->stackedWidget->setCurrentIndex(0);
		return;
	}
	QList<QList<float>> m;
	QTextStream text(&s), line;
	QString l, f;
	while (true) {
		l = text.readLine();
		if (l.isNull())
			break;
		line.setString(&l);
		QList<float> mLine;
		while (true) {
			line >> f;
			if(!f.isNull())
				mLine.append(f.toFloat());
			else
				break;
		}
		m.append(mLine);
	}
	this->matrix->setMatrix(m);
	setRowsB(m.size());
	ui->rowEdit->setText(QString::number(m.size()));
	ui->colEdit->setText(QString::number(MME::Matrix::findLargestRow(m)));
	ui->stackedWidget->setCurrentIndex(0);

	checkMatrixSize();
}

void MainWindow::on_detButton_clicked()
{
	log(tr("Determinant is ")
	    % QString::number(matrix->detSquareMatrix()));
	if (
	                matrix->columnCount() == 3
	                &&
	                matrix->rowCount()    == 3)
		printDet3x3();
}

void MainWindow::on_electricButton_clicked()
{
	if (!electricPlayer) {
		electricPlayer = std::make_unique<QMediaPlayer>();
		electricPlayer->setMedia(QUrl("qrc:/electric.mp3"));
		electricPlayer->setVolume(80);
		electricPlayer->play();
	} else {
		switch (electricPlayer->state()) {
		case QMediaPlayer::PlayingState:
			electricPlayer->pause();
			break;
		case QMediaPlayer::PausedState:
			electricPlayer->play();
			break;
		default:
			electricPlayer->pause();
			break;
		}
	}
}


void MainWindow::on_bareissButton_clicked()
{
	log(tr("Triangle by Bareiss"));
	matrix->toSquareByBareiss();
}


void MainWindow::on_transposeButton_clicked()
{
	log(tr("Transposing"));
	matrix->transpose();
}

void MainWindow::on_mulLastColButton_clicked()
{
	log(tr("Multiply on last row"));
	matrix->mulMatrixOnLastRow();
}
