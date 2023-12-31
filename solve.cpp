#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <chrono>

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
    carga = carga + demanda[indice];

    origen = nodos[indice];
    for (int i = 1; i < visitados.size(); i++) {
        indice = mas_cercano_con_riesgo(origen, nodos, visitados, max_risk, carga);

        if (indice <= 0) {
            ruta.push_back(0);
            break;
        }

        ruta.push_back(indice);
        visitados[indice] = true;
        carga = carga + demanda[indice];
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
 * Algoritmo de HCAM, recibe una solucion y devuelve la mejor solucion encontrada
 * generando el vecindario mediante swap y verifica si esta es de mayor calidad
 * si es así, se actualiza la mejor solucion y se itera nuevamente
 * Input:
 *      - solucion: vector con los indices de la ruta
 *      - nodos:    vector de puntos (x, y)
 *      - demanda:  vector de demandas
 *      - max_risk: float
 * Output:
 *     - mejor_solucion:   vector con los indices de la mejor ruta
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

/***
 * Algoritmo de Restart, recibe una solucion y la modifica intercambiando dos nodos
 * Se utiliza para diversificar la solucion y explorar el espacio de busqueda, 
 * puede buscar valores en lo infactible
 * Input:
 *      - solucion_inicial: vector con los indices de la ruta
 * Output:
 *      - solucion_inicial: vector con los indices de la ruta cambiados
 * ***/
void Restart(vector<vector<int>>& solucion_inicial) {
    
    int cant_rutas = solucion_inicial.size();
    vector<int> cant_nodos;

    for (int i = 0; i < cant_rutas; i++) {
        cant_nodos.push_back(solucion_inicial[i].size());
    }

    int ruta1 = rand() % cant_rutas;
    int nodo1 = rand() % cant_nodos.size();
    int ruta2 = rand() % cant_rutas;
    int nodo2 = rand() % cant_nodos.size();

    while (solucion_inicial[ruta1][nodo1] == 0 || solucion_inicial[ruta2][nodo2] == 0) {
        ruta1 = rand() % cant_rutas;
        nodo1 = rand() % cant_nodos.size();
        ruta2 = rand() % cant_rutas;
        nodo2 = rand() % cant_nodos.size();
    }

    int temp = solucion_inicial[ruta1][nodo1];
    solucion_inicial[ruta1][nodo1] = solucion_inicial[ruta2][nodo2];
    solucion_inicial[ruta2][nodo2] = temp;
}

int main() {
    int n;
    int cantidad_vehiculos = 0;
    float max_risk;
    vector<int> demanda;
    Punto deposit;
    vector<Punto> coordenadas;

    srand(time(NULL));

    // Comienza a leer el archivo
    DIR *dir;
    struct dirent *ent;
    string folder_path = "Instancias-RCVRP";
    string subfolder_path;
    string file_path;
    string file_name;
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

    cout << "Ingresa el numero del Set que deseas revisar: ";
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

        cout << "Ingresa el numero de la instancia: ";
        cin >> file_choice;

        file_path = subfolder_path + "/" + file_names[file_choice - 1];
        file_name = file_names[file_choice - 1];

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

    auto start = chrono::high_resolution_clock::now();
    vector<bool> visitados(coordenadas.size(), false);

    vector<vector<int>> soluciones, solucion_parcial, solucion_final;
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

    // Fase de diversificacion mediante el Restart y reverse
    for (int i = 0; i < solucion_parcial.size(); i++) {
        Restart(solucion_parcial);
    }

    for (int i = 0; i < solucion_parcial.size(); i++) {
        reverse(solucion_parcial[i].begin(), solucion_parcial[i].end());
    } 
    
    cout << "=====================================" << endl;
    cout << endl;
    
    cout << "La solucion obtenida por Restart es: " << endl;
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
    cout << endl;

    for (int i = 0; i < solucion_parcial.size(); i++) {
        solucion_final.push_back(HCAM(solucion_parcial[i], coordenadas, demanda, max_risk));
    }

    cout << "La solucion final es: " << endl;
    for (int i = 0; i < solucion_final.size(); i++) {
        cout << "Vehiculo " << i + 1 << ": ";

        for (int j = 0; j < solucion_final[i].size() - 1; j++) {
            cout << solucion_final[i][j] << " -> ";
        }
        cout << "0" << endl;
    }

    cout << endl;

    calidad = 0;

    for (int i = 0; i < solucion_final.size(); i++) {
        calidad += funcion_objetivo(solucion_final[i], coordenadas);
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);

    cout << "La calidad de la solucion es: " << calidad << endl;
    cout << "La cantidad de vehiculos es: " << cantidad_vehiculos << endl;
    cout << endl;

    cout << "=====================================" << endl;
    cout << "=====================================" << endl;
    cout << "==== Has llegado al final del =) ====" << endl;
    cout << "====          programa.          ====" << endl;
    cout << "=====================================" << endl;

    file_name.replace(file_name.find("txt"), 3, "out");
    
    ofstream salida(file_name);

    if (salida.is_open()) {
        salida << duration.count() << endl;
        salida << calidad << endl;
        salida << cantidad_vehiculos << endl;

        for (int i = 1; i < visitados.size(); i++) {
            if (visitados[i]) {
                salida << i << " ";
            }
        }
        salida << endl; 

        vector<float> riesgo;
        riesgo.reserve(solucion_final.size());

        for (int i = 0; i < solucion_final.size(); i++) {
            riesgo.push_back(0);
            for (int j = 0; j < solucion_final[i].size() - 1; j++) {
                riesgo[i] += calculate_risk(coordenadas[solucion_final[i][j]], coordenadas[solucion_final[i][j + 1]], demanda[solucion_final[i][j]]);
            }
        }

        vector<float> carga_vehiculo;
        carga_vehiculo.reserve(solucion_final.size());

        for (int i = 0; i < solucion_final.size(); i++) {
            carga_vehiculo.push_back(0);
            for (int j = 0; j < solucion_final[i].size() - 1; j++) {
                carga_vehiculo[i] += demanda[solucion_final[i][j]];
            }
        }

        for (int i = 0; i < solucion_final.size(); i++) {
            salida << carga_vehiculo[i] << " ";
            salida << riesgo[i] << " ";
            for (int j = 0; j < solucion_final[i].size() - 1; j++) {
                salida << solucion_final[i][j] << "->";
            }
            salida << "0" << endl;
        }
        salida.close();
    } else {
        cout << "Could not open file." << endl;
    }    
    
    return 0;
}