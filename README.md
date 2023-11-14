Algoritmo de solución al RCVRP.

Nombre: Felipe Rojas Saavedra
Rol:    201873112-9


## Instrucciones:
Para escoger la instancia a ejecutar, mover la instancia al directorio fuente, cambiar el campo filename en la línea 104 del código por el nombre de la instancia sin su extensión (si la instancia se llama '15.txt', poner '15').

Para compilar:
```
g++ -o out solve.cpp
```
Para ejecutar:
```
./out
```

## Estado actual:  05/11/2023
- El programa es capaz de leer una instancia a partir de un archivo de texto
- Está implementado un algoritmo Greedy, el cual presenta una solución inicial partiendo desde el depósito, y visitando el nodo más cercano cada vez hasta visitar todos los nodos.
- Puede calcular la calidad de la solución
- Aún no se presenta una solución que tenga en cuenta el riesgo.
- La solución se muestra por pantalla.