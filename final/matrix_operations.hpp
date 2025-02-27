#ifndef SIMU_PROJEKT_MATRIX_OPERATIONS_HPP
#define SIMU_PROJEKT_MATRIX_OPERATIONS_HPP

#include <cmath> // incluir biblioteca matematica estandar

#include "vector.hpp" // incluir la definicion de la clase Vector
#include "matrix.hpp"  // incluir la definicion de la clase Matrix

// metodo para multiplicar un escalar por una matriz
void product_scalar_by_matrix(float scalar, Matrix* M, int n, int m, Matrix* R) {
    for (int r = 0; r < n; r++) // recorrer cada fila de la matriz
        for (int c = 0; c < m; c++) // recorrer cada columna de la matriz
            // multiplicar el elemento de la matriz por el escalar y lo asigna en la matriz resultado
            R->set(scalar * M->get(r, c), r, c);
}

// metodo para multiplicar una matriz por un vector
void product_matrix_by_vector(Matrix* M, Vector* V, int n, int m, Vector* R) {
    for (int r = 0; r < n; r++) { // recorrer cada fila de la matriz
        float acc = 0; // acumulador para el producto escalar de la fila y el vector
        for (int c = 0; c < n; c++) // recorrer cada columna de la matriz
            acc += M->get(r, c) * V->get(c); // suma el producto del elemento de la matriz y el correspondiente elemento del vector al acumulador
        R->set(acc, r); // asigna el acumulador en el vector resultado
    }
}

// funcion para multiplicar una matriz por otra matriz
void product_matrix_by_matrix(Matrix* A, Matrix* B, Matrix* R) {
    int n = A->get_nrows(), m = A->get_ncols(), p = B->get_nrows(), q = B->get_ncols(); // obtener las dimensiones de las matrices
    if (m == p) { // verificar que el numero de columnas de A sea igual al numero de filas de B
        R->set_size(n, q); // establecer el tamano de la matriz resultado
        R->init(); // inicializar la matriz resultado con ceros

        for (int r = 0; r < n; r++) // recorrer cada fila de la matriz A
            for (int c = 0; c < q; c++) // recorrer cada columna de la matriz B
                for (int i = 0; i < m; i++)
                    R->add(A->get(r, i) * B->get(i, c), r, c);
    }
    else {
        // si las dimensiones no son compatibles, muestra un mensaje de error y termina el programa
        std::cout << "Incompatibilidad de dimensiones al multiplicar matrices.\n\nAbortando...\n";
        exit(EXIT_FAILURE);
    }
}

float determinant(Matrix* M); // declaracion de la funcion para calcular el determinante de una matriz

float determinant_auxiliar(Matrix* M) {
    int n = M->get_ncols();
    float acc = 0;

    for (int c = 0; c < n; c++) {
        Matrix clon(n, n);
        M->clone(&clon);
        clon.remove_row(0);
        clon.remove_column(c);
        acc += pow(-1, c) * M->get(0, c) * determinant(&clon);
    }

    return acc;
}

float determinant(Matrix* M) {
    float ans;
    switch (M->get_ncols()) {
    case 1: ans = M->get(0, 0); break;
    case 2: ans = M->get(0, 0) * M->get(1, 1) - M->get(0, 1) * M->get(1, 0); break;
    case 3: ans = M->get(0, 0) * M->get(1, 1) * M->get(2, 2) - M->get(0, 0) * M->get(1, 2) * M->get(2, 1) - M->get(0, 1) * M->get(1, 0) * M->get(2, 2) + M->get(0, 1) * M->get(1, 2) * M->get(2, 0) + M->get(0, 2) * M->get(1, 0) * M->get(2, 1) - M->get(0, 2) * M->get(1, 1) * M->get(2, 0); break;
    default: ans = determinant_auxiliar(M);
    }
    return ans;
}

float get_minor(Matrix* M, int n, int r, int c) {
    Matrix clon(n, n);
    M->clone(&clon);

    clon.remove_row(r);
    clon.remove_column(c);

    return determinant(&clon);
}

void conjugate_matrix(Matrix* M, int n, Matrix* C) {
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            C->set(pow(-1, r + c) * get_minor(M, n, r, c), r, c);
}

void transpose(Matrix* M, int n, int m, Matrix* T) {
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            T->set(M->get(r, c), c, r);
}

// calculando inversa utilizando el metodo de CHolesky
void calculate_inverse(Matrix* A, int n, Matrix* X) {
    Matrix L(n, n), Y(n, n);
    float acum;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                acum = 0;
                for (int k = 0; k < j; k++) {
                    acum += pow(L.get(j, k), 2);
                }
                float rad = A->get(j, j) - acum;
                L.set(((rad <= 0) ? 0.000001 : sqrt(rad)), j, j);
            }
            else {
                if (i > j) {
                    acum = 0;
                    for (int k = 0; k < j; k++) {
                        acum += L.get(i, k) * L.get(j, k);
                    }
                    L.set((1 / L.get(j, j)) * (A->get(i, j) - acum), i, j);
                }
                else {
                    L.set(0, i, j);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                Y.set(1 / L.get(i, i), i, i);
            }
            else {
                if (i > j) {
                    acum = 0;
                    for (int k = j; k < i; k++) {
                        acum += L.get(i, k) * Y.get(k, j);
                    }
                    Y.set(-(1 / L.get(i, i)) * acum, i, j);
                }
                else {
                    Y.set(0, i, j);
                }
            }
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < n; j++) {
            acum = 0;
            for (int k = i + 1; k < n; k++) {
                acum += L.get(k, i) * X->get(k, j);
            }
            X->set((1 / L.get(i, i)) * (Y.get(i, j) - acum), i, j);
        }
    }
}

#endif //SIMU_PROJEKT_MATRIX_OPERATIONS_HPP