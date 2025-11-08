#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura base do território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Protótipos das funções ---
void cadastrarTerritorios(Territorio *mapa, int n);
void exibirTerritorios(Territorio *mapa, int n);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa, char *missao1, char *missao2);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, Territorio *mapa, int n);
void exibirMissao(char *missao);

// --- Função principal ---
int main() {
    srand(time(NULL)); // Garante aleatoriedade nos sorteios

    int n;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &n);

    // Alocação dinâmica dos territórios
    Territorio *mapa = (Territorio *) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    // --- Sistema de missões ---
    char *missoes[] = {
        "Conquistar 3 territórios seguidos.",
        "Eliminar todas as tropas da cor vermelha.",
        "Dominar metade dos territórios do mapa.",
        "Manter ao menos 2 territórios com mais de 10 tropas.",
        "Conquistar um território de cada cor existente."
    };
    int totalMissoes = 5;

    // Aloca memória para armazenar missões dos dois jogadores
    char *missaoJogador1 = (char *) malloc(100 * sizeof(char));
    char *missaoJogador2 = (char *) malloc(100 * sizeof(char));

    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    printf("\n=== Missões Iniciais ===\n");
    printf("Jogador 1: ");
    exibirMissao(missaoJogador1);
    printf("Jogador 2: ");
    exibirMissao(missaoJogador2);

    // --- Simulação de um ataque por turno ---
    int at, def;
    char continuar;
    do {
        exibirTerritorios(mapa, n);
        printf("\nEscolha o território atacante (índice): ");
        scanf("%d", &at);
        printf("Escolha o território defensor (índice): ");
        scanf("%d", &def);

        if (at == def) {
            printf("\nUm território não pode atacar a si mesmo!\n");
        } else if (strcmp(mapa[at].cor, mapa[def].cor) == 0) {
            printf("\nNão é possível atacar um território da mesma cor!\n");
        } else {
            atacar(&mapa[at], &mapa[def]);
        }

        // Verifica se algum jogador cumpriu a missão
        if (verificarMissao(missaoJogador1, mapa, n)) {
            printf("\n🏆 Jogador 1 venceu! Missão cumprida!\n");
            break;
        }
        if (verificarMissao(missaoJogador2, mapa, n)) {
            printf("\n🏆 Jogador 2 venceu! Missão cumprida!\n");
            break;
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    return 0;
}

// --- Função de cadastro dos territórios ---
void cadastrarTerritorios(Territorio *mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i + 1);
        printf("Nome: ");
        scanf(" %29[^\n]", mapa[i].nome);
        printf("Cor do exército: ");
        scanf(" %9s", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// --- Exibição de todos os territórios ---
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n=== Estado Atual dos Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\n[%d] %s\n", i, mapa[i].nome);
        printf("Cor: %s | Tropas: %d\n", mapa[i].cor, mapa[i].tropas);
    }
}

// --- Função de ataque ---
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 Dado atacante (%s): %d", atacante->nome, dadoAtacante);
    printf("\n🛡️ Dado defensor (%s): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n✅ O atacante venceu o combate!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        printf("\n❌ O defensor resistiu ao ataque!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// --- Sorteia e atribui missão ---
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// --- Exibe missão ao jogador ---
void exibirMissao(char *missao) {
    printf("%s\n", missao);
}

// --- Verifica se missão foi cumprida (versão simples) ---
int verificarMissao(char *missao, Territorio *mapa, int n) {
    // Exemplo básico de checagem por string:
    if (strstr(missao, "vermelha") != NULL) {
        int existeVermelha = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0)
                existeVermelha = 1;
        }
        return !existeVermelha; // vitória se não existir vermelho
    } else if (strstr(missao, "3 territórios") != NULL) {
        int cont = 0;
        for (int i = 0; i < n; i++) {
            if (mapa[i].tropas > 0) cont++;
        }
        return (cont >= 3);
    } else if (strstr(missao, "10 tropas") != NULL) {
        int grandes = 0;
        for (int i = 0; i < n; i++) {
            if (mapa[i].tropas > 10) grandes++;
        }
        return (grandes >= 2);
    }
    // Pode ser expandido com mais condições futuramente
    return 0;
}

// --- Libera toda a memória dinâmica ---
void liberarMemoria(Territorio *mapa, char *missao1, char *missao2) {
    free(mapa);
    free(missao1);
    free(missao2);
}
