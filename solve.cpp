#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <string>

using namespace std;

typedef pair<float, float> Punto;

/***
 * Calcula la distancia entre dos puntos
 * Input:
 *      - a:    punto (x, y)
 *      - b:    punto (x, y)
 * Output:
 *      - distancia:    float
 * ***/
float distancia(Punto a, Punto b) {
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

/***
 * Calcula el riesgo de ir desde un punto a otro
 * Input: 
 *      - origen:   punto (x, y)
 *      - destino:  punto (x, y)
 *      - carga:    float
 * Output:
 *      - riesgo:   float
 * ***/
float calculate_risk(Punto origen, Punto destino, float carga) {
    return distancia(origen, destino) * carga;
}

/***
 * Calcula la calidad de la solucion con respecto al vector solucion y basado en
 * la distancia recorrida
 * Input:
 *      - vector_solucion:  vector de puntos (x, y)
 * Output:
 *      - value:    float
 *                  calidad de la solución basado en la distancia recorrida
 * ***/
float funcion_objetivo(vector<int> vector_solucion, vector<Punto> nodos) {
    float value = 0;
    for (int i = 0; i < vector_solucion.size() - 1; i++) {
        value = value + distancia(nodos[vector_solucion[i]], nodos[vector_solucion[i + 1]]);
    }
    return value;
}

/***
 * Indica el nodo más cercano al punto de origen que no supere el riesgo máximo
 * Input:
 *      - origen:   punto (x, y)
 *      - nodos:    vector de puntos (x, y)
 *      - visitados: vector de nodos visitados (x, y)
 *      - max_risk: float
 *      - carga:    float
 * Output:
 *      - mas_cercano:   float
 * 
 * ***/
int mas_cercano_con_riesgo(Punto origen, vector<Punto> nodos, vector<bool>& visitados, float max_risk, float carga) {
    float minimo = INFINITY;
    int mas_cercano = -1;

    for (int i = 0; i < nodos.size(); i++) {
        if (!visitados[i]) {
            float d = calculate_risk(origen, nodos[i], carga);

            if (d < minimo && d < max_risk) {
            mas_cercano = i;
            minimo = d;
            }
            else if (d >= max_risk) {
                return 0;
            }
            
        }
        //cout << "nodo: " << i << " distancia: " << d << endl;
    }
    
    return mas_cercano;
}

/***
 * Genera una solucion inicial buscando el nodo más cercano al origen que no supere el riesgo máximo
 * Input:
 *      - origen:   punto (x, y)
 *      - nodos:    vector de puntos (x, y)
 *      - visitados: vector de nodos visitados (x, y)
 *      - max_risk: float
 * Output:
 *      - ruta: vector con los indices de la ruta resultante
 * ***/
vector<int> greedy_with_risk(Punto origen, vector<Punto> nodos, vector<bool>& visitados, vector<int> demanda,float max_risk) {
    vector<int> ruta;
    ruta.reserve(visitados.size());
    float carga = 0;
    ruta.push_back(0);
    visitados[0] = true;

    int indice = mas_cercano_con_riesgo(origen, nodos, visitados, max_risk, carga);

    origen = nodos[indice];
    for (int i = 1; i < visitados.size(); i++) {
        indice = mas_cercano_con_riesgo(origen, nodos, visitados, max_risk, carga);

        if (indice <= 0) {
            ruta.push_back(0);
            break;
        }

        ruta.push_back(indice);
        visitados[indice] = true;
        carga += demanda[indice];
        origen = nodos[indice];
    }

    return ruta;
}

/***
 * Crea una nueva solución a partir de la anterior intercambiando dos nodos
 * Input:
 *      - solucion: vector con los indices de la ruta
 *      - i:    indice del nodo a intercambiar
 *      - j:    indice del nodo a intercambiar
 * Output:
 *      - nueva_solucion:   vector con los indices de la nueva ruta
 * ***/
vector<int> swap(vector<int> solucion, int i, int j) {
    vector<int> nueva_solucion = solucion;
    int aux = nueva_solucion[i];
    nueva_solucion[i] = nueva_solucion[j];
    nueva_solucion[j] = aux;
    return nueva_solucion;
}

/***
 * Verifica la factibilidad de la solucion con respecto a el inicio del recorrido
 * Input:
 *      - solucion: vector con los indices de la ruta
 * Output:
 *      - factible: bool
 * ***/
bool verify_factibility(vector<int> solucion) {
    if (solucion[0] != 0) {
        return false;
    }
    return true;
}

/***
 * Genera el vecindario mediante un movimiento swap, vecindario será utilizado en HCAM
 * Input:
 *      - solucion: vector con los indices de la ruta
 * Output:
 *      - vecindario: vector de vectores con los indices de las nuevas rutas
 * ***/
vector<vector<int>> generar_vecindario(vector<int> solucion) {
    vector<vector<int>> vecindario;

    if (solucion.size() <= 3) {
        vecindario.push_back(solucion);
    }

    for (int i = 1; i < solucion.size() - 2; i++) {
        if (verify_factibility(swap(solucion, i, i + 1))) {
            vecindario.push_back(swap(solucion, i, i + 1));
        }
    }

    if (verify_factibility(swap(solucion, solucion.size() - 2, 1))) {
        vecindario.push_back(swap(solucion, solucion.size() - 2, 1));
    }

    return vecindario;
}

/***
 * Algoritmo de HCAM, recibe una solucion y devuelve la mejor solucion encontrada
 * generando el vecindario mediante swap y verifica si esta es de mayor calidad
 * si es así, se actualiza la mejor solucion y se itera nuevamente
 * Input:
 *      - solucion: vector con los indices de la ruta
 *      - nodos:    vector de puntos (x, y)
 *      - demanda:  vector de demandas
 *      - max_risk: float 
 * Output:
 *      - mejor_solucion:   vector con los indices de la mejor ruta 
 * ***/
vector<int> HCAM(vector<int> solucion, vector<Punto> nodos, vector<int> demanda, float max_risk) {
    vector<int> mejor_solucion = solucion;
    float mejor_calidad = funcion_objetivo(mejor_solucion, nodos);

    if (solucion.size() <= 3) {
        return solucion;
    }

    for (int i = 1; i < solucion.size() - 2; i++) {

        if (verify_factibility(swap(solucion, i, i + 1))) {
            vector<int> nueva_solucion = swap(solucion, i, i + 1);

            float nueva_calidad = funcion_objetivo(nueva_solucion, nodos);
            if (nueva_calidad < mejor_calidad) {
                mejor_solucion = nueva_solucion;
                mejor_calidad = nueva_calidad;
            }
                
        }
    }

    if (verify_factibility(swap(solucion, solucion.size() - 2, 1))) {
        vector<int> nueva_solucion = swap(solucion, solucion.size() - 2, 1);

        float nueva_calidad = funcion_objetivo(nueva_solucion, nodos);
        if (nueva_calidad < mejor_calidad) {
            mejor_solucion = nueva_solucion;
            mejor_calidad = nueva_calidad;
        }
    }

    return mejor_solucion;
}

int main() {
    int n;
    float riesgo = 0;
    int cantidad_vehiculos = 0;
    float max_risk;
    vector<int> demanda;
    Punto deposit;
    vector<Punto> coordenadas;

    DIR *dir;
    struct dirent *ent;
    string folder_path = "Instancias-RCVRP";
    string subfolder_path;
    string file_path;
    int subfolder_choice;
    int file_choice;
    int i = 1;
    int j = 1;
    int k = 1;

    if ((dir = opendir(folder_path.c_str())) != NULL) {
        // print all the subdirectories within folder_path
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
                cout << i << ". " << ent->d_name << endl;
                i++;
            }
        }
        closedir(dir);
    } else {
        // could not open directory
        perror("");
        return EXIT_FAILURE;
    }

    cout << "Enter the number of the subfolder you want to enter: ";
    cin >> subfolder_choice;

    if ((dir = opendir(folder_path.c_str())) != NULL) {
        // enter the subdirectory selected by the user
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
                if (j == subfolder_choice) {
                    subfolder_path = folder_path + "/" + ent->d_name;
                    break;
                }
                j++;
            }
        }
        closedir(dir);
    } else {
        // could not open directory
        perror("");
        return EXIT_FAILURE;
    }

    if ((dir = opendir(subfolder_path.c_str())) != NULL) {
        // print all the files within subfolder_path
        vector<string> file_names;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                file_names.push_back(ent->d_name);
                cout << k << ". " << ent->d_name << endl;
                k++;
            }
        }
        closedir(dir);

        cout << "Enter the number of the file you want to read: ";
        cin >> file_choice;

        file_path = subfolder_path + "/" + file_names[file_choice - 1];

        ifstream file(file_path.c_str());
        if (file.is_open()) {

            file >> n;
            file >> max_risk;
            for (int i = 0; i < n; i++) {
                int d;
                file >> d;
                demanda.push_back(d);
            }

            file >> deposit.first >> deposit.second;

            for (int i = 0; i < n; i++) {
                float x, y;
                file >> x;
                file >> y;
                coordenadas.push_back(make_pair(x, y));
            }
            file.close();

        } else {
            cout << "Could not open file." << endl;
        } 
    } else { 
        // could not open directory
        perror("");
        return EXIT_FAILURE;
    }

    // Termina de leer el archivo
    cout << "=====================================" << endl;
    cout << "=====================================" << endl;
    cout << "==== Bienvenido al programa =)!  ====" << endl;
    cout << "=====================================" << endl;
    cout << "=====================================" << endl;
    cout << endl;

    vector<bool> visitados(coordenadas.size(), false);

    vector<vector<int>> soluciones, solucion_parcial, vecindario;
    soluciones.reserve(n);
    
    // partir desde el origen
    vector<int> solucion_origen = greedy_with_risk(deposit, coordenadas, visitados, demanda, max_risk);
    soluciones.push_back(solucion_origen);
    cantidad_vehiculos++;

    // Recorre todos los nodos no visitados, actualizando el estado de visitados
    for (int i = 0; i < visitados.size(); i++) {
        if (!visitados[i]) {
            vector<int> solucion = greedy_with_risk(coordenadas[i], coordenadas, visitados, demanda, max_risk);
            soluciones.push_back(solucion);
            cantidad_vehiculos++;
        }
    }
    
    float calidad = 0;

    cout << "La solucion obtenida por Greedy es: " << endl;
    for (int i = 0; i < soluciones.size(); i++) {
        cout << "Vehiculo " << i + 1 << ": ";

        for (int j = 0; j < soluciones[i].size() - 1; j++) {
            cout << soluciones[i][j] << " -> ";
        }
        cout << "0" << endl;
    }
    cout << endl;

    for (int i = 0; i < soluciones.size(); i++) {
        calidad += funcion_objetivo(soluciones[i], coordenadas);
    }

    cout << "La calidad de la solucion es: " << calidad << endl;
    cout << "La cantidad de vehiculos es: " << cantidad_vehiculos << endl;
    cout << endl;

    cout << "=====================================" << endl;
    cout << endl;

    cout << "Vecindario a generar: " << endl;
    for (int i = 0; i < soluciones.size(); i++) {
        vecindario = generar_vecindario(soluciones[i]);
        cout << "Vecindario de la solucion " << i + 1 << ": " << endl;
        for (int j = 0; j < vecindario.size(); j++) {
            cout << "Vecino " << j + 1 << ": ";
            for (int k = 0; k < vecindario[j].size() - 1; k++) {
                cout << vecindario[j][k] << " -> ";
            }
            cout << "0" << endl;
        }
    }

    cout << endl;
    cout << "=====================================" << endl;
    cout << endl;

    cantidad_vehiculos = 0;
    solucion_parcial.push_back(HCAM(soluciones[0], coordenadas, demanda, max_risk));
    cantidad_vehiculos++;

    for (int i = 1; i < soluciones.size(); i++) {
        vector<int> solucion = HCAM(soluciones[i], coordenadas, demanda, max_risk);
        solucion_parcial.push_back(solucion);
        cantidad_vehiculos++;
    }

    cout << "La solucion obtenida por HCAM es: " << endl;
    for (int i = 0; i < solucion_parcial.size(); i++) {
        cout << "Vehiculo " << i + 1 << ": ";

        for (int j = 0; j < solucion_parcial[i].size() - 1; j++) {
            cout << solucion_parcial[i][j] << " -> ";
        }
        cout << "0" << endl;
    }

    cout << endl;

    calidad = 0;

    for (int i = 0; i < solucion_parcial.size(); i++) {
        calidad += funcion_objetivo(solucion_parcial[i], coordenadas);
    }

    cout << "La calidad de la solucion es: " << calidad << endl;
    cout << "La cantidad de vehiculos es: " << cantidad_vehiculos << endl;
    cout << endl;

    cout << "=====================================" << endl;
    cout << "=====================================" << endl;
    cout << "==== Has llegado al final del =) ====" << endl;
    cout << "====          programa.          ====" << endl;
    cout << "=====================================" << endl;



    return 0;
}

