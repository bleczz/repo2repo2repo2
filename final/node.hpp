#ifndef SIMU_PROJEKT_NODE_HPP
#define SIMU_PROJEKT_NODE_HPP

class Node {
private:              // atributos privados
    int ID;              // identificador del nodo
    float x_coordinate;  // coordenada x del nodo
    float y_coordinate;  // coordenada y del nodo
    float z_coordinate;  // coordenada z del nodo

public:  // metodos publicos
    // constructor que inicializa el nodo con un identificador y coordenadas
    Node(int identifier, float x_value, float y_value, float z_value) {
        ID = identifier;         // asigna el identificador
        x_coordinate = x_value;  // asigna la coordenada x
        y_coordinate = y_value;  // asigna la coordenada y
        z_coordinate = z_value;  // asigna la coordenada z
    }

    // metodo para establecer el identificador del nodo
    void set_ID(int identifier) { ID = identifier; }

    // metodo para obtener el identificador del nodo
    int get_ID() { return ID; }

    // metodo para establecer la coordenada x del nodo
    void set_x_coordinate(float x_value) { x_coordinate = x_value; }

    // metodo para obtener la coordenada x del nodo
    float get_x_coordinate() { return x_coordinate; }

    // metodo para establecer la coordenada y del nodo
    void set_y_coordinate(float y_value) { y_coordinate = y_value; }

    // metodo para obtener la coordenada y del nodo
    float get_y_coordinate() { return y_coordinate; }

    // metodo para establecer la coordenada z del nodo
    void set_z_coordinate(float z_value) { z_coordinate = z_value; }

    // metodo para obtener la coordenada z del nodo
    float get_z_coordinate() { return z_coordinate; }
};

#endif  // SIMU_PROJEKT_NODE_HPP