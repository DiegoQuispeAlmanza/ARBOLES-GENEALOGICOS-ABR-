#include <iostream>
#include <locale.h>
#include <cstdio>
#include <cstring>

using namespace std;

#define MAX_STR_LEN 64
int next_member_id = 1;

void limpiarBuffer() {
    cin.ignore(10000, '\n');
}

void obtenerLinea(char buffer[]) {
    cin.getline(buffer, MAX_STR_LEN);
    if (cin.fail()) {
        cin.clear();
        limpiarBuffer();
        buffer[0] = '\0';
    }
}

bool compararCadenas(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

void copiarCadena(char* destino, const char* origen) {
    int i = 0;
    while ((destino[i] = origen[i]) != '\0') {
        i++;
    }
}

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

Nodo* crearNodo(const char* n, const char* fn, const char* t, const char* e) {
    Nodo* nodo = new Nodo();
    intToChar(next_member_id++, nodo->id); 
    copiarCadena(nodo->nombre, n);
    copiarCadena(nodo->fechaNacimiento, fn);
    copiarCadena(nodo->titulo, t);
    copiarCadena(nodo->etnia, e);
    nodo->padre = NULL;
    nodo->madre = NULL;
    nodo->hijoPrimero = NULL;
    nodo->siguienteHermano = NULL;
    nodo->visitado = false;
    return nodo;
}

void imprimirDetalles(Nodo* nodo) {
    if (nodo == NULL) return;
    cout << "---------------------------------\n";
    cout << "ID: " << nodo->id << "\n";
    cout << "Nombre: " << nodo->titulo << " " << nodo->nombre << "\n";
    cout << "Nacimiento: " << nodo->fechaNacimiento << "\n";
    cout << "Etnia: " << nodo->etnia << "\n";
    if (nodo->padre) cout << "Padre: " << nodo->padre->nombre << " (ID: " << nodo->padre->id << ")\n";
    if (nodo->madre) cout << "Madre: " << nodo->madre->nombre << " (ID: " << nodo->madre->id << ")\n";
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

Nodo* buscarNodoRec(Nodo* nodo, const char* nombreBuscar) {
    if (nodo == NULL || nodo->visitado) return NULL;
    nodo->visitado = true;

    if (compararCadenas(nodo->nombre, nombreBuscar)) return nodo;

    Nodo* encontrado = NULL;

    encontrado = buscarNodoRec(nodo->hijoPrimero, nombreBuscar);
    if (encontrado) return encontrado;

    encontrado = buscarNodoRec(nodo->siguienteHermano, nombreBuscar);
    if (encontrado) return encontrado;

    encontrado = buscarNodoRec(nodo->padre, nombreBuscar);
    if (encontrado) return encontrado;

    encontrado = buscarNodoRec(nodo->madre, nombreBuscar);
    if (encontrado) return encontrado;

    return NULL;
}

Nodo* buscarNodo(Nodo* raiz, const char* nombreBuscar) {
    Nodo* res = buscarNodoRec(raiz, nombreBuscar);
    limpiarVisitadosRec(raiz); 
    return res;
}

void agregarHijoAProgenitor(Nodo* progenitor, Nodo* nuevoNodo) {
    if (progenitor->hijoPrimero == NULL) {
        progenitor->hijoPrimero = nuevoNodo;
    } else {
        Nodo* temp = progenitor->hijoPrimero;
        while (temp->siguienteHermano != NULL) temp = temp->siguienteHermano;
        temp->siguienteHermano = nuevoNodo;
    }
}

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
            agregarHijoAProgenitor(nuevoNodo, hijo); 
            cout << "exitoso agregado como padre de " << hijo->nombre << " [ID: " << nuevoNodo->id << "]\n";
        } else {
            cout << "error " << hijo->nombre << " ya tiene padre: " << hijo->padre->nombre << "\n";
            delete nuevoNodo;
            return;
        }
    } else if (rol == 'M' || rol == 'm') {
        if (hijo->madre == NULL) {
            hijo->madre = nuevoNodo;
            agregarHijoAProgenitor(nuevoNodo, hijo); 
            cout << "exito agregado como madre de " << hijo->nombre << " [ID: " << nuevoNodo->id << "]\n";
        } else {
            cout << "error: " << hijo->nombre << " ya tiene madre: " << hijo->madre->nombre << "\n";
            delete nuevoNodo;
            return;
        }
    } else {
        cout << "error el rol es invalido \n";
        delete nuevoNodo;
        return;
    }
}

void agregarMiembro(Nodo* raiz) {
    char nNombre[MAX_STR_LEN], nFechaNac[MAX_STR_LEN];
    char nTitulo[MAX_STR_LEN], nEtnia[MAX_STR_LEN];
    char opcionRel[MAX_STR_LEN], nombreRelacionado[MAX_STR_LEN], rolRelacion[4];

    cout << "\n--- agregar nuevo miembro ---\n";
    cout << "nombre: "; obtenerLinea(nNombre);
    if (nNombre[0] == '\0') { cout << "nombre vacio\n"; return; }
    cout << "fecha de nacimiento: "; obtenerLinea(nFechaNac);
    cout << "titulo (don/do?a): "; obtenerLinea(nTitulo); 
    cout << "etnia: "; obtenerLinea(nEtnia);

    Nodo* nuevoNodo = crearNodo(nNombre, nFechaNac, nTitulo, nEtnia);

    cout << "\nselecciona: [H] hij@ | [P] padre o madre\n";
    cout << "opcion (H/P): "; obtenerLinea(opcionRel);
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
        cout << "opcion incorrecta\n";
        delete nuevoNodo;
    }
}

// Visualizacion recursiva pura
void mostrarArbolRecursivo(Nodo* nodo, int nivel) {
    if (nodo == NULL) return;
    if (nodo->visitado) return;
    nodo->visitado = true;

    for (int i = 0; i < nivel; ++i) {
        cout << "    "; 
    }
    
    if (nivel > 0) {
        cout << "+-- "; 
    }
    
    cout << nodo->titulo << " " << nodo->nombre
              << " (" << nodo->fechaNacimiento << ") [ID:" << nodo->id << "]\n";

    // Recorre todos los hijos
    Nodo* hijoActual = nodo->hijoPrimero;
    while (hijoActual != NULL) {
        mostrarArbolRecursivo(hijoActual, nivel + 1);
        hijoActual = hijoActual->siguienteHermano;
    }
}

// Encuentra el nodo mas alto (la verdadera raiz visual)
Nodo* encontrarRaizSuperior(Nodo* nodo) {
    if (nodo == NULL) return NULL;
    // Mientras tenga padre o madre, subimos
    while (nodo->padre != NULL || nodo->madre != NULL) {
        if (nodo->padre != NULL) {
            nodo = nodo->padre;
        } else {
            nodo = nodo->madre;
        }
    }
    return nodo;
}

void visualizarArbol(Nodo* nodoInicial) {
    cout << "\n--- arbol genealogico (visual) ---\n";
    
    // Primero buscamos quien es el jefe supremo actual del arbol
    // (en caso de que hayamos agregado un padre al nodo inicial)
    Nodo* raizReal = encontrarRaizSuperior(nodoInicial);
    
    cout << "el arbol se muestra desde: " << raizReal->nombre << "\n";
    mostrarArbolRecursivo(raizReal, 0); 
    limpiarVisitadosRec(raizReal); 
}

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
    cout << "\n----- lista completa de miembros -----\n";
    listarDetallesRec(RAIZ);
    limpiarVisitadosRec(RAIZ);
}

void buscarMiembro(Nodo* RAIZ) {
    char nombreBuscar[MAX_STR_LEN];
    cout << "\n----- buscar miembro -----\n";
    cout << "nombre para buscar: ";
    obtenerLinea(nombreBuscar);
    if (nombreBuscar[0] == '\0') { cout << "debes ingresar un nombre\n"; return; }
    Nodo* resultado = buscarNodo(RAIZ, nombreBuscar);
    if (resultado != NULL) {
        cout << "miembro encontrado:\n";
        imprimirDetalles(resultado);
    } else {
        cout << "no se encuentra al miembro\n";
    }
}

Nodo* crearArbolBorjaLoyolaInca() {
    Nodo* Francisco = crearNodo("FRANCISCO", "1510", "Don", "Virreinal");
    Nodo* Juana = crearNodo("JUANA", "1520", "Do?a", "Inca"); 
    Nodo* Pedro = crearNodo("PEDRO", "1540", "Don", "Virreinal");
    Nodo* Maria = crearNodo("MARIA", "1542", "Do?a", "Virreinal"); 
    Nodo* Juan = crearNodo("JUAN", "1565", "Don", "Mestizo");
    Nodo* Ana = crearNodo("ANA", "1568", "Do?a", "Mestiza"); 
    Nodo* Carlos = crearNodo("CARLOS", "1570", "Don", "Mestizo");
    Nodo* Nieta = crearNodo("NINA", "1575", "Do?a", "Mestiza"); 

    Francisco->madre = Juana;
    agregarHijoAProgenitor(Juana, Francisco); 

    agregarHijoAProgenitor(Francisco, Pedro);
    agregarHijoAProgenitor(Francisco, Maria);
    
    agregarHijoAProgenitor(Pedro, Juan);
    agregarHijoAProgenitor(Pedro, Nieta); 

    agregarHijoAProgenitor(Maria, Ana);
    agregarHijoAProgenitor(Maria, Carlos);
    
    Pedro->padre = Francisco;
    Maria->padre = Francisco;

    Juan->padre = Pedro;
    Nieta->padre = Pedro; 
    
    Ana->madre = Maria;
    Carlos->madre = Maria;

    return Francisco; 
}

int main() {
    setlocale(LC_ALL, "Spanish");
    
    Nodo* RAIZ = crearArbolBorjaLoyolaInca();

    int opcion = 0;
    cout << "  sistema de arbol genealogico - familia borja-loyola-inca\n";
    cout << "  nota: usa nombres exactos para la busqueda\n";

    do {
        cout << "\nmenu principal\n";
        cout << "1. agregar miembro\n";
        cout << "2. mostrar todos los miembros\n";
        cout << "3. buscar miembro\n";
        cout << "4. visualizar arbol genealogico\n";
        cout << "5. salir\n";
        cout << "seleccione una opcion: ";

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
            case 5: cout << "hasta pronto\n"; break;
            default:
                if (opcion != 0) cout << "fallaste vuelve a intentarlo\n";
        }
    } while (opcion != 5);

    return 0;
}
