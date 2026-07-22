# VRPT-SWTS

Resolución heurística del **Vehicle Routing Problem with Transfer stations for
Solid Waste Transportation Systems (VRPT-SWTS)**: un problema de rutas de
vehículos en dos niveles, típico de sistemas de recogida de residuos urbanos.

- Una flota de **vehículos de recolección** visita zonas con residuos, los
  recoge y los descarga en estaciones de transferencia (**SWTS**), volviendo
  siempre a su depósito de origen.
- Una flota de **vehículos de transporte** recoge los residuos acumulados en
  las SWTS y los lleva hasta el **vertedero** final.

El proyecto implementa tres algoritmos —**Voraz**, **GRASP** y **RVND**— e
incluye una visualización web de las rutas resultantes.

## Índice

- [Compilación y ejecución](#compilación-y-ejecución)
- [Visualización web de las rutas](#visualización-web-de-las-rutas)
- [Formato de las instancias](#formato-de-las-instancias)
- [Arquitectura](#arquitectura)
- [Referencia de clases](#referencia-de-clases)
  - [`Zona`](#zona-srczonazonah)
  - [`Vehiculo`](#vehiculo-srcvehiculovehiculohcc)
  - [`Recoleccion`](#recoleccion-srcvehiculorecoleccion)
  - [`Transporte`](#transporte-srcvehiculotransportetransporteh)
  - [`Tools`](#tools-srctoolstoolshcc)
  - [`Algoritmo`](#algoritmo-srcalgoritmoalgoritmoh)
  - [`Voraz`](#voraz-srcalgoritmovoraz)
  - [`Grasp`](#grasp-srcalgoritmograsp)
  - [`BusquedaLocal`](#busquedalocal-srcalgoritmograspbusqueda_local)
  - [`RVND`](#rvnd-srcalgoritmorvnd)
- [Decisiones de diseño y limitaciones conocidas](#decisiones-de-diseño-y-limitaciones-conocidas)

## Compilación y ejecución

Requiere un compilador con soporte de **C++20** (el `makefile` usa `g++`).

```sh
make                # compila todo el proyecto y genera el ejecutable "programa"
./programa data/    # el argumento es el directorio que contiene los ficheros "instanceN.txt"
make clean          # elimina el ejecutable
```

El programa busca en el directorio indicado todos los ficheros que sigan el
patrón `instanceN.txt` (con o sin `/` final en la ruta) y los procesa en
orden numérico. Si el directorio no existe o no contiene ningún fichero de
ese tipo, termina con un mensaje de error explícito en vez de continuar en
silencio con una instancia vacía.

Al ejecutarlo se muestra un menú:

```
Seleccione una opción:
1. Algoritmo Voraz
2. Algoritmo GRASP
3. Algoritmo RVND
4. Salir
```

Tras calcular las rutas, el programa imprime una tabla de resultados por
instancia (número de zonas, vehículos usados, distancia, tiempo de CPU...) y
**exporta automáticamente `resultados.json`** en el directorio desde el que se
ejecutó, con las zonas y las rutas de **todas** las instancias procesadas,
listo para la visualización web.

## Visualización web de las rutas

[`visualizacion/index.html`](visualizacion/index.html) es una página
autocontenida (sin dependencias externas ni build propio) que dibuja sobre un
plano las zonas y las rutas calculadas por el programa.

1. Ejecuta `./programa data/` y elige un algoritmo — esto genera
   `resultados.json` con los resultados de todas las instancias procesadas.
2. Abre `visualizacion/index.html` en el navegador (doble clic, o
   `open visualizacion/index.html` en macOS).
3. Pulsa **"Cargar resultados.json"** y selecciona el fichero generado en el
   paso 1.

Funcionalidades:

- Mapa con las zonas (depósito, SWTS, vertedero, zonas de recolección — el
  radio de estas últimas indica la cantidad de residuos) y las rutas de cada
  vehículo (sólidas para recolección, discontinuas para transporte).
- Leyenda interactiva: clic en un vehículo para ocultar/mostrar su ruta,
  botones "mostrar todas/ocultar todas" por grupo.
- Zoom (rueda del ratón o botones +/−) y paneo (arrastrar).
- Tooltips al pasar el cursor por una zona o una ruta.
- Selector de instancia si el JSON contiene varias.
- Vista alternativa en tabla (botón "Ver como tabla") con la secuencia de
  paradas de cada vehículo.
- Se adapta al tema claro/oscuro del sistema.

Si se sirve por HTTP (por ejemplo `python3 -m http.server` desde la raíz del
repositorio) la página intenta cargar `resultados.json` automáticamente; si se
abre como fichero local (`file://`) los navegadores bloquean esa carga
automática por CORS, por lo que hay que usar el selector de fichero.

## Formato de las instancias

Cada `data/instanceN.txt` es un fichero de texto con pares `clave valor` por
línea:

| Clave | Significado |
|---|---|
| `L1` / `L2` | Duración máxima (min) de la jornada de recolección / transporte |
| `num_vehicles` | Número de vehículos disponibles |
| `num_zones` | Número de zonas |
| `Lx`, `Ly` | Límites del plano (coordenadas máximas) |
| `Q1` / `Q2` | Capacidad de los vehículos de recolección / transporte |
| `V` | Velocidad de los vehículos |
| `Depot x y` | Depósito de los vehículos de recolección |
| `IF x y` / `IF1 x y` | Estaciones de transferencia (SWTS) |
| `Dumpsite x y` | Vertedero final |
| `id x y tiempo contenido` | Una zona de recolección (identificador numérico) |

Las líneas `epsilon`, `offset` y `k` se ignoran (parámetros no usados por
estos algoritmos).

## Arquitectura

```
Zona ── (contiene) ── Vehiculo ── Recoleccion
                              └── Transporte

Tools  (datos de una instancia: zonas, rutas, matriz de distancias...)

Algoritmo (interfaz)
  ├── Voraz
  ├── Grasp ── usa Voraz (fase de transporte) + BusquedaLocal
  └── RVND ── usa Grasp
```

`main.cc` lee la instancia indicada con `readData`, deja elegir un algoritmo
con `crearAlgoritmo`, lo ejecuta sobre ella y muestra/exporta los resultados.

## Referencia de clases

### `Zona` (`src/zona/zona.h`)

Representa un punto del plano: una zona de recolección, el depósito, una SWTS
o el vertedero. Es un objeto ligero, copiable, usado por valor en toda la base
de código.

| Miembro | Descripción |
|---|---|
| `Zona()` | Constructor por defecto (necesario para usarla en contenedores). |
| `Zona(id, posicion, contenido)` | Construye una zona sin tiempo de procesado (p. ej. el vertedero). |
| `Zona(id, posicion, tiempoRecoleccion, contenido)` | Constructor general. |
| `getId()` | Identificador de la zona (`"Depot"`, `"IF"`, `"Dumpsite"` o un número). |
| `getPosicion()` | Coordenadas `(x, y)` en el plano. |
| `getContenido()` | Cantidad de residuos pendientes en la zona. |
| `getDistancia(zona)` | Distancia euclídea a otra zona. |
| `getTiempoDeProcesado()` | Tiempo que tarda un vehículo en recoger/vaciar en esta zona. |
| `setContenido(c)` | Actualiza el contenido (usado al recoger/vaciar). |
| `esSWTS()` / `esDeposito()` / `esDumpsite()` | Comprueban el tipo de zona por su `id`. |
| `operator==` | Compara zonas por `id` (usado por `find`/`remove` en los algoritmos). |

### `Vehiculo` (`src/vehiculo/vehiculo.h`/`.cc`)

Clase base común a los dos tipos de vehículo. Mantiene la posición actual, el
contenido cargado, el histórico de zonas visitadas y el tiempo transcurrido.

| Método | Descripción |
|---|---|
| `Vehiculo(capacidad, velocidad, posicion, duracion)` | Constructor. |
| `llenarVehiculo(cantidad)` | `true` si `contenido + cantidad` cabe en la capacidad (no modifica el estado; sólo comprueba factibilidad). |
| `vaciarVehiculo(zona)` | Transfiere todo el contenido del vehículo a `zona` y lo deja vacío (usado al vaciar en una SWTS o en el vertedero). |
| `moverVehiculo(zona, distancia)` | Avanza el reloj interno según `distancia`/velocidad, registra la posición anterior en el histórico y actualiza la posición actual. |
| `calcularTiempo(distancia)` | Minutos que tarda en recorrer `distancia` a la velocidad del vehículo. |
| `volverAlInicio()` | Cierra la ruta: añade la posición actual y la primera zona visitada (el origen) al histórico, y sitúa el vehículo de vuelta en el origen. |
| `agregarContenido(c)` | Suma `c` al contenido cargado. |
| `getPosicion` / `getZonasVisitadas` / `getDuracion` / `getContenido` / `getCapacidad` / `getTiempoTranscurrido` | Getters. |
| `setPosicion(zona)` | Sitúa el vehículo en `zona` sin registrar el movimiento en el histórico (usado en cálculos auxiliares "¿qué pasaría si...?"). |

### `Recoleccion` (`src/vehiculo/recoleccion/`)

`Vehiculo` especializado en recoger residuos de las zonas.

- `vaciarZona(zona)`: recoge el contenido de `zona` (lo suma al vehículo, deja
  la zona a 0) y añade el tiempo de procesado de la zona al tiempo transcurrido
  del vehículo.

### `Transporte` (`src/vehiculo/transporte/transporte.h`)

`Vehiculo` especializado en transportar residuos desde las SWTS al vertedero.

- `struct Tarea { Dh, Sh, Th }`: una tarea de transporte — `Dh` cantidad de
  residuos a recoger, `Sh` la SWTS donde recogerla, `Th` el instante en que
  está disponible.
- `agregarTarea(tarea)` / `getTareasAsignadas()`: gestionan la lista de tareas
  asignadas al vehículo.

### `Tools` (`src/tools/tools.h`/`.cc`)

No es una clase de utilidades genérica sino el contenedor de **todos los datos
de una instancia del problema** (zonas, rutas calculadas, matriz de
distancias, parámetros de capacidad/duración/velocidad...). Cada algoritmo
trabaja sobre un `Tools` a la vez (`Algoritmo::dato_`) y puede generar varias
copias (una por ejecución, en el caso de GRASP).

| Función libre | Descripción |
|---|---|
| `readData(dirName)` | Busca en `dirName` todos los ficheros `instanceN.txt`, los ordena por número y devuelve un `Tools` por cada uno (ver `leerInstancia`). Lanza `std::invalid_argument` si el directorio no existe o no contiene ninguno. |
| `leerInstancia(rutaFichero)` | Lee un único fichero de instancia y devuelve su `Tools`, con la matriz de distancias ya calculada. Lanza `std::invalid_argument` si el fichero no existe o no contiene zonas. |
| `procesarLinea(linea, datos)` | Interpreta una línea del fichero de instancia y actualiza `datos`. |
| `crearZona(id, linea, datos)` | Construye una `Zona` a partir de una línea y valida sus datos (lanza `std::invalid_argument` si son inconsistentes). |
| `calcularDistancias(datos)` | Calcula la matriz de distancias euclídeas entre todas las zonas. |
| `mostrarZonas` / `mostrarDistancias` | Utilidades de depuración por consola. |
| `mostrarMenu()` | Imprime el menú de opciones. |
| `crearAlgoritmo(opcion)` | Fábrica: crea el `Algoritmo` (`Voraz`, `Grasp` o `RVND`) correspondiente a la opción elegida. |
| `exportarResultadosJSON(datos, ruta)` | Serializa zonas y rutas a JSON para `visualizacion/index.html`. |
| `Tools::calcularDistanciaRecoleccion()` | Suma la distancia recorrida por todas las rutas de recolección de esta instancia. |

### `Algoritmo` (`src/algoritmo/algoritmo.h`)

Interfaz común (clase abstracta) de los tres algoritmos.

- `dato_`: instancia (`Tools*`) sobre la que se está trabajando actualmente.
- `datos_`: histórico de instancias resueltas (una por cada ejecución del
  algoritmo), usado para mostrar/exportar resultados agregados. Se guarda como
  `shared_ptr<Tools>` para que las instancias sigan vivas mientras algún
  algoritmo (p. ej. `RVND` sobre los resultados de `Grasp`) las siga
  referenciando, sin fugas de memoria.
- `ejecutar()` / `mostrarResultados()`: métodos virtuales puros que implementa
  cada algoritmo concreto.
- `setDato(dato)` / `getDatos()`: setter/getter de los datos.

### `Voraz` (`src/algoritmo/voraz/`)

Construcción **golosa** (greedy) de la solución, en dos fases:

1. **`calcularRutasRecoleccion()`**: mientras queden zonas pendientes, crea un
   vehículo de recolección y, repetidamente, va a la `zonaMasCercana()` que
   quepa en su capacidad y le permita seguir volviendo a tiempo al depósito
   (`TiempoVolverDeposito`); si no puede recoger más, va a la `swtsMasCercana()`
   a descargar. Al terminar, cierra la ruta con `volverAlInicio()`.
2. **`calcularRutasTransporte()`**: convierte cada tramo de una ruta de
   recolección entre dos SWTS (o entre el depósito y una SWTS) en una `Tarea`
   (`crearConjuntoTareas`), las ordena por tiempo de disponibilidad
   (`ordenarTareas`) y las asigna una a una al vehículo de transporte que
   minimice el coste de inserción (`escogerVehiculo`/`calcularCostoInsercion`),
   creando uno nuevo si ninguno es factible. Un vehículo se envía al vertedero
   a descargar cuando ya no le cabe la siguiente tarea mínima.

`ejecutar()` llama a ambas fases y mide el tiempo de CPU;
`mostrarResultados()` imprime una tabla por instancia con el número de
vehículos usados y la media de todas las instancias procesadas.

### `Grasp` (`src/algoritmo/grasp/`)

**GRASP** (Greedy Randomized Adaptive Search Procedure): repite la
construcción golosa introduciendo aleatoriedad controlada, y mejora cada
solución con búsqueda local.

- `zonaMasCercana(vehiculo, numeroMejoresZonas)`: en vez de elegir siempre la
  zona más cercana, calcula las `numeroMejoresZonas` mejores candidatas (la
  *lista restringida de candidatos*, LRC) y escoge una al azar entre ellas —
  esto es lo que introduce diversidad entre ejecuciones.
- `calcularRutasRecoleccion(mejoresZonas, ejecucion)`: misma lógica que en
  `Voraz`, pero usando `zonaMasCercana` con LRC.
- `ejecutar()`: para cada tamaño de LRC (de 2 a `numeroMejoresZonasCercanas_`)
  y cada ejecución (hasta `numeroEjecuciones_`), construye una solución,
  la mejora con `BusquedaLocal::mejorarRutas()`, calcula las rutas de
  transporte reutilizando `Voraz::calcularRutasTransporte()`, y guarda el
  resultado. Cada combinación (LRC, ejecución) queda registrada en
  `datos_`/`mejoresZonasYEjecuciones_`, lo que permite a `RVND` comparar
  todas las soluciones generadas.
- `getDato(dato)`: devuelve todas las soluciones guardadas para una instancia
  concreta (usado por `RVND`).
- `mostrarResultados()` / `mostrarDistancias()`: tablas con el detalle de cada
  combinación (LRC, ejecución) y la distancia de recolección antes/después de
  la búsqueda local.

### `BusquedaLocal` (`src/algoritmo/grasp/busqueda_local/`)

Búsqueda local por intercambio de vecindarios sobre las rutas de recolección
de una solución (`setVehiculos`). `mejorarRutas()` aplica repetidamente, hasta
que ninguno mejore, estos operadores:

| Operador | Vecindario |
|---|---|
| `swapInter()` | Intercambia una zona de una ruta con una zona de otra ruta. |
| `swapIntra()` | Intercambia dos zonas dentro de la misma ruta. |
| `reinsertIntra()` | Mueve una zona a otra posición dentro de la misma ruta. |
| `reinsertInter()` | Mueve una zona de una ruta a otra. |
| `twoOptIntra()` | Invierte un segmento de la ruta (2-opt). |

Cada movimiento se prueba sobre una copia de la(s) ruta(s); si el resultado es
`esFactible()` (respeta capacidad y duración máxima) y reduce
`calcularCostoRuta()` (distancia total), se acepta.

### `RVND` (`src/algoritmo/rvnd/`)

**Randomized Variable Neighborhood Descent**: ejecuta un `Grasp` completo
(todas las combinaciones de LRC × ejecuciones) y, con `mejorRuta()`, se queda
con la solución que usa menos vehículos en total (recolección + transporte)
de entre todas las generadas. Es, por tanto, la envolvente de mejor calidad de
los tres algoritmos.

## Decisiones de diseño y limitaciones conocidas

- **Se procesan todas las instancias del directorio, no solo dos.** El
  programa original procesaba, de forma hardcodeada, sólo `instance1.txt` e
  `instance2.txt`; además, si la ruta no terminaba en `/` o el fichero no
  existía, seguía adelante en silencio con una instancia vacía (0 zonas, 0
  vehículos) en vez de dar un error. Ahora `readData` recorre el directorio
  con `std::filesystem`, procesa cualquier fichero `instanceN.txt` presente
  (ordenados por número) y lanza `std::invalid_argument` con un mensaje claro
  si el directorio no existe o no contiene ninguno; `leerInstancia` hace lo
  mismo para un único fichero y lanza si no se puede abrir o no contiene
  zonas.
- **Ownership de `Tools`.** `Algoritmo` guarda sus resultados como
  `shared_ptr<Tools>` en vez de punteros crudos: `Voraz` no es dueño del dato
  (apunta a un `Tools` del llamador, por eso usa un deleter vacío), mientras
  que `Grasp` sí crea instancias nuevas con `new` en cada ejecución y transfiere
  su propiedad al `shared_ptr`. Esto evita tanto la fuga de memoria como el
  puntero colgante que existía cuando `RVND` seguía leyendo los resultados de
  `Grasp` después de que este se hubiera destruido.
- **Estado sin variables globales.** La versión original usaba una variable
  global `Tools tools;` mutable durante el parseo de cada fichero; ahora
  `readData`/`procesarLinea`/`crearZona` reciben el `Tools` de la instancia
  actual por referencia.
- Los tres algoritmos son deterministas salvo `Grasp`/`RVND`, que usan
  `std::mt19937` sembrado con `std::random_device` — dos ejecuciones pueden
  dar soluciones distintas (por diseño).
