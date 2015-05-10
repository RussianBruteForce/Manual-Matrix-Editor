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

#ifndef BUTTONWITHNUM_H
#define BUTTONWITHNUM_H

#include <QWidget>
#include <QPushButton>

class ButtonWithNum : public QPushButton
{
	Q_OBJECT
public:
	explicit ButtonWithNum(int x, QWidget *parent=0);
	~ButtonWithNum() {}

signals:
	void selected(int r);
private:
	int num;
private slots:
	void onClicked();
};

#endif // BUTTONWITHNUM_H
