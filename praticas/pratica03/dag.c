#include "dag.h"

#include <stdlib.h>

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

int inserir_aresta_lista(GrafoLista *g, int u, int v)
{
    No *atual;
    No *novo;

    if (g == NULL || u < 0 || v < 0 || u >= g->n || v >= g->n) {
        return 0;
    }

    for (atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        if (atual->destino == v) {
            return 1;
        }
    }

    novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return 0;
    }

    novo->destino = v;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
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

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho)
{
    int *grau_entrada;
    int *fila;
    int *ordem;
    int inicio = 0;
    int fim = 0;
    int processados = 0;
    int u;

    if (tamanho == NULL) {
        return NULL;
    }
    *tamanho = 0;

    if (g == NULL || g->n <= 0 || g->adj == NULL) {
        return NULL;
    }

    grau_entrada = calloc((size_t)g->n, sizeof(*grau_entrada));
    fila = malloc((size_t)g->n * sizeof(*fila));
    ordem = malloc((size_t)g->n * sizeof(*ordem));
    if (grau_entrada == NULL || fila == NULL || ordem == NULL) {
        free(grau_entrada);
        free(fila);
        free(ordem);
        return NULL;
    }

    for (u = 0; u < g->n; u++) {
        No *atual;
        for (atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            grau_entrada[atual->destino]++;
        }
    }

    for (u = 0; u < g->n; u++) {
        if (grau_entrada[u] == 0) {
            fila[fim++] = u;
        }
    }

    while (inicio < fim) {
        No *atual;
        u = fila[inicio++];
        ordem[processados++] = u;

        for (atual = g->adj[u]; atual != NULL; atual = atual->prox) {
            int v = atual->destino;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) {
                fila[fim++] = v;
            }
        }
    }

    free(grau_entrada);
    free(fila);

    if (processados != g->n) {
        free(ordem);
        return NULL;
    }

    *tamanho = processados;
    return ordem;
}

static int visitar_dfs(GrafoLista *g, int u, int *estado, int *ordem,
                       int *posicao)
{
    No *atual;

    estado[u] = 1;
    for (atual = g->adj[u]; atual != NULL; atual = atual->prox) {
        int v = atual->destino;

        if (estado[v] == 1) {
            return 0;
        }
        if (estado[v] == 0 && !visitar_dfs(g, v, estado, ordem, posicao)) {
            return 0;
        }
    }

    estado[u] = 2;
    ordem[(*posicao)--] = u;
    return 1;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho)
{
    int *estado;
    int *ordem;
    int posicao;
    int u;

    if (tamanho == NULL) {
        return NULL;
    }
    *tamanho = 0;

    if (g == NULL || g->n <= 0 || g->adj == NULL) {
        return NULL;
    }

    estado = calloc((size_t)g->n, sizeof(*estado));
    ordem = malloc((size_t)g->n * sizeof(*ordem));
    if (estado == NULL || ordem == NULL) {
        free(estado);
        free(ordem);
        return NULL;
    }

    posicao = g->n - 1;
    for (u = 0; u < g->n; u++) {
        if (estado[u] == 0 && !visitar_dfs(g, u, estado, ordem, &posicao)) {
            free(estado);
            free(ordem);
            return NULL;
        }
    }

    free(estado);
    *tamanho = g->n;
    return ordem;
}

int eh_dag(GrafoLista *g)
{
    int tamanho;
    int *ordem = ordenacao_topologica_dfs(g, &tamanho);

    if (ordem == NULL) {
        return 0;
    }

    free(ordem);
    return tamanho == g->n;
}