
#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

static int adicionar_aresta_nao_direcionada(
    GrafoListaPonderado *g,
    int u,
    int v,
    int peso
) {
    if (!adicionar_aresta(g, u, v, peso)) {
        return 0;
    }

    if (!adicionar_aresta(g, v, u, peso)) {
        return 0;
    }

    return 1;
}

int main(void) {
    const int V = 6;
    const int origem = 0;
    const int destino = 4;

    GrafoListaPonderado *g = criar_grafo(V);

    if (g == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return EXIT_FAILURE;
    }

    int sucesso = 1;

    sucesso &= adicionar_aresta_nao_direcionada(g, 0, 1, 4);
    sucesso &= adicionar_aresta_nao_direcionada(g, 0, 2, 2);
    sucesso &= adicionar_aresta_nao_direcionada(g, 1, 2, 1);
    sucesso &= adicionar_aresta_nao_direcionada(g, 1, 3, 5);
    sucesso &= adicionar_aresta_nao_direcionada(g, 2, 3, 8);
    sucesso &= adicionar_aresta_nao_direcionada(g, 2, 4, 10);
    sucesso &= adicionar_aresta_nao_direcionada(g, 3, 4, 2);
    sucesso &= adicionar_aresta_nao_direcionada(g, 3, 5, 6);
    sucesso &= adicionar_aresta_nao_direcionada(g, 4, 5, 3);

    if (!sucesso) {
        fprintf(stderr, "Erro ao adicionar arestas.\n");
        liberar_grafo(g);
        return EXIT_FAILURE;
    }

    int *dist = malloc(V * sizeof(int));
    int *pred = malloc(V * sizeof(int));

    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro ao alocar vetores.\n");
        free(dist);
        free(pred);
        liberar_grafo(g);
        return EXIT_FAILURE;
    }

    dijkstra(g, origem, dist, pred);

    printf("Distancias a partir do vertice %d:\n", origem);

    for (int i = 0; i < V; i++) {
        if (dist[i] == INF) {
            printf("Vertice %d: infinito (inalcancavel)\n", i);
        } else {
            printf("Vertice %d: %d\n", i, dist[i]);
        }
    }

    printf("\nMenor caminho de %d ate %d:\n", origem, destino);

    if (dist[destino] == INF) {
        printf("Destino inalcancavel.\n");
    } else {
        reconstruir_caminho(origem, destino, pred);
        printf("Custo total: %d\n", dist[destino]);
    }

    free(dist);
    free(pred);
    liberar_grafo(g);

    return EXIT_SUCCESS;
}
