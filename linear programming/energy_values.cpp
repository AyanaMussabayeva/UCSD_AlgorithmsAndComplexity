#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const double EPS = 1e-6;
const int PRECISION = 20;

typedef vector<double> Column;
typedef vector<double> Row;
typedef vector<Row> Matrix;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row):
        column(column),
        row(row)
    {}

    int column;
    int row;
};

Equation ReadEquation() {
    int size;
    cin >> size;
    Matrix a(size, vector <double> (size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            cin >> a[row][column];
        cin >> b[row];
    }
    return Equation(a, b);
}

Position SelectPivotElement(
  const Matrix &a, 
  vector <bool> &used_rows, 
  vector <bool> &used_columns) {
    // This algorithm selects the first free element.
    // You'll need to improve it to pass the problem.
    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column])
        ++pivot_element.column;

    // find a pivot with the largest absolute value
    double max = 0.0;
    for (int from = pivot_element.row, size = a.size(); from < size; ++from) {
        if (fabs(a[from][pivot_element.column]) > fabs(max)) {
            max = a[from][pivot_element.column];
            pivot_element.row = from;
        }
    }

    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, vector <bool> &used_rows, Position &pivot_element) {
    swap(a[pivot_element.column], a[pivot_element.row]);
    swap(b[pivot_element.column], b[pivot_element.row]);
    swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}


void scalePivot(Matrix& a, Column& b, const Position& pivot_element)
{
    const double divisor = a[pivot_element.row][pivot_element.column];
    const int size = a.size();

    for (int j = pivot_element.column; j < size; ++j) {
        a[pivot_element.row][j] /= divisor;
    }

    b[pivot_element.row] /= divisor;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Write your code here
    const int size = a.size();
    double multiple = 0.0 ;

    scalePivot(a, b, pivot_element);

    for (int i = pivot_element.row + 1; i < size; ++i) {
        multiple = a[i][pivot_element.column];
        for (int j = pivot_element.column; j < size; ++j) {
            a[i][j] -= (a[pivot_element.row][j] * multiple);
        }
        b[i] -= (b[pivot_element.row] * multiple);
    }
}

void MarkPivotElementUsed(const Position &pivot_element, vector <bool> &used_rows, vector <bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    vector <bool> used_columns(size, false);
    vector <bool> used_rows(size, false);


    for (int step = 0; step < size; ++step) {
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);
    }

    return b;
}

void PrintColumn(const Column &column) {
    int size = column.size();
    cout.precision(PRECISION);
    for (int row = 0; row < size; ++row)
        cout << column[row] << endl;
}

int main() {
    Equation equation = ReadEquation();
    Column solution = SolveEquation(equation);
    PrintColumn(solution);
    return 0;
}
