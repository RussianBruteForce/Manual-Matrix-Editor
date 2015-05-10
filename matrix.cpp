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

#include "matrix.h"

using namespace MME;

Matrix::Matrix(int n, int m):
	N(n),
	M(m)
{
	clear();
	backup();
}

int Matrix::rowCount(const QModelIndex& parent) const
{
	return N;
}

int Matrix::columnCount(const QModelIndex& parent) const
{
	return M;
}

QVariant Matrix::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}
	if (role == Qt::DisplayRole || role == Qt::EditRole){
		return matrix[index.row()][index.column()];
	}
	return QVariant();
}

QVariant Matrix::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

bool Matrix::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole) {
		matrix[index.row()][index.column()] = value.toFloat();
		emit dataChanged(index,index);
		return true;
	} else
		return false;
}

Qt::ItemFlags Matrix::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

//float Matrix::detSquareMatrix(QList<QList<float>> m)
//{
//	int l;
//	float d;
//	float sum11=1,sum12=0, sum21=1, sum22=0;
//	for (int i=0;i<m.size();i++)
//	{
//		sum11=1; l=2*m.size()-1-i;sum21=1;
//		for (int j=0;j<m.size();j++)
//		{
//			sum21*=m[j][l%m.size()];
//			l--;
//			sum11*=m[j][(j+i)%(m.size())];
//		}
//		sum22+=sum21;
//		sum12+=sum11;
//	}
//	d=sum12-sum22;
//	return d;
//}

void Matrix::toSquareByBareiss()
{
	assert(isSquare());
	backup();
	BareissSquare();
	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::clear(float x)
{
	if (matrix.isEmpty()) {
		QList<float> tmp;
		for (int i = 0; i < M; i++)
			tmp.append(x);
		for (int i = 0; i < N; i++) {
			matrix.append(tmp);
		}
	} else {
		backup();
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				matrix[i][j] = x;
	}

	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Vertical,0,1);
}

void Matrix::mulMatrixOnLastRow()
{
	assert(N+1 == M);
	QList<QList<float>> answer;
	for (int i = 0; i < N; i++) {
		float x = 0;
		for(int j = 0; j < M - 1; j++)
			x += matrix[i][j] * matrix[j].last();
		answer += {x};
	}
	setMatrix(answer);
}

void Matrix::setRows(int n)
{
	assert(n > 0);

	backup();

	while (n != N) {
		if (n < N)
			decRows();
		else
			incRows();
	}
	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Vertical,0,1);
}

void Matrix::setCols(int m)
{
	assert(m > 0);

	backup();

	while (m != M) {
		if (m < M)
			decCols();
		else
			incCols();
	}
	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::setMatrix(QList<QList<float>>& newMatrix)
{
	int largestRow = Matrix::findLargestRow(newMatrix);
	assert(largestRow > 0);
	setRows(newMatrix.size());
	setCols(largestRow);
	for (int i = 0; i < newMatrix.size(); i++)
		for (int j = 0; j < newMatrix[i].size(); j++)
			matrix[i][j] = newMatrix[i][j];
	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::swapRows(int a, int b)
{
	assert(a > 0 &&
	       b > 0 &&
	       a < N &&
	       b << N);
	backup();
	qSwap(matrix[a],matrix[b]);
	emit dataChanged(index(a,0),index(b,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::plusRows(int a, int b, float mult)
{
	backup();
	for (int i = 0; i < M; i++)
		matrix[a][i] += matrix[b][i] * mult;
	emit dataChanged(index(a,0),index(b,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::minusRows(int a, int b, float mult)
{
	backup();
	for (int i = 0; i < M; i++)
		matrix[a][i] -= matrix[b][i] * mult;
	emit dataChanged(index(a,0),index(b,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::plusRow(int c, float x)
{
	backup();
	for (auto &e: matrix[c])
		e += x;
	emit dataChanged(index(c,0),index(c,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::minusRow(int c, float x)
{
	backup();
	for (auto &e: matrix[c])
		e -= x;
	emit dataChanged(index(c,0),index(c,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::mulRows(int a, int b, float mult)
{
	backup();
	for (int i = 0; i < M; i++)
		matrix[a][i] *= matrix[b][i] * mult;
	emit dataChanged(index(a,0),index(b,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::divRows(int a, int b, float mult)
{
	backup();
	for (int i = 0; i < M; i++){
		if (matrix[b][i] == 0 || matrix[a][i])
			continue;
		matrix[a][i] /= matrix[b][i] * mult;
	}
	emit dataChanged(index(a,0),index(b,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::mulRow(int c, float mult)
{
	backup();
	for (auto &e: matrix[c])
		e *= mult;
	emit dataChanged(index(c,0),index(c,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::divRow(int c, float by)
{
	if (by == 0)
		return;

	backup();

	for (auto &e: matrix[c]){
		if (e == 0)
			continue;
		e /= by;
	}
	emit dataChanged(index(c,0),index(c,columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::addIdentity()
{
	backup();

	int olds = columnCount();
	for (int r = 0; r < N; r++) {
		QList<float> tmp;
		for (int i = 0; i < N; i++) {
			if (r != i)
				tmp << 0;
			else
				tmp << 1;
		}
		matrix[r] << tmp;
	}
	M += N;
	emit dataChanged(index(0,olds),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::transpose()
{
	backup();
	auto old = matrix;
	setCols(old.size());
	setRows(old[0].size());
	clear();
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			matrix[i][j] = old[j][i];
	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::undo()
{
	if (oldMatrix.isEmpty())
		return;
	matrix = oldMatrix.takeLast();
	N = matrix.size();
	M = matrix[0].size();
	emit dataChanged(index(0,0),index(rowCount(),columnCount()));
	emit headerDataChanged(Qt::Horizontal,0,1);
}

void Matrix::decRows()
{
	matrix.pop_back();
	N--;
}

void Matrix::incRows()
{
	QList<float> tmp;
	for (int i = 0; i < M; i++)
		tmp.append(0);
	matrix.append(tmp);
	N++;
}

void Matrix::decCols()
{
	for (auto &x: matrix)
		x.pop_back();
	M--;
}

void Matrix::incCols()
{
	for (auto &x: matrix)
		x.append(0);
	M++;
}

void Matrix::BareissSquare()
{
	int N = matrix.size();
	float denom(1);
	int exchanges(0);

	for(int l1(0); l1<N-1; ++l1)
	{
		int maxN( l1 );
		float maxValue( fabs( matrix[l1][l1] ) );
		for(int l2(l1+1); l2<N; ++l2)
		{
			float const value( fabs( matrix[l2][l1] ) );
			if( value > maxValue ) { maxN=l2; maxValue=value; }
		}

		if( maxN > l1 )
		{
			std::swap(matrix[l1], matrix[maxN]);
			//swapRows(l1,maxN);
			//QList<T> temp(M[l1]); M[l1]=M[maxN]; M[maxN]=temp;
			++exchanges;
		} else {
			if( maxValue == float(0) ) return;
		}

		float const value1( matrix[l1][l1] );


		for(int l2(l1+1); l2<N; ++l2)
		{
			float const value2( matrix[l2][l1] );
			matrix[l2][l1] = float(0);
			for(int c(l1+1); c<N; ++c){

				matrix[l2][c]=(matrix[l2][c]*value1-matrix[l1][c]*value2)/denom;
			}
		}

		denom = value1;
	}
}

void Matrix::backup()
{
	oldMatrix.append(matrix);
	if (oldMatrix.size() == 15)
		oldMatrix.pop_front();
}
