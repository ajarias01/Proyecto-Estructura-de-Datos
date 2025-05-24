#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

template <typename T>
class Nodo {
public:
    Nodo(T _valor, Nodo* _siguiente, Nodo* _anterior);
    Nodo(T _valor, Nodo* _siguiente);
    Nodo();
    ~Nodo();
    T get_valor();
    void set_valor(T new_valor);
    Nodo<T>* get_siguiente();
    void set_siguiente(Nodo<T>* new_siguiente);
    Nodo<T>* get_anterior();
    void set_anterior(Nodo<T>* new_anterior);
private:
    T valor;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;
};

#endif