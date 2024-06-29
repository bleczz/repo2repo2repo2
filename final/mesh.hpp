#ifndef SIMU_PROJEKT_MESH_HPP
#define SIMU_PROJEKT_MESH_HPP

#include <iostream>

#include "condition.hpp"
#include "element.hpp"
#include "node.hpp"

// Enumeraciones para los parametros y cantidades del problema
enum parameter { THERMAL_CONDUCTIVITY, HEAT_SOURCE };
enum quantity { NUM_NODES, NUM_ELEMENTS, NUM_DIRICHLET, NUM_NEUMANN };

class Mesh {
private:
    float problem_data[2];             // Datos del problema
    int quantities[4];                 // Cantidades del problema
    Node** nodes;                      // Arreglo de nodos
    Element** elements;                // Arreglo de elementos
    Condition** dirichlet_conditions;  // Arreglo de condiciones de dirichlet
    Condition** neumann_conditions;    // Arreglo de condiciones de neumann

public:
    Mesh()
        : nodes(nullptr),
        elements(nullptr),
        dirichlet_conditions(nullptr),
        neumann_conditions(nullptr) {}  // Constructor

    // Destructor para liberar memoria
    ~Mesh() {
        for (int i = 0; i < quantities[NUM_NODES]; ++i) delete nodes[i];
        for (int i = 0; i < quantities[NUM_ELEMENTS]; ++i) delete elements[i];
        for (int i = 0; i < quantities[NUM_DIRICHLET]; ++i)
            delete dirichlet_conditions[i];
        for (int i = 0; i < quantities[NUM_NEUMANN]; ++i)
            delete neumann_conditions[i];

        delete[] nodes;
        delete[] elements;
        delete[] dirichlet_conditions;
        delete[] neumann_conditions;
    }

    // Metodo para asignar los datos del problema
    void set_problem_data(float k, float Q) {
        problem_data[THERMAL_CONDUCTIVITY] = k;
        problem_data[HEAT_SOURCE] = Q;
    }

    // Metodo para obtener los datos del problema
    float get_problem_data(parameter position) {
        return problem_data[position];
    }

    // Metodo para asignar las cantidades del problema
    void set_quantities(int num_nodes, int num_elements, int num_dirichlet,
        int num_neumann) {
        quantities[NUM_NODES] = num_nodes;
        quantities[NUM_ELEMENTS] = num_elements;
        quantities[NUM_DIRICHLET] = num_dirichlet;
        quantities[NUM_NEUMANN] = num_neumann;
    }

    // Metodo para obtener las cantidades del problema
    int get_quantity(quantity position) { return quantities[position]; }

    // Metodo para inicializar los arreglos
    void init_arrays() {
        // nodes = new Node*[quantities[NUM_NODES]]();
        // elements = new Element*[quantities[NUM_ELEMENTS]]();
        // dirichlet_conditions = new Condition*[quantities[NUM_DIRICHLET]]();
        // neumann_conditions = new Condition*[quantities[NUM_NEUMANN]]();
        nodes = (Node**)malloc(sizeof(Node*) * quantities[NUM_NODES]);
        elements =
            (Element**)malloc(sizeof(Element*) * quantities[NUM_ELEMENTS]);
        dirichlet_conditions =
            (Condition**)malloc(sizeof(Condition*) * quantities[NUM_DIRICHLET]);
        neumann_conditions =
            (Condition**)malloc(sizeof(Condition*) * quantities[NUM_NEUMANN]);
    }

    // Metodo para insertar un nodo en la posicion dada
    void insert_node(Node* node, short position) {
        if (position >= 0 && position < quantities[NUM_NODES]) {
            nodes[position] = node;
        }
        else {
            std::cerr << "Error: Node position out of bounds\n";
        }
    }

    // Metodo para obtener un nodo en la posicion dada
    Node* get_node(int position) {
        if (position >= 0 && position < quantities[NUM_NODES]) {
            return nodes[position];
        }
        else {
            std::cerr << "Error: Node position out of bounds\n";
            return nullptr;
        }
    }

    // Metodo para insertar un elemento en la posicion dada
    void insert_element(Element* element, short position) {
        if (position >= 0 && position < quantities[NUM_ELEMENTS]) {
            elements[position] = element;
        }
        else {
            std::cerr << "Error: Element position out of bounds\n";
        }
    }

    // Metodo para obtener un elemento en la posicion dada
    Element* get_element(short position) {
        if (position >= 0 && position < quantities[NUM_ELEMENTS]) {
            return elements[position];
        }
        else {
            std::cerr << "Error: Element position out of bounds\n";
            return nullptr;
        }
    }

    // metodo para insertar una condicion de dirichlet en la posicion dada
    void insert_dirichlet_condition(Condition* dirichlet_condition,
        short position) {
        if (position >= 0 && position < quantities[NUM_DIRICHLET]) {
            dirichlet_conditions[position] = dirichlet_condition;
        }
        else {
            std::cerr << "Error: Dirichlet condition position out of bounds\n";
        }
    }

    // metodo para obtener una condicion de dirichlet en la posicion dada
    Condition* get_dirichlet_condition(short position) {
        if (position >= 0 && position < quantities[NUM_DIRICHLET]) {
            return dirichlet_conditions[position];
        }
        else {
            std::cerr << "Error: Dirichlet condition position out of bounds\n";
            return nullptr;
        }
    }

    // metodo para verificar si un nodo tiene una condicion de dirichlet
    bool does_node_have_dirichlet_condition(int id) {
        for (int i = 0; i < quantities[NUM_DIRICHLET]; i++) {
            if (dirichlet_conditions[i]->get_node()->get_ID() == id)
                return true;
        }
        return false;
    }

    // metodo para insertar una condicion de neumann en la posicion dada
    void insert_neumann_condition(Condition* neumann_condition,
        short position) {
        if (position >= 0 && position < quantities[NUM_NEUMANN]) {
            neumann_conditions[position] = neumann_condition;
        }
        else {
            std::cerr << "Error: Neumann condition position out of bounds\n";
        }
    }

    // metodo para obtener una condicion de neumann en la posicion dada
    Condition* get_neumann_condition(short position) {
        if (position >= 0 && position < quantities[NUM_NEUMANN]) {
            return neumann_conditions[position];
        }
        else {
            std::cerr << "Error: Neumann condition position out of bounds\n";
            return nullptr;
        }
    }

    void report() {
        std::cout << "Datos del Problema\n**********************\n";
        std::cout << "Conductividad Termica: "
            << problem_data[THERMAL_CONDUCTIVITY] << "\n";
        std::cout << "Fuente de Calor: " << problem_data[HEAT_SOURCE] << "\n\n";
        std::cout << "Cantidades\n***********************\n";
        std::cout << "Número de nodos: " << quantities[NUM_NODES] << "\n";
        std::cout << "Número de elementos: " << quantities[NUM_ELEMENTS]
            << "\n";
        std::cout << "Número de condiciones de frontera de Dirichlet: "
            << quantities[NUM_DIRICHLET] << "\n";
        std::cout << "Número de condiciones de frontera de Neumann: "
            << quantities[NUM_NEUMANN] << "\n\n";

        std::cout << "Lista de nodos\n**********************\n";
        for (int i = 0; i < quantities[NUM_NODES]; i++)
            std::cout << "Nodo: " << nodes[i]->get_ID()
            << ", x= " << nodes[i]->get_x_coordinate()
            << ", y= " << nodes[i]->get_y_coordinate()
            << ", z= " << nodes[i]->get_z_coordinate() << "\n";

        std::cout << "\nLista de elementos\n**********************\n";
        for (int i = 0; i < quantities[NUM_ELEMENTS]; i++) {
            std::cout << "Elemento: " << elements[i]->get_ID();
            std::cout << ", Nodo 1= " << elements[i]->get_node1()->get_ID()
                << ", Nodo 2= " << elements[i]->get_node2()->get_ID()
                << ", Nodo 3= " << elements[i]->get_node3()->get_ID()
                << ", Nodo 4= " << elements[i]->get_node4()->get_ID()
                << "\n";
        }

        std::cout << "\nLista de condiciones de frontera de "
            "Dirichlet\n**********************\n";
        for (int i = 0; i < quantities[NUM_DIRICHLET]; i++)
            std::cout << "Condicion" << i + 1 << ": "
            << dirichlet_conditions[i]->get_node()->get_ID()
            << ", Valor= " << dirichlet_conditions[i]->get_value()
            << "\n";

        std::cout << "\nLista de condiciones de frontera de "
            "Neumann\n**********************\n";
        for (int i = 0; i < quantities[NUM_NEUMANN]; i++)
            std::cout << "Condicion" << i + 1 << ": "
            << neumann_conditions[i]->get_node()->get_ID()
            << ", Valor= " << neumann_conditions[i]->get_value()
            << "\n";

        std::cout << "\n";
    }
};

#endif  // SIMU_PROJEKT_MESH_HPP