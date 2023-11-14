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
 * Indica cual es el nodo más cercano al nodo de origen
 * Input:
 *      - origen:   punto (x, y)
 *      - nodos:    vector de puntos (x, y)
 * Output:
 *      - mas_cerano:   float
 *                      Indica el índice del nodo más cercano al origen
 * ***/
int mas_cercano(Punto origen, vector<Punto> nodos, vector<bool> visitados) {
    float minimo = INFINITY;
    int mas_cercano = -1;

    for (int i = 0; i < nodos.size(); i++) {
        if (!visitados[i]) {
            float d = distancia(origen, nodos[i]);

            if (d < minimo) {
            mas_cercano = i;
            minimo = d;
            }
        }
        //cout << "nodo: " << i << " distancia: " << d << endl;
    }
    
    return mas_cercano;
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
 * Aplica un algoritmo Greedy comenzando desde el origen para obtener una solución
 * Input:
 *      - origen: punto (x, y)
 *      - nodos: vector de puntos (x, y)
 *      - visitados: vector de nodos visitados (x, y)
 * Output:
 *      - ruta: vector con los indices de la ruta resultante
 * ***/
vector<int> greedy(Punto origen, vector<Punto> nodos, vector<bool> visitados) {
    vector<int> ruta(visitados.size() + 1);

    ruta[0] = 0;
    visitados[0] = true;

    int indice = mas_cercano(origen, nodos, visitados);

    origen = nodos[indice];
    for (int i = 1; i < visitados.size(); i++) {
        indice = mas_cercano(origen, nodos, visitados);
        ruta[i] = indice;
        visitados[indice] = true;
        origen = nodos[indice];
    }

    return ruta;
}

// make a new version of the greedy algorithm above but now make it so that it considers the risk of going from one node to another and the risk of going from the last node to the deposit. Make it stop when it reaches the maximum risk and return the solution. If it can't reach the maximum risk, return the best solution it could find.
vector<int> greedy_with_risk(Punto origen, vector<Punto> nodos, vector<bool> visitados, float max_risk) {
    vector<int> ruta(visitados.size() + 1);
    float riesgo = 0;
    ruta[0] = 0;
    visitados[0] = true;
    int indice = mas_cercano(origen, nodos, visitados);
    origen = nodos[indice];

    for (int i = 1; i < visitados.size(); i++) {
        indice = mas_cercano(origen, nodos, visitados);
        ruta[i] = indice;
        visitados[indice] = true;
        riesgo = riesgo + calculate_risk(origen, nodos[indice], 1);
        origen = nodos[indice];
        if (riesgo > max_risk) {
            break;
        }
    }

    return ruta;
}


int main() {
    int n;
    float riesgo = 0;
    int cantidad_vehiculos = 0;
    float max_risk;
    vector<float> demanda;
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
                float d;
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

    vector<bool> visitados(coordenadas.size(), false);

    vector<int> solucion, solucion_2;
    float calidad;

    solucion = greedy(deposit, coordenadas, visitados);
    solucion_2 = greedy_with_risk(deposit, coordenadas, visitados, max_risk);

    cout << "La solucion es ";
    for (int i = 0; i < n; i++) {
        cout << solucion[i] << "->";
    }
    cout << endl;

    calidad = funcion_objetivo(solucion, coordenadas);
    cout << "Y su calidad es: " << calidad << endl;

    cout << "La solucion con riesgo es ";
    for (int i = 0; i < n; i++) {
        cout << solucion_2[i] << "->";
    }
    cout << endl;

    calidad = funcion_objetivo(solucion_2, coordenadas);
    cout << "Y su calidad es: " << calidad << endl;


    return 0;
}

