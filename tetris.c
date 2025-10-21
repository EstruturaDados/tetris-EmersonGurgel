#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo a capacidade máxima da fila de peças futuras
#define CAPACIDADE 5
// Número de peças para a inicialização da fila
#define NUM_PECAS_INICIAIS 5

// Variável global para gerar IDs únicos para as peças
int proximo_id = 0;

// Estrutura para representar uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura para representar a Fila Circular de Peças
typedef struct {
    Peca elementos[CAPACIDADE];
    int frente;   // Índice do elemento da frente (próximo a ser removido)
    int traseira; // Índice da posição onde o próximo elemento será inserido
    int tamanho;  // Número atual de elementos na fila
} Fila;

/**
 * @brief Inicializa a fila definindo os índices e o tamanho inicial.
 * @param fila Ponteiro para a estrutura Fila a ser inicializada.
 */
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->traseira = 0; // Posição do próximo a ser inserido (vazio)
    fila->tamanho = 0;
}

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 * @return A nova Peca gerada.
 */
Peca gerarPeca() {
    Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice_tipo = rand() % 4; // Escolhe um tipo aleatório
    
    nova_peca.nome = tipos[indice_tipo];
    nova_peca.id = proximo_id++; // Atribui o ID e incrementa para a próxima peça
    
    return nova_peca;
}

/**
 * @brief Verifica se a fila está vazia.
 * @param fila Ponteiro para a estrutura Fila.
 * @return 1 se vazia, 0 caso contrário.
 */
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

/**
 * @brief Verifica se a fila está cheia.
 * @param fila Ponteiro para a estrutura Fila.
 * @return 1 se cheia, 0 caso contrário.
 */
int filaCheia(Fila *fila) {
    return fila->tamanho == CAPACIDADE;
}

/**
 * @brief Insere uma nova peça no final da fila (enqueue).
 * @param fila Ponteiro para a estrutura Fila.
 * @param nova_peca A peça a ser inserida.
 * @return 1 se a inserção foi bem-sucedida, 0 caso contrário.
 */
int enqueue(Fila *fila, Peca nova_peca) {
    if (filaCheia(fila)) {
        printf("\nERRO: Fila cheia. Não é possível inserir mais peças.\n");
        return 0;
    }
    
    // Insere a peça na posição 'traseira'
    fila->elementos[fila->traseira] = nova_peca;
    // Atualiza a 'traseira' para o próximo índice (operação circular)
    fila->traseira = (fila->traseira + 1) % CAPACIDADE;
    fila->tamanho++;
    
    printf("\nSUCESSO: Peça [%c %d] adicionada à fila.\n", nova_peca.nome, nova_peca.id);
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * @param fila Ponteiro para a estrutura Fila.
 * @param peca_removida Ponteiro para armazenar a peça que foi removida.
 * @return 1 se a remoção foi bem-sucedida, 0 caso contrário.
 */
int dequeue(Fila *fila, Peca *peca_removida) {
    if (filaVazia(fila)) {
        printf("\nERRO: Fila vazia. Não há peças para jogar.\n");
        return 0;
    }
    
    // Pega a peça da frente
    *peca_removida = fila->elementos[fila->frente];
    // Atualiza a 'frente' para o próximo índice (operação circular)
    fila->frente = (fila->frente + 1) % CAPACIDADE;
    fila->tamanho--;
    
    printf("\nSUCESSO: Peça [%c %d] jogada (removida da frente).\n", peca_removida->nome, peca_removida->id);
    return 1;
}

/**
 * @brief Exibe o estado atual da fila de forma clara.
 * @param fila Ponteiro para a estrutura Fila.
 */
void exibirFila(Fila *fila) {
    printf("\n--- Estado Atual da Fila de Peças Futuras (CAPACIDADE: %d, Tamanho Atual: %d) ---\n", CAPACIDADE, fila->tamanho);
    
    if (filaVazia(fila)) {
        printf("Fila de peças: [VAZIA]\n");
        return;
    }

    printf("Fila de peças: ");
    
    // O índice de início é a frente
    int i = fila->frente;
    // O número de iterações é o tamanho da fila
    for (int count = 0; count < fila->tamanho; count++) {
        // Exibe o elemento atual
        printf("[%c %d]", fila->elementos[i].nome, fila->elementos[i].id);
        
        // Move para o próximo índice de forma circular
        i = (i + 1) % CAPACIDADE;
        
        if (count < fila->tamanho - 1) {
            printf(" -> "); // Separador visual
        }
    }
    printf("\n--------------------------------------------------------------------------------\n");
}

/**
 * @brief Função principal que gerencia o menu e a simulação.
 */
int main() {
    // Inicializa o gerador de números aleatórios para a função gerarPeca
    srand(time(NULL));
    
    Fila fila_pecas;
    inicializarFila(&fila_pecas);
    
    // 1. Inicializa a fila com o número fixo de elementos
    printf("Inicializando a fila de peças com %d elementos...\n", NUM_PECAS_INICIAIS);
    for (int i = 0; i < NUM_PECAS_INICIAIS; i++) {
        Peca nova = gerarPeca();
        // Usamos enqueue, mas ignoramos a verificação de cheia aqui, pois sabemos que há espaço
        fila_pecas.elementos[fila_pecas.traseira] = nova;
        fila_pecas.traseira = (fila_pecas.traseira + 1) % CAPACIDADE;
        fila_pecas.tamanho++;
    }
    printf("Inicialização concluída.\n");
    
    int opcao = -1;
    Peca peca_removida;
    
    do {
        // Exibe o estado atual da fila, conforme requisito
        exibirFila(&fila_pecas);
        
        // Apresenta as opções do menu
        printf("\nOpções de ação:\n");
        printf("Código | Ação\n");
        printf("-------|------------------------------\n");
        printf("   1   | Jogar peça (dequeue)\n");
        printf("   2   | Inserir nova peça (enqueue)\n");
        printf("   0   | Sair\n");
        printf("--------------------------------------\n");
        printf("Digite o código da ação: ");
        
        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            // Se a leitura falhar (ex: usuário digitar texto)
            printf("\nOpção inválida. Por favor, digite um número.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            opcao = -1; // Garante que o loop continue
            continue;
        }
        
        // Lógica de controle baseada na opção
        switch (opcao) {
            case 1: // Jogar peça (dequeue)
                // O retorno da função indica se a operação foi um sucesso (não vazia)
                dequeue(&fila_pecas, &peca_removida);
                // A peça que foi "jogada" fica em 'peca_removida'
                break;
                
            case 2: // Inserir nova peça (enqueue)
                // 1. Gera automaticamente a nova peça
                peca_removida = gerarPeca(); // Reutilizando a variável para armazenar a nova peça
                // 2. Tenta inserir na fila
                enqueue(&fila_pecas, peca_removida);
                break;
                
            case 0: // Sair
                printf("\nSaindo do simulador Tetris Stack. Até a próxima!\n");
                break;
                
            default: // Opção inválida
                printf("\nOpção inválida. Por favor, escolha 1, 2 ou 0.\n");
                break;
        }
        
    } while (opcao != 0);
    
    return 0;
}