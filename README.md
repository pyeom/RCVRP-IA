Algoritmo de solución al RCVRP.

Nombre: Felipe Rojas Saavedra
Rol:    201873112-9


## Instrucciones:
Para escoger la instancia el programa brinda un menú interactuable por consola

Para compilar:
```
make
```
Para ejecutar:
```
./solve
```

## Estado actual:  05/11/2023
- El programa es capaz de leer una instancia a partir de un archivo de texto
- Está implementado un algoritmo Greedy, el cual presenta una solución inicial partiendo desde el depósito, y visitando el nodo más cercano cada vez hasta visitar todos los nodos teniendo en cuenta el riesgo asociado.
- Puede calcular la calidad de la solución.
- Aplica un algoritmo de Hill Climbing con alguna mejora generando un vecindario a través de un movimiento swap entre nodos visitados.
- Diversifica utilizando un movimiento de 2-opt entre dos nodos de rutas diferentes de forma aleatoria, esto lo realiza por cada ruta, además de esto da vuelta la solucion obtenida para diversificar aun mas.
- Aplica el algoritmo de Hill Climbing una vez más.
- La solución se muestra por pantalla y también se crea un archivo de texto "INSTANCIA.out" luego de ejecutada la instancia.