#ifndef LISTA_H
#define LISTA_H

#include <iostream>

template <class T>
struct Nodo {
    T info;          // Información almacenada en el nodo.
    Nodo<T>* sig;    // Puntero al siguiente nodo.
};

template <class T>
// Lista enlazada simple sin uso de excepciones para manejar rangos inválidos.
class Lista {
    Nodo<T>* cab;    // Primer nodo de la lista.
    Nodo<T>* cola;   // Último nodo de la lista.
    int tam;         // Cantidad de elementos.

public:
    // Construye una lista vacía.
    Lista();
    // Libera la memoria de todos los nodos.
    ~Lista();

    // Devuelve la cantidad de elementos almacenados.
    int getTam() const { return tam; }
    // Indica si la lista está vacía.
    bool Lista_Vacia() const;

    // Inserta un nuevo dato al inicio de la lista.
    void insertar_inicio(const T& dato);
    // Inserta un nuevo dato al final de la lista.
    void insertar_final(const T& dato);
    // Inserta un nuevo dato en la posición solicitada (ajustando a los límites).
    void insertar_pos(const T& infoNueva, int pos);

    // Obtiene el dato almacenado en la posición indicada (devuelve T{} si es inválida).
    T obtenerDato(int pos) const;
    // Modifica el contenido de la posición indicada.
    bool modificar(int pos, const T& infoNueva);
    // Elimina el nodo de la posición indicada.
    bool Eliminar(int pos);

        // Recorre e imprime todos los valores almacenados en el flujo indicado.
        void imprimir(std::ostream& out = std::cout) const;
};

template <class T>
Lista<T>::Lista() : cab(nullptr), cola(nullptr), tam(0) {}

template <class T>
Lista<T>::~Lista() {
    Nodo<T>* actual = cab;
    while (actual != nullptr) {
        Nodo<T>* siguiente = actual->sig;
        delete actual;
        actual = siguiente;
    }
    cab = nullptr;
    cola = nullptr;
    tam = 0;
}

template <class T>
bool Lista<T>::Lista_Vacia() const {
    return tam == 0;
}

template <class T>
void Lista<T>::insertar_inicio(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>{dato, cab};
    cab = nuevo;
    if (tam == 0) {
        cola = nuevo;
    }
    ++tam;
}

template <class T>
void Lista<T>::insertar_final(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>{dato, nullptr};
    if (Lista_Vacia()) {
        cab = cola = nuevo;
    } else {
        cola->sig = nuevo;
        cola = nuevo;
    }
    ++tam;
}

template <class T>
void Lista<T>::insertar_pos(const T& infoNueva, int pos) {
    if (pos <= 0 || Lista_Vacia()) {
        insertar_inicio(infoNueva);
        return;
    }
    if (pos >= tam) {
        insertar_final(infoNueva);
        return;
    }

    Nodo<T>* anterior = cab;
    for (int i = 0; i < pos - 1; ++i) {
        anterior = anterior->sig;
    }

    Nodo<T>* nuevo = new Nodo<T>{infoNueva, anterior->sig};
    anterior->sig = nuevo;
    ++tam;
}

template <class T>
T Lista<T>::obtenerDato(int pos) const {
    if (pos < 0 || pos >= tam) {
        return T{};
    }
    Nodo<T>* actual = cab;
    for (int i = 0; i < pos; ++i) {
        actual = actual->sig;
    }
    return (actual != nullptr) ? actual->info : T{};
}

template <class T>
bool Lista<T>::modificar(int pos, const T& infoNueva) {
    if (pos < 0 || pos >= tam) {
        return false;
    }
    Nodo<T>* actual = cab;
    for (int i = 0; i < pos; ++i) {
        actual = actual->sig;
    }
    if (actual == nullptr) {
        return false;
    }
    actual->info = infoNueva;
    return true;
}

template <class T>
bool Lista<T>::Eliminar(int pos) {
    if (pos < 0 || pos >= tam || Lista_Vacia()) {
        return false;
    }

    Nodo<T>* eliminado = nullptr;
    if (pos == 0) {
        eliminado = cab;
        cab = cab->sig;
        if (cab == nullptr) {
            cola = nullptr;
        }
    } else {
        Nodo<T>* anterior = cab;
        for (int i = 0; i < pos - 1; ++i) {
            anterior = anterior->sig;
        }
        eliminado = anterior->sig;
        anterior->sig = eliminado->sig;
        if (anterior->sig == nullptr) {
            cola = anterior;
        }
    }

    delete eliminado;
    --tam;
    return true;
}

template <class T>
void Lista<T>::imprimir(std::ostream& out) const {
    Nodo<T>* actual = cab;
    while (actual != nullptr) {
        out << actual->info;
        if (actual->sig != nullptr) {
            out << " ";
        }
        actual = actual->sig;
    }
    out << std::endl;
}

#endif // LISTA_H
