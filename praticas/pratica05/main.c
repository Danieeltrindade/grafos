#include "coloracao.h"

#include <stdio.h>
#include <stdlib.h>

static int coloracao_valida(const GrafoLista *g, const int *cores) {
    int u;

    if (g == NULL || cores == NULL) {
        return 0;
    }

    for (u = 0; u < g->n; u++) {
        const No *vizinho = g->adj[u];
        while (vizinho != NULL) {
            if (cores[u] == cores[vizinho->destino]) {
                return 0;
            }
            vizinho = vizinho->prox;
        }
    }

    return 1;
}

static void imprimir_coloracao(const char *nome, const GrafoLista *g,
                               const int *cores, int num_cores) {
    int i;

    printf("%s (%d cores):", nome, num_cores);
    for (i = 0; i < g->n; i++) {
        printf(" %d:%d", i, cores[i]);
    }
    printf(" | valida: %s\n", coloracao_valida(g, cores) ? "sim" : "nao");
}

static int demonstrar_coloracoes(GrafoLista *g) {
    int num_cores_gulosa;
    int num_cores_welsh_powell;
    int *gulosa = coloracao_gulosa(g, &num_cores_gulosa);
    int *welsh_powell = coloracao_welsh_powell(g, &num_cores_welsh_powell);

    if (gulosa == NULL || welsh_powell == NULL) {
        free(gulosa);
        free(welsh_powell);
        return 0;
    }

    imprimir_coloracao("Gulosa", g, gulosa, num_cores_gulosa);
    imprimir_coloracao("Welsh-Powell", g, welsh_powell,
                       num_cores_welsh_powell);
    free(gulosa);
    free(welsh_powell);
    return 1;
}

int main(void) {
    GrafoLista *estrela = criar_grafo_lista(6);
    GrafoLista *ciclo_par = criar_grafo_lista(4);
    GrafoLista *desconectado = criar_grafo_lista(7);

    if (estrela == NULL || ciclo_par == NULL || desconectado == NULL) {
        liberar_grafo_lista(estrela);
        liberar_grafo_lista(ciclo_par);
        liberar_grafo_lista(desconectado);
        fprintf(stderr, "Falha ao alocar os grafos.\n");
        return EXIT_FAILURE;
    }

    /* O vertice 4, de maior grau, deve ser considerado primeiro por Welsh-Powell. */
    inserir_aresta_lista(estrela, 4, 0);
    inserir_aresta_lista(estrela, 4, 1);
    inserir_aresta_lista(estrela, 4, 2);
    inserir_aresta_lista(estrela, 4, 3);
    inserir_aresta_lista(estrela, 4, 5);

    inserir_aresta_lista(ciclo_par, 0, 1);
    inserir_aresta_lista(ciclo_par, 1, 2);
    inserir_aresta_lista(ciclo_par, 2, 3);
    inserir_aresta_lista(ciclo_par, 3, 0);

    /* Caminho em 0--1--2 e triangulo separado em 3--4--5--3; 6 e isolado. */
    inserir_aresta_lista(desconectado, 0, 1);
    inserir_aresta_lista(desconectado, 1, 2);
    inserir_aresta_lista(desconectado, 3, 4);
    inserir_aresta_lista(desconectado, 4, 5);
    inserir_aresta_lista(desconectado, 5, 3);

    printf("Estrela com graus diferentes:\n");
    if (!demonstrar_coloracoes(estrela)) {
        fprintf(stderr, "Falha ao calcular as coloracoes.\n");
        liberar_grafo_lista(estrela);
        liberar_grafo_lista(ciclo_par);
        liberar_grafo_lista(desconectado);
        return EXIT_FAILURE;
    }

    printf("Bipartido (estrela): %s\n",
           eh_bipartido(estrela) ? "sim" : "nao");
    printf("Bipartido (ciclo par): %s\n",
           eh_bipartido(ciclo_par) ? "sim" : "nao");
    printf("Bipartido (desconectado com triangulo): %s\n",
           eh_bipartido(desconectado) ? "sim" : "nao");

    liberar_grafo_lista(estrela);
    liberar_grafo_lista(ciclo_par);
    liberar_grafo_lista(desconectado);
    return EXIT_SUCCESS;
}