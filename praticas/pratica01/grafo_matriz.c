#include <stdio.h>
#include <stdlib.h>
#include "grafo_matriz.h"

GrafoMatriz *criar_grafo_matriz(int n) {
    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));

    if (g == NULL) {
        return NULL;
    }

    g->n = n;

    g->adj = malloc(n * sizeof(int *));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->adj[i] = calloc(n, sizeof(int));

        if (g->adj[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(g->adj[j]);
            }

            free(g->adj);
            free(g);

            return NULL;
        }
    }

    return g;
}

void inserir_aresta_matriz(GrafoMatriz *g, int origem, int destino) {
    if (g == NULL) {
        return;
    }

    if (origem < 0 || origem >= g->n ||
        destino < 0 || destino >= g->n) {
        return;
    }

    g->adj[origem][destino] = 1;
}

void remover_aresta_matriz(GrafoMatriz *g, int origem, int destino) {
    if (g == NULL) {
        return;
    }

    if (origem < 0 || origem >= g->n ||
        destino < 0 || destino >= g->n) {
        return;
    }

    g->adj[origem][destino] = 0;
}

int grau_matriz(GrafoMatriz *g, int vertice) {
    if (g == NULL || vertice < 0 || vertice >= g->n) {
        return -1;
    }

    int grau = 0;

    for (int i = 0; i < g->n; i++) {
        grau += g->adj[vertice][i];
    }

    return grau;
}

int sao_adjacentes_matriz(GrafoMatriz *g, int origem, int destino) {
    if (g == NULL) {
        return 0;
    }

    if (origem < 0 || origem >= g->n ||
        destino < 0 || destino >= g->n) {
        return 0;
    }

    return g->adj[origem][destino];
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }

    free(g->adj);
    free(g);
}