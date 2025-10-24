
  # Lista Enlazada Simple

  > [!IMPORTANT]
  > Este proyecto implementa una lista enlazada simple genérica en C++ utilizando únicamente dos archivos (`lista.h` y `lista.cpp`). No se aplicó ningún patrón de diseño
  porque la restricción de archivos nos llevó a priorizar una estructura mínima y directa.

  ## ¿Qué es una lista enlazada simple?
  Una lista enlazada simple es una colección de nodos donde cada nodo conoce únicamente al siguiente. Esto permite:
  - Insertar o eliminar elementos sin desplazar al resto.
  - Recorrer la estructura desde la cabeza hasta `nullptr`.
  - Trabajar con costo lineal para búsquedas y constante para inserciones en extremos conocidos.

  En `lista.h`, la plantilla `struct Nodo<T>` almacena el dato (`info`) y el puntero al siguiente (`sig`). La clase `Lista<T>` mantiene referencia a la cabeza y la cola para
  optimizar inserciones tanto al inicio como al final.

  ## ¿Por qué templates?
  Toda la implementación está templada (`template <class T>`) para reutilizar la lógica con cualquier tipo que soporte copia y escritura vía `std::ostream`. Esto sigue el
  paradigma genérico de C++: una sola pieza de código para múltiples tipos.

  > [!TIP]
  > Si necesitas trabajar con tipos propios, define `operator<<` para que `Lista<T>::imprimir` pueda mostrarlos correctamente.

  ## Organización del código

  lista.h    // Declaración e implementación de Nodo<T> y Lista<T>
  lista.cpp  // Funciones de apoyo, configuración de escenarios y main interactivo


  - `lista.h` hace las veces de biblioteca reutilizable: expone la lista enlazada y sus operaciones fundamentales (insertar, obtener, modificar, eliminar, imprimir).
  - `lista.cpp` incorpora esa biblioteca, construye distintos escenarios de ejemplo y ofrece un menú interactivo que permite explorar el comportamiento de la lista.

  ## Compilación y ejecución
  ```bash
  g++ -std=c++17 lista.cpp -o lista
  ./lista

  > [!NOTE]
  > No se utilizan dependencias externas: basta con la biblioteca estándar (<array>, <iostream>, <sstream>, etc.).

  ## Anatomía del main

  El archivo lista.cpp está dividido en tres bloques principales:

  1. Escenarios preconfigurados (EstadoEscenario<T>): hay uno para int, uno para double y uno para char. Cada escenario guarda su configuración, resultados calculados y una
     bandera que indica cuándo debe recalcularse.
  2. Menús:
      - Menú principal para ver registros, estados finales, configurar o restaurar parámetros.
      - Menú de configuración que permite cambiar valores base, posiciones objetivo y opciones de eliminación sin recompilar.
  3. Funciones auxiliares:
      - llenarLista muestra paso a paso cómo se insertan valores en distintos puntos.
      - procesarLista aplica las operaciones principales (obtener, modificar, eliminar, insertar).
      - Las funciones solicitar... manejan la entrada de usuario con validaciones sencillas.

  Gracias a esta estructura puedes duplicar un escenario o modificar los flujos del menú sin tocar la implementación de la lista.

  ## Extensiones sugeridas

  - Añadir métodos como buscar, reverse o clear directamente en lista.h.
  - Ajustar los valores por defecto editando obtenerConfigPorDefecto<T>().
  - Crear un main alternativo que use llenarLista y procesarLista como demostración automatizada (sin interacción).

  > [!WARNING]
  > La implementación evita lanzar excepciones en operaciones fuera de rango: devuelve valores por defecto o false. Si necesitas un manejo estricto de errores, valida tus
  > posiciones antes de llamar a los métodos o modifica la plantilla para lanzar std::out_of_range.

  ## Estado del proyecto

  - [x] Lista enlazada simple genérica
  - [x] Menú interactivo para explorar operaciones
  - [x] Configuración modificable sin recompilar
  - [ ] Persistencia de datos
  - [ ] Interfaz gráfica