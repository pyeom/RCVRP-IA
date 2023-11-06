#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string.h>

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

int main() {
    string filename = "10";
    string in_filename, out_filename;
    in_filename = filename + ".txt";
    out_filename = filename + ".out";

    ifstream archivo(in_filename);
    int n;
    float riesgo = 0;
    int cantidad_vehiculos = 0;
    float max_risk;
    vector<float> demanda;
    Punto deposit;
    vector<Punto> coordenadas;

    if (archivo.is_open()) {
        archivo >> n;
        archivo >> max_risk;

        for(int i = 0; i < n; i++) {
            float d;
            archivo >> d;
            demanda.push_back(d);
        }

        archivo >> deposit.first >> deposit.second;

        for (int i = 0; i < n; i++) {
            float x, y;
            archivo >> x >> y;
            coordenadas.push_back(make_pair(x, y));
        }

        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }

    /***    Esto me imprime los valores de las constantes
    cout << "n: " << n << endl;
    cout << "riesgo: " << max_risk << endl;
    for (int i = 0; i < n; i++) {
        cout << demanda[i] << " ";
    }
    cout << endl;
    cout << "x: " << deposit.first << " ";
    cout << "y: " << deposit.second << endl;

    for (int i = 0; i < n - 1; i++) {
        cout << "nodo " << i + 1 << " ";
        cout << "x: " << coordenadas[i].first << " ";
        cout << "y: " << coordenadas[i].second << endl;
    }
    ***/

    vector<bool> visitados(coordenadas.size(), false);
    vector<int> solucion;
    float calidad;

    solucion = greedy(deposit, coordenadas, visitados);


    cout << "La solucion es ";
    for (int i = 0; i < n; i++) {
        cout << solucion[i] << "->";
    }
    cout << endl;

    calidad = funcion_objetivo(solucion, coordenadas);
    cout << "Y su calidad es: " << calidad << endl;
    return 0;
}

