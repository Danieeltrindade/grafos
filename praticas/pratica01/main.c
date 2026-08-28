#include <stdio.h>

#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {

    printf("=== GRAFO COM MATRIZ DE ADJACENCIA ===\n");

    GrafoMatriz *gm = criar_grafo_matriz(5);

    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    inserir_aresta_matriz(gm, 1, 3);
    inserir_aresta_matriz(gm, 2, 4);

    printf("Grau do vertice 0: %d\n", grau_matriz(gm, 0));

    printf("0 e 1 sao adjacentes? %d\n",
           sao_adjacentes_matriz(gm, 0, 1));

    printf("0 e 4 sao adjacentes? %d\n",
           sao_adjacentes_matriz(gm, 0, 4));

    remover_aresta_matriz(gm, 0, 1);

    printf("Depois de remover a aresta 0 -> 1:\n");

    printf("0 e 1 sao adjacentes? %d\n",
           sao_adjacentes_matriz(gm, 0, 1));

    liberar_grafo_matriz(gm);


    printf("\n=== GRAFO COM LISTA DE ADJACENCIA ===\n");

    GrafoLista *gl = criar_grafo_lista(5);

    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    inserir_aresta_lista(gl, 1, 3);
    inserir_aresta_lista(gl, 2, 4);

    printf("Grau do vertice 0: %d\n", grau_lista(gl, 0));

    printf("0 e 1 sao adjacentes? %d\n",
           sao_adjacentes_lista(gl, 0, 1));

    printf("0 e 4 sao adjacentes? %d\n",
           sao_adjacentes_lista(gl, 0, 4));

    remover_aresta_lista(gl, 0, 1);

    printf("Depois de remover a aresta 0 -> 1:\n");

    printf("0 e 1 sao adjacentes? %d\n",
           sao_adjacentes_lista(gl, 0, 1));

    liberar_grafo_lista(gl);

    return 0;
}