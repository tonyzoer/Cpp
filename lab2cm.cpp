#include <iostream>
#include <vector>

using namespace std;

#define EPS 0.000001
#define INF 10e9

bool gauss(vector<vector<double> > a, vector<double> &ans) {
	int n = a.size();
	int m = a[0].size() - 1;

	vector<int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; ++col) {
		int sel = row;
		for (int i = row; i < n; ++i)
			if (abs(a[i][col]) > abs(a[sel][col]))
				sel = i;
		if (abs(a[sel][col]) < EPS)
			continue;
		for (int i = col; i <= m; ++i)
			swap(a[sel][i], a[row][i]);
		where[col] = row;

		for (int i = 0; i < n; ++i)
			if (i != row) {
				double c = a[i][col] / a[row][col];
				for (int j = col; j <= m; ++j)
					a[i][j] -= a[row][j] * c;
			}
		++row;
	}

	ans.assign(m, 0);
	for (int i = 0; i < m; ++i)
		if (where[i] != -1)
			ans[i] = a[where[i]][m] / a[where[i]][i];
	for (int i = 0; i < n; ++i) {
		double sum = 0;
		for (int j = 0; j < m; ++j)
			sum += ans[j] * a[i][j];
		if (abs(sum - a[i][m]) > EPS)
			return 0;
	}

	for (int i = 0; i < m; ++i)
		if (where[i] == -1)
			return INF;
	return true;
}

int main()
{
#pragma region Gauss
	vector<vector<double> > A1 =
	{
		{ 8.64, 1.71, 5.42, 10.21 },
		{ -6.39, 4.25, 1.84, 3.41 },
		{ 4.21, 7.92, -3.41, 12.29 }
	};
	vector<double> an;
	gauss(A1, an);
	for (int i = 0; i < an.size(); i++)
	{
		cout << an[i] << " ";
	}
	cout << "\n";
#pragma endregion

#pragma region squared solution
	vector<vector<double> > A2 = 
	{
		{5.18, 1.12, 0.95, 1.32, 0.83},
		{1.12, 4.28, 2.12, 0.57, 0.91},
		{0.95, 2.12, 6.13, 1.29, 1.57},
		{1.32, 0.57, 1.29, 4.57, 1.25},
		{0.83, 0.91, 1.57, 1.25, 5.21}
	};
	vector<double> b = { 6.19, 3.21, 4.28, 6.25, 4.95 };
	double U[10][10];
	double x[10], y[10];
	int n = 5;
	double temp;

	for (int i = 0; i < n; i++)
	{
		temp = 0;
		for (int k = 0; k < i; k++)
			temp = temp + U[k][i] * U[k][i];
		U[i][i] = sqrt(A2[i][i] - temp);
		for (int j = i; j < n; j++)
		{
			temp = 0;
			for (int k = 0; k < i; k++)
				temp = temp + U[k][i] * U[k][j];
			U[i][j] = (A2[i][j] - temp) / U[i][i];
		}
	}
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n; j++) {
	//		cout << U[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	for (int i = 0; i < n; i++)
	{
		temp = 0;
		for (int k = 0; k < i; k++)
			temp = temp + U[k][i] * y[k];
		y[i] = (b[i] - temp) / U[i][i];
	}
	for (int i = n - 1; i >= 0; i--)
	{
		temp = 0;
		for (int k = i + 1; k < n; k++)
			temp = temp + U[i][k] * x[k];
		x[i] = (y[i] - temp) / U[i][i];
	}
	cout << "\n";
	for (int i = 0; i < n; i++)
		cout << "x" << i << " = " << x[i] << endl;
#pragma endregion
	
#pragma region zeidel
	vector<vector<double> > A3 =
	{
		{ 6.1, 2.2, 1.2, 16.55 },
		{ 2.2, 5.5, -1.5, 10.55 },
		{ 1.2, -1.5, 7.2, 16.80 }
	};
	int	size = 3;
	// Введем вектор значений неизвестных на предыдущей итерации,
	// размер которого равен числу строк в матрице, т.е. size,
	// причем согласно методу изначально заполняем его нулями
	vector <double> previousVariableValues(size, 0.0);

	// Будем выполнять итерационный процесс до тех пор, 
	// пока не будет достигнута необходимая точность    
	while (true)
	{
		// Введем вектор значений неизвестных на текущем шаге       
		vector <double> currentVariableValues(size);

		// Посчитаем значения неизвестных на текущей итерации
		// в соответствии с теоретическими формулами
		for (int i = 0; i < size; i++)
		{
			// Инициализируем i-ую неизвестную значением 
			// свободного члена i-ой строки матрицы
			currentVariableValues[i] = A3[i][size];

			// Вычитаем сумму по всем отличным от i-ой неизвестным
			for (int j = 0; j < size; j++)
			{
				// При j < i можем использовать уже посчитанные
				// на этой итерации значения неизвестных
				if (j < i)
				{
					currentVariableValues[i] -= A3[i][j] * currentVariableValues[j];
				}

				// При j > i используем значения с прошлой итерации
				if (j > i)
				{
					currentVariableValues[i] -= A3[i][j] * previousVariableValues[j];
				}
			}

			// Делим на коэффициент при i-ой неизвестной
			currentVariableValues[i] /= A3[i][i];
		}

		// Посчитаем текущую погрешность относительно предыдущей итерации
		double error = 0.0;

		for (int i = 0; i < size; i++)
		{
			error += abs(currentVariableValues[i] - previousVariableValues[i]);
		}

		// Если необходимая точность достигнута, то завершаем процесс
		if (error < EPS)
		{
			break;
		}

		// Переходим к следующей итерации, так 
		// что текущие значения неизвестных 
		// становятся значениями на предыдущей итерации
		previousVariableValues = currentVariableValues;
	}
	cout << "\n";
	// Выводим найденные значения неизвестных с 8 знаками точности
	for (int i = 0; i < size; i++)
	{
		cout << previousVariableValues[i] << " ";
	}
#pragma endregion
	return 0;
}