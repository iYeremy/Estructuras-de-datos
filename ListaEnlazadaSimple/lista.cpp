#include <array>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <stdexcept>

#include "lista.h"

// ---------- Utilidades de formateo ----------

template <typename T>
std::string formatearValor(const T& valor) {
    std::ostringstream ss;
    ss << valor;
    return ss.str();
}

template <>
std::string formatearValor<char>(const char& valor) {
    if (valor == '\0') {
        return "\\0";
    }
    return std::string(1, valor);
}

void limpiarRestante() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ---------- Vista rápida de una lista ----------

template <typename T>
std::string vistaLinea(const Lista<T>& lista) {
    if (lista.Lista_Vacia()) {
        return "(vacia)";
    }
    std::ostringstream ss;
    lista.imprimir(ss);
    std::string resultado = ss.str();
    if (!resultado.empty() && resultado.back() == '\n') {
        resultado.pop_back();
    }
    return resultado;
}

// ---------- Configuración de escenarios ----------

template <typename T>
struct EscenarioConfig {
    std::array<T, 10> valores;
    int posicionObjetivo;
    T valorModificado;
    bool eliminarInvalida;
    T valorMedio;
    T valorInicio;
    T valorFinal;
};

template <typename T>
EscenarioConfig<T> obtenerConfigPorDefecto();

template <>
EscenarioConfig<int> obtenerConfigPorDefecto<int>() {
    return EscenarioConfig<int>{
        {10, 20, 30, 5, 4, 3, 15, 25, 35, 45},
        3,
        999,
        true,
        111,
        555,
        777};
}

template <>
EscenarioConfig<double> obtenerConfigPorDefecto<double>() {
    return EscenarioConfig<double>{
        {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1},
        2,
        42.42,
        false,
        77.77,
        0.01,
        12.34};
}

template <>
EscenarioConfig<char> obtenerConfigPorDefecto<char>() {
    return EscenarioConfig<char>{
        {'a', 'b', 'c', 'x', 'y', 'z', 'd', 'e', 'f', 'g'},
        4,
        'Q',
        false,
        'H',
        'M',
        'Z'};
}

template <typename T>
struct EscenarioResultado {
    std::string registroPreparacion;
    std::string registroOperaciones;
    std::string estadoFinal;
    int tamFinal{};
};

template <typename T>
struct EstadoEscenario {
    std::string nombre;
    EscenarioConfig<T> config;
    EscenarioConfig<T> configDefault;
    EscenarioResultado<T> resultado;
    bool necesitaActualizar{true};
};

// ---------- Funciones que ejecutan el flujo ----------

template <typename T>
std::string llenarLista(const std::string& nombreLista,
                        Lista<T>& lista,
                        const std::array<T, 10>& valores) {
    std::ostringstream log;

    log << "\n--- Preparando " << nombreLista << " ---" << std::endl;
    log << "Insertando 3 elementos al final:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        lista.insertar_final(valores[i]);
        log << "  final += " << formatearValor(valores[i]) << " -> "
            << vistaLinea(lista) << std::endl;
    }

    log << "Insertando 3 elementos al inicio:" << std::endl;
    for (int i = 3; i < 6; ++i) {
        lista.insertar_inicio(valores[i]);
        log << "  inicio += " << formatearValor(valores[i]) << " -> "
            << vistaLinea(lista) << std::endl;
    }

    log << "Insertando 4 elementos en posiciones intermedias:" << std::endl;
    for (int i = 6; i < 10; ++i) {
        int tamActual = lista.getTam();
        int pos = tamActual / 2;
        try {
            lista.insertar_pos(valores[i], pos);
            log << "  pos(" << pos << ") += " << formatearValor(valores[i]) << " -> "
                << vistaLinea(lista) << std::endl;
        } catch (const std::out_of_range& e) {
            log << "  pos(" << pos << ") += " << formatearValor(valores[i])
                << " fallo: " << e.what() << std::endl;
        }
    }

    log << "Resultado inicial de " << nombreLista << ": " << vistaLinea(lista) << std::endl;
    return log.str();
}

template <typename T>
std::string procesarLista(const std::string& nombreLista,
                          Lista<T>& lista,
                          const EscenarioConfig<T>& cfg) {
    std::ostringstream log;

    log << "\n=== " << nombreLista << " ===" << std::endl;
    log << "Lista inicial: " << vistaLinea(lista) << std::endl;
    log << "Tamano inicial: " << lista.getTam() << std::endl;
    log << "¿Lista vacia?: " << (lista.Lista_Vacia() ? "si" : "no") << std::endl;

    if (!lista.Lista_Vacia()) {
        log << "Primer elemento: " << formatearValor(lista.obtenerDato(0)) << std::endl;
        log << "Ultimo elemento: "
            << formatearValor(lista.obtenerDato(lista.getTam() - 1)) << std::endl;
    }

    int posFuera = lista.getTam();
    try {
        T lecturaFuera = lista.obtenerDato(posFuera);
        log << "Dato en posicion " << posFuera
            << " (fuera de rango, regresa valor por defecto): "
            << formatearValor(lecturaFuera) << std::endl;
    } catch (const std::out_of_range& e) {
        log << "Dato en posicion " << posFuera
            << " (fuera de rango, lanza excepcion): " << e.what() << std::endl;
    }

    if (cfg.posicionObjetivo >= 0 && cfg.posicionObjetivo < lista.getTam()) {
        log << "Dato en posicion " << cfg.posicionObjetivo << ": "
            << formatearValor(lista.obtenerDato(cfg.posicionObjetivo)) << std::endl;
    } else {
        log << "Posicion " << cfg.posicionObjetivo
            << " no valida para obtener dato." << std::endl;
    }

    if (cfg.eliminarInvalida) {
        int posInvalida = lista.getTam() + 5;
        bool eliminadoInvalido = lista.Eliminar(posInvalida);
        log << "Intento eliminar posicion invalida " << posInvalida
            << (eliminadoInvalido ? " (exitoso)" : " (fallido)") << std::endl;
    }

    bool eliminado = lista.Eliminar(cfg.posicionObjetivo);
    log << "Eliminar posicion " << cfg.posicionObjetivo
        << (eliminado ? " (exitoso)" : " (fallido)") << std::endl;

    if (cfg.posicionObjetivo >= 0 && cfg.posicionObjetivo < lista.getTam()) {
        log << "Dato tras eliminar en posicion " << cfg.posicionObjetivo
            << ": " << formatearValor(lista.obtenerDato(cfg.posicionObjetivo)) << std::endl;
    } else {
        log << "Posicion " << cfg.posicionObjetivo
            << " quedo fuera de rango tras eliminar." << std::endl;
    }

    bool modificado = lista.modificar(cfg.posicionObjetivo, cfg.valorModificado);
    log << "Modificar posicion " << cfg.posicionObjetivo
        << (modificado ? " (exitoso)" : " (fallido)") << std::endl;

    int posModInvalida = lista.getTam();
    bool modInvalido = lista.modificar(posModInvalida, cfg.valorModificado);
    log << "Modificar posicion invalida " << posModInvalida
        << (modInvalido ? " (exitoso)" : " (fallido)") << std::endl;

    if (modificado) {
        if (cfg.posicionObjetivo >= 0 && cfg.posicionObjetivo < lista.getTam()) {
            log << "Dato modificado en posicion " << cfg.posicionObjetivo
                << ": " << formatearValor(lista.obtenerDato(cfg.posicionObjetivo)) << std::endl;
        } else {
            log << "Posicion " << cfg.posicionObjetivo
                << " no disponible tras modificar." << std::endl;
        }
    }

    int posicionInsercion = cfg.posicionObjetivo;
    if (posicionInsercion < 0) {
        posicionInsercion = 0;
    }
    if (posicionInsercion > lista.getTam()) {
        posicionInsercion = lista.getTam();
    }

    lista.insertar_inicio(cfg.valorInicio);
    log << "Insertar al inicio el valor: " << formatearValor(cfg.valorInicio) << std::endl;
    lista.insertar_final(cfg.valorFinal);
    log << "Insertar al final el valor: " << formatearValor(cfg.valorFinal) << std::endl;
    try {
        lista.insertar_pos(cfg.valorMedio, posicionInsercion);
        log << "Insertar valor " << formatearValor(cfg.valorMedio)
            << " en posicion " << posicionInsercion << " (exitoso)" << std::endl;
    } catch (const std::out_of_range& e) {
        log << "Insertar valor " << formatearValor(cfg.valorMedio)
            << " en posicion " << posicionInsercion << " fallo: " << e.what() << std::endl;
    }

    log << "Lista final: " << vistaLinea(lista) << std::endl;
    log << "Tamano final: " << lista.getTam() << std::endl;

    return log.str();
}

template <typename T>
void actualizarEscenario(EstadoEscenario<T>& estado) {
    Lista<T> lista;
    estado.resultado.registroPreparacion =
        llenarLista(estado.nombre, lista, estado.config.valores);
    estado.resultado.registroOperaciones =
        procesarLista(estado.nombre, lista, estado.config);
    estado.resultado.estadoFinal = vistaLinea(lista);
    estado.resultado.tamFinal = lista.getTam();
    estado.necesitaActualizar = false;
}

template <typename T>
void asegurarActualizado(EstadoEscenario<T>& estado) {
    if (estado.necesitaActualizar) {
        actualizarEscenario(estado);
    }
}

// ---------- Entrada de datos ----------

int solicitarEntero(const std::string& prompt, int actual) {
    while (true) {
        std::cout << prompt << " (actual: " << actual << "): ";
        int valor;
        if (std::cin >> valor) {
            limpiarRestante();
            return valor;
        }
        std::cin.clear();
        limpiarRestante();
        std::cout << "Entrada invalida, intenta nuevamente.\n";
    }
}

double solicitarDouble(const std::string& prompt, double actual) {
    while (true) {
        std::cout << prompt << " (actual: " << actual << "): ";
        double valor;
        if (std::cin >> valor) {
            limpiarRestante();
            return valor;
        }
        std::cin.clear();
        limpiarRestante();
        std::cout << "Entrada invalida, intenta nuevamente.\n";
    }
}

char solicitarChar(const std::string& prompt, char actual) {
    while (true) {
        std::cout << prompt << " (actual: " << formatearValor(actual) << "): ";
        std::string entrada;
        if (std::getline(std::cin >> std::ws, entrada) && !entrada.empty()) {
            return entrada[0];
        }
        std::cin.clear();
        std::cout << "Entrada invalida, intenta nuevamente.\n";
    }
}

template <typename T>
T solicitarValor(const std::string& prompt, T actual);

template <>
int solicitarValor<int>(const std::string& prompt, int actual) {
    return solicitarEntero(prompt, actual);
}

template <>
double solicitarValor<double>(const std::string& prompt, double actual) {
    return solicitarDouble(prompt, actual);
}

template <>
char solicitarValor<char>(const std::string& prompt, char actual) {
    return solicitarChar(prompt, actual);
}

// ---------- Configuración interactiva ----------

template <typename T>
void mostrarArreglo(const std::array<T, 10>& valores) {
    for (std::size_t i = 0; i < valores.size(); ++i) {
        std::cout << "[" << i << "]=" << formatearValor(valores[i]);
        if (i + 1 < valores.size()) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

template <typename T>
void editarValoresBase(EscenarioConfig<T>& cfg) {
    while (true) {
        std::cout << "\nValores actuales: ";
        mostrarArreglo(cfg.valores);
        std::cout << "Selecciona indice a modificar (0-9) o -1 para volver: ";
        int indice;
        if (!(std::cin >> indice)) {
            std::cin.clear();
            limpiarRestante();
            std::cout << "Entrada invalida.\n";
            continue;
        }
        limpiarRestante();
        if (indice == -1) {
            break;
        }
        if (indice < 0 || indice >= static_cast<int>(cfg.valores.size())) {
            std::cout << "Indice fuera de rango.\n";
            continue;
        }
        cfg.valores[static_cast<std::size_t>(indice)] =
            solicitarValor<T>("Nuevo valor", cfg.valores[static_cast<std::size_t>(indice)]);
    }
}

template <typename T>
void configurarEscenario(EstadoEscenario<T>& estado) {
    int opcion = 0;
    do {
        std::cout << "\n=== Configuracion de " << estado.nombre << " ===" << std::endl;
        std::cout << "Valores base: ";
        mostrarArreglo(estado.config.valores);
        std::cout << "Posicion objetivo: " << estado.config.posicionObjetivo << std::endl;
        std::cout << "Valor modificado: " << formatearValor(estado.config.valorModificado) << std::endl;
        std::cout << "Valor insercion intermedia: " << formatearValor(estado.config.valorMedio) << std::endl;
        std::cout << "Valor insercion inicio: " << formatearValor(estado.config.valorInicio) << std::endl;
        std::cout << "Valor insercion final: " << formatearValor(estado.config.valorFinal) << std::endl;
        std::cout << "Eliminar posicion invalida extra: "
                  << (estado.config.eliminarInvalida ? "si" : "no") << std::endl;

        std::cout << "\nOpciones:" << std::endl;
        std::cout << "1. Cambiar posicion objetivo" << std::endl;
        std::cout << "2. Cambiar valor modificado" << std::endl;
        std::cout << "3. Cambiar valor insercion intermedia" << std::endl;
        std::cout << "4. Cambiar valor insercion inicio" << std::endl;
        std::cout << "5. Cambiar valor insercion final" << std::endl;
        std::cout << "6. Alternar intento de eliminacion invalida" << std::endl;
        std::cout << "7. Editar valores base" << std::endl;
        std::cout << "8. Restaurar valores por defecto" << std::endl;
        std::cout << "9. Volver" << std::endl;
        std::cout << "Elige una opcion: ";

        if (!(std::cin >> opcion)) {
            std::cin.clear();
            limpiarRestante();
            opcion = 0;
            std::cout << "Entrada invalida, intenta nuevamente.\n";
            continue;
        }
        limpiarRestante();

        switch (opcion) {
            case 1:
                estado.config.posicionObjetivo =
                    solicitarEntero("Nueva posicion objetivo", estado.config.posicionObjetivo);
                estado.necesitaActualizar = true;
                break;
            case 2:
                estado.config.valorModificado =
                    solicitarValor<T>("Nuevo valor modificado", estado.config.valorModificado);
                estado.necesitaActualizar = true;
                break;
            case 3:
                estado.config.valorMedio =
                    solicitarValor<T>("Nuevo valor para insercion intermedia", estado.config.valorMedio);
                estado.necesitaActualizar = true;
                break;
            case 4:
                estado.config.valorInicio =
                    solicitarValor<T>("Nuevo valor para insercion al inicio", estado.config.valorInicio);
                estado.necesitaActualizar = true;
                break;
            case 5:
                estado.config.valorFinal =
                    solicitarValor<T>("Nuevo valor para insercion al final", estado.config.valorFinal);
                estado.necesitaActualizar = true;
                break;
            case 6:
                estado.config.eliminarInvalida = !estado.config.eliminarInvalida;
                std::cout << "Intento de eliminacion invalida "
                          << (estado.config.eliminarInvalida ? "activado." : "desactivado.") << std::endl;
                estado.necesitaActualizar = true;
                break;
            case 7:
                editarValoresBase(estado.config);
                estado.necesitaActualizar = true;
                break;
            case 8:
                estado.config = estado.configDefault;
                estado.necesitaActualizar = true;
                std::cout << "Valores restaurados.\n";
                break;
            case 9:
                break;
            default:
                std::cout << "Opcion no valida.\n";
                break;
        }
    } while (opcion != 9);
}

// ---------- Mostrar resultados ----------

template <typename T>
void imprimirRegistroPreparacion(EstadoEscenario<T>& estado) {
    asegurarActualizado(estado);
    std::cout << estado.resultado.registroPreparacion;
}

template <typename T>
void imprimirRegistroOperaciones(EstadoEscenario<T>& estado) {
    asegurarActualizado(estado);
    std::cout << estado.resultado.registroOperaciones;
}

template <typename T>
void imprimirEstadoFinal(const EstadoEscenario<T>& estado) {
    std::cout << estado.nombre << ": " << estado.resultado.estadoFinal
              << " (tam=" << estado.resultado.tamFinal << ")\n";
}

template <typename T>
void restaurarPorDefecto(EstadoEscenario<T>& estado) {
    estado.config = estado.configDefault;
    estado.necesitaActualizar = true;
}

// ---------- Menús ----------

void menuConfiguracion(EstadoEscenario<int>& enteros,
                       EstadoEscenario<double>& reales,
                       EstadoEscenario<char>& caracteres) {
    int opcion = 0;
    do {
        std::cout << "\n=== Menu de configuracion ===" << std::endl;
        std::cout << "1. Configurar lista de enteros" << std::endl;
        std::cout << "2. Configurar lista de reales" << std::endl;
        std::cout << "3. Configurar lista de caracteres" << std::endl;
        std::cout << "4. Volver" << std::endl;
        std::cout << "Elige una opcion: ";

        if (!(std::cin >> opcion)) {
            std::cin.clear();
            limpiarRestante();
            opcion = 0;
            std::cout << "Entrada invalida.\n";
            continue;
        }
        limpiarRestante();

        switch (opcion) {
            case 1:
                configurarEscenario(enteros);
                break;
            case 2:
                configurarEscenario(reales);
                break;
            case 3:
                configurarEscenario(caracteres);
                break;
            case 4:
                break;
            default:
                std::cout << "Opcion no valida.\n";
                break;
        }
    } while (opcion != 4);
}

int main() {
    EstadoEscenario<int> escenarioEnteros{
        "Lista de enteros",
        obtenerConfigPorDefecto<int>(),
        obtenerConfigPorDefecto<int>(),
        {},
        true};

    EstadoEscenario<double> escenarioReales{
        "Lista de reales",
        obtenerConfigPorDefecto<double>(),
        obtenerConfigPorDefecto<double>(),
        {},
        true};

    EstadoEscenario<char> escenarioCaracteres{
        "Lista de caracteres",
        obtenerConfigPorDefecto<char>(),
        obtenerConfigPorDefecto<char>(),
        {},
        true};

    bool salir = false;
    while (!salir) {
        std::cout << "\n=== Menu principal (Lista) ===" << std::endl;
        std::cout << "1. Ver registro de preparacion" << std::endl;
        std::cout << "2. Ver registro de operaciones" << std::endl;
        std::cout << "3. Ver estado final de las listas" << std::endl;
        std::cout << "4. Configurar escenarios" << std::endl;
        std::cout << "5. Restaurar configuraciones por defecto" << std::endl;
        std::cout << "6. Salir" << std::endl;
        std::cout << "Elige una opcion: ";

        int opcion = 0;
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            limpiarRestante();
            std::cout << "Entrada invalida, intenta nuevamente.\n";
            continue;
        }
        limpiarRestante();

        switch (opcion) {
            case 1:
                imprimirRegistroPreparacion(escenarioEnteros);
                imprimirRegistroPreparacion(escenarioReales);
                imprimirRegistroPreparacion(escenarioCaracteres);
                break;
            case 2:
                imprimirRegistroOperaciones(escenarioEnteros);
                imprimirRegistroOperaciones(escenarioReales);
                imprimirRegistroOperaciones(escenarioCaracteres);
                break;
            case 3:
                asegurarActualizado(escenarioEnteros);
                asegurarActualizado(escenarioReales);
                asegurarActualizado(escenarioCaracteres);
                std::cout << "\n=== Estado global final (Lista) ===" << std::endl;
                imprimirEstadoFinal(escenarioEnteros);
                imprimirEstadoFinal(escenarioReales);
                imprimirEstadoFinal(escenarioCaracteres);
                break;
            case 4:
                menuConfiguracion(escenarioEnteros, escenarioReales, escenarioCaracteres);
                break;
            case 5:
                restaurarPorDefecto(escenarioEnteros);
                restaurarPorDefecto(escenarioReales);
                restaurarPorDefecto(escenarioCaracteres);
                std::cout << "Configuraciones restauradas.\n";
                break;
            case 6:
                std::cout << "Saliendo...\n";
                salir = true;
                break;
            default:
                std::cout << "Opcion no valida, intenta nuevamente.\n";
                break;
        }
    }

    return 0;
}
