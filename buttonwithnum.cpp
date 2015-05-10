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

#include "buttonwithnum.h"

ButtonWithNum::ButtonWithNum(int x, QWidget* parent):
	QPushButton(QString::number(x), parent),
	num(x)
{
	setMaximumWidth(40);
	setMaximumHeight(24);
	connect(this, &ButtonWithNum::clicked,
		this, &ButtonWithNum::onClicked);
}

void ButtonWithNum::onClicked()
{
	emit selected(num);
}

