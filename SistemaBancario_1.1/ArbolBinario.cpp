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
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>

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

// Enum para los colores del árbol Rojo-Negro
enum Color { RED, BLACK };

/**
 * @class Tree
 * @brief Nodo de un árbol Rojo-Negro de clientes.
 *
 * Esta clase representa un nodo de un árbol Rojo-Negro, utilizado para almacenar y organizar
 * clientes según diferentes campos (DNI, nombre, apellido, teléfono o email).
 * Cada nodo contiene un puntero al cliente, el valor del campo por el que se ordena,
 * punteros a los hijos izquierdo y derecho, y el color para el balanceo Rojo-Negro.
 */
class Tree {
public:
    Tree* left, *right; ///< Punteros a los hijos izquierdo y derecho
    Tree* parent;       ///< Puntero al nodo padre
    string element;     ///< Valor del campo por el que se ordena
    Cliente* cliente;   ///< Puntero al cliente asociado
    Color color;        ///< Color del nodo (RED o BLACK)
    /**
     * @brief Constructor de Tree
     * @param elem Valor del campo por el que se ordena
     * @param cli Puntero al cliente asociado
     */
    Tree(string elem, Cliente* cli) : element(elem), cliente(cli), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
};

// Función para obtener el valor a mostrar según el campo
/**
 * @brief Obtiene el valor de un campo específico de un cliente.
 * @param cliente Puntero al cliente
 * @param campo Índice del campo (0: DNI, 1: Nombre, 2: Apellido, 3: Teléfono, 4: Email)
 * @return Valor del campo como string
 */
string obtener_valor_campo(Cliente* cliente, int campo) {
    switch(campo) {
        case 0: // DNI completo
            return cliente->get_dni();
        case 1: // Nombre completo
            return cliente->get_nombres();
        case 2: // Apellido completo
            return cliente->get_apellidos();
        case 3: // Teléfono completo
            return cliente->get_telefono();
        case 4: // Email (hasta @)
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
    if (campo == 0 || campo == 3) { // DNI o Teléfono (numérico)
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

// FUNCIONES PARA ÁRBOL ROJO-NEGRO

// Nodo NIL para el árbol Rojo-Negro
Tree* NIL = nullptr;

/**
 * @brief Inicializa el nodo NIL para el árbol Rojo-Negro.
 */
void inicializar_nil() {
    if (NIL == nullptr) {
        NIL = new Tree("", nullptr);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = nullptr;
    }
}

/**
 * @brief Obtiene el color de un nodo.
 * @param nodo Nodo a consultar
 * @return Color del nodo (BLACK si es nulo)
 */
Color obtener_color(Tree* nodo) {
    if (nodo == nullptr || nodo == NIL) return BLACK;
    return nodo->color;
}

/**
 * @brief Establece el color de un nodo.
 * @param nodo Nodo a modificar
 * @param color Color a establecer
 */
void establecer_color(Tree* nodo, Color color) {
    if (nodo != nullptr && nodo != NIL) {
        nodo->color = color;
    }
}

/**
 * @brief Realiza una rotación a la izquierda en un subárbol Rojo-Negro.
 * @param raiz Referencia a la raíz del árbol
 * @param x Nodo sobre el que realizar la rotación
 */
void rotacion_izquierda_rb(Tree*& raiz, Tree* x) {
    Tree* y = x->right;
    x->right = y->left;
    
    if (y->left != NIL && y->left != nullptr) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        raiz = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

/**
 * @brief Realiza una rotación a la derecha en un subárbol Rojo-Negro.
 * @param raiz Referencia a la raíz del árbol
 * @param y Nodo sobre el que realizar la rotación
 */
void rotacion_derecha_rb(Tree*& raiz, Tree* y) {
    Tree* x = y->left;
    y->left = x->right;
    
    if (x->right != NIL && x->right != nullptr) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == nullptr) {
        raiz = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

/**
 * @brief Corrige las violaciones de las propiedades Rojo-Negro después de la inserción.
 * @param raiz Referencia a la raíz del árbol
 * @param z Nodo recién insertado
 */
void arreglar_insercion_rb(Tree*& raiz, Tree* z) {
    while (z != raiz && obtener_color(z->parent) == RED) {
        if (z->parent == z->parent->parent->left) {
            Tree* y = z->parent->parent->right; // Tío
            
            if (obtener_color(y) == RED) {
                // Caso 1: El tío es rojo
                establecer_color(z->parent, BLACK);
                establecer_color(y, BLACK);
                establecer_color(z->parent->parent, RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Caso 2: El tío es negro y z es hijo derecho
                    z = z->parent;
                    rotacion_izquierda_rb(raiz, z);
                }
                // Caso 3: El tío es negro y z es hijo izquierdo
                establecer_color(z->parent, BLACK);
                establecer_color(z->parent->parent, RED);
                rotacion_derecha_rb(raiz, z->parent->parent);
            }
        } else {
            Tree* y = z->parent->parent->left; // Tío
            
            if (obtener_color(y) == RED) {
                // Caso 1: El tío es rojo
                establecer_color(z->parent, BLACK);
                establecer_color(y, BLACK);
                establecer_color(z->parent->parent, RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // Caso 2: El tío es negro y z es hijo izquierdo
                    z = z->parent;
                    rotacion_derecha_rb(raiz, z);
                }
                // Caso 3: El tío es negro y z es hijo derecho
                establecer_color(z->parent, BLACK);
                establecer_color(z->parent->parent, RED);
                rotacion_izquierda_rb(raiz, z->parent->parent);
            }
        }
    }
    establecer_color(raiz, BLACK);
}

/**
 * @brief Inserta un nodo en un árbol Rojo-Negro, manteniendo las propiedades.
 * @param raiz Referencia a la raíz del árbol
 * @param valor Valor a insertar
 * @param cliente Puntero al cliente
 * @param campo Campo por el que se ordena
 */
void insertar_rb(Tree*& raiz, string valor, Cliente* cliente, int campo) {
    inicializar_nil();
    
    Tree* z = new Tree(valor, cliente);
    z->left = z->right = NIL;
    
    Tree* y = nullptr;
    Tree* x = raiz;
    
    // Búsqueda del lugar de inserción
    while (x != nullptr && x != NIL) {
        y = x;
        
        string valor_x = obtener_valor_campo(x->cliente, campo);
        bool menor = false;
        
        if (campo == 0 || campo == 3) { // DNI o Teléfono (numérico)
            menor = stoi(valor) < stoi(valor_x);
        } else { // Texto (nombres, apellidos, email)
            string val_lower = valor, x_lower = valor_x;
            transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
            transform(x_lower.begin(), x_lower.end(), x_lower.begin(), ::tolower);
            menor = val_lower < x_lower;
        }
        
        if (menor) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    z->parent = y;
    
    if (y == nullptr) {
        raiz = z;
    } else {
        string valor_y = obtener_valor_campo(y->cliente, campo);
        bool menor = false;
        
        if (campo == 0 || campo == 3) {
            menor = stoi(valor) < stoi(valor_y);
        } else {
            string val_lower = valor, y_lower = valor_y;
            transform(val_lower.begin(), val_lower.end(), val_lower.begin(), ::tolower);
            transform(y_lower.begin(), y_lower.end(), y_lower.begin(), ::tolower);
            menor = val_lower < y_lower;
        }
        
        if (menor) {
            y->left = z;
        } else {
            y->right = z;
        }
    }
    
    z->color = RED;
    arreglar_insercion_rb(raiz, z);
}

/**
 * @brief Calcula la altura de un árbol Rojo-Negro.
 * @param nodo Nodo raíz
 * @return Altura del árbol
 */
int calcular_altura_rb(Tree* nodo) {
    if (nodo == nullptr || nodo == NIL) return 0;
    return 1 + MAX(calcular_altura_rb(nodo->left), calcular_altura_rb(nodo->right));
}

/**
 * @brief Cuenta los nodos negros en un camino desde la raíz hasta una hoja.
 * @param nodo Nodo actual
 * @return Número de nodos negros
 */
int contar_nodos_negros(Tree* nodo) {
    if (nodo == nullptr || nodo == NIL) return 1;
    
    int left_count = contar_nodos_negros(nodo->left);
    if (left_count == -1) return -1;
    
    int right_count = contar_nodos_negros(nodo->right);
    if (right_count == -1) return -1;
    
    if (left_count != right_count) return -1;
    
    return left_count + (nodo->color == BLACK ? 1 : 0);
}

/**
 * @brief Verifica si un árbol cumple las propiedades Rojo-Negro.
 * @param nodo Nodo raíz
 * @return true si es válido, false en caso contrario
 */
bool verificar_rb(Tree* nodo) {
    if (nodo == nullptr) return true;
    
    // Propiedad 1: La raíz debe ser negra
    if (nodo->color != BLACK) return false;
    
    // Propiedad 2: No puede haber dos nodos rojos consecutivos
    if (nodo->color == RED) {
        if ((nodo->left != NIL && nodo->left != nullptr && nodo->left->color == RED) ||
            (nodo->right != NIL && nodo->right != nullptr && nodo->right->color == RED)) {
            return false;
        }
    }
    
    // Verificar recursivamente
    return verificar_rb(nodo->left) && verificar_rb(nodo->right) && (contar_nodos_negros(nodo) != -1);
}

/**
 * @brief Muestra estadísticas del árbol Rojo-Negro.
 * @param raiz Nodo raíz del árbol
 */
void mostrar_estadisticas_arbol_rb(Tree* raiz) {
    if (raiz == nullptr) {
        cout << "Árbol vacío" << endl;
        return;
    }
    
    cout << "=== ESTADÍSTICAS DEL ÁRBOL ROJO-NEGRO ===" << endl;
    cout << "Altura total: " << calcular_altura_rb(raiz) << endl;
    cout << "Color de la raíz: " << (raiz->color == BLACK ? "NEGRO" : "ROJO") << endl;
    cout << "Altura negra: " << contar_nodos_negros(raiz) << endl;
    cout << "¿Cumple propiedades RB?: " << (verificar_rb(raiz) ? "SÍ" : "NO") << endl;
    cout << "=========================================" << endl;
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

    if (t == NULL || t == NIL) return NULL;

    node = (asciinode*)malloc(sizeof(asciinode));
    node->left = build_ascii_tree_recursive(t->left);
    node->right = build_ascii_tree_recursive(t->right);

    if (node->left != NULL) {
        node->left->parent_dir = -1;
    }

    if (node->right != NULL) {
        node->right->parent_dir = 1;
    }

    // Crear etiqueta con color para Rojo-Negro
    string etiqueta = crear_etiqueta(t->element);
    if (t->color == RED) {
        etiqueta = "R:" + etiqueta; // Prefijo R para rojo
    } else {
        etiqueta = "B:" + etiqueta; // Prefijo B para negro
    }
    
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
 * @brief Imprime el árbol Rojo-Negro en formato ASCII por consola.
 * @param t Nodo raíz del árbol
 */
void imprimir_arbol_ascii(Tree* t) {
    asciinode* proot;
    int xmin, i;
    if (t == NULL || t == NIL) return;
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
 * @brief Libera la memoria de un árbol Rojo-Negro.
 * @param nodo Nodo raíz a liberar
 */
void liberar_arbol(Tree* nodo) {
    if (nodo && nodo != NIL) {
        liberar_arbol(nodo->left);
        liberar_arbol(nodo->right);
        delete nodo;
    }
}

/**
 * @brief Muestra el menú y visualiza el árbol Rojo-Negro de clientes según el campo seleccionado.
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
    cout << "===    ÁRBOL ROJO-NEGRO DE CLIENTES  ===" << endl;
    
    const char* campos[] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    cout << "===    ORDENADO POR: " << campos[campo] << "    ===" << endl;
    cout << "===========================================" << endl;

    // Inicializar NIL para el árbol Rojo-Negro
    inicializar_nil();
    
    // Crear árbol Rojo-Negro
    Tree* raiz = nullptr;
    auto& lista = *clientes;
    int n = lista.getTam();
    
    cout << "\nCreando árbol Rojo-Negro autobalanceado..." << endl;
    
    for (int i = 0; i < n; ++i) {
        Cliente* cliente = lista.get_contador(i);
        string valor = obtener_valor_campo(cliente, campo);
        insertar_rb(raiz, valor, cliente, campo);
    }

    // Mostrar estadísticas del árbol
    mostrar_estadisticas_arbol_rb(raiz);

    cout << "\nVisualizacion del Arbol Rojo-Negro (Auto-Balanceado):\n";
    cout << "===================================================\n";
    cout << "Leyenda: R: = Nodo Rojo, B: = Nodo Negro\n";
    cout << "===================================================\n\n";
    
    if (raiz) {
        imprimir_arbol_ascii(raiz);
    } else {
        cout << "Arbol vacio\n";
    }
    
    cout << "\n===================================================\n";
    cout << "Total de clientes en el arbol: " << n << endl;
    cout << "Arbol Rojo-Negro: Todas las operaciones en O(log n)" << endl;
    cout << "Propiedades: Altura balanceada, sin nodos rojos consecutivos" << endl;
    cout << "Nota: Si los nombres son muy largos, se muestran truncados" << endl;
    cout << "===================================================\n";

    // Limpiar memoria
    liberar_arbol(raiz);
    
    cout << "\nPresione cualquier tecla para continuar...";
    getch();
    
    // REINICIAR LA MARQUESINA DESPUÉS DE MOSTRAR EL ÁRBOL
    inicializar_marquesina();
}

// =================== IMPLEMENTACIÓN SFML ===================

/**
 * @brief Estructura para manejar la posición y datos de un nodo en la visualización SFML.
 */
struct NodoSFML {
    float x, y;           // Posición en pantalla
    Tree* nodo;           // Puntero al nodo del árbol
    string etiqueta;      // Texto a mostrar
    bool es_rojo;         // Color del nodo
    
    NodoSFML(float _x, float _y, Tree* _nodo, const string& _etiqueta, bool _es_rojo) 
        : x(_x), y(_y), nodo(_nodo), etiqueta(_etiqueta), es_rojo(_es_rojo) {}
};

/**
 * @brief Calcula las posiciones de los nodos para la visualización SFML.
 * @param nodo Nodo actual del árbol
 * @param x Posición X actual
 * @param y Posición Y actual
 * @param espaciado Espaciado horizontal entre nodos
 * @param nivel_altura Altura entre niveles
 * @param nodos_sfml Vector para almacenar los nodos con sus posiciones
 * @param campo Campo por el que está ordenado el árbol
 */
void calcular_posiciones_sfml(Tree* nodo, float x, float y, float espaciado, float nivel_altura, 
                             vector<NodoSFML>& nodos_sfml, int campo) {
    if (!nodo || nodo == NIL) return;
    
    // Crear etiqueta del nodo
    string etiqueta = crear_etiqueta(obtener_valor_campo(nodo->cliente, campo));
    bool es_rojo = (nodo->color == RED);
    
    // Agregar nodo actual
    nodos_sfml.push_back(NodoSFML(x, y, nodo, etiqueta, es_rojo));
    
    // Calcular posiciones de los hijos con mejor espaciado
    float nuevo_espaciado = espaciado * 0.5f; // Reducir más el espaciado para acomodar mejor
    
    if (nodo->left && nodo->left != NIL) {
        calcular_posiciones_sfml(nodo->left, x - espaciado, y + nivel_altura, 
                                nuevo_espaciado, nivel_altura, nodos_sfml, campo);
    }
    
    if (nodo->right && nodo->right != NIL) {
        calcular_posiciones_sfml(nodo->right, x + espaciado, y + nivel_altura, 
                                nuevo_espaciado, nivel_altura, nodos_sfml, campo);
    }
}

/**
 * @brief Dibuja las conexiones entre nodos padre e hijo.
 * @param window Ventana de SFML
 * @param nodos_sfml Vector de nodos con posiciones
 */
void dibujar_conexiones_sfml(sf::RenderWindow& window, const vector<NodoSFML>& nodos_sfml) {
    for (const auto& nodo_sfml : nodos_sfml) {
        if (!nodo_sfml.nodo || nodo_sfml.nodo == NIL) continue;
        
        // Buscar hijos y dibujar líneas
        for (const auto& hijo_sfml : nodos_sfml) {
            if (!hijo_sfml.nodo || hijo_sfml.nodo == NIL) continue;
            
            // Verificar si es hijo izquierdo o derecho
            if (nodo_sfml.nodo->left == hijo_sfml.nodo || nodo_sfml.nodo->right == hijo_sfml.nodo) {
                sf::Vertex linea[2];
                linea[0].position = sf::Vector2f(nodo_sfml.x, nodo_sfml.y);
                linea[0].color = sf::Color::White;
                linea[1].position = sf::Vector2f(hijo_sfml.x, hijo_sfml.y);
                linea[1].color = sf::Color::White;
                window.draw(linea, 2, sf::PrimitiveType::Lines);
            }
        }
    }
}

/**
 * @brief Dibuja los nodos del árbol con círculos coloreados y texto.
 * @param window Ventana de SFML
 * @param nodos_sfml Vector de nodos con posiciones
 * @param font Fuente para el texto
 */
void dibujar_nodos_sfml(sf::RenderWindow& window, const vector<NodoSFML>& nodos_sfml, sf::Font& font) {
    const float radio_nodo = 25.0f;
    
    for (const auto& nodo_sfml : nodos_sfml) {
        // Crear círculo para el nodo
        sf::CircleShape circulo(radio_nodo);
        circulo.setPosition(sf::Vector2f(nodo_sfml.x - radio_nodo, nodo_sfml.y - radio_nodo));
        
        // Color según el tipo de nodo (Rojo-Negro)
        if (nodo_sfml.es_rojo) {
            circulo.setFillColor(sf::Color::Red);
            circulo.setOutlineColor(sf::Color::White);
        } else {
            circulo.setFillColor(sf::Color::Black);
            circulo.setOutlineColor(sf::Color::White);
        }
        circulo.setOutlineThickness(2.0f);
        
        window.draw(circulo);
        
        // Crear texto para el nodo
        sf::Text texto(font, nodo_sfml.etiqueta, 12);
        texto.setFillColor(nodo_sfml.es_rojo ? sf::Color::White : sf::Color::White);
        
        // Centrar texto en el círculo
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setPosition(sf::Vector2f(nodo_sfml.x - bounds.size.x / 2, nodo_sfml.y - bounds.size.y / 2));
        
        window.draw(texto);
    }
}

/**
 * @brief Muestra información adicional del árbol en la ventana SFML.
 * @param window Ventana de SFML
 * @param font Fuente para el texto
 * @param raiz Raíz del árbol
 * @param campo Campo por el que está ordenado
 * @param total_nodos Total de nodos en el árbol
 */
void dibujar_informacion_arbol_sfml(sf::RenderWindow& window, sf::Font& font, Tree* raiz, int campo, int total_nodos) {
    const char* campos[] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    
    // Título principal
    sf::Text titulo(font, "ARBOL ROJO-NEGRO DE CLIENTES", 24);
    titulo.setFillColor(sf::Color::Yellow);
    titulo.setPosition(sf::Vector2f(10, 10));
    window.draw(titulo);
    
    // Campo de ordenamiento
    sf::Text campo_texto(font, "Ordenado por: " + string(campos[campo]), 16);
    campo_texto.setFillColor(sf::Color::Cyan);
    campo_texto.setPosition(sf::Vector2f(10, 40));
    window.draw(campo_texto);
    
    // Estadísticas
    string stats = "Total de nodos: " + to_string(total_nodos) + 
                  "\nAltura: " + to_string(calcular_altura_rb(raiz)) +
                  "\nAltura negra: " + to_string(contar_nodos_negros(raiz)) +
                  "\nPropiedades RB: " + (verificar_rb(raiz) ? "VALIDAS" : "INVALIDAS");
    sf::Text estadisticas(font, stats, 14);
    estadisticas.setFillColor(sf::Color::White);
    estadisticas.setPosition(sf::Vector2f(10, 70));
    window.draw(estadisticas);
    
    // Leyenda
    sf::Text leyenda(font, "LEYENDA:\nCirculos Rojos = Nodos Rojos\nCirculos Negros = Nodos Negros\n\nControles:\nESC - Salir\nSPACE - Ver en consola\nR - Regenerar\nFlechas - Mover vista\nRueda - Zoom", 12);
    leyenda.setFillColor(sf::Color::Green);
    leyenda.setPosition(sf::Vector2f(10, static_cast<float>(window.getSize().y) - 140));
    window.draw(leyenda);
}

/**
 * @brief Muestra el árbol Rojo-Negro de clientes con visualización SFML gráfica.
 * @param banco Referencia al objeto Banco
 */
void mostrar_arbol_binario_sfml(Banco& banco) {
    system("cls");
    auto* clientes = banco.getClientes();
    if (!clientes || clientes->esta_vacia()) {
        cout << "No hay clientes registrados en la base de datos.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Selección del campo
    ajustar_cursor_para_marquesina();
    const int NUM_CAMPOS = 5;
    const char* CAMPOS[NUM_CAMPOS] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    int campo = seleccionar_opcion("===== SELECCIONAR CAMPO PARA ÁRBOL BINARIO GRÁFICO =====", CAMPOS, NUM_CAMPOS, 4);

    // Detener marquesina
    detener_marquesina();
    
    // Inicializar NIL y crear árbol
    inicializar_nil();
    Tree* raiz = nullptr;
    auto& lista = *clientes;
    int n = lista.getTam();
    
    cout << "\nCreando árbol Rojo-Negro para visualización gráfica..." << endl;
    
    for (int i = 0; i < n; ++i) {
        Cliente* cliente = lista.get_contador(i);
        string valor = obtener_valor_campo(cliente, campo);
        insertar_rb(raiz, valor, cliente, campo);
    }

    // Configurar ventana SFML con mayor tamaño
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1400, 900)), "Arbol Rojo-Negro - Sistema Bancario");
    window.setFramerateLimit(60);
    
    // Cargar fuente (usar fuente del sistema)
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Si no se puede cargar arial, intentar con otra fuente del sistema
        if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf")) {
            cout << "Error: No se pudo cargar la fuente. Usando fuente por defecto." << endl;
            // Continuar sin fuente personalizada
        }
    }
    
    // Calcular posiciones de los nodos con mejor distribución
    vector<NodoSFML> nodos_sfml;
    if (raiz) {
        float centro_x = window.getSize().x / 2.0f;
        float inicio_y = 200.0f; // Más espacio para la información superior
        float espaciado_inicial = min(400.0f, (window.getSize().x - 100.0f) / 4.0f); // Ajustar según ventana
        float altura_nivel = 70.0f; // Reducir altura entre niveles
        
        calcular_posiciones_sfml(raiz, centro_x, inicio_y, espaciado_inicial, altura_nivel, nodos_sfml, campo);
    }
    
    cout << "Ventana gráfica abierta. Controles disponibles:" << endl;
    cout << "- ESC: Cerrar ventana" << endl;
    cout << "- SPACE: Mostrar versión ASCII en consola" << endl;
    cout << "- R: Regenerar árbol" << endl;
    cout << "- Clic derecho: Información del nodo" << endl;
    cout << "- Rueda del ratón: Zoom in/out" << endl;
    cout << "- Flechas: Mover vista" << endl;
    
    // Variables para zoom y desplazamiento
    sf::View vista = window.getDefaultView();
    float zoom_factor = 1.0f;
    sf::Vector2f offset(0, 0);
    
    // Loop principal de SFML
    while (window.isOpen()) {
        std::optional<sf::Event> eventOpt;
        while ((eventOpt = window.pollEvent()).has_value()) {
            const sf::Event& event = eventOpt.value();
            
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Escape:
                        window.close();
                        break;
                        
                    case sf::Keyboard::Key::Space:
                        // Mostrar versión ASCII en consola
                        system("cls");
                        cout << "\n=== VERSIÓN ASCII DEL MISMO ÁRBOL ===" << endl;
                        mostrar_estadisticas_arbol_rb(raiz);
                        if (raiz) {
                            imprimir_arbol_ascii(raiz);
                        }
                        cout << "\nPresione cualquier tecla para volver a la ventana gráfica...";
                        getch();
                        break;
                        
                    case sf::Keyboard::Key::R:
                        // Regenerar árbol (útil para debugging)
                        cout << "Regenerando árbol..." << endl;
                        liberar_arbol(raiz);
                        raiz = nullptr;
                        nodos_sfml.clear();
                        
                        // Recrear árbol
                        for (int i = 0; i < n; ++i) {
                            Cliente* cliente = lista.get_contador(i);
                            string valor = obtener_valor_campo(cliente, campo);
                            insertar_rb(raiz, valor, cliente, campo);
                        }
                        
                        // Recalcular posiciones
                        if (raiz) {
                            float centro_x = window.getSize().x / 2.0f;
                            float inicio_y = 200.0f;
                            float espaciado_inicial = min(400.0f, (window.getSize().x - 100.0f) / 4.0f);
                            float altura_nivel = 70.0f;
                            calcular_posiciones_sfml(raiz, centro_x, inicio_y, espaciado_inicial, altura_nivel, nodos_sfml, campo);
                        }
                        break;
                        
                    case sf::Keyboard::Key::Up:
                        offset.y -= 20;
                        break;
                    case sf::Keyboard::Key::Down:
                        offset.y += 20;
                        break;
                    case sf::Keyboard::Key::Left:
                        offset.x -= 20;
                        break;
                    case sf::Keyboard::Key::Right:
                        offset.x += 20;
                        break;
                }
            }
            
            if (const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
                if (wheelScrolled->delta > 0) {
                    zoom_factor *= 0.9f; // Zoom in
                } else {
                    zoom_factor *= 1.1f; // Zoom out
                }
                zoom_factor = max(0.3f, min(zoom_factor, 3.0f)); // Limitar zoom
            }
            
            if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Right) {
                    // Detectar clic en nodo para mostrar información
                    float mouse_x = static_cast<float>(mousePressed->position.x);
                    float mouse_y = static_cast<float>(mousePressed->position.y);
                    
                    for (const auto& nodo_sfml : nodos_sfml) {
                        float dist = sqrt(pow(mouse_x - nodo_sfml.x, 2) + pow(mouse_y - nodo_sfml.y, 2));
                        if (dist <= 25.0f) { // Radio del nodo
                            // Mostrar información del cliente en consola
                            cout << "\n=== INFORMACIÓN DEL NODO ===" << endl;
                            cout << "Valor: " << nodo_sfml.etiqueta << endl;
                            cout << "Color: " << (nodo_sfml.es_rojo ? "ROJO" : "NEGRO") << endl;
                            if (nodo_sfml.nodo && nodo_sfml.nodo->cliente) {
                                Cliente* cliente = nodo_sfml.nodo->cliente;
                                cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << endl;
                                cout << "DNI: " << cliente->get_dni() << endl;
                                cout << "Email: " << cliente->get_email() << endl;
                            }
                            cout << "===========================" << endl;
                            break;
                        }
                    }
                }
            }
        }
        
        // Actualizar vista con zoom y desplazamiento
        vista.setSize(window.getDefaultView().getSize() * zoom_factor);
        vista.setCenter(window.getDefaultView().getCenter() + offset);
        window.setView(vista);
        
        // Limpiar ventana
        window.clear(sf::Color(30, 30, 30)); // Fondo gris oscuro
        
        // Dibujar conexiones primero (para que queden detrás de los nodos)
        dibujar_conexiones_sfml(window, nodos_sfml);
        
        // Dibujar nodos
        dibujar_nodos_sfml(window, nodos_sfml, font);
        
        // Restaurar vista para información de interfaz
        window.setView(window.getDefaultView());
        
        // Dibujar información del árbol (siempre visible)
        dibujar_informacion_arbol_sfml(window, font, raiz, campo, n);
        
        // Mostrar todo en pantalla
        window.display();
    }
    
    // Limpiar memoria
    liberar_arbol(raiz);
    
    cout << "\nVentana gráfica cerrada. Regresando al menú..." << endl;
    
    // Reiniciar marquesina
    inicializar_marquesina();
}

