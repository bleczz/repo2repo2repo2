#include <iostream>

#include "mesh.hpp"
#include "input_output.hpp"
#include "matrix_operations.hpp"
#include "mef_process.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Incorrect use of the program, it must be: mef filename\n";
        exit(EXIT_FAILURE);
    }

    Mesh M;

    std::cout << "Reading geometry and mesh data...\n\n";
    std::string filename(argv[1]);
    read_input(filename, &M);
    //M.report();

    int num_nodes = M.get_quantity(NUM_NODES);
    int num_elements = M.get_quantity(NUM_ELEMENTS);
    Matrix K(num_nodes, num_nodes), local_Ks[1694];
    Vector b(num_nodes), local_bs[1694];

    std::cout << "Creating local systems...\n\n";
    create_local_systems(local_Ks, local_bs, num_elements, &M);

    std::cout << "Performing Assembly...\n\n";
    assembly(&K, &b, local_Ks, local_bs, num_elements, &M);

    //K.show();
    //b.show();

    std::cout << "Applying Neumann Boundary Conditions...\n\n";
    apply_neumann_boundary_conditions(&b, &M);

    //b.show();

    std::cout << "Applying Dirichlet Boundary Conditions...\n\n";
    apply_dirichlet_boundary_conditions(&K, &b, &M);

    //K.show();
    //b.show();

    std::cout << "Solving global system...\n\n";
    Vector T(b.get_size()), T_full(num_nodes);
    solve_system(&K, &b, &T);
    //T.show();

    std::cout << "Preparing results...\n\n";
    merge_results_with_dirichlet(&T, &T_full, num_nodes, &M);
    //T_full.show();

    std::cout << "Writing output file...\n\n";
    write_output(filename, &T_full);

    return 0;
}