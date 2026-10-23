
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#define INF 1000000000

typedef struct ArestaPonderada {
    int destino;
    int peso;
    struct ArestaPonderada *prox;
} ArestaPonderada;

typedef struct {
    int num_vertices;
    ArestaPonderada **adj;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade;
    int tamanho;
    int *posicao;
} MinHeap;

GrafoListaPonderado *criar_grafo(int num_vertices);
int adicionar_aresta(GrafoListaPonderado *g, int origem,
                     int destino, int peso);
void liberar_grafo(GrafoListaPonderado *g);

MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *heap);
NoHeap extrair_minimo(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia);

void dijkstra(GrafoListaPonderado *g, int origem,
              int *dist, int *pred);

int reconstruir_caminho(int origem, int destino, int *pred);

#endif
