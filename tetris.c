#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Capacidades definidas no requisito
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3
#define NUM_PECAS_INICIAIS CAPACIDADE_FILA

// Variável global para gerar IDs únicos para as peças
int proximo_id = 0;

// Estrutura para representar uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura para representar a Fila Circular de Peças
typedef struct {
    Peca elementos[CAPACIDADE_FILA];
    int frente;   // Índice do elemento da frente (próximo a ser removido)
    int traseira; // Índice da posição onde o próximo elemento será inserido
    int tamanho;  // Número atual de elementos na fila
} Fila;

// Estrutura para representar a Pilha de Peças Reservadas
typedef struct {
    Peca elementos[CAPACIDADE_PILHA];
    int topo; // Índice do topo da pilha (próxima posição livre)
} Pilha;


/* =========================================================================
 * 1. FUNÇÕES GERAIS DE PEÇAS E ESTRUTURAS
 * ========================================================================= */

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 * @return A nova Peca gerada.
 */
Peca gerarPeca() {
    Peca nova_peca;
    // Tipos de peças do Tetris
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    // Usamos 7 tipos, mas você pode ajustar para os 4 especificados ('I', 'O', 'T', 'L')
    int num_tipos = 7; 
    
    int indice_tipo = rand() % num_tipos; // Escolhe um tipo aleatório
    
    nova_peca.nome = tipos[indice_tipo];
    nova_peca.id = proximo_id++; // Atribui o ID e incrementa para a próxima peça
    
    return nova_peca;
}

/**
 * @brief Inicializa a fila.
 */
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->traseira = 0;
    fila->tamanho = 0;
}

/**
 * @brief Inicializa a pilha.
 */
void inicializarPilha(Pilha *pilha) {
    // O 'topo' inicia em 0, indicando que a primeira posição (índice 0) está livre
    pilha->topo = 0; 
}


/* =========================================================================
 * 2. FUNÇÕES DA FILA CIRCULAR (QUEUE)
 * ========================================================================= */

/**
 * @brief Verifica se a fila está vazia.
 */
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

/**
 * @brief Verifica se a fila está cheia.
 */
int filaCheia(Fila *fila) {
    return fila->tamanho == CAPACIDADE_FILA;
}

/**
 * @brief Insere uma nova peça no final da fila (enqueue).
 */
int enqueue(Fila *fila, Peca nova_peca) {
    if (filaCheia(fila)) {
        // No contexto deste jogo, a fila nunca deve estar cheia se a reposição for feita corretamente
        return 0; 
    }
    
    fila->elementos[fila->traseira] = nova_peca;
    fila->traseira = (fila->traseira + 1) % CAPACIDADE_FILA;
    fila->tamanho++;
    
    printf("-> Reposição: Peça [%c %d] adicionada ao final da fila.\n", nova_peca.nome, nova_peca.id);
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 */
int dequeue(Fila *fila, Peca *peca_removida) {
    if (filaVazia(fila)) {
        printf("\nERRO: Fila vazia. Não há peças futuras.\n");
        return 0;
    }
    
    *peca_removida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;
    
    return 1;
}

/* =========================================================================
 * 3. FUNÇÕES DA PILHA (STACK)
 * ========================================================================= */

/**
 * @brief Verifica se a pilha está vazia.
 */
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == 0;
}

/**
 * @brief Verifica se a pilha está cheia.
 */
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == CAPACIDADE_PILHA;
}

/**
 * @brief Adiciona uma peça ao topo da pilha (push).
 */
int push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("\nERRO: Pilha de reserva cheia (Máx: %d). Não foi possível reservar a peça.\n", CAPACIDADE_PILHA);
        return 0;
    }
    
    pilha->elementos[pilha->topo] = peca;
    pilha->topo++; // O topo aponta para a próxima posição livre
    
    printf("\nSUCESSO: Peça [%c %d] reservada (adicionada ao topo da pilha).\n", peca.nome, peca.id);
    return 1;
}

/**
 * @brief Remove a peça do topo da pilha (pop).
 */
int pop(Pilha *pilha, Peca *peca_removida) {
    if (pilhaVazia(pilha)) {
        printf("\nERRO: Pilha de reserva vazia. Não há peças reservadas para usar.\n");
        return 0;
    }
    
    pilha->topo--; // Move o topo para o elemento que será removido
    *peca_removida = pilha->elementos[pilha->topo];
    
    printf("\nSUCESSO: Peça [%c %d] removida da reserva (usada).\n", peca_removida->nome, peca_removida->id);
    return 1;
}


/* =========================================================================
 * 4. FUNÇÕES DE EXIBIÇÃO
 * ========================================================================= */

/**
 * @brief Exibe o estado atual da fila e da pilha.
 */
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n=======================================================================\n");
    printf("ESTADO ATUAL\n");
    
    // --- Exibição da Fila ---
    printf("Fila de peças futuras (Frente -> Traseira, Tam: %d/%d): ", fila->tamanho, CAPACIDADE_FILA);
    if (filaVazia(fila)) {
        printf("[VAZIA]");
    } else {
        int i = fila->frente;
        for (int count = 0; count < fila->tamanho; count++) {
            printf("[%c %d]", fila->elementos[i].nome, fila->elementos[i].id);
            i = (i + 1) % CAPACIDADE_FILA;
            if (count < fila->tamanho - 1) {
                printf(" -> ");
            }
        }
    }
    printf("\n");
    
    // --- Exibição da Pilha ---
    printf("Pilha de reserva (Topo -> Base, Tam: %d/%d): ", pilha->topo, CAPACIDADE_PILHA);
    if (pilhaVazia(pilha)) {
        printf("[VAZIA]");
    } else {
        // Itera de cima para baixo (topo-1 até 0)
        for (int i = pilha->topo - 1; i >= 0; i--) {
            printf("[%c %d]", pilha->elementos[i].nome, pilha->elementos[i].id);
            if (i > 0) {
                printf(" -> ");
            }
        }
    }
    printf("\n");
    printf("=======================================================================\n");
}

/* =========================================================================
 * 5. FUNÇÃO PRINCIPAL (MAIN)
 * ========================================================================= */

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    Fila fila_pecas;
    Pilha pilha_reserva;
    
    inicializarFila(&fila_pecas);
    inicializarPilha(&pilha_reserva);
    
    // 1. Inicializa a fila com o número fixo de elementos
    printf("Inicializando o simulador Tetris Stack...\n");
    printf("Preenchendo a fila de peças com %d elementos iniciais...\n", NUM_PECAS_INICIAIS);
    for (int i = 0; i < NUM_PECAS_INICIAIS; i++) {
        Peca nova = gerarPeca();
        fila_pecas.elementos[fila_pecas.traseira] = nova;
        fila_pecas.traseira = (fila_pecas.traseira + 1) % CAPACIDADE_FILA;
        fila_pecas.tamanho++;
    }
    printf("Fila inicializada.\n");
    
    int opcao = -1;
    Peca peca_operacao; // Variável temporária para guardar a peça movida/jogada
    
    do {
        // Exibe o estado atual da fila e da pilha
        exibirEstado(&fila_pecas, &pilha_reserva);
        
        // Apresenta as opções do menu
        printf("\nOpções de Ação:\n");
        printf("Código | Ação\n");
        printf("-------|--------------------------------------\n");
        printf("   1   | Jogar peça (Dequeue da Fila)\n");
        printf("   2   | Reservar peça (Fila -> Pilha)\n");
        printf("   3   | Usar peça reservada (Pop da Pilha)\n");
        printf("   0   | Sair\n");
        printf("---------------------------------------------\n");
        printf("Digite o código da ação: ");
        
        // Leitura da opção, com tratamento de erro básico
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpção inválida. Por favor, digite um número.\n");
            while (getchar() != '\n'); // Limpa o buffer
            opcao = -1; 
            continue;
        }
        
        // Flag para verificar se a fila precisa de reposição
        int precisa_reposicao = 0; 
        
        switch (opcao) {
            case 1: // 1. Jogar peça (dequeue da fila)
                if (dequeue(&fila_pecas, &peca_operacao)) {
                    printf("\nSUCESSO: Peça [%c %d] jogada (removida da frente da fila).\n", peca_operacao.nome, peca_operacao.id);
                    precisa_reposicao = 1; // A fila precisa ser reabastecida
                }
                break;
                
            case 2: // 2. Reservar peça (Fila -> Pilha)
                // Tenta remover da fila primeiro
                if (dequeue(&fila_pecas, &peca_operacao)) {
                    // Tenta adicionar à pilha
                    if (push(&pilha_reserva, peca_operacao)) {
                        precisa_reposicao = 1; // A fila precisa ser reabastecida, pois a peça saiu dela
                    } else {
                        // Se falhar o push, a peça é perdida (simplificação)
                        printf("AVISO: A peça [%c %d] foi perdida pois a reserva está cheia.\n", peca_operacao.nome, peca_operacao.id);
                        precisa_reposicao = 1;
                    }
                }
                break;
                
            case 3: // 3. Usar peça reservada (pop da pilha)
                // Não causa reposição na fila, pois a peça saiu da pilha, não da fila
                pop(&pilha_reserva, &peca_operacao);
                break;
                
            case 0: // Sair
                printf("\nSaindo do simulador Tetris Stack. Desenvolvido por ByteBros.\n");
                break;
                
            default: // Opção inválida
                printf("\nOpção inválida. Por favor, escolha 1, 2, 3 ou 0.\n");
                break;
        }
        
        // REQUISITO: A cada ação que remove da fila (1 ou 2), uma nova peça 
        // é gerada e adicionada ao final da fila, mantendo-a sempre cheia.
        if (precisa_reposicao) {
            Peca nova_peca = gerarPeca();
            enqueue(&fila_pecas, nova_peca);
        }
        
    } while (opcao != 0);
    
    return 0;
}