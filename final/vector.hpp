#ifndef SIMU_PROJEKT_VECTOR_HPP
#define SIMU_PROJEKT_VECTOR_HPP

#include <iostream>

// definicion de la clase vector
class Vector {
private:       // atributos privados
    int size;     // tamano del vector
    float* data;  // puntero para almacenar los datos del vector

    // metodo para crear la estructura de datos del vector
    void create() {
        data = (float*)malloc(
            sizeof(float) * size);  // asigna memoria para los datos del vector
    }

public:       // metodos publicos
    Vector() {}  // constructor

    // constructor que inicializa el vector con un numero de datos
    Vector(int data_qty) {
        size = data_qty;  // asigna el tamano del vector
        create();         // crea la estructura de datos del vector
    }

    // destructor para liberar la memoria asignada
    ~Vector() {
        free(data);  // libera la memoria de los datos del vector
    }

    // metodo que inicializa el vector con ceros
    void init() {
        for (int i = 0; i < size; i++)
            data[i] = 0;  // asigna cero a cada elemento del vector
    }

    // metodo que establece el tamano del vector y crea la estructura de datos
    void set_size(int num_values) {
        size = num_values;  // asigna el tamano del vector
        create();           // crea la estructura de datos del vector
    }

    //  metodo que obtiene el tamano del vector
    int get_size() {
        return size;  // retorna el tamano del vector
    }

    // metodo que establece el valor de un elemento del vector en una posicion
    // dada
    void set(float value, int position) {
        data[position] = value;  // asigna el valor en la posicion indicada
    }

    // metodo que suma un valor a un elemento del vector en una posicion dada
    void add(float value, int position) {
        data[position] +=
            value;  // suma el valor al elemento en la posicion indicada
    }

    // metodo que devuelve el valor de un elemento del vector en una posicion
    // dada
    float get(int position) { return data[position]; }

    // metodo para eliminar un elemento del vector
    void remove_row(int row) {
        int neo_index = 0;
        float* neo_data = (float*)malloc(sizeof(float) * (size - 1));
        for (int i = 0; i < size; i++)
            if (i != row) {
                neo_data[neo_index] = data[i];
                neo_index++;
            }
        free(data);
        data = neo_data;
        size--;
    }

    // metodo para mostrar el vector en la consola
    void show() {
        std::cout << "[ " << data[0];
        for (int i = 1; i < size; i++) std::cout << "; " << data[i];
        std::cout << " ]\n\n";
    }
};

#endif  // SIMU_PROJEKT_VECTOR_HPP