#ifndef SIMU_PROJEKT_MEF_PROCESS_HPP
#define SIMU_PROJEKT_MEF_PROCESS_HPP

#include <cmath>

#include "mesh.hpp"
#include "matrix.hpp"
#include "matrix_operations.hpp"

/*
  el volumen V del tetraedro definido por los vertices (x1, y1, z1), (x2, y2,
  z2), (x3, y3, z3) y (x4, y4, z4) se puede calcular utilizando el metodo del
  determinante. la formula esta dada por:

  V = (1/6) * | (x2 - x1)(y3 - y1)(z4 - z1) + (y2 - y1)(z3 - z1)(x4 - x1) + (z2
  - z1)(x3 - x1)(y4 - y1)
               - (z2 - z1)(y3 - y1)(x4 - x1) - (x2 - x1)(z3 - z1)(y4 - y1) - (y2
  - y1)(x3 - x1)(z4 - z1) |

    basado en la formula anterior, se construye la siguiente funcion para
  calcular el determinante de una matriz formada por las diferencias de las
  coordenadas de los vertices
 */
float calculate_local_volume(float x1, float y1, float z1, float x2, float y2,
    float z2, float x3, float y3, float z3, float x4,
    float y4, float z4) {
    float A = std::abs((x2 - x1) * (y3 - y1) * (z4 - z1) +
        (y2 - y1) * (z3 - z1) * (x4 - x1) +
        (z2 - z1) * (x3 - x1) * (y4 - y1) -
        (z2 - z1) * (y3 - y1) * (x4 - x1) -
        (x2 - x1) * (z3 - z1) * (y4 - y1) -
        (y2 - y1) * (x3 - x1) * (z4 - z1)) /
        6;
    return ((A == 0) ? 0.000001 : A);
}

/*
  ecuaciones de transformacion

  expresando las coordenadas (x, y, z) en terminos de  coordenadas (ξ, η, φ):

  x(ξ, η, φ) = (x2 - x1)ξ + (x3 - x1)η + (x4 - x1)φ + x1
  y(ξ, η, φ) = (y2 - y1)ξ + (y3 - y1)η + (y4 - y1)φ + y1
  z(ξ, η, φ) = (z2 - z1)ξ + (z3 - z1)η + (z4 - z1)φ + z1

  matriz jacobiana J:

  la matriz jacobiana J se construye como:
  J = [ ∂x/∂ξ  ∂x/∂η  ∂x/∂φ ]
      [ ∂y/∂ξ  ∂y/∂η  ∂y/∂φ ]
      [ ∂z/∂ξ  ∂z/∂η  ∂z/∂φ ]

  en este contexto, las derivadas parciales son
  J = [ x2 - x1  x3 - x1  x4 - x1 ]
      [ y2 - y1  y3 - y1  y4 - y1 ]
      [ z2 - z1  z3 - z1  z4 - z1 ]

  basado en el analisis previo, se construye la siguiente funcion para calcular
  el determinante del jacobiano de una matriz para un elemento tetraedrico 3D.
 */
float calculate_local_jacobian(float x1, float y1, float z1, float x2, float y2,
    float z2, float x3, float y3, float z3, float x4,
    float y4, float z4) {
    float J =
        (x2 - x1) * (y3 - y1) * (z4 - z1) + (x3 - x1) * (y4 - y1) * (z2 - z1) +
        (x4 - x1) * (y2 - y1) * (z3 - z1) - (x4 - x1) * (y3 - y1) * (z2 - z1) -
        (x3 - x1) * (y2 - y1) * (z4 - z1) - (x2 - x1) * (y4 - y1) * (z3 - z1);
    return ((J == 0) ? 0.000001 : J);
}

/*
  la matriz B para un elemento tetraedrico se define como:

  B = [ -1  1  0  0 ]
      [ -1  0  1  0 ]
      [ -1  0  0  1 ]

  basado en la definicion anterior, se construye la siguiente funcion para
  calcular la matriz B para un elemento tetraedrico 3D
 */
void calculate_B(Matrix* B) {
    B->set(-1, 0, 0);  // B[0][0] = -1
    B->set(1, 0, 1);   // B[0][1] = 1
    B->set(0, 0, 2);   // B[0][2] = 0
    B->set(0, 0, 3);   // B[0][3] = 0

    B->set(-1, 1, 0);  // B[1][0] = -1
    B->set(0, 1, 1);   // B[1][1] = 0
    B->set(1, 1, 2);   // B[1][2] = 1
    B->set(0, 1, 3);   // B[1][3] = 0

    B->set(-1, 2, 0);  // B[2][0] = -1
    B->set(0, 2, 1);   // B[2][1] = 0
    B->set(0, 2, 2);   // B[2][2] = 0
    B->set(1, 2, 3);   // B[2][3] = 1
}

/*
 la matriz A^e para un elemento tetraedrico se define por los siguientes
 elementos:

 A^e = [ (y3 - y1) * (z4 - z1) - (y4 - y1) * (z3 - z1)   (x4 - x1) * (z3 - z1) -
 (x3 - x1) * (z4 - z1)   (x3 - x1) * (y4 - y1) - (x4 - x1) * (y3 - y1) ] [ (y4 -
 y1) * (z2 - z1) - (y2 - y1) * (z4 - z1)   (x2 - x1) * (z4 - z1) - (x4 - x1) *
 (z2 - z1)   (x4 - x1) * (y2 - y1) - (x2 - x1) * (y4 - y1) ] [ (y2 - y1) * (z3 -
 z1) - (y3 - y1) * (z2 - z1)   (x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1) (x2
 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1) ]

 por lo cual, se construye la siguiente funcion para calcular la matriz A^e para
 un elemento tetraedrico 3D
 */
void calculate_local_A(Matrix* A, float x1, float y1, float z1, float x2,
    float y2, float z2, float x3, float y3, float z3,
    float x4, float y4, float z4) {
    A->set((y3 - y1) * (z4 - z1) - (y4 - y1) * (z3 - z1), 0, 0);  // A[0][0]
    A->set((x4 - x1) * (z3 - z1) - (x3 - x1) * (z4 - z1), 0, 1);  // A[0][1]
    A->set((x3 - x1) * (y4 - y1) - (x4 - x1) * (y3 - y1), 0, 2);  // A[0][2]

    A->set((y4 - y1) * (z2 - z1) - (y2 - y1) * (z4 - z1), 1, 0);  // A[1][0]
    A->set((x2 - x1) * (z4 - z1) - (x4 - x1) * (z2 - z1), 1, 1);  // A[1][1]
    A->set((x4 - x1) * (y2 - y1) - (x2 - x1) * (y4 - y1), 1, 2);  // A[1][2]

    A->set((y2 - y1) * (z3 - z1) - (y3 - y1) * (z2 - z1), 2, 0);  // A[2][0]
    A->set((x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1), 2, 1);  // A[2][1]
    A->set((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1), 2, 2);  // A[2][2]
}

/*
  la matriz de rigidez K^e se define por:

  K^e = (k * V^e / J^e * J^e) * B^T * (A^e)^T * A^e * B

  por lo que la funcion para crear la matriz de rigidez local K^e para un
  elemento tetraedrico 3D se implementa de la siguiente forma
 */
void create_local_K(Matrix* K, int element_id, Mesh* M) {
    // establecer el tamano de la matriz K
    K->set_size(4, 4);

    // obtener la conductividad termica
    float k = M->get_problem_data(THERMAL_CONDUCTIVITY);

    // obtener las coordenadas de los nodos del elemento
    float x1 = M->get_element(element_id)->get_node1()->get_x_coordinate();
    float y1 = M->get_element(element_id)->get_node1()->get_y_coordinate();
    float z1 = M->get_element(element_id)->get_node1()->get_z_coordinate();

    float x2 = M->get_element(element_id)->get_node2()->get_x_coordinate();
    float y2 = M->get_element(element_id)->get_node2()->get_y_coordinate();
    float z2 = M->get_element(element_id)->get_node2()->get_z_coordinate();

    float x3 = M->get_element(element_id)->get_node3()->get_x_coordinate();
    float y3 = M->get_element(element_id)->get_node3()->get_y_coordinate();
    float z3 = M->get_element(element_id)->get_node3()->get_z_coordinate();

    float x4 = M->get_element(element_id)->get_node4()->get_x_coordinate();
    float y4 = M->get_element(element_id)->get_node4()->get_y_coordinate();
    float z4 = M->get_element(element_id)->get_node4()->get_z_coordinate();


    // calcular el volumen del elemento tetraedrico
    float Volume =
        calculate_local_volume(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);

    // calcular el determinante de la matriz jacobiana
    float J = calculate_local_jacobian(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4,
        y4, z4);

    std::cout << "\t\tVolumen para el elemento " << element_id + 1 << ": "
        << Volume << "\n";
    std::cout << "\t\tJacobiano para el elemento " << element_id + 1 << ": "
        << J << "\n";

    // inicializar las matrices B y A^e para posteriormente calcularlas
    Matrix B(3, 4), A(3, 3);
    calculate_B(&B);
    calculate_local_A(&A, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);

    // mostrar las matrices B y A^e
    B.show();
    A.show();

    // inicializar las matrices transpuestas B^T y (A^e)^T para posteriormente
    // calcularlas
    Matrix Bt(4, 3), At(3, 3);
    transpose(&B, 3, 4, &Bt);
    transpose(&A, 3, 3, &At);

    // calcular la matriz de rigidez local K^e
    Matrix res1, res2, res3;
    product_matrix_by_matrix(&A, &B, &res1);      // A * B
    product_matrix_by_matrix(&At, &res1, &res2);  // (A^T) * (A * B)
    product_matrix_by_matrix(&Bt, &res2, &res3);  // B^T * (A^T * (A * B))
    product_scalar_by_matrix(k * Volume / (J * J), &res3, 4, 4,
        K);  // k * V^e / (J^e * J^e) * res3

    std::cout << "\t\tMatriz local creada para el elemento " << element_id + 1
        << ": ";
    K->show();
    std::cout << "\n";
}

/*
  funcion para crear el vector de carga local b^e para un elemento tetraedrico
  3D

  el vector de carga b^e se define por:

  b^e = (Q * J^e / 24) * [1 1 1 1]
 */

void create_local_b(Vector* b, int element_id, Mesh* M) {
    b->set_size(4);

    // obtener el termino de fuente de calor
    float Q = M->get_problem_data(HEAT_SOURCE);

    // obtener las coordenadas de los nodos del elemento
    float x1 = M->get_element(element_id)->get_node1()->get_x_coordinate();
    float y1 = M->get_element(element_id)->get_node1()->get_y_coordinate();
    float z1 = M->get_element(element_id)->get_node1()->get_z_coordinate();

    float x2 = M->get_element(element_id)->get_node2()->get_x_coordinate();
    float y2 = M->get_element(element_id)->get_node2()->get_y_coordinate();
    float z2 = M->get_element(element_id)->get_node2()->get_z_coordinate();

    float x3 = M->get_element(element_id)->get_node3()->get_x_coordinate();
    float y3 = M->get_element(element_id)->get_node3()->get_y_coordinate();
    float z3 = M->get_element(element_id)->get_node3()->get_z_coordinate();

    float x4 = M->get_element(element_id)->get_node4()->get_x_coordinate();
    float y4 = M->get_element(element_id)->get_node4()->get_y_coordinate();
    float z4 = M->get_element(element_id)->get_node4()->get_z_coordinate();

    // calcular el determinante de la matriz jacobiana
    float J = calculate_local_jacobian(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4,
        y4, z4);

    // establecer las entradas del vector b^e
    b->set(Q * J / 24, 0);
    b->set(Q * J / 24, 1);
    b->set(Q * J / 24, 2);
    b->set(Q * J / 24, 3);

    std::cout << "\t\tVector local creado para el elemento " << element_id + 1
        << ": ";
    b->show();
    std::cout << "\n";
}

/*
     funcion para crear las matrices de rigidez locales y los vectores de carga
     la funcion itera sobre  todos los elementos en la malla y llama a las
   funciones para crear la matriz de rigidez local (K^e) y el vector de carga
   local (b^e) para cada elemento.
 */
void create_local_systems(Matrix* Ks, Vector* bs, int num_elements, Mesh* M) {
    for (int e = 0; e < num_elements; e++) {
        std::cout << "\tCreating local system for Element " << e + 1
            << "...\n\n";
        create_local_K(&Ks[e], e,
            M);  // crear matriz de rigidez local para el elemento e
        create_local_b(&bs[e], e,
            M);  // crear vector de carga local para el elemento e
    }
}

/*
    función para ensamblar la matriz de rigidez K a partir de las matrices
   locales
 */
void assembly_K(Matrix* K, Matrix* local_K, int index1, int index2, int index3,
    int index4) {
    K->add(local_K->get(0, 0), index1,
        index1);  // sumar local_K[0][0] a K[index1][index1]
    K->add(local_K->get(0, 1), index1,
        index2);  // sumar local_K[0][1] a K[index1][index2]
    K->add(local_K->get(0, 2), index1,
        index3);  // sumar local_K[0][2] a K[index1][index3]
    K->add(local_K->get(0, 3), index1,
        index4);  // sumar local_K[0][3] a K[index1][index4]

    K->add(local_K->get(1, 0), index2,
        index1);  // sumar local_K[1][0] a K[index2][index1]
    K->add(local_K->get(1, 1), index2,
        index2);  // sumar local_K[1][1] a K[index2][index2]
    K->add(local_K->get(1, 2), index2,
        index3);  // sumar local_K[1][2] a K[index2][index3]
    K->add(local_K->get(1, 3), index2,
        index4);  // sumar local_K[1][3] a K[index2][index4]

    K->add(local_K->get(2, 0), index3,
        index1);  // sumar local_K[2][0] a K[index3][index1]
    K->add(local_K->get(2, 1), index3,
        index2);  // sumar local_K[2][1] a K[index3][index2]
    K->add(local_K->get(2, 2), index3,
        index3);  // sumar local_K[2][2] a K[index3][index3]
    K->add(local_K->get(2, 3), index3,
        index4);  // sumar local_K[2][3] a K[index3][index4]

    K->add(local_K->get(3, 0), index4,
        index1);  // sumar local_K[3][0] a K[index4][index1]
    K->add(local_K->get(3, 1), index4,
        index2);  // sumar local_K[3][1] a K[index4][index2]
    K->add(local_K->get(3, 2), index4,
        index3);  // sumar local_K[3][2] a K[index4][index3]
    K->add(local_K->get(3, 3), index4,
        index4);  // sumar local_K[3][3] a K[index4][index4]
}

/*
    función para ensamblar el vector de carga b a partir de los vectores de
   carga locales.
 */
void assembly_b(Vector* b, Vector* local_b, int index1, int index2, int index3,
    int index4) {
    b->add(local_b->get(0), index1);  // sumar local_b[0] a b[index1]
    b->add(local_b->get(1), index2);  // sumar local_b[1] a b[index2]
    b->add(local_b->get(2), index3);  // sumar local_b[2] a b[index3]
    b->add(local_b->get(3), index4);  // sumar local_b[3] a b[index4]
}

/*
    función para ensamblar la matriz global de rigidez K y el vector de carga
   global b a partir de las matrices de rigidez locales y los vectores de carga
   locales.
 */
void assembly(Matrix* K, Vector* b, Matrix* Ks, Vector* bs, int num_elements,
    Mesh* M) {
    K->init();  // inicializar la matriz global de rigidez
    b->init();  // inicializar el vector de carga global
    K->show();
    b->show();  // mostrar las matrices inicializadas

    // iterar sobre cada elemento del mallado
    for (int e = 0; e < num_elements; e++) {
        std::cout << "\tEnsamblando para el elemento " << e + 1 << "...\n\n";

        // obtener los índices globales correspondientes a los nodos del
        // elemento
        int index1 = M->get_element(e)->get_node1()->get_ID() - 1;
        int index2 = M->get_element(e)->get_node2()->get_ID() - 1;
        int index3 = M->get_element(e)->get_node3()->get_ID() - 1;
        int index4 = M->get_element(e)->get_node4()->get_ID() - 1;

        // ensamblar la matriz de rigidez global K
        assembly_K(K, &Ks[e], index1, index2, index3, index4);

        // ensamblar el vector de carga global b
        assembly_b(b, &bs[e], index1, index2, index3, index4);

        // mostrar las matrices ensambladas (opcional)
        std::cout << "\t\t";
        K->show();
        std::cout << "\t\t";
        b->show();
        std::cout << "\n";
    }
}

/*
    función para aplicar las condiciones de contorno de Neumann al vector de
   carga global b.
 */
void apply_neumann_boundary_conditions(Vector* b, Mesh* M) {
    int num_conditions = M->get_quantity(
        NUM_NEUMANN);  // obtener la cantidad de condiciones de Neumann

    // iterar sobre cada condición de Neumann en el mallado y aplicarla al
    // vector de carga global
    for (int c = 0; c < num_conditions; c++) {
        Condition* cond =
            M->get_neumann_condition(c);  // obtener la condición de Neumann

        int index = cond->get_node()->get_ID() -
            1;  // obtener el índice global del nodo
        b->add(cond->get_value(),
            index);  // sumar el valor de la condición de Neumann al vector
        // de carga global b
    }
    std::cout << "\t\t";
    b->show();
    std::cout << "\n";  // mostrar el vector de carga actualizado
}

/*
     TODO: agregar comentario descriptivo
 */
void add_column_to_RHS(Matrix* K, Vector* b, int col, float T_bar) {
    for (int r = 0; r < K->get_nrows(); r++) {
        // obtener el valor de la matriz K en la fila r y la columna col
        float value = K->get(r, col);
        // ajustar este valor por -T_bar y sumarlo al vector de carga b en el
        // índice correspondiente a la fila r
        b->add(-T_bar * value, r);
    }
}

/*
    función para aplicar las condiciones de contorno de Dirichlet a la matriz
   global de rigidez K y al vector de carga global b. las condiciones de
   contorno de Dirichlet se aplican eliminando las filas y columnas
   correspondientes en la matriz de rigidez global K y ajustando el vector de
   carga global b en consecuencia.
 */
void apply_dirichlet_boundary_conditions(Matrix* K, Vector* b, Mesh* M) {
    int num_conditions = M->get_quantity(
        NUM_DIRICHLET);  // obtener la cantidad de condiciones de Dirichlet
    int previous_removed = 0;

    for (int c = 0; c < num_conditions; c++) {
        Condition* cond =
            M->get_dirichlet_condition(c);  // obtener la condición de Dirichlet

        int index = cond->get_node()->get_ID() - 1 -
            previous_removed;  // obtener el índice global del nodo
        float cond_value =
            cond->get_value();  // obtener el valor de la condición de Dirichlet

        // eliminar la fila correspondiente en la matriz de rigidez K
        K->remove_row(index);

        // eliminar la fila correspondiente en el vector de carga b
        b->remove_row(index);

        // ajustar el vector de carga global b sumando la contribución de la
        // columna eliminada
        add_column_to_RHS(K, b, index, cond_value);

        // eliminar la columna correspondiente en la matriz de rigidez K
        K->remove_column(index);

        previous_removed++;
    }
}

/*
    función para resolver el sistema de ecuaciones K T = b utilizando la matriz
   inversa.
 */
void solve_system(Matrix* K, Vector* b, Vector* T) {
    int n = K->get_nrows();  // obtener el número de filas de la matriz K

    Matrix Kinv(n, n);  // inicializar la matriz para almacenar la inversa de K

    std::cout << "\tCalculando la inversa de la matriz global K...\n\n";
    calculate_inverse(K, n, &Kinv);  // calcular la inversa de la matriz K

    std::cout << "\tEjecutando calculo final...\n\n";
    product_matrix_by_vector(
        &Kinv, b, n, n,
        T);  // multiplicar Kinv por b para obtener la solución T
}

/*
    función para combinar los resultados obtenidos con las condiciones de
   contorno de Dirichlet en el vector de temperatura final.
 */
void merge_results_with_dirichlet(Vector* T, Vector* Tf, int n, Mesh* M) {
    int num_dirichlet = M->get_quantity(
        NUM_DIRICHLET);  // obtener la cantidad de condiciones de Dirichlet

    int cont_dirichlet =
        0;  // inicializar contador para las condiciones de Dirichlet
    int cont_T =
        0;  // inicializar contador para el vector de temperaturas calculadas

    for (int i = 0; i < n; i++) {
        if (M->does_node_have_dirichlet_condition(
            i + 1)) {  // si el nodo i tiene una condición de Dirichlet
            Condition* cond = M->get_dirichlet_condition(
                cont_dirichlet);  // obtener la condición de Dirichlet
            cont_dirichlet++;

            float cond_value = cond->get_value();  // obtener el valor de la
            // condición de Dirichlet

            Tf->set(cond_value, i);  // establecer el valor de Dirichlet en el
            // vector de temperaturas final
        }
        else {
            Tf->set(T->get(cont_T), i);  // establecer el valor calculado en el
            // vector de temperaturas final
            cont_T++;
        }
    }
}

#endif  // SIMU_PROJEKT_MEF_PROCESS_HPP