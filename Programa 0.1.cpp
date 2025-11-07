#include <iostream>
using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso* sig;
};

Proceso* listaProcesos = NULL;

void insertarProceso() {
    cout << "\n(Aún no implementado)\n";
}

void mostrarProcesos() {
    cout << "\n(Aún no implementado)\n";
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

