#include <iostream>
#include <iomanip>
#include "matrix.h"
using namespace std;

// =================================================================================
int main(int argc, char* argv[]) {
    (void)argv;
    (void)argc;
    char choice = '?';
    while (choice != 'x') {
        choice = matrix_menu();
        if (choice == 't') {
            matrix_transposer();
        }
        else if (choice == 's') {
            augmented_matrix_solver();
        }
        else if (choice == 'i') {
            matrix_inverter();
        }
        else if (choice == 'd') {
            find_determinant();
        }
        else if (choice == 'a') {
            matrix_adder();
        }
        else if (choice == 'm') {
            matrix_multiplier();
        }
        else if (choice == 'p') {
            raise_matrix_to_a_power();
        }
        if (choice != 'x') {
            cout << "\nDisplaying menu again.\n\n";
        } 
    }
    return 0;
}
// =================================================================================
