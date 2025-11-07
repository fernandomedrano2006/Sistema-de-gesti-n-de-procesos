#include <iostream>
#include <string>
using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    string estado;
    Proceso* sig;
};

Proceso *listaProcesos = NULL, *colaCPU = NULL;
int nextID = 1;

void insertarProceso() {
    Proceso* nuevo = new Proceso();
    nuevo->id = nextID++;
    cout << "\nNombre: ";
    cin >> nuevo->nombre;
    cout << "Prioridad (1-10): ";
    cin >> nuevo->prioridad;
    nuevo->estado = "nuevo";
    nuevo->sig = listaProcesos;
    listaProcesos = nuevo;
}

Proceso* buscar(int id) {
    Proceso* aux = listaProcesos;
    while(aux) {
        if(aux->id == id) return aux;
        aux = aux->sig;
    }
    return NULL;
}

void eliminarProceso() {
    int id;
    cout << "\nID a eliminar: ";
    cin >> id;
    Proceso *aux = listaProcesos, *ant = NULL;
    while(aux) {
        if(aux->id == id) {
            if(ant) ant->sig = aux->sig;
            else listaProcesos = aux->sig;
            delete aux;
            return;
        }
        ant = aux;
        aux = aux->sig;
    }
}

void modificarPrioridad() {
    int id, p;
    cout << "\nID: ";
    cin >> id;
    Proceso* pr = buscar(id);
    if(!pr) return;
    cout << "Nueva prioridad: ";
    cin >> p;
    pr->prioridad = p;
}

void mostrar() {
    Proceso* aux = listaProcesos;
    cout << "\n--- PROCESOS ---\n";
    while(aux) {
        cout << aux->id << " | " << aux->nombre << " | P:" << aux->prioridad << "\n";
        aux = aux->sig;
    }
}

void encolarCPU() {
    if(!listaProcesos) return;
    Proceso *aux=listaProcesos, *mejor=aux, *ant=NULL, *antM=NULL;
    while(aux) {
        if(aux->prioridad < mejor->prioridad) { mejor = aux; antM = ant; }
        ant = aux;
        aux = aux->sig;
    }
    if(antM) antM->sig = mejor->sig;
    else listaProcesos = mejor->sig;
    mejor->sig = colaCPU;
    colaCPU = mejor;
}

void mostrarCPU() {
    cout << "\n--- COLA CPU ---\n";
    Proceso* aux = colaCPU;
    while(aux) {
        cout << aux->id << " | " << aux->nombre << " | P:" << aux->prioridad << "\n";
        aux = aux->sig;
    }
}

int main() {
    int op;
    do {
        cout << "\n1) Insertar\n2) Mostrar\n3) Buscar\n4) Eliminar\n5) Modificar prioridad\n6) Encolar CPU\n7) Mostrar CPU\n0) Salir\n> ";
        cin >> op;
        switch(op) {
            case 1: insertarProceso(); break;
            case 2: mostrar(); break;
            case 4: eliminarProceso(); break;
            case 5: modificarPrioridad(); break;
            case 6: encolarCPU(); break;
            case 7: mostrarCPU(); break;
        }
    } while(op != 0);
}

