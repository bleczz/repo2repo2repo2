#ifndef SIMU_PROJEKT_ELEMENT_HPP
#define SIMU_PROJEKT_ELEMENT_HPP

#include "node.hpp"

// definicion de la clase Element que representa un elemento de la malla
class Element {
private:
    short ID;     // identificador del elemento
    Node* node1;  // primer nodo del elemento
    Node* node2;  // segundo nodo del elemento
    Node* node3;  // tercer nodo del elemento
    Node* node4;  // cuarto nodo del elemento

public:  // metodos publicos
    // constructor que inicializa el elemento con un identificador y tres nodos
    Element(short identifier, Node* first_node, Node* second_node,
        Node* third_node, Node* fourth_node) {
        ID = identifier;
        node1 = first_node;
        node2 = second_node;
        node3 = third_node;
        node4 = fourth_node;
    }

    // metodo para establecer el identificador del elemento
    void set_ID(short identifier) { ID = identifier; }

    // metodo para obtener el identificador del elemento
    short get_ID() { return ID; }

    // metodo para establecer el primer nodo del elemento
    void set_node1(Node* node) { node1 = node; }

    // metodo para obtener el primer nodo del elemento
    Node* get_node1() { return node1; }

    // metodo para establecer el segundo nodo del elemento
    void set_node2(Node* node) { node2 = node; }

    // metodo para obtener el segundo nodo del elemento
    Node* get_node2() { return node2; }

    // metodo para establecer el tercer nodo del elemento
    void set_node3(Node* node) { node3 = node; }

    // metodo para obtener el tercer nodo del elemento
    Node* get_node3() { return node3; }

    // metodo para establecer el cuarto nodo del elemento
    void set_node4(Node* node) { node4 = node; }

    // metodo para obtener el cuarto nodo del elemento
    Node* get_node4() { return node4; }
};

#endif  // SIMU_PROJEKT_ELEMENT_HPP