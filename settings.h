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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <memory>
#include <QSettings>
#include <QStandardPaths>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QStyle>
#include <QDebug>
#include "ui_main.h"

#define BUTTONS_SPLITTER "Splitters/Buttons"
#define HELP_SPLITTER "Splitters/Help"
#define LOG_SPLITTER "Splitters/Log"
#define THEME "Theme"


namespace MME{

class Settings : public QSettings
{
public:
	explicit Settings(std::shared_ptr<Ui::MME> form);
	~Settings();
	void setStyleFusionDark();
	void setStyleFusion();
	void setStyleWindows();
	void setStyleAndroid();

	void connections();

	void restore();

private slots:
	void buttonsSplitterMoved(int pos, int index);

	void logSplitterMoved(int pos, int index);

	void helpSplitterMoved(int pos, int index);

	void saveTheme(int t);

private:
	QPalette defaultPalette;
	std::shared_ptr<Ui::MME> ui;
};

}

#endif // SETTINGS_H
