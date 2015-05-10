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

#ifndef FLOATDELEGATE_H
#define FLOATDELEGATE_H

#include <memory>
#include <QObject>
#include <QItemDelegate>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QAbstractItemModel>

class FloatDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	FloatDelegate();
	~FloatDelegate() {}

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;

	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;


private:
	std::shared_ptr<QDoubleValidator> validator;
};

#endif // FLOATDELEGATE_H
