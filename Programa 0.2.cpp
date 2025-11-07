#include <iostream>
#include <string>
using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso* sig;
};

Proceso* listaProcesos = NULL;
int nextID = 1;

void insertarProceso() {
    Proceso* nuevo = new Proceso();
    nuevo->id = nextID++;

    cout << "\nNombre del proceso: ";
    cin >> nuevo->nombre;
    cout << "Prioridad: ";
    cin >> nuevo->prioridad;

    nuevo->sig = listaProcesos;
    listaProcesos = nuevo;

    cout << "\nProceso agregado correctamente.";
}

void mostrarProcesos() {
    cout << "\n--- LISTA DE PROCESOS ---\n";
    Proceso* aux = listaProcesos;
    if (!aux) cout << "(vacio)\n";
    while(aux) {
        cout << aux->id << " | " << aux->nombre << " | P:" << aux->prioridad << "\n";
        aux = aux->sig;
    }
}

void menuProcesos() {
    int op;
    do {
        cout << "\n--- MENU PROCESOS ---\n";
        cout << "1) Insertar proceso\n";
        cout << "2) Mostrar procesos\n";
        cout << "0) Volver\n> ";
        cin >> op;

        switch(op) {
            case 1: insertarProceso(); break;
            case 2: mostrarProcesos(); break;
        }
    } while(op != 0);
}

int main() {
    int op;
    do {
        cout << "\n===== MENU PRINCIPAL =====\n";
        cout << "1) Gestion de procesos\n";
        cout << "0) Salir\n> ";
        cin >> op;
        switch(op) {
            case 1: menuProcesos(); break;
        }
    } while(op != 0);
    return 0;
}

