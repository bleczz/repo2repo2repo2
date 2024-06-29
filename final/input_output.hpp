#ifndef SIMU_PROJEKT_INPUT_OUTPUT_HPP
#define SIMU_PROJEKT_INPUT_OUTPUT_HPP

#include <fstream>

#include "mesh.hpp"
#include "vector.hpp"
#include "element.hpp"

// metodo para leer los datos de entrada desde un archivo y poblar el objeto
// Mesh
void read_input(std::string filename, Mesh* M) {
    std::string line;
    float k, Q, T_bar, T_hat;
    int num_nodes, num_elements, num_dirichlet, num_neumann;
    std::ifstream dat_file(filename);  // abrir archivo de datos en modo lectura

    // leer los datos desde archivo
    dat_file >> k >> Q >> T_bar >> T_hat >> num_nodes >> num_elements >>
        num_dirichlet >> num_neumann;

    M->set_problem_data(k, Q);  // establecer los datos del problema
    M->set_quantities(num_nodes, num_elements, num_dirichlet,
        num_neumann);  // establecer las cantidades

    M->init_arrays();  // inicializar los arreglos

    dat_file >> line;

    // insertar nodos en el arreglo de nodos
    for (int i = 0; i < num_nodes; i++) {
        short id;
        float x, y, z;
        dat_file >> id >> x >> y >> z;
        M->insert_node(new Node(id, x, y, z), i);
    }

    dat_file >> line;


    for (int i = 0; i < num_elements; i++) {
        short id, node1_id, node2_id, node3_id, node4_id;
        dat_file >> id >> node1_id >> node2_id >> node3_id >> node4_id;
        M->insert_element(new Element(id, M->get_node(node1_id - 1),
                M->get_node(node2_id - 1), M->get_node(node3_id - 1),
                M->get_node(node4_id - 1)),
            i);
    }

    dat_file >> line;

    // insertar condiciones de dirichlet en el arreglo de condiciones de
    // dirichlet
    for (int i = 0; i < num_dirichlet; i++) {
        int id;
        dat_file >> id;
        M->insert_dirichlet_condition(new Condition(M->get_node(id - 1), T_bar),
            i);
    }

    dat_file >> line;

    // insertar condiciones de neumann en el arreglo de condiciones de neumann
    for (int i = 0; i < num_neumann; i++) {
        int id;
        dat_file >> id;
        M->insert_neumann_condition(new Condition(M->get_node(id - 1), T_hat),
            i);
    }

    dat_file.close();  // cerrar archivo
}

// metodo para escribir los resultados en un archivo de salida
void write_output(std::string filename, Vector* T) {
    std::ofstream res_file(
        filename +
        ".post.res");  // abrir archivo de resultados en modo escritura

    res_file << "GiD Post Results File 1.0\n";  // escribir encabezado del
    // archivo de resultados

    int n = T->get_size();

    res_file << R"(Result "Temperature" "Load Case 1" )" << 1
        << R"( Scalar OnNodes\n)";

    res_file << "ComponentNames \"T\"\n";  // Nombre genérico de la variable
    res_file << "Values\n";

    for (int i = 0; i < n; i++)
        res_file << i + 1 << "     " << T->get(i) << "\n";

    res_file << "End values\n";

    res_file.close();  // cerrar el archivo
}

#endif  // SIMU_PROJEKT_INPUT_OUTPUT_HPP