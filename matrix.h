#include <iostream>
#include <iomanip>
using namespace std;

// constants
const char delimiter = ' ';
const int cell_width = 8; // ±3.3e+03

// display a matrix
void display_matrix(double*,int,int);

// set a matrix
double* initialize_matrix(int,int);
double* create_identity_matrix(int,int);

// matrix operations
double* add_matrices(double*,double*,int,int,double,double);
double* multiply_matrices(double*,int,int,double*,int,int);
double* matrix_power(double*,int,int);
double* transpose_matrix(double*,int,int);
double* invert_matrix(double*,int);
double  det_matrix(double*,int);


// row operations
double* swap_rows(double*,int,int,int,int);
double* multiply_row(double*,int,int,int,double);
double* add_another_row(double*,int,int,int,int,double);

// put matrix in row-echelon form
double* row_echelon_form(double*,int,int);

// solve an augmented matrix
void re_matrix_solver(double*,int, int);

void augmented_matrix_solver();
void matrix_inverter();
void matrix_adder();
void matrix_multiplier();
void matrix_transposer();
void find_determinant();

char matrix_menu();

// display a matrix 
void display_matrix(double* matrix,int m,int n) {
    for (int i = 0; i < m; i++) {
        cout << "| ";
        for (int j = 0; j < n; j++) {
            cout << left
                << setw(cell_width) 
                << setfill(delimiter) 
                << setprecision(5) 
                << matrix[i*n+j]+0.0
                << " ";
        }
        cout << "|" << endl;
    }
}

// set a matrix
double* initialize_matrix(int m, int n) {
    double* matrix = new double[m * n];
    cout << "Initializing a " << m << "x" << n << " matrix.\n" << "Press enter to move to the next cell." << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << "(" << i << "," << j << "): " ; // ordered pair indexing the selection
            cin >> matrix[i*n+j]; // input the cells
        }
    }
    return matrix;
}

double* create_identity_matrix(int m, int n) {
    if (m != n) {
        cout << "ERROR: Identity matrix must be square\n";
        return 0;
    }
    double* identity = new double[m*n];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                identity[i*n+j] = 1; 
            }
            else {
                identity[i*n+j] = 0;
            }
        }
    }
    return identity;
}

// matrix operations
double* add_matrices(double* A,double* B,int m,int n,double A_factor = 1,double B_factor = 1) {
    // to add two matrices, both need to have equal m and n dimensions
    double* sum_matrix = new double[m*n];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            sum_matrix[i*n+j] = A_factor*A[i*n+j] + B_factor*B[i*n+j];
        }
    }
    return sum_matrix;
}
double* multiply_matrices(double* A, int Am, int An, double* B, int Bm,int Bn) {
    // # of rows = # of rows of matrix A = Am
    // # of cols = # of cols of matrix B = Bn
    // if(An != Bm) { condition checked in matrix_multiplier() }
    (void)Bm; // unused but would be awkward to not have as a parameter
    double* product_matrix = new double[Am*Bn];
    double sum;
    for (int i = 0; i < Am; i++) {
        for (int j = 0; j < Bn; j++) {
            sum = 0;
            for (int addend = 0; addend < An; addend++) {
                sum += A[i*An+addend]*B[j+Bn*addend];
            }
            product_matrix[i*Bn+j] = sum;
        }
    }
    return product_matrix;
}

// raise a matrix to a power
double* matrix_power(double* matrix, int dimension, int power) {
    double* result = new double[dimension*dimension];
    if (power == 0) {
        return create_identity_matrix(dimension,dimension);
    }
    else if (power == 1) {
        return matrix;
    }
    else if (power > 1) {
        result = multiply_matrices(matrix,dimension,dimension,matrix,dimension,dimension);
        for (int p = 2; p < power; p++) {
            result = multiply_matrices(result,dimension,dimension,matrix,dimension,dimension);
        }
        return result;
    }
    else if (power < 0) {
        matrix = invert_matrix(matrix,dimension);
        if (power == -1) {
            return matrix;
        }
        else {
            result = multiply_matrices(matrix,dimension,dimension,matrix,dimension,dimension);
            for (int p = -2; p > power; p--) {
                result = multiply_matrices(result,dimension,dimension,matrix,dimension,dimension);
            }
            return result;
        }
    }
    return 0;
}

// transpose a matrix
double* transpose_matrix(double* matrix,int m, int n) {
    double* transposed = new double[m*n]; 
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            transposed[j*m+i] = matrix[i*n+j];
        }
    }
    return transposed;
}

// row operations
double* swap_rows(double* matrix,int m, int n, int row1, int row2) {
    (void)m;
    if (m <= row1 || m <= row2) {
        cout << "ERROR: row specified does not exist...\n\nOOPS\n\n";
        return 0;
    }
    double hold[n];
    for (int i = 0; i < n; i++) {
        hold[i] = matrix[row1*n+i];
        matrix[row1*n+i] = matrix[row2*n+i];
        matrix[row2*n+i] = hold[i];
    }
    return matrix;
}
double* multiply_row(double* matrix,int m, int n,int row, double k) {
    (void)m;
    if (k == 0) {
        cout << "ERROR: cannot multiply a row by zero...\n\nOOPS\n\n";
        return 0;
    }
    if (m <= row) {
        cout << "ERROR: row specified doesn't exist...\n\nOOPS\n\n";
        return 0;
    }
    for (int i = 0; i < n; i++) {
        matrix[row*n+i] *= k;
    }
    return matrix;
}
double* add_another_row(double* matrix,int m, int n, int row_src,int row_dest, double nonzero_multiple = 1) {
    (void)m;
    if (nonzero_multiple == 0) {
        cout << "ERROR: cannot multiply a row by zero...\n\nOOPS\n\n";
        return 0;
    }
    if (m <= row_src || m <= row_dest) {
        cout << "ERROR: row specified does not exist...\n\nOOPS\n\n";
    }
    double multiplied_row[n];
    for (int i = 0; i < n; i++) {
        multiplied_row[i] = nonzero_multiple * matrix[row_src*n+i];
    }
    for (int i = 0; i < n; i++) {
        matrix[row_dest*n+i] += multiplied_row[i];
    }
    return matrix;
}

// put matrix in row-echelon form
double* row_echelon_form(double* matrix,int m,int n) {
    for (int j = 0; j < n; j++) {
        for (int i = j; i < m; i++) { // travelling vertically downwards
            if (i == j) { // 1 desired
                if (matrix[i*n+j] == 0 && i != m-1) {
                    // second condition is false when the last row 
                    // is considered. 
                    // Leave the row alone in this case.
                    // | 0 x x | (first row)
                    for (int k = i+1; k < m; k++) {
                        // starting with the next row (i+1),
                        // swap rows i and k until a nonzero value is found
                        // k is row indices for lower rows
                        // cout << "Swapping rows " << i << " and " << k << endl; // debug
                        matrix = swap_rows(matrix,m,n,i,k);
                        // display_matrix(matrix,m,n); // (debug)
                    }
                }
                // in case where entire column is 0s,
                // move to next column
                if (matrix[i*n+j] != 1 && matrix[i*n+j] != 0) {
                    // divide entire row by leading coefficient
                    //cout << "Multiplying row " << i << " by 1 / (" << matrix[i*n+j] << ")\n"; // debug
                    matrix = multiply_row(matrix,m,n,i,1/matrix[i*n+j]);
                    // display_matrix(matrix,m,n); // debug
                }
            }
            else { // 0 desired
                if (matrix[i*n+j] != 0) {
                    int reference_row = j; 
                    // row that aligns with j (column) is going to have the leading 1

                    // row = previous row + row/(opposite of leading coefficient)
                    //cout << "Setting row " << i << " equal to row " << i << " + (row " << reference_row << ") * (-" << matrix[i*n+j] << ")\n"; // debug
                    matrix = add_another_row(matrix,m,n,reference_row,i,-1*matrix[i*n+j]);
                    //display_matrix(matrix,m,n); // debug
                }
            }
        }
    }
    return matrix;
}

// display solution of an augmented matrix
void re_matrix_solver(double* matrix,int m, int n) {
    if (m != n-1) {
        cout << "This is not an augmented matrix.\nCannot find a solution.\n";
        return;
    }
    double* re_matrix = row_echelon_form(matrix,m,n);
    // check for infinite and no solution
    for (int i = 0; i < m; i++) {
        int nonzeros = 0;
        for (int j = 0; j < n; j++) {
            // iterating through each row, summing nonzero cells
            if (re_matrix[i*n+j] != 0) {
                nonzeros++;
            }
            if (j == n - 1) { // last column
                if (nonzeros == 0) {
                    // INFINITE SOLUTIONS
                    cout << "The matrix has infinite solutions.\n";
                    return;
                }
                if (nonzeros == 1 && re_matrix[i*n+j] != 0) {
                    // if there is only one nonzero, and it's in the last column...
                    // NO SOLUTION
                    cout << "The matrix has no solution.\n";
                    return;
                }
            }
        }
    }
    // find one solution
    double solution_matrix[m]; // m rows, m variables
    solution_matrix[m-1] = re_matrix[m*n-1]; // last variable is a freebie
    for (int i = m - 2; i >= 0; i--) { // starting on second to last row
        double variable_arithmetic = 0;
        for (int j = n - 2; j > 0; j--) {
            variable_arithmetic -= re_matrix[i*n+j]*solution_matrix[j];
            // negative because it will be subtracted from other side
        }
        solution_matrix[i] = re_matrix[i*n+(n-1)] + variable_arithmetic;
        // if the value is very very close to 0, it's probably a repeating 0,
        // so assume it is zero
        if (solution_matrix[i] < (0.0000000001) && solution_matrix[i] > -0.0000000001) {
            solution_matrix[i] = 0;
        }
    }
    display_matrix(solution_matrix,m,1);
    return;
}
void matrix_adder() {
    int m;
    int n;
    cout << "Input the dimensions of the matrices.\n";
    cout << "m: ";
    cin >> m;
    cout << "n: ";
    cin >> n;
    double* matrix_A = new double[m*n];
    double* matrix_B = new double[m*n];
    cout << "Matrix A: \n";
    matrix_A = initialize_matrix(m,n);
    cout << "Outputting matrix A:\n";
    display_matrix(matrix_A,m,n);
    cout << "Matrix B: \n";
    matrix_B = initialize_matrix(m,n);
    cout << "Outputting matrix B:\n";
    display_matrix(matrix_B,m,n);
    double A_factor;
    double B_factor;
    cout << "Please input a factor for each matrix.\nFactor of A: ";
    cin >> A_factor;
    cout << "Please input a factor for each matrix.\nFactor of B: ";
    cin >> B_factor;
    cout << "Adding the matrices...\n";
    display_matrix(add_matrices(matrix_A,matrix_B,m,n,A_factor,B_factor),m,n);
}
void matrix_multiplier() {
    int Am;
    int An;
    int Bm;
    int Bn;
    cout << "Input the dimensions of matrix A.\n";
    cout << "m: ";
    cin >> Am;
    cout << "n: ";
    cin >> An;
    cout << "Input the dimensions of matrix B.\n(The number of rows must be equal to the number of columns of A)\n";
    cout << "m: ";
    cin >> Bm;
    cout << "n: ";
    cin >> Bn;
    if(An != Bm) {
        // to be compatible for multiplication,
        // # of columns (n) in matrix A
        // # of    rows (m) in matrix B
        cout << "ERROR: your matrices are not compatible.\n"
             << "# of columns of first matrix must be equal to # of rows of second matrix.\n";
        cout << "Your matrices are " << Am << "x" << An << " and " << Bm << "x" << Bn << ".\n";
        return;
    }
    double* matrix_A = new double[Am*An];
    double* matrix_B = new double[Bm*Bn];
    cout << "Matrix A: \n";
    matrix_A = initialize_matrix(Am,An);
    cout << "Outputting matrix A: \n";
    display_matrix(matrix_A,Am,An);
    cout << "Matrix B: \n";
    matrix_B = initialize_matrix(Bm,Bn);
    cout << "Outputting matrix B: \n";
    display_matrix(matrix_B,Bm,Bn);
    cout << "\n The product AB:\n";
    display_matrix(multiply_matrices(matrix_A,Am,An,matrix_B,Bm,Bn),Am,Bn);
    return;
}
void matrix_transposer() {
    int m;
    int n;
    cout << "Input the dimensions of a matrix.\n";
    cout << "m: ";
    cin >> m;
    cout << "n: ";
    cin >> n;
    double* matrix = new double[m*n];
    matrix = initialize_matrix(m,n);
    cout << "Outputting matrix:\n";
    display_matrix(matrix,m,n);
    cout << "Transposing matrix.\n";
    display_matrix(transpose_matrix(matrix,m,n),n,m);
    return;
}


void augmented_matrix_solver() {
    int m; // vertically indexed rows
    //     n =   0       1       2
    // m = 0 |-------|-------|-------|
    //     1 |-------|-------|-------|
    //     2 |-------|-------|-------|
    int n; // horizontally indexed columns
    double* augmented_matrix;
    cout << "Input the dimensions of an augmented matrix.\n";
    cout << "m: ";
    cin >> m;
    cout << "n: ";
    cin >> n;
    if (m != n-1) {
        cout << "ERROR: the number of columns must be equal to the number of rows plus one\n";
        return;
    }
    augmented_matrix = initialize_matrix(m,n);
    cout << "Outputting the matrix:\n";
    display_matrix(augmented_matrix,m,n);
    char method;
    cout << "Would you like to solve the augmented matrix using row-operations (r) or inverse matrices (i)?\n";
    cin >> method;
    if (method == 'r') {
        cout << "Putting augmented matrix in row-echelon form...\n";
        double* re_matrix;
        re_matrix = row_echelon_form(augmented_matrix,m,n);
        display_matrix(re_matrix,m,n);
        cout << "Solution:\n";
        re_matrix_solver(re_matrix,m,n);
    }
    else if (method == 'i') {
        cout << "Separating the augmented matrix into matrices A and B\n";
        double* matrix_A  = new double[m*m];
        double* matrix_B  = new double[m];
        double* A_inverse = new double[m*m];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                matrix_A[i*m+j] = augmented_matrix[i*n+j];
            }
        }
        for (int i = 0; i < m; i++) {
            matrix_B[i] = augmented_matrix[i*n+(n-1)];
        }
        cout << "Matrix A:\n";
        display_matrix(matrix_A,m,m);
        cout << "Matrix B:\n";
        display_matrix(matrix_B,m,1);
        cout << "Inverting A.\nA^(-1):\n";
        A_inverse = invert_matrix(matrix_A,m);
        display_matrix(A_inverse,m,m);
        cout << "Multiplying A^(-1) and B\n";
        cout << "Solution:\n";
        display_matrix(multiply_matrices(A_inverse,m,m,matrix_B,m,1),m,1);
    }
    delete[] augmented_matrix;
    return;
}
void matrix_inverter() {
    int n; // horizontally indexed columns
    double* matrix;
    cout << "Input the dimension of an invertible matrix.\n";
    cout << "n: ";
    cin >> n;
    matrix = initialize_matrix(n,n);
    cout << "Outputting the matrix:\n";
    display_matrix(matrix,n,n);
    if (det_matrix(matrix,n) == 0) {
        cout << "ERROR: determinant is 0. matrix is not invertible...OOPS\n";
        return;
    }
    cout << "Outputting the inverted matrix:\n";
    display_matrix(invert_matrix(matrix,n),n,n);
    return;
}
void find_determinant() {
    int n;
    double* matrix;
    cout << "Input the dimension of a square matrix\n";
    cout << "n: ";
    cin >> n;
    matrix = initialize_matrix(n,n);
    cout << "Outputting the matrix:\n";
    display_matrix(matrix,n,n);
    cout << "Determinant: " << det_matrix(matrix,n) << endl;
    return;
}
void raise_matrix_to_a_power() {
    int n;
    int p;
    double* matrix;
    cout << "Input the dimension of a square matrix\n";
    cout << "n: ";
    cin >> n;
    matrix = initialize_matrix(n,n);
    cout << "Outputting the matrix:\n";
    display_matrix(matrix,n,n);
    cout << "Raise the matrix to what power?\n";
    cout << "power: ";
    cin >> p;
    cout << "Outputting the matrix raised to the " << p << " power\n";
    display_matrix(matrix_power(matrix,n,p),n,n);
    return;
}
double* invert_matrix(double* matrix,int n) {
    // determinant of matrix is not zero (checked in matrix_inverter)
    // n is the dimension (m and n) of the matrix
    int m = n;
    // create an identity matrix m x n
    double* identity_matrix = create_identity_matrix(m,n);

    // create the matrix m x n*2 (input and identity as one big matrix)
    int extended_n = n*2;
    double* extended_inverted_matrix = new double[m*extended_n]; // 
    double* inverted_matrix = new double[m*n];
    // initialize extended_inverted_matrix as uninverted
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) { // og matrix
            extended_inverted_matrix[i*extended_n+j] = matrix[i*n+j]; 
        }
        for (int j = n; j < extended_n; j++) {
            extended_inverted_matrix[i*extended_n+j] = identity_matrix[i*n+(j-n)];
        }
    }

    // swap identity matrix and og matrix within 
    // extended_matrix_uninverted using row operations
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            if (i == j && extended_inverted_matrix[i*extended_n+j] != 1) { // 1 desired
                // multiply row by 1/(i,j cell) to get 1
                if (extended_inverted_matrix[i*extended_n+j] == 0) {
                    extended_inverted_matrix = add_another_row(extended_inverted_matrix,m,extended_n,i+1,i,1/extended_inverted_matrix[(i+1)*extended_n+j]);
                }
                extended_inverted_matrix = multiply_row(extended_inverted_matrix,m,extended_n,i,1/(extended_inverted_matrix[i*extended_n+j]));
            }
            else if (i > j && extended_inverted_matrix[i*extended_n+j] != 0) { // 0 desired, cells below the cell that has a 1
                // take the row with the one (when i was equal to j, that row. hence jth row)
                // multiply it by the opposite of the value in the cell of the row we want to be zero (ith row).
                // add the rows and put the resulting row in the ith row.
                extended_inverted_matrix = add_another_row(extended_inverted_matrix,m,extended_n,j,i,-1*extended_inverted_matrix[i*extended_n+j]);
            }
            else if (i < j && extended_inverted_matrix[i*extended_n+j] != 0) { // 0 desired, cells above the cell that has a 1.
                // because we are going top-down column by column, by the time
                // this condition is true, all other rows have zeros in previous columns where i != j.
                // additionally, the 1 of this column has not been set yet.
                // the approach is therefore to multiply the next row by whatever makes 
                // it opposite to the cell in question, so that when the cells are added,
                // the 1 from previous column is in tact, and there is now a zero in the current cell.
                extended_inverted_matrix = add_another_row(extended_inverted_matrix,m,extended_n,j,i,-1*extended_inverted_matrix[i*extended_n+j]/extended_inverted_matrix[j*extended_n+j]);
            }
        }
    }
    // extract the right half of the extended matrix into an m x n matrix.
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            inverted_matrix[i*n+j] = extended_inverted_matrix[i*extended_n+n+j];
                                    // i*extended_n because the source matrix is longer
                                    // +n because we need to start from the middle of the matrix
                                    // +j as normal
        }
    }

    return inverted_matrix;
}
double det_matrix(double* matrix,int n) {
    // square matrix, n x n
    if (n == 1) {
        // base case
        return matrix[0];
    }
    else {
        double term = 0;
        double* sub_matrix = new double[(n-1)*(n-1)];
        // in the case of a 3x3, sub_matrix is 2x2
        // and is different for each column.
        // below is sub_matrix for each j in a 3x3
        // j x x    x j x   x x j
        // x s s    s x s   s s x
        // x s s    s x s   s s x
        // function takes the determinant of sub_matrix
        // and multiplies it by the value in cell j.
        // this recurses until sub_matrix is base case (1x1)
        double determinant = 0;
        for (int j = 0; j < n; j++) { // column by column
            int cell = 0;
            for (int q = 0; q < n*n; q++) {
                if (q % n != j) { // if the cell is in column j, exclude it
                    sub_matrix[cell] = matrix[q+n];
                    cell++;
                }
            }
            term = matrix[j]*det_matrix(sub_matrix,n-1);
            if (j % 2 != 0) {
                term *= -1; // negate the term if j is even
            }
            determinant += term;
        }
        delete[] sub_matrix;
        return determinant;
    }
}

char matrix_menu() {
    cout << "\nWelcome to the matrix program.\n";
    cout << "Please make a selection:\n\n";
    // whitespace
    cout << "Type t to transpose a matrix.\n";
    cout << "Type s to solve an augmented matrix.\n";
    cout << "Type i to invert a matrix\n";
    cout << "Type d to find the determinant of a matrix.\n";
    cout << "Type a to find the sum of two matrices with equal dimensions.\n";
    cout << "Type m to find the product of two matrices\n";
    cout << "Type p to raise a matrix to a power\n";
    cout << "Type x to exit\n";
    cout << "\nPress return to confirm your selection\n\n";
    char selection;
    cin >> selection;
    return selection;
}