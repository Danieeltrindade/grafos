#include "coloracao.h"

#include <stdlib.h>

GrafoLista *criar_grafo_lista(int n) {
    GrafoLista *g;

    if (n <= 0) {
        return NULL;
    }

    g = malloc(sizeof(GrafoLista));
    if (g == NULL) {
        return NULL;
    }

    g->adj = calloc((size_t)n, sizeof(No *));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    g->n = n;
    return g;
}

static int sao_adjacentes(const GrafoLista *g, int u, int v) {
    const No *atual = g->adj[u];

    while (atual != NULL) {
        if (atual->destino == v) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    No *novo_u;
    No *novo_v;

    if (g == NULL || g->adj == NULL || u < 0 || v < 0 ||
        u >= g->n || v >= g->n || u == v || sao_adjacentes(g, u, v)) {
        return;
    }

    novo_u = malloc(sizeof(No));
    novo_v = malloc(sizeof(No));
    if (novo_u == NULL || novo_v == NULL) {
        free(novo_u);
        free(novo_v);
        return;
    }

    novo_u->destino = v;
    novo_u->prox = g->adj[u];
    novo_v->destino = u;
    novo_v->prox = g->adj[v];
    g->adj[u] = novo_u;
    g->adj[v] = novo_v;
}

void liberar_grafo_lista(GrafoLista *g) {
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

static int grafo_valido(const GrafoLista *g) {
    int u;

    if (g == NULL || g->n <= 0 || g->adj == NULL) {
        return 0;
    }

    for (u = 0; u < g->n; u++) {
        const No *atual = g->adj[u];

        while (atual != NULL) {
            if (atual->destino < 0 || atual->destino >= g->n ||
                atual->destino == u) {
                return 0;
            }
            atual = atual->prox;
        }
    }

    return 1;
}

static int *colorir_na_ordem(GrafoLista *g, const int *ordem,
                             int *num_cores) {
    int *cores;
    int *indisponivel;
    int posicao;
    int maior_cor = -1;

    *num_cores = 0;
    if (!grafo_valido(g)) {
        return NULL;
    }

    cores = malloc((size_t)g->n * sizeof(int));
    indisponivel = calloc((size_t)g->n, sizeof(int));
    if (cores == NULL || indisponivel == NULL) {
        free(cores);
        free(indisponivel);
        return NULL;
    }

    for (posicao = 0; posicao < g->n; posicao++) {
        cores[posicao] = -1;
    }

    for (posicao = 0; posicao < g->n; posicao++) {
        int u = ordem == NULL ? posicao : ordem[posicao];
        int cor;
        const No *vizinho = g->adj[u];

        while (vizinho != NULL) {
            int cor_vizinho = cores[vizinho->destino];
            if (cor_vizinho >= 0) {
                indisponivel[cor_vizinho] = 1;
            }
            vizinho = vizinho->prox;
        }

        /* A primeira cor livre e a escolha gulosa de menor cor. */
        for (cor = 0; cor < g->n && indisponivel[cor]; cor++) {
        }
        cores[u] = cor;
        if (cor > maior_cor) {
            maior_cor = cor;
        }

        vizinho = g->adj[u];
        while (vizinho != NULL) {
            int cor_vizinho = cores[vizinho->destino];
            if (cor_vizinho >= 0) {
                indisponivel[cor_vizinho] = 0;
            }
            vizinho = vizinho->prox;
        }
    }

    *num_cores = maior_cor + 1;
    return cores;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    if (num_cores == NULL) {
        return NULL;
    }

    /* Sem uma ordem explicita, colorir_na_ordem usa 0, 1, ..., n - 1. */
    return colorir_na_ordem(g, NULL, num_cores);
}

static int grau_vertice(const GrafoLista *g, int u) {
    int grau = 0;
    const No *atual = g->adj[u];

    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }

    return grau;
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int *ordem;
    int *graus;
    int *cores;
    int i;

    if (num_cores == NULL) {
        return NULL;
    }
    *num_cores = 0;
    if (!grafo_valido(g)) {
        return NULL;
    }

    ordem = malloc((size_t)g->n * sizeof(int));
    graus = malloc((size_t)g->n * sizeof(int));
    if (ordem == NULL || graus == NULL) {
        free(ordem);
        free(graus);
        return NULL;
    }

    for (i = 0; i < g->n; i++) {
        ordem[i] = i;
        graus[i] = grau_vertice(g, i);
    }

    /* Insertion sort: grau decrescente e, no empate, indice crescente. */
    for (i = 1; i < g->n; i++) {
        int vertice = ordem[i];
        int j = i - 1;

        while (j >= 0 &&
               (graus[ordem[j]] < graus[vertice] ||
                (graus[ordem[j]] == graus[vertice] &&
                 ordem[j] > vertice))) {
            ordem[j + 1] = ordem[j];
            j--;
        }
        ordem[j + 1] = vertice;
    }

    cores = colorir_na_ordem(g, ordem, num_cores);
    free(ordem);
    free(graus);
    return cores;
}

int eh_bipartido(GrafoLista *g) {
    int *cores;
    int *fila;
    int inicio;

    if (!grafo_valido(g)) {
        return 0;
    }

    cores = malloc((size_t)g->n * sizeof(int));
    fila = malloc((size_t)g->n * sizeof(int));
    if (cores == NULL || fila == NULL) {
        free(cores);
        free(fila);
        return 0;
    }

    for (inicio = 0; inicio < g->n; inicio++) {
        cores[inicio] = -1;
    }

    /* Cada vertice ainda sem cor inicia a BFS de um novo componente. */
    for (inicio = 0; inicio < g->n; inicio++) {
        int frente = 0;
        int fim = 0;

        if (cores[inicio] != -1) {
            continue;
        }

        cores[inicio] = 0;
        fila[fim++] = inicio;

        while (frente < fim) {
            int u = fila[frente++];
            const No *vizinho = g->adj[u];

            while (vizinho != NULL) {
                int v = vizinho->destino;

                if (cores[v] == -1) {
                    cores[v] = 1 - cores[u];
                    fila[fim++] = v;
                } else if (cores[v] == cores[u]) {
                    free(cores);
                    free(fila);
                    return 0;
                }
                vizinho = vizinho->prox;
            }
        }
    }

    free(cores);
    free(fila);
    return 1;
}