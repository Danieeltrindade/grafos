#include "grafo_lista.h"
#include <stdlib.h>

static int vertice_valido(const GrafoLista *g, int v)
{
    return g != NULL && v >= 0 && v < g->n;
}

GrafoLista *criar_grafo_lista(int n)
{
    GrafoLista *g;

    if (n <= 0) {
        return NULL;
    }

    g = malloc(sizeof(*g));
    if (g == NULL) {
        return NULL;
    }

    g->adj = calloc((size_t)n, sizeof(*g->adj));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    g->n = n;
    return g;
}

int sao_adjacentes_lista(const GrafoLista *g, int u, int v)
{
    const No *atual;

    if (!vertice_valido(g, u) || !vertice_valido(g, v)) {
        return 0;
    }

    atual = g->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

int inserir_aresta_lista(GrafoLista *g, int u, int v)
{
    No *novo_u;
    No *novo_v;

    if (!vertice_valido(g, u) || !vertice_valido(g, v) || u == v) {
        return 0;
    }
    if (sao_adjacentes_lista(g, u, v)) {
        return 1;
    }

    novo_u = malloc(sizeof(*novo_u));
    novo_v = malloc(sizeof(*novo_v));
    if (novo_u == NULL || novo_v == NULL) {
        free(novo_u);
        free(novo_v);
        return 0;
    }

    novo_u->destino = v;
    novo_u->prox = g->adj[u];
    novo_v->destino = u;
    novo_v->prox = g->adj[v];
    g->adj[u] = novo_u;
    g->adj[v] = novo_v;
    return 1;
}

void liberar_grafo_lista(GrafoLista *g)
{
    int i;

    if (g == NULL) {
        return;
    }

    for (i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }

    free(g->adj);
    free(g);
}