#include <stdio.h>    // printf, scanf
#include <stdlib.h>   // rand, srand
#include <time.h>     // time (semente para rand)

/* Poucos assentos, fácil de visualizar na tela */
#define LINHAS 5
#define COLUNAS 6

/* ---------------------- PROTÓTIPOS (funções) ------------------- */
void zerarSala(int sala[LINHAS][COLUNAS]);
void preencherManual(int sala[LINHAS][COLUNAS]);
void preencherAutomatico(int sala[LINHAS][COLUNAS], double probOcupado);
void exibirSala(int sala[LINHAS][COLUNAS]);
void alternarAssento(int sala[LINHAS][COLUNAS], int i, int j);
void contarAssentos(int sala[LINHAS][COLUNAS], int *ocupados, int *livres);
int  buscarPrimeiroLivre(int sala[LINHAS][COLUNAS], int *outI, int *outJ);
void transporSala(int sala[LINHAS][COLUNAS], int T[COLUNAS][LINHAS]);

/* ------------------------------ MAIN --------------------------- */
int main(void) {
    /* Matriz 2D representando a sala: linhas = fileiras, colunas = cadeiras */
    int sala[LINHAS][COLUNAS];

    /* Matriz auxiliar para transposição (somente para a opção 7) */
    int T[COLUNAS][LINHAS];

    /* Semente p/ aleatoriedade (apenas uma vez no programa) */
    srand((unsigned)time(NULL));

    /* Começamos com a sala toda zerada (todo mundo livre) */
    zerarSala(sala);

    /* Variáveis para o menu e entradas do usuário */
    int opcao = -1;
    while (opcao != 0) {
        
        /* Menu textual simples */
        printf("\n========== CINEMA: Mapa de Ocupacao (%dx%d) ==========\n", LINHAS, COLUNAS);
        printf("1) Preencher MANUALMENTE (0=livre, 1=ocupado)\n");
        printf("2) Preencher AUTOMATICAMENTE (prob. fixa de ocupado)\n");
        printf("3) Exibir mapa de assentos\n");
        printf("4) Reservar/Cancelar (alternar) um assento\n");
        printf("5) Contar ocupados e livres (soma)\n");
        printf("6) Buscar primeiro assento LIVRE\n");
        printf("7) [Opcional] Mostrar TRANSPOSTA da sala\n");
        printf("0) Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            /* Falha de leitura (ex.: digitou letra, encerra educadamente */
            printf("Entrada invalida. Encerrando.\n");
            return 1;
        }

        /* IF...ELSE encadeado */
        if (opcao == 1) {
            /* Preenchimento manual reforça a indexação [i][j] e scanf */
            preencherManual(sala);
        }
        else if (opcao == 2) {
            /* Probabilidade fixa de ocupado (ex.: 40%%) para gerar cenários */
            double prob = 0.40; /* ajuste se quiser um cinema mais vazio/cheio */
            preencherAutomatico(sala, prob);
            printf("Sala preenchida automaticamente com prob=%.0f%% ocupado.\n", prob*100.0);
        }
        else if (opcao == 3) {
            /* Exibição formatada, com rótulos de linha/coluna */
            exibirSala(sala);
        }
        else if (opcao == 4) {
            /* Alternar assento: se 0 - 1 (reservar), se 1 - 0 (cancelar) */
            int i, j;
            printf("Informe linha (0..%d) e coluna (0..%d): ", LINHAS-1, COLUNAS-1);
            if (scanf("%d %d", &i, &j) == 2 &&
                i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS) {
                alternarAssento(sala, i, j);
                printf("Estado do assento [%d][%d] foi alternado.\n", i, j);
            } else {
                printf("Coordenadas invalidas.\n");
            }
        }
        else if (opcao == 5) {
            /* Soma de elementos: total de ocupados e livres */
            int ocup, liv;
            contarAssentos(sala, &ocup, &liv);
            printf("Ocupados: %d | Livres: %d | Total: %d\n", ocup, liv, ocup+liv);
        }
        else if (opcao == 6) {
            /* Busca linear 2D pelo primeiro assento livre */
            int iLivre, jLivre;
            if (buscarPrimeiroLivre(sala, &iLivre, &jLivre)) {
                printf("Primeiro livre encontrado em [%d][%d]\n", iLivre, jLivre);
            } else {
                printf("Nao ha assentos livres.\n");
            }
        }
        else if (opcao == 7) {
            /* Transpor a sala */
            transporSala(sala, T);
            printf("\n--- TRANSPOSTA (colunas viram linhas) ---\n");
            /* Exibe T usando a mesma lógica de exibição, adaptada */
            printf("      ");
            for (int j = 0; j < LINHAS; j++) printf(" C%d", j);
            printf("\n");
            for (int i = 0; i < COLUNAS; i++) {
                printf("L%d -> ", i);
                for (int j = 0; j < LINHAS; j++) {
                    printf(" %d", T[i][j]);
                }
                printf("\n");
            }
        }
        else if (opcao == 0) {
            printf("Saindo. Ate a proxima sessao!\n");
        }
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}

/* -------------------------- IMPLEMENTAÇÕES ---------------------- */

/* Coloca tudo como livre (0) — útil para reiniciar a atividade */
void zerarSala(int sala[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            sala[i][j] = 0;
        }
    }
}

/* Preenchimento MANUAL: aluno digita 0/1 para cada [linha][coluna] */
void preencherManual(int sala[LINHAS][COLUNAS]) {
    printf("\nPreenchimento manual (0=livre, 1=ocupado):\n");
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            int v;
            printf("Assento [%d][%d]: ", i, j);
            if (scanf("%d", &v) != 1 || (v != 0 && v != 1)) {
                printf("Valor invalido. Definindo 0 (livre).\n");
                v = 0;
            }
            sala[i][j] = v;
        }
    }
}

/* Preenchimento AUTOMÁTICO: probabilidade p de um assento estar ocupado */
void preencherAutomatico(int sala[LINHAS][COLUNAS], double probOcupado) {
    /* Garante probabilidade no intervalo [0,1] de forma simples */
    if (probOcupado < 0.0) probOcupado = 0.0;
    if (probOcupado > 1.0) probOcupado = 1.0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            /* rand()%%100 gera 0..99 → compara com prob*100 */
            int r = rand() % 100;
            sala[i][j] = (r < (int)(probOcupado * 100.0)) ? 1 : 0;
        }
    }
}

/* Exibe a sala com rótulos e 0/1 (poderia usar 'L' e 'X', mas manteremos 0/1) */
void exibirSala(int sala[LINHAS][COLUNAS]) {
    printf("\nMapa (0=livre, 1=ocupado):\n");
    printf("      ");
    for (int j = 0; j < COLUNAS; j++) printf(" C%d", j);
    printf("\n");
    for (int i = 0; i < LINHAS; i++) {
        printf("L%d -> ", i);
        for (int j = 0; j < COLUNAS; j++) {
            printf(" %d", sala[i][j]);
        }
        printf("\n");
    }
}

/* Alterna um assento: se 0 vira 1, se 1 vira 0 (reservar/cancelar) */
void alternarAssento(int sala[LINHAS][COLUNAS], int i, int j) {
    sala[i][j] = (sala[i][j] == 0) ? 1 : 0;
}

/* Conta quantos ocupados (=1) e livres (=0) existem (soma dos elementos) */
void contarAssentos(int sala[LINHAS][COLUNAS], int *ocupados, int *livres) {
    int oc = 0, lv = 0;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (sala[i][j] == 1) oc++;
            else                 lv++;
        }
    }
    *ocupados = oc;
    *livres   = lv;
}

/* Busca linear: retorna 1 se achou um livre, salvando coordenadas; caso contrário, 0 */
int buscarPrimeiroLivre(int sala[LINHAS][COLUNAS], int *outI, int *outJ) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (sala[i][j] == 0) {
                *outI = i;
                *outJ = j;
                return 1; /* encontrou o primeiro livre */
            }
        }
    }
    return 0; /* não encontrou nenhum livre */
}

/* Transposição: T[coluna][linha] = sala[linha][coluna] (recurso didático) */
void transporSala(int sala[LINHAS][COLUNAS], int T[COLUNAS][LINHAS]) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            T[j][i] = sala[i][j];
        }
    }
}