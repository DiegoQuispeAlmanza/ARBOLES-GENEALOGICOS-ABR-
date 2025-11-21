#include <iostream>
#include <locale.h>
using namespace std;

#define MAX_STR_LEN 30

int next_member_id = 1;

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
};

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
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    buffer[j] = '\0';
}

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
    return nodo;
}

void imprimirDetalles(Nodo* nodo) {
    cout << "ID: " << nodo->id << endl;
    cout << "Nombre: " << nodo->titulo << " " << nodo->nombre << endl;
    cout << "Nacimiento: " << nodo->fechaNacimiento << endl;
    cout << "Etnia: " << nodo->etnia << endl;
    if (nodo->padre != NULL) {
        cout << "Padre: " << nodo->padre->nombre << endl;
    }
    if (nodo->madre != NULL) {
        cout << "Madre: " << nodo->madre->nombre << endl;
    }
}

Nodo* buscarNodo(Nodo* nodo, const char* nombreBuscar) {
    if (nodo == NULL) return NULL;
    if (compararCadenas(nodo->nombre, nombreBuscar)) {
        return nodo;
    }
    // Buscar en padres
    Nodo* encontrado = buscarNodo(nodo->padre, nombreBuscar);
    if (encontrado != NULL) return encontrado;
    
    // Buscar en madres
    encontrado = buscarNodo(nodo->madre, nombreBuscar);
    if (encontrado != NULL) return encontrado;
    
    // Buscar en hijos
    encontrado = buscarNodo(nodo->hijoPrimero, nombreBuscar);
    if (encontrado != NULL) return encontrado;
    
    // Buscar en hermanos
    return buscarNodo(nodo->siguienteHermano, nombreBuscar);
}

void mostrarArbolRecursivo(Nodo* nodo, int nivel, const char* prefijo) {
    if (nodo == NULL) return;
    for (int i = 0; i < nivel; i++) {
        cout << "    ";
    }
    cout << prefijo << nodo->titulo << " " << nodo->nombre 
         << " (" << nodo->fechaNacimiento << ") [ID:" << nodo->id << "]\n";
    
    // Mostrar hijos
    if (nodo->hijoPrimero != NULL) {
        mostrarArbolRecursivo(nodo->hijoPrimero, nivel + 1, "H: ");
    }
    
    // Mostrar padre
    if (nodo->padre != NULL) {
        mostrarArbolRecursivo(nodo->padre, nivel + 1, "P: ");
    }
    
    // Mostrar madre
    if (nodo->madre != NULL) {
        mostrarArbolRecursivo(nodo->madre, nivel + 1, "M: ");
    }
    
    // Mostrar hermanos
    if (nodo->siguienteHermano != NULL) {
        mostrarArbolRecursivo(nodo->siguienteHermano, nivel, "");
    }
}

void visualizarArbol(Nodo* RAIZ) {
    cout << "   ARBOL GENEALOGICO - VISUALIZACION      \n";
    mostrarArbolRecursivo(RAIZ, 0, "RAIZ: ");
}

void listarDetalles(Nodo* nodo) {
    if (nodo != NULL) {
        imprimirDetalles(nodo); 
        listarDetalles(nodo->padre); 
        listarDetalles(nodo->madre);
        listarDetalles(nodo->hijoPrimero);
        listarDetalles(nodo->siguienteHermano);
    }
}

void mostrarMiembros(Nodo* RAIZ) {
    cout << "\n----- LISTA COMPLETA DE MIEMBROS -----\n";
    listarDetalles(RAIZ);
}

void agregarMiembro(Nodo* RAIZ) {
    char nNombre[MAX_STR_LEN], nFechaNac[MAX_STR_LEN];
    char nTitulo[MAX_STR_LEN], nEtnia[MAX_STR_LEN], nombreRelacionado[MAX_STR_LEN];
    char tipoRelacion[10], rolRelacion[10];
    
    cout << "\nAGREGAR NUEVO MIEMBRO\n";
    
    cout << "\nDatos del nuevo miembro:\n";
    cout << "Nombre: "; 
    obtenerLinea(nNombre);
    
    if (nNombre[0] == '\0') {
        cout << "Error: Nombre vacio\n";
        return;
    }
    
    cout << "Fecha de Nacimiento: "; obtenerLinea(nFechaNac);
    cout << "Titulo (Don/Doña): "; obtenerLinea(nTitulo);
    cout << "Etnia: "; obtenerLinea(nEtnia);
    
    Nodo* nuevoNodo = crearNodo(nNombre, nFechaNac, nTitulo, nEtnia);
    
    cout << "Seleccione el tipo de relacion:\n\n";
    cout << "[H] " << nNombre << " es el hijo o hija de alguien existente\n";
    cout << "    (agregara a " << nNombre << " como descendiente)\n\n";
    cout << "[P] " << nNombre << " es el padre o madre de alguien existente\n";
    cout << "    (agregara a " << nNombre << " como ancestro)\n\n";
    cout << "Ingrese opcion (H/P): ";
    obtenerLinea(tipoRelacion);
    
    char tipo = tipoRelacion[0];
    
    if (tipo == 'H' || tipo == 'h') {
        // OPCION 1: El nuevo miembro es HIJO de alguien existente
        cout << "\n--- AGREGAR COMO HIJO/A ---\n";
        cout << "Nombre del padre o madre existente: "; 
        obtenerLinea(nombreRelacionado);
        
        Nodo* progenitor = buscarNodo(RAIZ, nombreRelacionado);
        
        if (progenitor == NULL) {
            cout << "\nError: mienbro '" << nombreRelacionado << "' no encontrado.\n"; 
            delete nuevoNodo; 
            return;
        }
        
        cout << "\n" << nombreRelacionado << " es el o la:\n";
        cout << "'P' padre de " << nNombre << "\n";
        cout << "'M' madre de " << nNombre << "\n";
        cout << "Selecciona (P o M): ";
        obtenerLinea(rolRelacion);
        
        char rol = rolRelacion[0];
        
        if (rol == 'P' || rol == 'p') {
            if (nuevoNodo->padre == NULL) {
                nuevoNodo->padre = progenitor;
                // Agregar como hijo del progenitor
                if (progenitor->hijoPrimero == NULL) {
                    progenitor->hijoPrimero = nuevoNodo;
                } else {
                    Nodo* temp = progenitor->hijoPrimero;
                    while (temp->siguienteHermano != NULL) {
                        temp = temp->siguienteHermano;
                    }
                    temp->siguienteHermano = nuevoNodo;
                }
                cout << "\n? Exito: " << nNombre << " agregado como HIJO de " 
                     << progenitor->nombre << " (linea paterna) [ID: " << nuevoNodo->id << "]\n";
            } else {
                cout << "\nError: " << nNombre << " ya tiene padre asignado.\n";
                delete nuevoNodo;
            }
        } else if (rol == 'M' || rol == 'm') {
            if (nuevoNodo->madre == NULL) {
                nuevoNodo->madre = progenitor;
                // Agregar como hijo del progenitor
                if (progenitor->hijoPrimero == NULL) {
                    progenitor->hijoPrimero = nuevoNodo;
                } else {
                    Nodo* temp = progenitor->hijoPrimero;
                    while (temp->siguienteHermano != NULL) {
                        temp = temp->siguienteHermano;
                    }
                    temp->siguienteHermano = nuevoNodo;
                }
                cout << "\n? Exito: " << nNombre << " agregado como hijo de " 
                     << progenitor->nombre << " (linea materna) [ID: " << nuevoNodo->id << "]\n";
            } else {
                cout << "\nError: " << nNombre << " ya tiene una madre\n";
                delete nuevoNodo;
            }
        } else {
            cout << "\nError: Opcion incorrecta, Usa 'P' o 'M'\n";
            delete nuevoNodo;
        }
        
    } else if (tipo == 'P' || tipo == 'p') {
        // OPCION 2: El nuevo miembro es PADRE/MADRE de alguien existente
        cout << "\n--- AGREGAR COMO PADRE/MADRE ---\n";
        cout << "Nombre del hijo o hija existente: "; 
        obtenerLinea(nombreRelacionado);
        
        Nodo* hijo = buscarNodo(RAIZ, nombreRelacionado);
        
        if (hijo == NULL) {
            cout << "\nError: Miembro '" << nombreRelacionado << "' no encontrado.\n"; 
            delete nuevoNodo; 
            return;
        }
        
        cout << "\n" << nNombre << " es el/la:\n";
        cout << "'P' PADRE de " << hijo->nombre << "\n";
        cout << "'M' MADRE de " << hijo->nombre << "\n";
        cout << "Seleccione (P o M): ";
        obtenerLinea(rolRelacion);
        
        char rol = rolRelacion[0];
        
        if (rol == 'P' || rol == 'p') {
            if (hijo->padre == NULL) {
                hijo->padre = nuevoNodo;
                cout << "\n? Exito: " << nNombre << " agregado como padre de " 
                     << hijo->nombre << " [ID: " << nuevoNodo->id << "]\n";
            } else {
                cout << "\nError: " << hijo->nombre << " ya tiene padre: " 
                     << hijo->padre->nombre << "\n";
                delete nuevoNodo;
            }
        } else if (rol == 'M' || rol == 'm') {
            if (hijo->madre == NULL) {
                hijo->madre = nuevoNodo;
                cout << "\n? Exito: " << nNombre << " agregado como madre de " 
                     << hijo->nombre << " [ID: " << nuevoNodo->id << "]\n";
            } else {
                cout << "\nError: " << hijo->nombre << " ya tiene madre: " 
                     << hijo->madre->nombre << "\n";
                delete nuevoNodo;
            }
        } else {
            cout << "\nError: Opcion incorrecta, Usa 'P' o 'M'.\n";
            delete nuevoNodo;
        }
        
    } else {
        cout << "\nError: Opcion incorrecta, Usa 'H' para hijo o 'P' para padre/madre\n";
        delete nuevoNodo;
    }
}

void buscarMiembro(Nodo* RAIZ) {
    char nombreBuscar[MAX_STR_LEN];
    cout << "\n--- BUSCAR MIEMBRO ---\n";
    cout << "Nombre a buscar: ";
    obtenerLinea(nombreBuscar);
    
    if (nombreBuscar[0] == '\0') {
        cout << "\n mal, Debes ingresar un nombre\n";
        return;
    }
    
    Nodo* resultado = buscarNodo(RAIZ, nombreBuscar);
    if (resultado != NULL) {
        cout << "\n miembro encontrado\n";
        imprimirDetalles(resultado);
    } else {
        cout << "\n no se encontro al miembro\n";
    }
}

Nodo* crearArbolBorjaLoyolaInca() {
    Nodo* Francisco = crearNodo("FRANCISCO", "1510", "Don", "Virreinal");
    Nodo* Juana = crearNodo("JUANA", "1520", "Dona", "Inca");
    Francisco->madre = Juana; 
    return Francisco;
}

int main() {
    // Configurar locale para español
    setlocale(LC_ALL, "spanish");
    
    Nodo* RAIZ = crearArbolBorjaLoyolaInca(); 
    int opcion;
    
    cout << "  SISTEMA DE ARBOL GENEALOGICO                  \n";
    cout << "  Familia Borja-Loyola-Inca                     \n";
    cout << "\nNOTA: usa nombres exactos para la busqueda \n";
    
    do {
        cout << "         MENU PRINCIPAL           \n";
        cout << "1. Agregar Miembro               \n";
        cout << "2. Mostrar Todos los Miembros    \n";
        cout << "3. Buscar Miembro                \n";
        cout << "4. Visualizar Arbol Genealogico  \n";
        cout << "5. Salir                         \n";
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
            case 5: cout << "\nHasta pronto\n"; break;
            default: 
                if (opcion != 0) 
                    cout << "\nX Opcion incorrecta, Intenta de nuevo\n"; 
                break;
        }
    } while (opcion != 5);
    
    return 0;
}
