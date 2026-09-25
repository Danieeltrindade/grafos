#include "dag.h"

#include <stdio.h>
#include <stdlib.h>

static void imprimir_ordem(const char *nome, int *ordem, int tamanho)
{
    int i;

    printf("%s:", nome);
    if (ordem == NULL) {
        printf(" impossivel (o digrafo possui ciclo)\n");
        return;
    }

    for (i = 0; i < tamanho; i++) {
        printf(" %d", ordem[i]);
    }
    printf("\n");
}

int main(void)
{
    GrafoLista *dag = criar_grafo_lista(6);
    GrafoLista *ciclico = criar_grafo_lista(3);
    int *ordem_kahn;
    int *ordem_dfs;
    int tamanho_kahn;
    int tamanho_dfs;

    if (dag == NULL || ciclico == NULL) {
        fprintf(stderr, "Erro ao criar os grafos.\n");
        liberar_grafo_lista(dag);
        liberar_grafo_lista(ciclico);
        return EXIT_FAILURE;
    }

    if (!inserir_aresta_lista(dag, 0, 1) ||
        !inserir_aresta_lista(dag, 0, 2) ||
        !inserir_aresta_lista(dag, 1, 3) ||
        !inserir_aresta_lista(dag, 2, 3) ||
        !inserir_aresta_lista(dag, 4, 5) ||
        !inserir_aresta_lista(ciclico, 0, 1) ||
        !inserir_aresta_lista(ciclico, 1, 2) ||
        !inserir_aresta_lista(ciclico, 2, 0)) {
        fprintf(stderr, "Erro ao inserir uma aresta.\n");
        liberar_grafo_lista(dag);
        liberar_grafo_lista(ciclico);
        return EXIT_FAILURE;
    }

    printf("Primeiro digrafo e DAG? %s\n", eh_dag(dag) ? "sim" : "nao");
    ordem_kahn = ordenacao_topologica_kahn(dag, &tamanho_kahn);
    ordem_dfs = ordenacao_topologica_dfs(dag, &tamanho_dfs);
    imprimir_ordem("Kahn", ordem_kahn, tamanho_kahn);
    imprimir_ordem("DFS", ordem_dfs, tamanho_dfs);
    free(ordem_kahn);
    free(ordem_dfs);

    printf("Segundo digrafo e DAG? %s\n",
           eh_dag(ciclico) ? "sim" : "nao");
    ordem_kahn = ordenacao_topologica_kahn(ciclico, &tamanho_kahn);
    ordem_dfs = ordenacao_topologica_dfs(ciclico, &tamanho_dfs);
    imprimir_ordem("Kahn", ordem_kahn, tamanho_kahn);
    imprimir_ordem("DFS", ordem_dfs, tamanho_dfs);
    free(ordem_kahn);
    free(ordem_dfs);

    liberar_grafo_lista(dag);
    liberar_grafo_lista(ciclico);
    return EXIT_SUCCESS;
}