#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    int memoria; // <--- NUEVO CAMPO
    string estado;
    Proceso* sig;
};

struct PilaMemoria {
    int valor;
    PilaMemoria* sig;
};

Proceso* listaProcesos = NULL;
Proceso* colaCPU = NULL;
Proceso* listaTerminados = NULL;
PilaMemoria* pilaMemoria = NULL;

int nextID = 1; // ID auto-incrementable

// ===== UTILIDADES =====

void pausar() {
    cout << "\nPresiona ENTER para continuar...";
    cin.ignore();
    cin.get();
}

void limpiarPantalla() {
    system("cls");
}

void titulo() {
    cout << "==============================================\n";
    cout << "   SISTEMA DE GESTIÓN DE PROCESOS Y MEMORIA   \n";
    cout << "==============================================\n";
}

// ===== GUARDAR EN ARCHIVO =====

void guardarEnArchivo() {
    ofstream archivo("procesos.txt");
    archivo << "=== LISTA DE PROCESOS ===\n";
    Proceso* aux = listaProcesos;
    while (aux) {
        archivo << aux->id << " | " << aux->nombre << " | P:"
            << aux->prioridad << " | Mem: " << aux->memoria 
            << "MB | Estado: " << aux->estado << "\n";
        aux = aux->sig;
    }

    archivo << "\n=== COLA DE CPU ===\n";
    aux = colaCPU;
    while (aux) {
        archivo << aux->id << " | " << aux->nombre << " | P:"
            << aux->prioridad << " | Mem: " << aux->memoria 
            << "MB | Estado: " << aux->estado << "\n";
        aux = aux->sig;
    }

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

// ===== LISTA PROCESOS =====

void insertarProceso() {
    Proceso* nuevo = new Proceso();

    nuevo->id = nextID++; // ID automático

    cout << "\nIngrese nombre del proceso: ";
    cin >> nuevo->nombre;

    cout << "Prioridad (1 = más alta, 10 = más baja): ";
    cin >> nuevo->prioridad;

    if (nuevo->prioridad < 1 || nuevo->prioridad > 10) {
        cout << "\n Prioridad inválida (debe ser entre 1 y 10).\n";
        delete nuevo;
        return;
    }

    cout << "Ingrese cantidad de memoria (1 - 4096 MB): ";
    cin >> nuevo->memoria;
    if (nuevo->memoria < 1 || nuevo->memoria > 4096) {
        cout << "\n Memoria inválida (debe estar entre 1 y 4096 MB).\n";
        delete nuevo;
        return;
    }

    nuevo->estado = "nuevo";
    nuevo->sig = listaProcesos;
    listaProcesos = nuevo;

    cout << "\n Proceso agregado correctamente. (ID asignado: " << nuevo->id << ")";
}

Proceso* buscarProceso(int id) {
    Proceso* aux = listaProcesos;
    while (aux != NULL) {
        if (aux->id == id) return aux;
        aux = aux->sig;
    }
    return NULL;
}

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

void eliminarProceso() {
    int id;
    cout << "\nIngrese ID a eliminar: ";
    cin >> id;

    Proceso* aux = listaProcesos, *ant = NULL;
    while (aux != NULL) {
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

// ===== COLA CPU =====

void encolarCPU() {
    if (!listaProcesos) {
        cout << "\n No hay procesos disponibles.";
        return;
    }

    Proceso* aux = listaProcesos, *mejor = aux, *ant = NULL, *antMejor = NULL;
    while (aux) {
        if (aux->prioridad < mejor->prioridad) {
            mejor = aux;
            antMejor = ant;
        }
        ant = aux;
        aux = aux->sig;
    }

    if (antMejor) antMejor->sig = mejor->sig;
    else listaProcesos = mejor->sig;

    mejor->estado = "listo";
    mejor->sig = colaCPU;
    colaCPU = mejor;

    cout << "\n Proceso encolado en CPU.";
}

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

// ===== MEMORIA PILA =====

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

// ===== MENUS =====

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
        cout << "Ingrese opción (0-5):";
        cin >> op;

        switch (op) {
        case 1: insertarProceso(); break;
        case 2: buscarProcesoMenu(); break;
        case 3: eliminarProceso(); break;
        case 4: modificarPrioridad(); break;
        case 5: mostrarProcesos(); break;
        case 0: break;
        default: cout << "\n Opción inválida."; break;
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
        cout << "Ingrese opción (0-3):";
        cin >> op;

        switch (op) {
        case 1: encolarCPU(); break;
        case 2: ejecutarCPU(); break;
        case 3: mostrarColaCPU(); break;
        case 0: break;
        default: cout << "\n Opción inválida."; break;
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
        cout << "Ingrese opción (0-3):";
        cin >> op;

        switch (op) {
        case 1: pushMemoria(); break;
        case 2: popMemoria(); break;
        case 3: mostrarMemoria(); break;
        case 0: break;
        default: cout << "\n Opción inválida."; break;
        }
        pausar();
    } while (op != 0);
}

int main() {
    setlocale(LC_CTYPE, "Spanish");
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
        cout << "Ingrese opción (0-4):";
        cin >> op;

        switch (op) {
        case 1: menuLista(); break;
        case 2: menuCPU(); break;
        case 3: menuMemoria(); break;
        case 4: guardarEnArchivo(); pausar(); break;
        case 0: break;
        default: cout << "\n Opción inválida."; pausar(); break;
        }
    } while (op != 0);

    return 0;
}

