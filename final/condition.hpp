#ifndef SIMU_PROJEKT_CONDITION_HPP
#define SIMU_PROJEKT_CONDITION_HPP

#include "node.hpp"

// definicion de la clase Condition
class Condition {
private: // atributos privados
    Node* node;  // nodo al que se le aplica la condicion
    float value; // valor de la condicion

public: // metodos publicos
    // constructor para asignar nodo y valor
    Condition(Node* node_to_assign, float value_to_assign) {
        node = node_to_assign; // asigna nodo
        value = value_to_assign; // asigna valor
    }

    // metodo para asignar el nodo
    void set_node(Node* node_to_assign) {
        node = node_to_assign;
    }

    // metodo para obtener el nodo
    Node* get_node() {
        return node;
    }

    // metodo para asignar el valor de la condicion
    void set_value(float value_to_assign) {
        value = value_to_assign;
    }

    // metodo para obtener el valor de la condicion
    float get_value() {
        return value;
    }
};

#endif //SIMU_PROJEKT_CONDITION_HPP