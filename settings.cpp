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

#include "settings.h"

using namespace MME;

Settings::Settings(std::shared_ptr<Ui::MME> form):
	QSettings(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/config.ini", QSettings::IniFormat),
        defaultPalette(qApp->palette()),
        ui(form)
{
	qApp->setOrganizationName("Hackeridze");
	qApp->setApplicationName("MME");
	qDebug() << fileName() << QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/config.ini";
	sync();
}

Settings::~Settings()
{

}

void Settings::setStyleFusionDark()
{
	qApp->setStyle(QStyleFactory::create("fusion"));

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53,53,53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(15,15,15));
	palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Button, QColor(53,53,53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);

	palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

	palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
	palette.setColor(QPalette::HighlightedText, Qt::black);

	qApp->setPalette(palette);
}

void Settings::setStyleFusion()
{
	qApp->setStyle(QStyleFactory::create("fusion"));
	qApp->setPalette(defaultPalette);
}

void Settings::setStyleWindows()
{
	qApp->setStyle(QStyleFactory::create("windows"));
	qApp->setPalette(defaultPalette);
}

void Settings::setStyleAndroid()
{
	qApp->setStyle(QStyleFactory::create("android"));
	qApp->setPalette(defaultPalette);
}

void Settings::connections()
{
	connect(ui->buttonsSplitter, &QSplitter::splitterMoved,
	        this, &Settings::buttonsSplitterMoved);
	connect(ui->helpSplitter, &QSplitter::splitterMoved,
	        this, &Settings::helpSplitterMoved);
	connect(ui->logSplitter, &QSplitter::splitterMoved,
	        this, &Settings::logSplitterMoved);
}

void Settings::restore()
{
	qDebug() << "rest" << value(LOG_SPLITTER).toByteArray();
	ui->buttonsSplitter->restoreState(
	                        value(BUTTONS_SPLITTER).toByteArray());
	ui->helpSplitter->restoreState(
	                        value(HELP_SPLITTER).toByteArray());
	ui->logSplitter->restoreState(
	                        value(LOG_SPLITTER).toByteArray());
	ui->logSplitter->setOrientation(Qt::Vertical);

	switch (value(THEME).toInt(), 2) {
	case 1:
		setStyleFusion();
		break;
	case 2:
		setStyleFusionDark();
		break;
	case 3:
		setStyleAndroid();
		break;
	case 4:
		setStyleWindows();
		break;
	default:
		break;
	}
}

void MME::Settings::buttonsSplitterMoved(int pos, int index)
{
	setValue(BUTTONS_SPLITTER, ui->buttonsSplitter->saveState());
	sync();
}

void MME::Settings::logSplitterMoved(int pos, int index)
{
	setValue(LOG_SPLITTER, ui->helpSplitter->saveState());
	sync();
}

void MME::Settings::helpSplitterMoved(int pos, int index)
{
	setValue(HELP_SPLITTER, ui->logSplitter->saveState());
	sync();
}

void Settings::saveTheme(int t)
{

}
