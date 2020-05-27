#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

constexpr int PRECISION = 10;

using Column = vector<double>;
using Row = vector<double>;
using Matrix = vector<Row>;

struct Equation {
    Matrix a;
    Column b;
};

struct Position {
    short column;
    short row;
};

Equation readEquation()
{
    int size;
    cin >> size;

    Matrix a(size, vector<double>(size, 0.0));
    Column b(size, 0.0);

    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            cin >> a[row][column];
        cin >> b[row];
    }

    return Equation{ move(a), move(b) };
}

Position selectPivot(const Matrix& a, vector<bool>& used_rows, vector<bool>& used_columns)
{
    // First, select the first free element.
    Position pivot_element{ 0, 0 };
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column])
        ++pivot_element.column;

    // Then, amongs free elements, select a pivot with the largest absolute value
    double max = 0.0;
    for (int from = pivot_element.row, size = a.size(); from < size; ++from) {
        if (fabs(a[from][pivot_element.column]) > fabs(max)) {
            max = a[from][pivot_element.column];
            pivot_element.row = from;
        }
    }

    return pivot_element;
}

inline void swapRows(Matrix& a, Column& b, vector<bool>& used_rows, Position& pivot_element)
{
    swap(a[pivot_element.column], a[pivot_element.row]);
    swap(b[pivot_element.column], b[pivot_element.row]);
    swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void substitute(Matrix& a, Column& b)
{
    for (int i = a.size() - 1; i; --i) {
        double v = b[i];
        for (int j = 0; j != i; ++j) {
            b[j] -= a[j][i] * v;
            a[j][i] = 0;
        }
    }
}

inline void scalePivot(Matrix& a, Column& b, const Position& pivot_element)
{
    const double divisor = a[pivot_element.row][pivot_element.column];
    const int size = a.size();

    for (int j = pivot_element.column; j < size; ++j) {
        a[pivot_element.row][j] /= divisor;
    }

    b[pivot_element.row] /= divisor;
}

void processPivotElement(Matrix& a, Column& b, const Position& pivot_element)
{
    const int size = a.size();
    double multiple{ 0.0 };

    scalePivot(a, b, pivot_element);

    for (int i = pivot_element.row + 1; i < size; ++i) {
        multiple = a[i][pivot_element.column];
        for (int j = pivot_element.column; j < size; ++j) {
            a[i][j] -= (a[pivot_element.row][j] * multiple);
        }
        b[i] -= (b[pivot_element.row] * multiple);
    }
}

inline void findUsedPivot(const Position& pivot, vector<bool>& used_rows, vector<bool>& used_columns)
{
    used_rows[pivot.row] = true;
    used_columns[pivot.column] = true;
}

Column solveEquation(Equation equation)
{
    Matrix& a = equation.a;
    Column& b = equation.b;

    vector<bool> used_columns(a.size(), false);
    vector<bool> used_rows(a.size(), false);

    for (int steps = a.size(); steps; --steps) {
        Position pivot_element = selectPivot(a, used_rows, used_columns);
        swapRows(a, b, used_rows, pivot_element);
        processPivotElement(a, b, pivot_element);
        findUsedPivot(pivot_element, used_rows, used_columns);
    }

    substitute(a, b);

    return b;
}

void printColumn(const Column& column)
{
    cout.precision(PRECISION);

    for (const auto s : column) {
        cout << s << ' ';
    }

    cout << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    Equation equation = readEquation();

    if (equation.a.size()) {
        Column solution = solveEquation(equation);
        printColumn(solution);
    }

    return 0;
}