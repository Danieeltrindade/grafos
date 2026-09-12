#include "grafo_lista.h"
#include <stdlib.h>

GrafoLista *criar_grafo_lista(int n)
{
    GrafoLista *g = (GrafoLista *)malloc(sizeof(GrafoLista));

    g->num_vertices = n;
    g->lista = (No **)malloc(n * sizeof(No *));

    for(int i = 0; i < n; i++)
    {
        g->lista[i] = NULL;
    }

    return g;
}

void adicionar_aresta(GrafoLista *g, int u, int v)
{
    No *proximo = g->lista[u];

    No *no = (No *)malloc(sizeof(No));
    no->vertice = v;
    no->proximo = proximo;
    g->lista[u] = no;

    proximo = g->lista[v];

    no = (No *)malloc(sizeof(No));
    no->vertice = u;
    no->proximo = proximo;
    g->lista[v] = no;
}