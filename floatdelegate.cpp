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

#include "floatdelegate.h"

FloatDelegate::FloatDelegate()
{
	validator = std::make_shared<QDoubleValidator>(-9999, 9999, 16, this);
}

QWidget* FloatDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto *editor = new QLineEdit(parent);
	editor->setFrame(false);
	editor->setValidator(validator.get());
	editor->setAlignment(Qt::AlignCenter);

	return editor;
}

void FloatDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	float value = index.model()->data(index, Qt::EditRole).toFloat();

	QLineEdit *edit = static_cast<QLineEdit*>(editor);
	edit->setText(QString::number(value));
}

void FloatDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QLineEdit *edit = static_cast<QLineEdit*>(editor);

	float value = edit->text().toFloat();

	model->setData(index, value, Qt::EditRole);
}

void FloatDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

