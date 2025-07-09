/**
 * @file ArbolBinario.cpp
 * @brief Implementación de las funciones para el manejo de árboles binarios y AVL de clientes.
 *
 * Este archivo contiene la implementación de las funciones para la creación, inserción,
 * balanceo y visualización de árboles binarios y AVL, utilizados para organizar y mostrar
 * clientes en el sistema bancario. Incluye algoritmos para impresión en formato ASCII y
 * utilidades para estadísticas y manejo de memoria.
 */

#include "ArbolBinario.h"
#include "validaciones.h" 
#include "Banco.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <conio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

using namespace std;


extern void ajustar_cursor_para_marquesina();
extern int seleccionar_opcion(const char* titulo, const char* opciones[], int num_opciones, int fila_inicio);
extern void detener_marquesina();
extern void inicializar_marquesina();

// Constantes para el algoritmo ASCII
#define MAX_HEIGHT 1000
#define ASCII_INFINITY INT_MAX
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

// Variables globales para el algoritmo ASCII
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
int print_next;
int gap = 3;

// Clase para nodos ASCII
class asciinode_struct {
public:
    asciinode_struct* left, *right;
    int edge_length;
    int height;
    int lablen;
    int parent_dir;
    char label[20]; // Aumentado para nombres más largos
};

typedef class asciinode_struct asciinode;

/**
 * @class Tree
 * @brief Nodo de un árbol AVL de clientes.
 *
 * Esta clase representa un nodo de un árbol AVL, utilizado para almacenar y organizar
 * clientes según diferentes campos (DNI, nombre, apellido, teléfono o email).
 * Cada nodo contiene un puntero al cliente, el valor del campo por el que se ordena,
 * y punteros a los hijos izquierdo y derecho, así como la altura para el balanceo AVL.
 */
class Tree {
public:
    Tree* left, *right; ///< Punteros a los hijos izquierdo y derecho
    string element;     ///< Valor del campo por el que se ordena
    Cliente* cliente;   ///< Puntero al cliente asociado
    int height;         ///< Altura del nodo para balanceo AVL
    /**
     * @brief Constructor de Tree
     * @param elem Valor del campo por el que se ordena
     * @param cli Puntero al cliente asociado
     */
    Tree(string elem, Cliente* cli) : element(elem), cliente(cli), left(nullptr), right(nullptr), height(1) {}
};

// Función para obtener el valor a mostrar según el campo
/**
 * @brief Obtiene el valor de un campo específico de un cliente.
 * @param cliente Puntero al cliente
 * @param campo Índice del campo (1: DNI, 2: Nombre, 3: Apellido, 4: Teléfono, 5: Email)
 * @return Valor del campo como string
 */
string obtener_valor_campo(Cliente* cliente, int campo) {
    switch(campo) {
        case 1: // DNI completo
            return cliente->get_dni();
        case 2: // Nombre completo
            return cliente->get_nombres();
        case 3: // Apellido completo
            return cliente->get_apellidos();
        case 4: // Teléfono completo
            return cliente->get_telefono();
        case 5: // Email (hasta @)
        {
            string email = cliente->get_email();
            size_t pos = email.find('@');
            return (pos != string::npos) ? email.substr(0, pos) : email;
        }
        default:
            return cliente->get_dni();
    }
}

/**
 * @brief Inserta un nodo en un árbol binario de búsqueda (BST).
 * @param nodo Nodo raíz actual
 * @param valor Valor a insertar
 * @param cliente Puntero al cliente
 * @param campo Campo por el que se ordena
 * @return Nueva raíz del subárbol
 */
Tree* insertar_bst(Tree* nodo, string valor, Cliente* cliente, int campo) {
    if (!nodo) {
        return new Tree(valor, cliente);
    }
    
    string valor_nodo = obtener_valor_campo(nodo->cliente, campo);
    
    // Comparación para diferentes tipos de datos
    bool menor = false;
    if (campo == 1 || campo == 4) { // DNI o Teléfono (numérico)
        menor = stoi(valor) < stoi(valor_nodo);
    } else { // Texto (nombres, apellidos, email)
        string val_lower = valor, nodo_lower = valor_nodo;
        transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
        transform(nodo_lower.begin(), nodo_lower.end(), nodo_lower.begin(), ::tolower);
        menor = val_lower < nodo_lower;
    }
    
    if (menor) {
        nodo->left = insertar_bst(nodo->left, valor, cliente, campo);
    } else {
        nodo->right = insertar_bst(nodo->right, valor, cliente, campo);
    }
    
    return nodo;
}

/**
 * @brief Crea una etiqueta truncada para visualización en el árbol ASCII.
 * @param valor Valor original
 * @return Etiqueta truncada a 8 caracteres
 */
string crear_etiqueta(string valor) {
    if (valor.length() <= 8) {
        return valor;
    } else {
        return valor.substr(0, 8);
    }
}

// FUNCIONES PARA ÁRBOL AVL

/**
 * @brief Obtiene la altura de un nodo AVL.
 * @param nodo Nodo a consultar
 * @return Altura del nodo
 */
int obtener_altura(Tree* nodo) {
    if (nodo == nullptr) return 0;
    return nodo->height;
}

/**
 * @brief Calcula el factor de balanceo de un nodo AVL.
 * @param nodo Nodo a consultar
 * @return Factor de balanceo (altura izquierda - altura derecha)
 */
int factor_balanceo(Tree* nodo) {
    if (nodo == nullptr) return 0;
    return obtener_altura(nodo->left) - obtener_altura(nodo->right);
}

/**
 * @brief Actualiza la altura de un nodo AVL.
 * @param nodo Nodo a actualizar
 */
void actualizar_altura(Tree* nodo) {
    if (nodo != nullptr) {
        nodo->height = 1 + MAX(obtener_altura(nodo->left), obtener_altura(nodo->right));
    }
}

/**
 * @brief Realiza una rotación simple a la derecha (LL) en un subárbol AVL.
 * @param y Raíz del subárbol
 * @return Nueva raíz tras la rotación
 */
Tree* rotacion_derecha(Tree* y) {
    Tree* x = y->left;
    Tree* T2 = x->right;

    // Realizar rotación
    x->right = y;
    y->left = T2;

    // Actualizar alturas
    actualizar_altura(y);
    actualizar_altura(x);

    // Retornar nueva raíz
    return x;
}

/**
 * @brief Realiza una rotación simple a la izquierda (RR) en un subárbol AVL.
 * @param x Raíz del subárbol
 * @return Nueva raíz tras la rotación
 */
Tree* rotacion_izquierda(Tree* x) {
    Tree* y = x->right;
    Tree* T2 = y->left;

    // Realizar rotación
    y->left = x;
    x->right = T2;

    // Actualizar alturas
    actualizar_altura(x);
    actualizar_altura(y);

    // Retornar nueva raíz
    return y;
}

/**
 * @brief Inserta un nodo en un árbol AVL, manteniendo el balanceo.
 * @param nodo Nodo raíz actual
 * @param valor Valor a insertar
 * @param cliente Puntero al cliente
 * @param campo Campo por el que se ordena
 * @return Nueva raíz del subárbol
 */
Tree* insertar_avl(Tree* nodo, string valor, Cliente* cliente, int campo) {
    // 1. Inserción BST normal
    if (nodo == nullptr) {
        return new Tree(valor, cliente);
    }

    string valor_nodo = obtener_valor_campo(nodo->cliente, campo);
    
    // Comparación para diferentes tipos de datos
    bool menor = false;
    if (campo == 1 || campo == 4) { // DNI o Teléfono (numérico)
        menor = stoi(valor) < stoi(valor_nodo);
    } else { // Texto (nombres, apellidos, email)
        string val_lower = valor, nodo_lower = valor_nodo;
        transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
        transform(nodo_lower.begin(), nodo_lower.end(), nodo_lower.begin(), ::tolower);
        menor = val_lower < nodo_lower;
    }

    if (menor) {
        nodo->left = insertar_avl(nodo->left, valor, cliente, campo);
    } else if (!menor) { // Mayor o igual
        nodo->right = insertar_avl(nodo->right, valor, cliente, campo);
    } else {
        // Valores iguales no se insertan en AVL
        return nodo;
    }

    // 2. Actualizar altura del nodo actual
    actualizar_altura(nodo);

    // 3. Obtener factor de balanceo
    int balance = factor_balanceo(nodo);

    // 4. Si el nodo está desbalanceado, hay 4 casos:

    // Caso Izquierda-Izquierda (LL)
    if (balance > 1) {
        string valor_izq = obtener_valor_campo(nodo->left->cliente, campo);
        bool menor_izq = false;
        if (campo == 1 || campo == 4) {
            menor_izq = stoi(valor) < stoi(valor_izq);
        } else {
            string val_lower = valor, izq_lower = valor_izq;
            transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
            transform(izq_lower.begin(), izq_lower.end(), izq_lower.begin(), ::tolower);
            menor_izq = val_lower < izq_lower;
        }
        
        if (menor_izq) {
            return rotacion_derecha(nodo);
        }
    }

    // Caso Derecha-Derecha (RR)
    if (balance < -1) {
        string valor_der = obtener_valor_campo(nodo->right->cliente, campo);
        bool mayor_der = false;
        if (campo == 1 || campo == 4) {
            mayor_der = stoi(valor) >= stoi(valor_der);
        } else {
            string val_lower = valor, der_lower = valor_der;
            transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
            transform(der_lower.begin(), der_lower.end(), der_lower.begin(), ::tolower);
            mayor_der = val_lower >= der_lower;
        }
        
        if (mayor_der) {
            return rotacion_izquierda(nodo);
        }
    }

    // Caso Izquierda-Derecha (LR)
    if (balance > 1) {
        string valor_izq = obtener_valor_campo(nodo->left->cliente, campo);
        bool mayor_izq = false;
        if (campo == 1 || campo == 4) {
            mayor_izq = stoi(valor) >= stoi(valor_izq);
        } else {
            string val_lower = valor, izq_lower = valor_izq;
            transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
            transform(izq_lower.begin(), izq_lower.end(), izq_lower.begin(), ::tolower);
            mayor_izq = val_lower >= izq_lower;
        }
        
        if (mayor_izq) {
            nodo->left = rotacion_izquierda(nodo->left);
            return rotacion_derecha(nodo);
        }
    }

    // Caso Derecha-Izquierda (RL)
    if (balance < -1) {
        string valor_der = obtener_valor_campo(nodo->right->cliente, campo);
        bool menor_der = false;
        if (campo == 1 || campo == 4) {
            menor_der = stoi(valor) < stoi(valor_der);
        } else {
            string val_lower = valor, der_lower = valor_der;
            transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
            transform(der_lower.begin(), der_lower.end(), der_lower.begin(), ::tolower);
            menor_der = val_lower < der_lower;
        }
        
        if (menor_der) {
            nodo->right = rotacion_derecha(nodo->right);
            return rotacion_izquierda(nodo);
        }
    }

    // Retornar el nodo sin cambios
    return nodo;
}

/**
 * @brief Verifica si un árbol es AVL (balanceado).
 * @param nodo Nodo raíz
 * @return true si está balanceado, false en caso contrario
 */
bool verificar_avl(Tree* nodo) {
    if (nodo == nullptr) return true;
    
    int balance = factor_balanceo(nodo);
    if (abs(balance) > 1) return false;
    
    return verificar_avl(nodo->left) && verificar_avl(nodo->right);
}

/**
 * @brief Muestra estadísticas del árbol AVL (altura, balanceo, etc).
 * @param raiz Nodo raíz del árbol
 */
void mostrar_estadisticas_arbol(Tree* raiz) {
    if (raiz == nullptr) {
        cout << "Árbol vacío" << endl;
        return;
    }
    
    cout << "=== ESTADÍSTICAS DEL ÁRBOL AVL ===" << endl;
    cout << "Altura total: " << obtener_altura(raiz) << endl;
    cout << "Factor de balanceo de la raíz: " << factor_balanceo(raiz) << endl;
    cout << "¿Está balanceado?: " << (verificar_avl(raiz) ? "SÍ" : "NO") << endl;
    cout << "===================================" << endl;
}

// Implementación del algoritmo ASCII tree
void print_level(asciinode *node, int x, int level) {
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    if (level == 0) {
        for (i = 0; i < (x - print_next - ((node->lablen - isleft) / 2)); i++) {
            printf(" ");
        }
        print_next += i;
        printf("%s", node->label);
        print_next += node->lablen;
    } else if (node->edge_length >= level) {
        if (node->left != NULL) {
            for (i = 0; i < (x - print_next - (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("/");
            print_next++;
        }
        if (node->right != NULL) {
            for (i = 0; i < (x - print_next + (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("\\");
            print_next++;
        }
    } else {
        print_level(node->left, 
                    x - node->edge_length - 1, 
                    level - node->edge_length - 1);
        print_level(node->right, 
                    x + node->edge_length + 1, 
                    level - node->edge_length - 1);
    }
}

void compute_lprofile(asciinode *node, int x, int y) {
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    lprofile[y] = MIN(lprofile[y], x - ((node->lablen - isleft) / 2));
    if (node->left != NULL) {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
            lprofile[y + i] = MIN(lprofile[y + i], x - i);
        }
    }
    compute_lprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_lprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

void compute_rprofile(asciinode *node, int x, int y) {
    int i, notleft;
    if (node == NULL) return;
    notleft = (node->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x + ((node->lablen - notleft) / 2));
    if (node->right != NULL) {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
            rprofile[y + i] = MAX(rprofile[y + i], x + i);
        }
    }
    compute_rprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_rprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

void compute_edge_lengths(asciinode *node) {
    int h, hmin, i, delta;
    if (node == NULL) return;
    compute_edge_lengths(node->left);
    compute_edge_lengths(node->right);

    if (node->right == NULL && node->left == NULL) {
        node->edge_length = 0;
    } else {
        if (node->left != NULL) {
            for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++) {
                rprofile[i] = -ASCII_INFINITY;
            }
            compute_rprofile(node->left, 0, 0);
            hmin = node->left->height;
        } else {
            hmin = 0;
        }
        if (node->right != NULL) {
            for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++) {
                lprofile[i] = ASCII_INFINITY;
            }
            compute_lprofile(node->right, 0, 0);
            hmin = MIN(node->right->height, hmin);
        } else {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++) {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }

        if (((node->left != NULL && node->left->height == 1) ||
             (node->right != NULL && node->right->height == 1)) && delta > 4) {
            delta--;
        }

        node->edge_length = ((delta + 1) / 2) - 1;
    }

    h = 1;
    if (node->left != NULL) {
        h = MAX(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL) {
        h = MAX(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}

asciinode* build_ascii_tree_recursive(Tree* t) {
    asciinode* node;

    if (t == NULL) return NULL;

    node = (asciinode*)malloc(sizeof(asciinode));
    node->left = build_ascii_tree_recursive(t->left);
    node->right = build_ascii_tree_recursive(t->right);

    if (node->left != NULL) {
        node->left->parent_dir = -1;
    }

    if (node->right != NULL) {
        node->right->parent_dir = 1;
    }

    string etiqueta = crear_etiqueta(t->element);
    strcpy(node->label, etiqueta.c_str());
    node->lablen = etiqueta.length();

    return node;
}

asciinode* build_ascii_tree(Tree* t) {
    asciinode* node;
    if (t == NULL) return NULL;
    node = build_ascii_tree_recursive(t);
    node->parent_dir = 0;
    return node;
}

void free_ascii_tree(asciinode* node) {
    if (node == NULL) return;
    free_ascii_tree(node->left);
    free_ascii_tree(node->right);
    free(node);
}

/**
 * @brief Imprime el árbol AVL en formato ASCII por consola.
 * @param t Nodo raíz del árbol
 */
void imprimir_arbol_ascii(Tree* t) {
    asciinode* proot;
    int xmin, i;
    if (t == NULL) return;
    proot = build_ascii_tree(t);
    compute_edge_lengths(proot);
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
        lprofile[i] = ASCII_INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
        xmin = MIN(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->height; i++) {
        print_next = 0;
        print_level(proot, -xmin, i);
        printf("\n");
    }
    if (proot->height >= MAX_HEIGHT) {
        printf("(Este árbol es demasiado alto para mostrar completamente)\n");
    }
    free_ascii_tree(proot);
}

/**
 * @brief Libera la memoria de un árbol AVL.
 * @param nodo Nodo raíz a liberar
 */
void liberar_arbol(Tree* nodo) {
    if (nodo) {
        liberar_arbol(nodo->left);
        liberar_arbol(nodo->right);
        delete nodo;
    }
}

/**
 * @brief Muestra el menú y visualiza el árbol AVL de clientes según el campo seleccionado.
 * @param banco Referencia al objeto Banco
 */
void mostrar_arbol_binario(Banco& banco) {
    system("cls");
    auto* clientes = banco.getClientes();
    if (!clientes || clientes->esta_vacia()) {
        cout << "No hay clientes registrados en la base de datos.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    ajustar_cursor_para_marquesina();
    const int NUM_CAMPOS = 5;
    const char* CAMPOS[NUM_CAMPOS] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    int campo = seleccionar_opcion("===== SELECCIONAR CAMPO PARA ÁRBOL BINARIO =====", CAMPOS, NUM_CAMPOS, 4);

    // DETENER LA MARQUESINA ANTES DE MOSTRAR EL ÁRBOL
    detener_marquesina();
    
    system("cls");
    cout << "\n===========================================" << endl;
    cout << "===      ÁRBOL AVL DE CLIENTES       ===" << endl;
    
    const char* campos[] = {"", "DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    cout << "===    ORDENADO POR: " << campos[campo] << "    ===" << endl;
    cout << "===========================================" << endl;

    // Crear árbol AVL
    Tree* raiz = nullptr;
    auto& lista = *clientes;
    int n = lista.getTam();
    
    cout << "\nCreando árbol AVL autobalanceado..." << endl;
    
    for (int i = 0; i < n; ++i) {
        Cliente* cliente = lista.get_contador(i);
        string valor = obtener_valor_campo(cliente, campo);
        raiz = insertar_avl(raiz, valor, cliente, campo);
    }

    // Mostrar estadísticas del árbol
    mostrar_estadisticas_arbol(raiz);

    cout << "\nVisualizacion del Arbol AVL (Auto-Balanceado):\n";
    cout << "============================================\n\n";
    
    if (raiz) {
        imprimir_arbol_ascii(raiz);
    } else {
        cout << "Arbol vacio\n";
    }
    
    cout << "\n============================================\n";
    cout << "Total de clientes en el arbol: " << n << endl;
    cout << "Arbol AVL: Todas las operaciones en O(log n)" << endl;
    cout << "Factores de balanceo: siempre entre -1, 0, 1" << endl;
    cout << "Nota: Si los nombres son muy largos, se muestran truncados" << endl;
    cout << "============================================\n";

    // Limpiar memoria
    liberar_arbol(raiz);
    
    cout << "\nPresione cualquier tecla para continuar...";
    getch();
    
    // REINICIAR LA MARQUESINA DESPUÉS DE MOSTRAR EL ÁRBOL
    inicializar_marquesina();
}
