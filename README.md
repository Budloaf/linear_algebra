### linear_algebra
A program I wrote in the fall of 2024 for fun while taking the linear algebra portion of the Applied Mathematics course at University.

Bug: If a dimension is initialized at 0, there is no error catching, the result will simply not make sense.

Output example:
```
Welcome to the matrix program.
Please make a selection:

Type t to transpose a matrix.
Type s to solve an augmented matrix.
Type i to invert a matrix
Type d to find the determinant of a matrix.
Type a to find the sum of two matrices with equal dimensions.
Type m to find the product of two matrices
Type p to raise a matrix to a power
Type x to exit

Press return to confirm your selection

t
Input the dimensions of a matrix.
m: 2
n: 3
Initializing a 2x3 matrix.
Press enter to move to the next cell.
(0,0): 1
(0,1): 2
(0,2): 3
(1,0): 4
(1,1): 5
(1,2): 6
Outputting matrix:
| 1        2        3        |
| 4        5        6        |
Transposing matrix.
| 1        4        |
| 2        5        |
| 3        6        |

Displaying menu again.
```
