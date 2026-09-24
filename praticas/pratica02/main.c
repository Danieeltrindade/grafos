#include <stdio.h>
#include <stdlib.h>

#include "busca_largura.h"
#include "busca_profundidade.h"
#include "grafo_lista.h"

static int falhas = 0;

static void verificar(int condicao, const char *descricao)
{
    if (!condicao) {
        fprintf(stderr, "FALHA: %s\n", descricao);
        falhas++;
    }
}

static GrafoLista *novo_grafo(int n)
{
    GrafoLista *g = criar_grafo_lista(n);
    if (g == NULL) {
        fprintf(stderr, "Falha de alocacao ao criar grafo.\n");
        exit(EXIT_FAILURE);
    }
    return g;
}

static void adicionar(GrafoLista *g, int u, int v)
{
    if (!inserir_aresta_lista(g, u, v)) {
        fprintf(stderr, "Falha ao inserir aresta %d-%d.\n", u, v);
        liberar_grafo_lista(g);
        exit(EXIT_FAILURE);
    }
}

static void testar_bfs(void)
{
    GrafoLista *g = novo_grafo(7);
    int dist[7];
    int pred[7];

    adicionar(g, 0, 1);
    adicionar(g, 0, 2);
    adicionar(g, 1, 3);
    adicionar(g, 3, 4);
    adicionar(g, 5, 6);

    bfs(g, 0, dist, pred);
    verificar(dist[0] == 0 && pred[0] == -1, "BFS: origem");
    verificar(dist[1] == 1 && pred[1] == 0, "BFS: distancia e predecessor");
    verificar(dist[3] == 2 && pred[3] == 1, "BFS: multiplos niveis");
    verificar(dist[4] == 3 && pred[4] == 3, "BFS: vertice distante");
    verificar(dist[5] == -1 && pred[5] == -1, "BFS: componente inalcançavel");
    verificar(dist[6] == -1 && pred[6] == -1, "BFS: vertices desconexos");

    bfs(g, -1, dist, pred);
    verificar(dist[0] == -1 && pred[0] == -1, "BFS: origem invalida");
    liberar_grafo_lista(g);

    g = novo_grafo(1);
    bfs(g, 0, dist, pred);
    verificar(dist[0] == 0 && pred[0] == -1, "BFS: vertice isolado");
    liberar_grafo_lista(g);
}

static void testar_dfs(void)
{
    GrafoLista *g = novo_grafo(6);
    int visitado[6] = {0};
    int entrada[6] = {0};
    int saida[6] = {0};
    int tempo = 0;
    int i;

    adicionar(g, 0, 1);
    adicionar(g, 0, 2);
    adicionar(g, 1, 3);
    adicionar(g, 2, 4);
    dfs_recursiva(g, 0, visitado, entrada, saida, &tempo);

    for (i = 0; i < 5; i++) {
        verificar(visitado[i] == 1, "DFS: vertice alcancavel visitado");
        verificar(entrada[i] > 0 && entrada[i] < saida[i],
                  "DFS: entrada anterior a saida");
    }
    verificar(visitado[5] == 0, "DFS: isolado nao visitado pela origem");
    verificar(tempo == 10, "DFS: cada vertice visitado uma unica vez");
    verificar(entrada[0] < entrada[1] && saida[1] < saida[0],
              "DFS: tempos coerentes entre ancestral e descendente");
    liberar_grafo_lista(g);
}

static void testar_componentes(void)
{
    GrafoLista *g = novo_grafo(5);

    adicionar(g, 0, 1);
    adicionar(g, 1, 2);
    adicionar(g, 2, 3);
    adicionar(g, 3, 4);
    verificar(contar_componentes(g) == 1, "componentes: grafo conexo");
    liberar_grafo_lista(g);

    g = novo_grafo(6);
    adicionar(g, 0, 1);
    adicionar(g, 2, 3);
    verificar(contar_componentes(g) == 4,
              "componentes: desconexo e vertices isolados");
    liberar_grafo_lista(g);
}

static void testar_ciclos(void)
{
    GrafoLista *g = novo_grafo(5);

    adicionar(g, 0, 1);
    adicionar(g, 1, 2);
    adicionar(g, 2, 3);
    adicionar(g, 3, 4);
    verificar(!tem_ciclo(g), "ciclo: caminho aciclico");
    liberar_grafo_lista(g);

    g = novo_grafo(3);
    adicionar(g, 0, 1);
    adicionar(g, 1, 2);
    adicionar(g, 2, 0);
    verificar(tem_ciclo(g), "ciclo: triangulo");
    liberar_grafo_lista(g);

    g = novo_grafo(7);
    adicionar(g, 0, 1);
    adicionar(g, 2, 3);
    adicionar(g, 3, 4);
    adicionar(g, 4, 2);
    adicionar(g, 5, 6);
    verificar(tem_ciclo(g), "ciclo: apenas um componente ciclico");
    liberar_grafo_lista(g);
}

static void testar_biparticao(void)
{
    GrafoLista *g = novo_grafo(4);

    adicionar(g, 0, 1);
    adicionar(g, 1, 2);
    adicionar(g, 2, 3);
    verificar(eh_bipartido(g), "biparticao: caminho");
    adicionar(g, 3, 0);
    verificar(eh_bipartido(g), "biparticao: ciclo par");
    liberar_grafo_lista(g);

    g = novo_grafo(3);
    adicionar(g, 0, 1);
    adicionar(g, 1, 2);
    adicionar(g, 2, 0);
    verificar(!eh_bipartido(g), "biparticao: ciclo impar");
    liberar_grafo_lista(g);

    g = novo_grafo(8);
    adicionar(g, 0, 1);
    adicionar(g, 2, 3);
    adicionar(g, 3, 4);
    adicionar(g, 4, 2);
    verificar(!eh_bipartido(g),
              "biparticao: ciclo impar em componente sem o vertice zero");
    liberar_grafo_lista(g);

    g = novo_grafo(5);
    adicionar(g, 0, 1);
    adicionar(g, 2, 3);
    verificar(eh_bipartido(g), "biparticao: desconexo com isolado");
    liberar_grafo_lista(g);
}

int main(void)
{
    testar_bfs();
    testar_dfs();
    testar_componentes();
    testar_ciclos();
    testar_biparticao();

    if (falhas != 0) {
        fprintf(stderr, "%d teste(s) falharam.\n", falhas);
        return EXIT_FAILURE;
    }

    printf("Pratica 02: todos os testes funcionais passaram.\n");
    return EXIT_SUCCESS;
}