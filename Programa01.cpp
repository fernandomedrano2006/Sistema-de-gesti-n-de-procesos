#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// ===========================================
// Estructura para manejar procesos en listas
// ===========================================
struct Proceso {
    int id;             // Identificador único del proceso
    string nombre;      // Nombre del proceso
    int prioridad;      // Prioridad (1 = alta, 10 = baja)
    int memoria;        // Cantidad de memoria requerida (MB)
    string estado;      // Estado del proceso (nuevo, listo, terminado, etc.)
    Proceso* sig;       // Puntero al siguiente proceso (lista enlazada)
};

// ===========================================
// Estructura para manejar memoria usando Pila
// ===========================================
struct PilaMemoria {
    int valor;              // Cantidad de memoria almacenada
    PilaMemoria* sig;       // Puntero al siguiente elemento en la pila
};

// Punteros globales a las estructuras principales
Proceso* listaProcesos = NULL;      // Lista general de procesos
Proceso* colaCPU = NULL;            // Cola de procesos listos para CPU (prioridad)
Proceso* listaTerminados = NULL;    // Lista de procesos finalizados
PilaMemoria* pilaMemoria = NULL;    // Pila de memoria asignada

int nextID = 1; // ID autoincrementable para procesos

// ===========================================
// FUNCIONES DE UTILIDAD
// ===========================================

// Pausa la ejecución para permitir lectura de resultados
void pausar() {
    cout << "\nPresiona ENTER para continuar...";
    cin.ignore();
    cin.get();
}

// Limpia la pantalla
void limpiarPantalla() {
    system("cls");
}

// Imprime el título principal del programa
void titulo() {
    cout << "==============================================\n";
    cout << "   SISTEMA DE GESTIÓN DE PROCESOS Y MEMORIA   \n";
    cout << "==============================================\n";
}

// ===========================================
// GUARDAR DATOS EN ARCHIVO
// ===========================================

void guardarEnArchivo() {
    ofstream archivo("procesos.txt");

    // Guardar lista principal
    archivo << "=== LISTA DE PROCESOS ===\n";
    Proceso* aux = listaProcesos;
    while (aux) {
        archivo << aux->id << " | " << aux->nombre << " | P:"
            << aux->prioridad << " | Mem: " << aux->memoria 
            << "MB | Estado: " << aux->estado << "\n";
        aux = aux->sig;
    }

    // Guardar cola CPU
    archivo << "\n=== COLA DE CPU ===\n";
    aux = colaCPU;
    while (aux) {
        archivo << aux->id << " | " << aux->nombre << " | P:"
            << aux->prioridad << " | Mem: " << aux->memoria
            << "MB | Estado: " << aux->estado << "\n";
        aux = aux->sig;
    }

    // Guardar procesos terminados
    archivo << "\n=== PROCESOS TERMINADOS ===\n";
    aux = listaTerminados;
    while (aux) {
        archivo << aux->id << " | " << aux->nombre << " | P:"
            << aux->prioridad << " | Mem: " << aux->memoria 
            << "MB | Estado: " << aux->estado << "\n";
        aux = aux->sig;
    }

    archivo.close();
    cout << "\n Datos guardados en procesos.txt";
}

// ===========================================
// GESTIÓN DE LISTA DE PROCESOS
// ===========================================

// Inserta un proceso nuevo a la lista
void insertarProceso() {
    Proceso* nuevo = new Proceso();
    nuevo->id = nextID++; // Asignación automática de ID

    cout << "\nIngrese nombre del proceso: ";
    cin >> nuevo->nombre;

    cout << "Prioridad (1 = más alta, 10 = más baja): ";
    cin >> nuevo->prioridad;

    // Validación de prioridad
    if (nuevo->prioridad < 1 || nuevo->prioridad > 10) {
        cout << "\n Prioridad inválida.\n";
        delete nuevo;
        return;
    }

    cout << "Ingrese cantidad de memoria (1 - 4096 MB): ";
    cin >> nuevo->memoria;

    // Validación de memoria
    if (nuevo->memoria < 1 || nuevo->memoria > 4096) {
        cout << "\n Memoria inválida.\n";
        delete nuevo;
        return;
    }

    nuevo->estado = "nuevo";        // Estado inicial
    nuevo->sig = listaProcesos;     // Inserción al inicio
    listaProcesos = nuevo;

    cout << "\n Proceso agregado correctamente. (ID asignado: " << nuevo->id << ")";
}

// Busca un proceso por ID y lo retorna
Proceso* buscarProceso(int id) {
    Proceso* aux = listaProcesos;
    while (aux) {
        if (aux->id == id) return aux;
        aux = aux->sig;
    }
    return NULL;
}

// Interfaz para buscar proceso
void buscarProcesoMenu() {
    int id;
    cout << "\nIngrese ID a buscar: ";
    cin >> id;
    Proceso* p = buscarProceso(id);

    if (p)
        cout << "\n Proceso encontrado: " << p->nombre << " | P:" 
        << p->prioridad << " | Mem: " << p->memoria << "MB | " << p->estado;
    else
        cout << "\n No existe un proceso con ese ID.";
}

// Elimina proceso por ID
void eliminarProceso() {
    int id;
    cout << "\nIngrese ID a eliminar: ";
    cin >> id;

    Proceso* aux = listaProcesos, *ant = NULL;
    while (aux) {
        if (aux->id == id) {
            if (ant) ant->sig = aux->sig;
            else listaProcesos = aux->sig;
            delete aux;
            cout << "\n Proceso eliminado.";
            return;
        }
        ant = aux;
        aux = aux->sig;
    }
    cout << "\n Proceso no encontrado.";
}

// Modifica la prioridad de un proceso
void modificarPrioridad() {
    int id;
    cout << "\nIngrese ID del proceso: ";
    cin >> id;

    Proceso* p = buscarProceso(id);
    if (!p) {
        cout << "\n Proceso no encontrado.";
        return;
    }

    int nueva;
    cout << "Nueva prioridad (1-10): ";
    cin >> nueva;

    if (nueva < 1 || nueva > 10) {
        cout << "\n Valor inválido.";
        return;
    }

    p->prioridad = nueva;
    cout << "\n Prioridad actualizada.";
}

// Muestra la lista de procesos
void mostrarProcesos() {
    cout << "\n=== LISTA DE PROCESOS ===\n";
    Proceso* aux = listaProcesos;
    if (!aux) cout << "(vacío)\n";
    while (aux) {
        cout << aux->id << " | " << aux->nombre << " | P:" 
        << aux->prioridad << " | Mem: " << aux->memoria 
        << "MB | " << aux->estado << "\n";
        aux = aux->sig;
    }
}

// ===========================================
// COLA DE PRIORIDAD (CPU)
// ===========================================

// Selecciona el proceso de mayor prioridad (número menor) y lo pasa a la cola de CPU
void encolarCPU() {
    if (!listaProcesos) {
        cout << "\n No hay procesos disponibles.";
        return;
    }

    Proceso* aux = listaProcesos, *mejor = aux, *ant = NULL, *antMejor = NULL;

    // Se busca el proceso con menor prioridad (más importante)
    while (aux) {
        if (aux->prioridad < mejor->prioridad) {
            mejor = aux;
            antMejor = ant;
        }
        ant = aux;
        aux = aux->sig;
    }

    // Se desconecta de la lista principal
    if (antMejor) antMejor->sig = mejor->sig;
    else listaProcesos = mejor->sig;

    mejor->estado = "listo";
    mejor->sig = colaCPU;
    colaCPU = mejor;

    cout << "\n Proceso encolado en CPU.";
}

// Ejecuta el proceso en CPU y lo mueve a la lista de terminados
void ejecutarCPU() {
    if (!colaCPU) {
        cout << "\n La cola está vacía.";
        return;
    }

    Proceso* p = colaCPU;
    colaCPU = colaCPU->sig;

    p->estado = "terminado";
    p->sig = listaTerminados;
    listaTerminados = p;

    cout << "\n Proceso ejecutado.";
}

// Muestra la cola CPU
void mostrarColaCPU() {
    cout << "\n=== COLA CPU ===\n";
    Proceso* aux = colaCPU;
    if (!aux) cout << "(vacío)\n";
    while (aux) {
        cout << aux->id << " | " << aux->nombre << " | P:" 
        << aux->prioridad << " | Mem: " << aux->memoria 
        << "MB | " << aux->estado << "\n";
        aux = aux->sig;
    }
}

// ===========================================
// MANEJO DE MEMORIA (PILA)
// ===========================================

// Agrega memoria a la pila (push)
void pushMemoria() {
    int v;
    cout << "\nIngrese el valor de memoria: ";
    cin >> v;
    PilaMemoria* nuevo = new PilaMemoria();
    nuevo->valor = v;
    nuevo->sig = pilaMemoria;
    pilaMemoria = nuevo;

    cout << "\n Memoria asignada.";
}

// Libera memoria de la pila (pop)
void popMemoria() {
    if (!pilaMemoria) {
        cout << "\n No hay memoria que liberar.";
        return;
    }
    PilaMemoria* temp = pilaMemoria;
    pilaMemoria = pilaMemoria->sig;
    delete temp;
    cout << "\n Memoria liberada.";
}

// Muestra el estado actual de la pila de memoria
void mostrarMemoria() {
    cout << "\n=== ESTADO DE LA PILA DE MEMORIA ===\n";
    PilaMemoria* aux = pilaMemoria;
    if (!aux) cout << "(vacío)\n";
    while (aux) {
        cout << aux->valor << " <- ";
        aux = aux->sig;
    }
    cout << "\n";
}

// ===========================================
// MENÚS DE OPCIONES
// ===========================================

void menuLista() {
    int op;
    do {
        limpiarPantalla();
        titulo();
        cout << "\n--- GESTIÓN DE LISTAS DE PROCESOS ---\n";
        cout << "1) Insertar proceso\n";
        cout << "2) Buscar proceso\n";
        cout << "3) Eliminar proceso\n";
        cout << "4) Modificar prioridad\n";
        cout << "5) Mostrar procesos\n";
        cout << "0) Volver\n> ";
        cin >> op;

        switch (op) {
        case 1: insertarProceso(); break;
        case 2: buscarProcesoMenu(); break;
        case 3: eliminarProceso(); break;
        case 4: modificarPrioridad(); break;
        case 5: mostrarProcesos(); break;
        }
        pausar();
    } while (op != 0);
}

void menuCPU() {
    int op;
    do {
        limpiarPantalla();
        titulo();
        cout << "\n--- GESTIÓN COLA PRIORIDAD (CPU) ---\n";
        cout << "1) Encolar proceso\n";
        cout << "2) Ejecutar proceso (desencolar)\n";
        cout << "3) Mostrar cola\n";
        cout << "0) Volver\n> ";
        cin >> op;

        switch (op) {
        case 1: encolarCPU(); break;
        case 2: ejecutarCPU(); break;
        case 3: mostrarColaCPU(); break;
        }
        pausar();
    } while (op != 0);
}

void menuMemoria() {
    int op;
    do {
        limpiarPantalla();
        titulo();
        cout << "\n--- GESTIÓN DE MEMORIA (PILA) ---\n";
        cout << "1) Asignar memoria (push)\n";
        cout << "2) Liberar memoria (pop)\n";
        cout << "3) Mostrar estado de la memoria\n";
        cout << "0) Volver\n> ";
        cin >> op;

        switch (op) {
        case 1: pushMemoria(); break;
        case 2: popMemoria(); break;
        case 3: mostrarMemoria(); break;
        }
        pausar();
    } while (op != 0);
}

// ===========================================
// PROGRAMA PRINCIPAL
// ===========================================

int main() {
    setlocale(LC_CTYPE, "Spanish"); // Permite acentos en consola

    int op;
    do {
        limpiarPantalla();
        titulo();
        cout << "\n======= MENÚ PRINCIPAL =======\n";
        cout << "\n1) Gestión de listas de procesos\n";
        cout << "2) Gestión de cola prioridad (CPU)\n";
        cout << "3) Gestión de memoria (pila)\n";
        cout << "4) Guardar todo en archivo\n";
        cout << "0) Salir\n> ";
        cin >> op;

        switch (op) {
        case 1: menuLista(); break;
        case 2: menuCPU(); break;
        case 3: menuMemoria(); break;
        case 4: guardarEnArchivo(); pausar(); break;
        }
    } while (op != 0);

    return 0;
}

