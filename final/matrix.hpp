#ifndef SIMU_PROJEKT_MATRIX_HPP
#define SIMU_PROJEKT_MATRIX_HPP

#include <stdio.h>
#include <iostream>

// definicion de la clase Matrix (Matriz)
class Matrix {
private: // atributos privados
    int nrows, ncols; // numero de filas y columnas de la matriz
    float** data; // doble puntero para almacenar los datos de la matriz

    // metodo para crear la estructura de datos de la matriz
    void create() {
        data = (float**)malloc(sizeof(float*) * nrows); // asigna memoria para las filas de la matriz
        for (int r = 0; r < nrows; r++)
            data[r] = (float*)malloc(sizeof(float) * ncols); // asigna memoria para las columnas de la matriz
    }

public: // metodos publicos
    Matrix() {} // constructor

    // constructor que inicializa la matriz con un numero de filas y columnas
    Matrix(int rows, int cols) {
        nrows = rows; // asigna el numero de filas
        ncols = cols; // asigna el numero de columnas
        create(); // crea la estructura de datos de la matriz
    }

    // destructor para liberar la memoria asignada
    ~Matrix() {
        for (int r = 0; r < nrows; r++)
            free(data[r]); // libera la memoria de cada fila
        free(data); // libera la memoria del puntero de filas
    }

    // metodo para inicializar la matriz con ceros
    void init() {
        for (int r = 0; r < nrows; r++)
            for (int c = 0; c < ncols; c++)
                data[r][c] = 0; // asigna cero a cada elemento de la matriz
    }

    // metodo para establecer el tamano de la matriz y crear la estructura de datos
    void set_size(int rows, int cols) {
        nrows = rows;
        ncols = cols;
        create();
    }

    // metodo para obtener el numero de filas de la matriz
    int get_nrows() {
        return nrows;
    }

    // metodo para obtener el numero de columnas de la matriz
    int get_ncols() {
        return ncols;
    }


    // metodo para establecer el valor de un elemento de la matriz
    void set(float value, int row, int col) {
        data[row][col] = value;
    }

    // metodo para sumar un valor a un elemento de la matriz
    void add(float value, int row, int col) {
        data[row][col] += value;
    }

    // metodo para devolver el valor de un elemento de la matriz
    float get(int row, int col) {
        return data[row][col];
    }


    // metodo para eliminar una fila de la matriz
    void remove_row(int row) {
        int neo_index = 0;
        auto** neo_data = (float**)malloc(sizeof(float*) * (nrows - 1));
        for (int i = 0; i < nrows; i++)
            if (i != row) {
                neo_data[neo_index] = data[i];
                neo_index++;
            }
        free(data[row]);
        free(data);
        data = neo_data;
        nrows--;
    }

    // metodo para eliminar una columna de la matriz
    void remove_column(int col) {
        int neo_index = 0;
        auto** neo_data = (float**)malloc(sizeof(float*) * nrows);
        for (int r = 0; r < nrows; r++)
            neo_data[r] = (float*)malloc(sizeof(float) * (ncols - 1));

        for (int r = 0; r < nrows; r++) {
            for (int c = 0; c < ncols; c++)
                if (c != col) {
                    neo_data[r][neo_index] = data[r][c];
                    neo_index++;
                }
            neo_index = 0;
        }

        for (int r = 0; r < nrows; r++)
            free(data[r]);
        free(data);
        data = neo_data;
        ncols--;
    }


    // metodo para clonar una matriz
    void clone(Matrix* other) {
        for (int r = 0; r < nrows; r++)
            for (int c = 0; c < ncols; c++)
                other->set(data[r][c], r, c);
    }

    // metodo para mostrar la matriz en consola
    void show() {
        std::cout << "[ ";
        for (int r = 0; r < nrows; r++) {
            std::cout << "[ " << data[r][0];
            for (int c = 1; c < ncols; c++) {
                std::cout << ", " << data[r][c];
            }
            std::cout << " ] ";
        }
        std::cout << " ]\n\n";
    }
};

#endif //SIMU_PROJEKT_MATRIX_HPP