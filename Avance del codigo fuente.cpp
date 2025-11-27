#include <iostream>
#include <locale.h>
using namespace std;

#define MAX_STR_LEN 64

int next_member_id = 1;

//estructura del nodo 
struct Nodo {
    char id[MAX_STR_LEN];
    char nombre[MAX_STR_LEN];
    char fechaNacimiento[MAX_STR_LEN];
    char titulo[MAX_STR_LEN];
    char etnia[MAX_STR_LEN];
    Nodo* padre;
    Nodo* madre;
    Nodo* hijoPrimero;
    Nodo* siguienteHermano;
    bool visitado; 
};

//limpiador del teclado 
void limpiarBuffer() {
    cin.ignore(10000, '\n');
}

// paa leer el texto
void obtenerLinea(char buffer[]) {
    cin.getline(buffer, MAX_STR_LEN);
    if (cin.fail()) {
        cin.clear();
        limpiarBuffer();
        buffer[0] = '\0';
    }
}

//funciones
//comparar las cadaenas caracter por caracter 
bool compararCadenas(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

//copia una cadena a otra
void copiarCadena(char* destino, const char* origen) {
    int i = 0;
    while ((destino[i] = origen[i]) != '\0') {
        i++;
    }
}

//convierte de entero a texto
void intToChar(int n, char buffer[]) {
    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    char temp[MAX_STR_LEN];
    int i = 0;
    while (n > 0) {
        temp[i++] = (n % 10) + '0';
        n /= 10;
    }
    int j = 0;
    while (i > 0) buffer[j++] = temp[--i];
    buffer[j] = '\0';
}

// crear e imprimir

//construye un nuevo mienbro de la familia 
Nodo* crearNodo(const char* n, const char* fn, const char* t, const char* e) {
    Nodo* nodo = new Nodo();
    intToChar(next_member_id++, nodo->id);
    copiarCadena(nodo->nombre, n);
    copiarCadena(nodo->fechaNacimiento, fn);
    copiarCadena(nodo->titulo, t);
    copiarCadena(nodo->etnia, e);
    nodo->padre = nullptr;
    nodo->madre = nullptr;
    nodo->hijoPrimero = nullptr;
    nodo->siguienteHermano = nullptr;
    nodo->visitado = false;
    return nodo;
}

//informacion del miembro 
void imprimirDetalles(Nodo* nodo) {
    if (nodo == nullptr) return;
    cout << "---------------------------------\n";
    cout << "ID: " << nodo->id << "\n";
    cout << "Nombre: " << nodo->titulo << " " << nodo->nombre << "\n";
    cout << "Nacimiento: " << nodo->fechaNacimiento << "\n";
    cout << "Etnia: " << nodo->etnia << "\n";
    if (nodo->padre) cout << "Padre: " << nodo->padre->nombre << "\n";
    if (nodo->madre) cout << "Madre: " << nodo->madre->nombre << "\n";
}

// Búsqueda

//revursiva
Nodo* buscarNodoRec(Nodo* nodo, const char* nombreBuscar) {
    if (nodo == NULL) return NULL;
    if (nodo->visitado) return NULL;
    nodo->visitado = true;

    if (compararCadenas(nodo->nombre, nombreBuscar)) return nodo;

    Nodo* encontrado = buscarNodoRec(nodo->hijoPrimero, nombreBuscar);
    if (encontrado) return encontrado;

    encontrado = buscarNodoRec(nodo->siguienteHermano, nombreBuscar);
    if (encontrado) return encontrado;

    encontrado = buscarNodoRec(nodo->padre, nombreBuscar);
    if (encontrado) return encontrado;

    encontrado = buscarNodoRec(nodo->madre, nombreBuscar);
    if (encontrado) return encontrado;

    return NULL;
}

void limpiarVisitadosRec(Nodo* nodo) {
    if (nodo == NULL) return;
    if (!nodo->visitado) return;
    nodo->visitado = false;
    limpiarVisitadosRec(nodo->hijoPrimero);
    limpiarVisitadosRec(nodo->siguienteHermano);
    limpiarVisitadosRec(nodo->padre);
    limpiarVisitadosRec(nodo->madre);
}

Nodo* buscarNodo(Nodo* raiz, const char* nombreBuscar) {
    Nodo* res = buscarNodoRec(raiz, nombreBuscar);
    limpiarVisitadosRec(raiz);
    return res;
}

// Funciones para agregar con modulos?

// Agrega nuevoNodo como hijo del progenitor 
void agregarHijoAProgenitor(Nodo* progenitor, Nodo* nuevoNodo) {
    if (progenitor->hijoPrimero == NULL) {
        progenitor->hijoPrimero = nuevoNodo;
    } else {
        Nodo* temp = progenitor->hijoPrimero;
        while (temp->siguienteHermano != NULL) temp = temp->siguienteHermano;
        temp->siguienteHermano = nuevoNodo;
    }
}

// Agregar como hijo: 
void agregarComoHijo(Nodo* raiz, Nodo* nuevoNodo, const char* nombreProgenitor, char rol) {
    Nodo* progenitor = buscarNodo(raiz, nombreProgenitor);
    if (progenitor == NULL) {
        cout << "error progenitor '" << nombreProgenitor << "' no encontrado\n";
        delete nuevoNodo;
        return;
    }
    if (rol == 'P' || rol == 'p') {
        if (nuevoNodo->padre == NULL) {
            nuevoNodo->padre = progenitor;
            agregarHijoAProgenitor(progenitor, nuevoNodo);
            cout << "exito agregado como hijo (padre: " << progenitor->nombre << ") [ID: " << nuevoNodo->id << "]\n";
        } else {
            cout << "error el miembro ya tiene padre\n";
            delete nuevoNodo;
        }
    } else if (rol == 'M' || rol == 'm') {
        if (nuevoNodo->madre == NULL) {
            nuevoNodo->madre = progenitor;
            agregarHijoAProgenitor(progenitor, nuevoNodo);
            cout << "exito agregado como hijo (madre: " << progenitor->nombre << ") [ID: " << nuevoNodo->id << "]\n";
        } else {
            cout << "error el miembro ya tiene madre\n";
            delete nuevoNodo;
        }
    } else {
        cout << "error rol invalido\n";
        delete nuevoNodo;
    }
}

// Agregar como progenitor: nuevoNodo se asigna como padre/madre de 'hijo'
void agregarComoProgenitor(Nodo* raiz, Nodo* nuevoNodo, const char* nombreHijo, char rol) {
    Nodo* hijo = buscarNodo(raiz, nombreHijo);
    if (hijo == NULL) {
        cout << "error hijo '" << nombreHijo << "' no encontrado\n";
        delete nuevoNodo;
        return;
    }
    if (rol == 'P' || rol == 'p') {
        if (hijo->padre == NULL) {
            hijo->padre = nuevoNodo;
            cout << "exitoso agregado como padre de " << hijo->nombre << " [ID: " << nuevoNodo->id << "]\n";
        } else {
            cout << "error " << hijo->nombre << " ya tiene padre: " << hijo->padre->nombre << "\n";
            delete nuevoNodo;
        }
    } else if (rol == 'M' || rol == 'm') {
        if (hijo->madre == NULL) {
            hijo->madre = nuevoNodo;
            cout << "exito agregado como madre de " << hijo->nombre << " [ID: " << nuevoNodo->id << "]\n";
        } else {
            cout << "error: " << hijo->nombre << " ya tiene madre: " << hijo->madre->nombre << "\n";
            delete nuevoNodo;
        }
    } else {
        cout << "error el rol es invalido \n";
        delete nuevoNodo;
    }
}

// Función principal de agregar 

void agregarMiembro(Nodo* raiz) {
    char nNombre[MAX_STR_LEN], nFechaNac[MAX_STR_LEN];
    char nTitulo[MAX_STR_LEN], nEtnia[MAX_STR_LEN];
    char opcionRel[MAX_STR_LEN], nombreRelacionado[MAX_STR_LEN], rolRelacion[4];

    cout << "\n--- AGREGAR NUEVO MIEMBRO ---\n";
    cout << "Nombre: "; obtenerLinea(nNombre);
    if (nNombre[0] == '\0') { cout << "Nnombre vacio \n"; return; }
    cout << "fecha de nacimiento: "; obtenerLinea(nFechaNac);
    cout << "titulo (Don/Doña): "; obtenerLinea(nTitulo);
    cout << "etnia: "; obtenerLinea(nEtnia);

    Nodo* nuevoNodo = crearNodo(nNombre, nFechaNac, nTitulo, nEtnia);

    cout << "\nselecciona: [H] hijo@  |  [P] padre o madre\n";
    cout << "Opcion (H/P): "; obtenerLinea(opcionRel);
    char tipo = opcionRel[0];

    if (tipo == 'H' || tipo == 'h') {
        cout << "nombre del antecesor existente: "; obtenerLinea(nombreRelacionado);
        cout << "indica si es 'P' (padre) o 'M' (madre): "; obtenerLinea(rolRelacion);
        agregarComoHijo(raiz, nuevoNodo, nombreRelacionado, rolRelacion[0]);
    } else if (tipo == 'P' || tipo == 'p') {
        cout << "nombre del hijo existente: "; obtenerLinea(nombreRelacionado);
        cout << "indica si sera 'P' (padre) o 'M' (madre): "; obtenerLinea(rolRelacion);
        agregarComoProgenitor(raiz, nuevoNodo, nombreRelacionado, rolRelacion[0]);
    } else {
        cout << "Opcion incorrecta \n";
        delete nuevoNodo;
    }
}

// Visualización del árbol

void mostrarArbolRecursivo(Nodo* nodo, int nivel, const char* prefijo) {
    if (nodo == NULL) return;
    if (nodo->visitado) return;
    nodo->visitado = true;

    for (int i = 0; i < nivel; ++i) cout << "    ";
    cout << prefijo << nodo->titulo << " " << nodo->nombre
         << " (" << nodo->fechaNacimiento << ") [ID:" << nodo->id << "]\n";

// Mostrar hijos
    if (nodo->hijoPrimero != NULL) {
        mostrarArbolRecursivo(nodo->hijoPrimero, nivel + 1, "H: ");
    }
// Mostrar padre y madre
    if (nodo->padre != NULL) {
        mostrarArbolRecursivo(nodo->padre, nivel + 1, "P: ");
    }
    if (nodo->madre != NULL) {
        mostrarArbolRecursivo(nodo->madre, nivel + 1, "M: ");
    }
//mostrar hermanos
    if (nodo->siguienteHermano != NULL) {
        mostrarArbolRecursivo(nodo->siguienteHermano, nivel, "");
    }
}

void visualizarArbol(Nodo* RAIZ) {
    cout << "\n--- ARBOL GENEALOGICO (VISUAL) ---\n";
    mostrarArbolRecursivo(RAIZ, 0, "RAIZ: ");
    limpiarVisitadosRec(RAIZ); // limpiar 
}

// Listado completo (usa visitado) 

void listarDetallesRec(Nodo* nodo) {
    if (nodo == NULL) return;
    if (nodo->visitado) return;
    nodo->visitado = true;

    imprimirDetalles(nodo);
    listarDetallesRec(nodo->padre);
    listarDetallesRec(nodo->madre);
    listarDetallesRec(nodo->hijoPrimero);
    listarDetallesRec(nodo->siguienteHermano);
}

void mostrarMiembros(Nodo* RAIZ) {
    cout << "\n----- LISTA COMPLETA DE MIEMBROS -----\n";
    listarDetallesRec(RAIZ);
    limpiarVisitadosRec(RAIZ);
}

//  Funciones de búsqueda por menú 

void buscarMiembro(Nodo* RAIZ) {
    char nombreBuscar[MAX_STR_LEN];
    cout << "\n----- BUSCAR MIEMBRO -----\n";
    cout << "nombre para buscar : ";
    obtenerLinea(nombreBuscar);
    if (nombreBuscar[0] == '\0') { cout << "debes ingresar un nombre\n"; return; }
    Nodo* resultado = buscarNodo(RAIZ, nombreBuscar);
    if (resultado != NULL) {
        cout << "miembro encontrado:\n";
        imprimirDetalles(resultado);
    } else {
        cout << "no se encuentra al mienbro \n";
    }
}

// ejmplod de árbol inicial

Nodo* crearArbolBorjaLoyolaInca() {
    Nodo* Francisco = crearNodo("FRANCISCO", "1510", "Don", "Virreinal");
    Nodo* Juana = crearNodo("JUANA", "1520", "Doña", "Inca");
    // relacion simple inicial
    Francisco->madre = Juana;
    return Francisco;
}

// menu

int main() {
    setlocale(LC_ALL, "spanish");
    Nodo* RAIZ = crearArbolBorjaLoyolaInca();

    int opcion = 0;
    cout << "  SISTEMA DE ARBOL GENEALOGICO - Familia Borja-Loyola-Inca\n";
    cout << "  NOTA: usa nombres exactos para la busqueda\n";

    do {
        cout << "\nMENU PRINCIPAL\n";
        cout << "1. Agregar Miembro\n";
        cout << "2. Mostrar Todos los Miembros\n";
        cout << "3. Buscar Miembro\n";
        cout << "4. Visualizar Arbol Genealogico\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            limpiarBuffer();
            opcion = 0;
        } else {
            limpiarBuffer();
        }

        switch (opcion) {
            case 1: agregarMiembro(RAIZ); break;
            case 2: mostrarMiembros(RAIZ); break;
            case 3: buscarMiembro(RAIZ); break;
            case 4: visualizarArbol(RAIZ); break;
            case 5: cout << "Hasta pronto\n"; break;
            default:
                if (opcion != 0) cout << "fallaste vuelve a intentarlo \n";
        }
    } while (opcion != 5);

    return 0;
}

