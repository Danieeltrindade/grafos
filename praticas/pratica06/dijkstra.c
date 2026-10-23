#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void trocar(MinHeap *heap, int i, int j) {
    NoHeap temp = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temp;

    heap->posicao[heap->dados[i].vertice] = i;
    heap->posicao[heap->dados[j].vertice] = j;
}

static void subir(MinHeap *heap, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;

        if (heap->dados[pai].distancia <=
            heap->dados[i].distancia) {
            break;
        }

        trocar(heap, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *heap, int i) {
    while (1) {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < heap->tamanho &&
            heap->dados[esq].distancia <
            heap->dados[menor].distancia) {
            menor = esq;
        }

        if (dir < heap->tamanho &&
            heap->dados[dir].distancia <
            heap->dados[menor].distancia) {
            menor = dir;
        }

        if (menor == i) {
            break;
        }

        trocar(heap, i, menor);
        i = menor;
    }
}

GrafoListaPonderado *criar_grafo(int num_vertices) {
    if (num_vertices <= 0) {
        return NULL;
    }

    GrafoListaPonderado *g = malloc(sizeof(*g));

    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->adj = calloc(num_vertices, sizeof(*g->adj));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

int adicionar_aresta(GrafoListaPonderado *g, int origem,
                     int destino, int peso) {
    if (g == NULL || origem < 0 ||
        destino < 0 || origem >= g->num_vertices ||
        destino >= g->num_vertices || peso < 0) {
        return 0;
    }

    ArestaPonderada *a = malloc(sizeof(*a));

    if (a == NULL) {
        return 0;
    }

    a->destino = destino;
    a->peso = peso;
    a->prox = g->adj[origem];
    g->adj[origem] = a;

    return 1;
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        ArestaPonderada *atual = g->adj[i];

        while (atual != NULL) {
            ArestaPonderada *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }

    free(g->adj);
    free(g);
}

MinHeap *criar_min_heap(int capacidade) {
    if (capacidade <= 0) {
        return NULL;
    }

    MinHeap *heap = malloc(sizeof(*heap));

    if (heap == NULL) {
        return NULL;
    }

    heap->dados = malloc(capacidade * sizeof(*heap->dados));
    heap->posicao = malloc(capacidade * sizeof(*heap->posicao));

    if (heap->dados == NULL || heap->posicao == NULL) {
        free(heap->dados);
        free(heap->posicao);
        free(heap);
        return NULL;
    }

    heap->capacidade = capacidade;
    heap->tamanho = 0;

    return heap;
}

void liberar_min_heap(MinHeap *heap) {
    if (heap != NULL) {
        free(heap->dados);
        free(heap->posicao);
        free(heap);
    }
}

NoHeap extrair_minimo(MinHeap *heap) {
    NoHeap vazio = {-1, INF};

    if (heap == NULL || heap->tamanho == 0) {
        return vazio;
    }

    NoHeap minimo = heap->dados[0];
    heap->posicao[minimo.vertice] = -1;
    heap->tamanho--;

    if (heap->tamanho > 0) {
        heap->dados[0] = heap->dados[heap->tamanho];
        heap->posicao[heap->dados[0].vertice] = 0;
        descer(heap, 0);
    }

    return minimo;
}

void diminuir_chave(MinHeap *heap, int vertice,
                    int nova_distancia) {
    if (heap == NULL || vertice < 0 ||
        vertice >= heap->capacidade) {
        return;
    }

    int i = heap->posicao[vertice];

    if (i < 0 || i >= heap->tamanho ||
        nova_distancia >= heap->dados[i].distancia) {
        return;
    }

    heap->dados[i].distancia = nova_distancia;
    subir(heap, i);
}

void dijkstra(GrafoListaPonderado *g, int origem,
              int *dist, int *pred) {
    if (g == NULL || dist == NULL || pred == NULL ||
        origem < 0 || origem >= g->num_vertices) {
        return;
    }

    int n = g->num_vertices;
    MinHeap *heap = criar_min_heap(n);

    if (heap == NULL) {
        for (int i = 0; i < n; i++) {
            dist[i] = INF;
            pred[i] = -1;
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        pred[i] = -1;
        heap->dados[i].vertice = i;
        heap->dados[i].distancia = INF;
        heap->posicao[i] = i;
    }

    heap->tamanho = n;
    dist[origem] = 0;
    heap->dados[origem].distancia = 0;
    subir(heap, heap->posicao[origem]);

    while (heap->tamanho > 0) {
        NoHeap atual = extrair_minimo(heap);
        int u = atual.vertice;

        if (atual.distancia == INF) {
            break;
        }

        for (ArestaPonderada *a = g->adj[u];
             a != NULL; a = a->prox) {
            int v = a->destino;

            if (heap->posicao[v] == -1) {
                continue;
            }

            if (dist[u] <= INF - a->peso &&
                dist[u] + a->peso < dist[v]) {
                dist[v] = dist[u] + a->peso;
                pred[v] = u;
                diminuir_chave(heap, v, dist[v]);
            }
        }
    }

    liberar_min_heap(heap);
}

int reconstruir_caminho(int origem, int destino, int *pred) {
    if (pred == NULL || origem < 0 || destino < 0) {
        return 0;
    }

    int tamanho = 0;
    int atual = destino;

    while (atual != -1) {
        tamanho++;

        if (atual == origem) {
            break;
        }

        atual = pred[atual];
    }

    if (atual != origem) {
        printf("Nao existe caminho de %d ate %d.\n",
               origem, destino);
        return 0;
    }

    int *caminho = malloc((size_t)tamanho * sizeof(int));

    if (caminho == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o caminho.\n");
        return 0;
    }

    atual = destino;

    for (int i = tamanho - 1; i >= 0; i--) {
        caminho[i] = atual;

        if (atual == origem) {
            break;
        }

        atual = pred[atual];
    }

    printf("Caminho: ");

    for (int i = 0; i < tamanho; i++) {
        printf("%d", caminho[i]);

        if (i < tamanho - 1) {
            printf(" -> ");
        }
    }

    printf("\n");

    free(caminho);
    return tamanho;
}