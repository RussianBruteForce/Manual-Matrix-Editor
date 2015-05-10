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

#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <QAbstractTableModel>
#include <QtAlgorithms>
#include <QList>
#include <QLinkedList>
#include <QDebug>

namespace MME {

class Matrix : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit Matrix(int n, int m);
	~Matrix() {}

	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex & index) const;


	/*
	 * Check on square matrix
	 */
	bool isSquare() const {
		return N == M;
	}

	/*
	 * Find determinant
	 */
	float detSquareMatrix() const
	{
		assert(isSquare());
		//return Matrix::detSquareMatrix(matrix);
		return BareissDeterminant<float>(matrix);
	}

	/*
	 * Returns laegest row size
	 */
	static int findLargestRow(QList<QList<float>> &m) {
		int largestRow = 0;
		for (auto &x: m)
			if (x.size() > largestRow)
				largestRow = x.size();
		return largestRow;
	}

public slots:
	/*
	 * Set all elements to x
	 */
	void clear(float x = 0);

	/* If matrix look like
	 *	a a a x
	 *	a a a x
	 *	a a a x
	 * we may multiply matrix A on vector X
	 * and print answer like
	 *	x'
	 *	x'
	 *	x'
	 */
	void mulMatrixOnLastRow();

	/*
	 * Set rows and colums of matrix
	 */
	void setRows(int n);
	void setCols(int m);

	/*
	 * Export matrix
	 */
	void setMatrix(QList<QList<float>> &newMatrix);

	void swapRows(int a, int b);
	void plusRows(int a, int b, float mult = 1);
	void minusRows(int a, int b, float mult = 1);
	void plusRow(int c, float x);
	void minusRow(int c, float x);
	void mulRows(int a, int b, float mult = 1);
	void divRows(int a, int b, float mult = 1);
	void mulRow(int c, float mult);
	void divRow(int c, float by);

	/*
	 * Add identity matrix to the right
	 */
	void addIdentity();
	/*
	 * Transpose matrix
	 */
	void transpose();
	/*
	 * Lead square matrix to row echelon form by Bareiss algo
	 */
	void toSquareByBareiss();
	// next function copied from forum
	//static float detSquareMatrix(QList<QList<float>> m);

	/*
	 * Call off previous action
	 */
	void undo();

private:
	/*
	 * Decrement and increment of rows and columns
	 */
	void decRows();
	void incRows();
	void decCols();
	void incCols();

	/*
	 * Lead square matrix to row echelon form by Bareiss algo
	 * called from toSquareByBareiss();
	 */
	void BareissSquare();//QList<QList<T>> &M);
	/*
	 * Find determinant by Bareiss algo
	 * called from detSquareMatrix();
	 */
	template<class T> static T BareissDeterminant(QList<QList<T>> M);

	/*
	 * Backup matrix state
	 */
	void backup();

	int N, M; // Matrix size N -- rows, M -- columns
	QList<QList<float>> matrix; // A way to store matrix
	QLinkedList<QList<QList<float>>> oldMatrix; // List of old matrixes
};

template<class T>
T Matrix::BareissDeterminant(QList<QList<T> > M)
{
	int N = M.size();
	T denom(1);
	int exchanges(0);

	for(int l1(0); l1<N-1; ++l1)
	{
		int maxN( l1 );
		T maxValue( fabs( M[l1][l1] ) );
		for(int l2(l1+1); l2<N; ++l2)
		{
			T const value( fabs( M[l2][l1] ) );
			if( value > maxValue ) { maxN=l2; maxValue=value; }
		}

		if( maxN > l1 )
		{
			std::swap(M[l1], M[maxN]);
			//QList<T> temp(M[l1]); M[l1]=M[maxN]; M[maxN]=temp;
			++exchanges;
		} else {
			if( maxValue == T(0) ) return maxValue;
		}

		T const value1( M[l1][l1] );

		for(int l2(l1+1); l2<N; ++l2)
		{
			T const value2( M[l2][l1] );
			M[l2][l1] = T(0);
			for(int c(l1+1); c<N; ++c)
				M[l2][c]=(M[l2][c]*value1-M[l1][c]*value2)/denom;
		}

		denom = value1;
	}
	if(exchanges%2) return -M[N-1][N-1]; else return M[N-1][N-1];
}

}
#endif // MATRIX_H
