#include <iostream>

using namespace std;

// Definición de tamaño máximo para las cadenas 
#define MAX_STR_LEN 30

// ESTRUCTURA DEL NODO 
struct Nodo {
    char id[MAX_STR_LEN];
    char nombre[MAX_STR_LEN];
    char fechaNacimiento[MAX_STR_LEN];
    char titulo[MAX_STR_LEN];
    char etnia[MAX_STR_LEN];
    Nodo* padre;
    Nodo* madre;
};

// FUNCIONES ESENCIALES Y AUXILIARES

void limpiarBuffer() { 
    // Limpieza simple del buffer de entrada después de cin
    cin.ignore(MAX_STR_LEN, '\n');
}

void obtenerLinea(char buffer[]) {
    // Simula getline: Lee una línea completa y limpia el buffer
    cin.get(buffer, MAX_STR_LEN, '\n'); 
    cin.ignore(MAX_STR_LEN, '\n'); 
}

// Función auxiliar para comparar cadenas (sustituye a <cstring>::strcmp)
bool compararCadenas(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i]; 
}

// Función auxiliar para copiar cadenas (sustituye a <cstring>::strcpy)
void copiarCadena(char* destino, const char* origen) {
    int i = 0;
    while ((destino[i] = origen[i]) != '\0') {
        i++;
    }
}

// Función para imprimir todos los detalles de un miembro
void imprimirDetalles(Nodo* nodo) {
    cout << "---------------------------------\n";
    cout << "ID: " << nodo->id << endl;
    cout << "Nombre: " << nodo->titulo << " " << nodo->nombre << endl;
    cout << "Nacimiento: " << nodo->fechaNacimiento << endl;
    cout << "Etnia: " << nodo->etnia << endl;
    cout << "---------------------------------\n";
}

// Función para inicializar un nuevo nodo
Nodo* crearNodo(const char* i, const char* n, const char* fn, const char* t, const char* e) {
    Nodo* nodo = new Nodo();
    copiarCadena(nodo->id, i);
    copiarCadena(nodo->nombre, n);
    copiarCadena(nodo->fechaNacimiento, fn);
    copiarCadena(nodo->titulo, t);
    copiarCadena(nodo->etnia, e);
    nodo->padre = NULL;
    nodo->madre = NULL;
    return nodo;
}

// ÚNICA FUNCIÓN DE BÚSQUEDA 
Nodo* buscarNodo(Nodo* nodo, const char* nombreBuscar) {
    if (nodo == NULL) return NULL;
    
    // Búsqueda simplificada: debe coincidir el nombre exacto
    if (compararCadenas(nodo->nombre, nombreBuscar)) {
        return nodo;
    }

    Nodo* encontrado = buscarNodo(nodo->padre, nombreBuscar);
    if (encontrado != NULL) return encontrado;

    return buscarNodo(nodo->madre, nombreBuscar);
}


// 1. FUNCIONALIDAD PRINCIPAL: AGREGAR MIEMBRO
void agregarMiembro(Nodo* RAIZ) {
    char nId[MAX_STR_LEN], nNombre[MAX_STR_LEN], nFechaNac[MAX_STR_LEN];
    char nTitulo[MAX_STR_LEN], nEtnia[MAX_STR_LEN], nombreAncestro[MAX_STR_LEN];
    char relacion[2];
    
    cout << "\n--- 1. AGREGAR NUEVO MIEMBRO ---\n";
    
    cout << "ID: "; obtenerLinea(nId);
    cout << "Nombre: "; obtenerLinea(nNombre);
    cout << "Nacimiento: "; obtenerLinea(nFechaNac);
    cout << "Titulo: "; obtenerLinea(nTitulo);
    cout << "Etnia: "; obtenerLinea(nEtnia);

    Nodo* nuevoNodo = crearNodo(nId, nNombre, nFechaNac, nTitulo, nEtnia);
    
    cout << "Ancestro: "; obtenerLinea(nombreAncestro);
    Nodo* ancestro = buscarNodo(RAIZ, nombreAncestro);

    if (ancestro == NULL) {
        cout << "Error: Ancestro no encontrado. Miembro no insertado.\n"; 
        delete nuevoNodo; return;
    }

    cout << "Conectar a [P]adre/[M]adre: "; cin >> relacion; limpiarBuffer();
    
    // Lógica de Inserción Simplificada (Usando puntero a puntero conceptual)
    Nodo** punteroDestino = NULL; 
    char opcion = relacion[0];

    if (opcion == 'P' || opcion == 'p') {
        punteroDestino = &ancestro->padre;
    } else if (opcion == 'M' || opcion == 'm') {
        punteroDestino = &ancestro->madre;
    }
    
    if (punteroDestino == NULL) {
        cout << "Error: Opcion no valida.\n";
        delete nuevoNodo;
        return;
    }

    if (*punteroDestino == NULL) {
        *punteroDestino = nuevoNodo;
        cout << nNombre << " agregado a " << ancestro->nombre << ".\n";
    } else {
        cout << "Error: Posicion ocupada por " << (*punteroDestino)->nombre << ".\n";
        delete nuevoNodo;
    }
}

// 2. FUNCIONALIDAD RESTAURADA: MOSTRAR MIEMBROS
void listarDetalles(Nodo* nodo) {
    if (nodo != NULL) {
        imprimirDetalles(nodo); 
        listarDetalles(nodo->padre); 
        listarDetalles(nodo->madre);
    }
}

void mostrarMiembros(Nodo* RAIZ) {
    cout << "\n--- 2. LISTA COMPLETA DE MIEMBROS ---\n";
    listarDetalles(RAIZ);
}

// 3. FUNCIONALIDAD RESTAURADA: BUSCAR MIEMBRO
void buscarMiembro(Nodo* RAIZ) {
    char nombreBuscar[MAX_STR_LEN];
    cout << "\n--- 3. BUSCAR MIEMBRO ---\n";
    cout << "Nombre a buscar: ";
    obtenerLinea(nombreBuscar);

    Nodo* resultado = buscarNodo(RAIZ, nombreBuscar);

    if (resultado != NULL) {
        cout << "\n¡Miembro Encontrado!\n";
        imprimirDetalles(resultado);
    } else {
        cout << "\nMiembro no encontrado.\n";
    }
}

// CREACIÓN DEL ÁRBOL INICIAL Y MAIN
Nodo* crearArbolBorjaLoyolaInca() {
    Nodo* FranciscoBorja = crearNodo("B1", "Francisco de Borja", "1510", "Don", "Virreinal");
    Nodo* Carlos = crearNodo("M2.1", "Carlos de Borja", "1540", "Don", "Mestizo");
    
    FranciscoBorja->padre = Carlos; 
    
    return FranciscoBorja;
}

int main() {
    Nodo* RAIZ = crearArbolBorjaLoyolaInca(); 
    int opcion;

    cout << "ADVERTENCIA: La busqueda y la insercion requieren el nombre exacto." << endl;

    do {
        cout << "\n==============================\n";
        cout << " ARBOL GENEALOGICO COMPLETO\n";
        cout << "==============================\n";
        cout << "1. Agregar Miembro\n";
        cout << "2. Mostrar Miembros\n";
        cout << "3. Buscar Miembro\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { 
            cin.clear(); limpiarBuffer(); opcion = 0; 
        } else { 
            cout << "\n"; 
        }

        switch (opcion) {
            case 1: agregarMiembro(RAIZ); break;
            case 2: mostrarMiembros(RAIZ); break;
            case 3: buscarMiembro(RAIZ); break;
            
            case 4: cout << "Saliendo del programa.\n"; break; 
            
            default: if (opcion != 0) cout << "Opcion no valida.\n"; break;
        }
    } while (opcion != 4);

    return 0;
}
