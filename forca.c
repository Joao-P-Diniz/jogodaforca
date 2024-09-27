#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define  TAMANHO_ALFABETO 26
#define TENTATIVAS_MAXIMAS 6 // Limite de chances

// Estrutura de nó da arvore TRIE
typedef struct TrieNode {
    struct TrieNode* filhos[TAMANHO_ALFABETO];
    bool fimDaPalavra; // Marca se é o fim de uma palavra
} TrieNode;

// Cria um novo nó da TRIE
TrieNode* criarNo() {
    TrieNode* novoNo = (TrieNode*)malloc(sizeof(TrieNode));
    novoNo->fimDaPalavra = false;
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        novoNo->filhos[i] = NULL; // Inicializa filhos como NULL
    }
    return novoNo;
}

// Adiciona uma palavra na TRIE
void adicionarPalavra(TrieNode* raiz, const char* palavra) {
    TrieNode* noAtual = raiz;
    while (*palavra) {
        int indice = *palavra - 'a'; // Pega o índice baseado na letra
        if (noAtual->filhos[indice] == NULL) {
            noAtual->filhos[indice] = criarNo(); // Cria novo nó se não existir
        }
        noAtual = noAtual->filhos[indice];
        palavra++;
    }
    noAtual->fimDaPalavra = true; // Marca o fim da palavra
}

// Conta quantas palavras tem na TRIE
int contarPalavras(TrieNode* raiz) {
    if (!raiz) return 0;
    int total = 0;
    if (raiz->fimDaPalavra) total++; // Se é fim de palavra, conta

    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (raiz->filhos[i]) {
            total += contarPalavras(raiz->filhos[i]); // Conta recursivamente
        }
    }
    return total;
}

// Seleciona uma palavra aleatória pelo índice
bool pegarPalavraAleatoriaPorIndice(TrieNode* raiz, char* palavraResultado, int* indiceAleatorio, int nivel) {
    if (!raiz) return false;

    if (raiz->fimDaPalavra) {
        if (*indiceAleatorio == 0) {
            palavraResultado[nivel] = '\0'; // Termina a palavra
            return true;
        }
        (*indiceAleatorio)--; // Decrementa o índice
    }

    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (raiz->filhos[i]) {
            palavraResultado[nivel] = i + 'a'; // Coloca a letra na palavra
            if (pegarPalavraAleatoriaPorIndice(raiz->filhos[i], palavraResultado, indiceAleatorio, nivel + 1)) {
                return true;
            }
        }
    }
    return false;
}

// Pega uma palavra aleatória da TRIE
void pegarPalavraAleatoria(TrieNode* raiz, char* palavraResultado) {
    int totalPalavras = contarPalavras(raiz);
    if (totalPalavras == 0) {
        printf("O dicionario ta vazio.\n");
        return;
    }

    // Gera um índice aleatório
    int indiceAleatorio = rand() % totalPalavras;

    // Busca a palavra com o índice aleatório
    pegarPalavraAleatoriaPorIndice(raiz, palavraResultado, &indiceAleatorio, 0);
}

// Carrega palavras para o dicionário (simulado)
void carregarDicionario(TrieNode* raiz) {
    const char* palavras[] = {"vermelho", "amarelo", "verde","branco","mexerica", "laranja", "melancia", "pera", "abacaxi", "morango", "azul"};
    int qtdPalavras = sizeof(palavras) / sizeof(palavras[0]);

    for (int i = 0; i < qtdPalavras; i++) {
        adicionarPalavra(raiz, palavras[i]); // adiciona palavras
    }

    printf("%d palavras foram carregadas no dicionario.\n", qtdPalavras);
}

// Verifica se a letra tá na palavra
bool verificarLetraNaPalavra(char letra, const char* palavraSecreta, char* palavraTentativa) {
    bool achou = false;
    for (int i = 0; palavraSecreta[i] != '\0'; i++) {
        if (palavraSecreta[i] == letra) {
            palavraTentativa[i] = letra; // Revela a letra na tentativa
            achou = true; // Marca que achou a letra
        }
    }
    return achou;
}

// Verifica se o jogador adivinhou a palavra toda
bool palavraTotalmenteAdivinhada(const char* palavraTentativa) {
    for (int i = 0; palavraTentativa[i] != '\0'; i++) {
        if (palavraTentativa[i] == '_') {
            return false; // Se tiver sublinhado, não tá adivinhada
        }
    }
    return true; // Tudo adivinhado!
}

// Mostra o estado da palavra
void mostrarEstadoDaPalavra(const char* palavraTentativa) {
    printf("\nPalavra: ");
    for (int i = 0; palavraTentativa[i] != '\0'; i++) {
        printf("%c ", palavraTentativa[i]); // Mostra cada letra ou sublinhado
    }
    printf("\n");
}

// Mostra todas as palavras na TRIE
void mostrarPalavrasDaTrie(TrieNode* raiz, char* palavraAtual, int nivel) {
    if (raiz->fimDaPalavra) {
        palavraAtual[nivel] = '\0'; // Termina a palavra
        printf("%s\n", palavraAtual); // Mostra a palavra
    }

    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (raiz->filhos[i] != NULL) {
            palavraAtual[nivel] = 'a' + i; // Adiciona a letra atual
            mostrarPalavrasDaTrie(raiz->filhos[i], palavraAtual, nivel + 1); // Busca recursivamente
        }
    }
}

// Chama a função de mostrar as palavras com parâmetros iniciais
void exibirTrie(TrieNode* raiz) {
    char palavraAtual[100];
    mostrarPalavrasDaTrie(raiz, palavraAtual, 0);
}

int main() {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios

    TrieNode* raiz = criarNo();
    
    // Carregar um conjunto de palavras no dicionário
    carregarDicionario(raiz);

    // Mostrar a árvore TRIE
    printf("\nPalavras na arvore TRIE:\n");
    exibirTrie(raiz);

    // Selecionar uma palavra aleatória
    char palavraSecreta[100];
    pegarPalavraAleatoria(raiz, palavraSecreta);

    int tamanhoPalavra = strlen(palavraSecreta);
    char palavraTentativa[100];

    // Inicializa a palavra escondida com sublinhados
    for (int i = 0; i < tamanhoPalavra; i++) {
        palavraTentativa[i] = '_';
    }
    palavraTentativa[tamanhoPalavra] = '\0';

    int tentativasRestantes = TENTATIVAS_MAXIMAS;  // chances restantes
    char letraTentativa;
    bool ganhou = false;

    printf("\nBem-vindo ao jogo da forca!\n");
    printf("A palavra tem %d letras.\n", tamanhoPalavra);

    // Loop principal do jogo
    while (tentativasRestantes > 0 && !ganhou) {
        mostrarEstadoDaPalavra(palavraTentativa); // Exibe o estado atual da palavra
        printf("chances restantes: %d\n", tentativasRestantes);
        printf("Digite uma letra: ");
        scanf(" %c", &letraTentativa);

        if (verificarLetraNaPalavra(letraTentativa, palavraSecreta, palavraTentativa)) {
            printf("Acertou!\n");
        } else {
            tentativasRestantes--; // Perde uma tentativa
            printf("Letra errada.\n");
        }

        // Verifica se a palavra foi adivinhada
        if (palavraTotalmenteAdivinhada(palavraTentativa)) {
            ganhou = true; // Se a palavra foi adivinhada, marca como ganhador
        }
    }

    // Resultado final
    if (ganhou) {
        printf("\nParabens! Voce acertou a palavra: %s\n", palavraSecreta);
    } else {
        mostrarEstadoDaPalavra(palavraTentativa); // Mostra a última tentativa
        printf("\nVoce perdeu. A palavra era: %s\n", palavraSecreta);
    }

    return 0;
}
