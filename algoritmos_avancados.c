#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

Sala* criarSala(const char *nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala* atual) {
    char opcao;
    if (!atual) return;

    while (1) {
        printf("\nVoce esta na sala: %s\n", atual->nome);
        printf("Mover para (e) esquerda, (d) direita, (s) sair: ");
        scanf(" %c", &opcao);

        if (opcao == 's') break;
        else if (opcao == 'e' && atual->esquerda)
            atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita)
            atual = atual->direita;
        else
            printf("Caminho inexistente!\n");
    }
}

Sala* criarMapaFixo() {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sotão");
    Sala *porao = criarSala("Porão");

    // Conexões do mapa
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;
    cozinha->direita = porao;

    return hall;
}

typedef struct Pista {
    char texto[100];
    struct Pista *esq, *dir;
} Pista;

Pista* criarPista(const char *texto) {
    Pista *nova = (Pista*)malloc(sizeof(Pista));
    strcpy(nova->texto, texto);
    nova->esq = nova->dir = NULL;
    return nova;
}

Pista* inserirPista(Pista* raiz, const char *texto) {
    if (!raiz) return criarPista(texto);
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->dir = inserirPista(raiz->dir, texto);
    return raiz;
}

void listarPistas(Pista* raiz) {
    if (!raiz) return;
    listarPistas(raiz->esq);
    printf("🔹 %s\n", raiz->texto);
    listarPistas(raiz->dir);
}

#define TAM_Hash 10

typedef struct NodoPista {
    char pista[100];
    struct NodoPista *prox;
} NodoPista;

typedef struct Suspeito {
    char nome[50];
    NodoPista *pistas;
    struct Suspeito *prox;
} Suspeito;

typedef struct {
    Suspeito *tabela[TAM_Hash];
} Hash;

int hashFunc(const char *nome) {
    return toupper(nome[0]) % TAM_Hash;
}

void inicializarHash(Hash *h) {
    for (int i = 0; i < TAM_Hash; i++)
        h->tabela[i] = NULL;
}

Suspeito* buscarSuspeito(Hash *h, const char *nome) {
    int idx = hashFunc(nome);
    Suspeito *aux = h->tabela[idx];
    while (aux) {
        if (strcmp(aux->nome, nome) == 0) return aux;
        aux = aux->prox;
    }
    return NULL;
}

void inserirHash(Hash *h, const char *nome, const char *pista) {
    int idx = hashFunc(nome);
    Suspeito *sus = buscarSuspeito(h, nome);

    if (!sus) {
        sus = (Suspeito*)malloc(sizeof(Suspeito));
        strcpy(sus->nome, nome);
        sus->pistas = NULL;
        sus->prox = h->tabela[idx];
        h->tabela[idx] = sus;
    }

    NodoPista *nova = (NodoPista*)malloc(sizeof(NodoPista));
    strcpy(nova->pista, pista);
    nova->prox = sus->pistas;
    sus->pistas = nova;
}

void listarAssociacoes(Hash *h) {
    printf("\n=== SUSPEITOS E SUAS PISTAS ===\n");
    for (int i = 0; i < TAM_Hash; i++) {
        Suspeito *sus = h->tabela[i];
        while (sus) {
            printf("\n %s:\n", sus->nome);
            NodoPista *p = sus->pistas;
            while (p) {
                printf("    %s\n", p->pista);
                p = p->prox;
            }
            sus = sus->prox;
        }
    }
}

int main() {
    Sala *mapa = criarMapaFixo();
    Pista *arvorePistas = NULL;
    Hash tabelaSuspeitos;
    inicializarHash(&tabelaSuspeitos);

    int opcao;
    do {
        printf("\n=== DETECTIVE QUEST ===\n");
        printf("1 - Explorar a mansão\n");
        printf("2 - Coletar e listar pistas\n");
        printf("3 - Associar pista a suspeito\n");
        printf("4 - Mostrar suspeitos e pistas\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            explorarSalas(mapa);
        } 
        else if (opcao == 2) {
            char texto[100];
            printf("Digite a pista coletada: ");
            scanf(" %[^\n]", texto);
            arvorePistas = inserirPista(arvorePistas, texto);
            printf("Pista adicionada!\n\nPistas atuais:\n");
            listarPistas(arvorePistas);
        }
        else if (opcao == 3) {
            char nome[50], pista[100];
            printf("Nome do suspeito: ");
            scanf(" %[^\n]", nome);
            printf("Pista associada: ");
            scanf(" %[^\n]", pista);
            inserirHash(&tabelaSuspeitos, nome, pista);
        }
        else if (opcao == 4) {
            listarAssociacoes(&tabelaSuspeitos);
        }

    } while (opcao != 0);

    printf("\n Missão encerrada. Boa sorte na próxima investigação!\n");
    return 0;
}
