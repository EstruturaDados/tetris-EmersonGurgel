#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Capacidades definidas
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3
#define NUM_PECAS_INICIAIS CAPACIDADE_FILA

// Variável global para gerar IDs únicos para as peças
int proximo_id = 0;

// Estrutura para representar uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;    // Identificador único da peça
} Peca;

// Estrutura para representar a Fila Circular de Peças
typedef struct {
    Peca elementos[CAPACIDADE_FILA];
    int frente;   
    int traseira; 
    int tamanho;  
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
 */
Peca gerarPeca() {
    Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    int num_tipos = 7; 
    
    int indice_tipo = rand() % num_tipos; 
    
    nova_peca.nome = tipos[indice_tipo];
    nova_peca.id = proximo_id++; 
    
    return nova_peca;
}

void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->traseira = 0;
    fila->tamanho = 0;
}

void inicializarPilha(Pilha *pilha) {
    pilha->topo = 0; 
}


/* =========================================================================
 * 2. FUNÇÕES DA FILA CIRCULAR (QUEUE)
 * ========================================================================= */

int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

int filaCheia(Fila *fila) {
    return fila->tamanho == CAPACIDADE_FILA;
}

/**
 * @brief Insere uma nova peça no final da fila (enqueue) e ajusta os índices.
 */
int enqueue(Fila *fila, Peca nova_peca) {
    if (filaCheia(fila)) {
        return 0; 
    }
    
    fila->elementos[fila->traseira] = nova_peca;
    fila->traseira = (fila->traseira + 1) % CAPACIDADE_FILA;
    fila->tamanho++;
    
    printf("-> Reposição: Peça [%c %d] adicionada ao final da fila.\n", nova_peca.nome, nova_peca.id);
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (dequeue) e ajusta os índices.
 */
int dequeue(Fila *fila, Peca *peca_removida) {
    if (filaVazia(fila)) {
        printf("\nERRO: Fila vazia. Não há peças futuras para remover.\n");
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

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == 0;
}

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
    pilha->topo++; 
    
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
    
    pilha->topo--; 
    *peca_removida = pilha->elementos[pilha->topo];
    
    printf("\nSUCESSO: Peça [%c %d] removida da reserva (usada).\n", peca_removida->nome, peca_removida->id);
    return 1;
}


/* =========================================================================
 * 4. FUNÇÕES AVANÇADAS DE MOVIMENTAÇÃO
 * ========================================================================= */

/**
 * @brief Troca a peça da frente da fila com o topo da pilha.
 * @return 1 se a troca foi bem-sucedida, 0 caso contrário.
 */
int trocarPecaAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nERRO: A fila está vazia. Impossível realizar a troca.\n");
        return 0;
    }
    if (pilhaVazia(pilha)) {
        printf("\nERRO: A pilha está vazia. Impossível realizar a troca.\n");
        return 0;
    }

    // A peça da frente da fila está em fila->elementos[fila->frente]
    // A peça do topo da pilha está em pilha->elementos[pilha->topo - 1]
    
    Peca peca_fila = fila->elementos[fila->frente];
    Peca peca_pilha = pilha->elementos[pilha->topo - 1];
    
    // Realiza a troca
    fila->elementos[fila->frente] = peca_pilha;
    pilha->elementos[pilha->topo - 1] = peca_fila;
    
    printf("\nSUCESSO: Troca realizada!\n");
    printf("   Fila: [%c %d] <-> Pilha: [%c %d]\n", peca_fila.nome, peca_fila.id, peca_pilha.nome, peca_pilha.id);
    return 1;
}

/**
 * @brief Troca as 3 primeiras peças da fila com as 3 peças da pilha.
 * @return 1 se a troca foi bem-sucedida, 0 caso contrário.
 */
int trocaMultipla(Fila *fila, Pilha *pilha) {
    const int NUM_TROCA = 3;

    if (fila->tamanho < NUM_TROCA) {
        printf("\nERRO: Fila de peças tem menos de %d peças (%d/%d). Impossível realizar a troca múltipla.\n", NUM_TROCA, fila->tamanho, CAPACIDADE_FILA);
        return 0;
    }
    if (pilha->topo < NUM_TROCA) {
        printf("\nERRO: Pilha de reserva tem menos de %d peças (%d/%d). Impossível realizar a troca múltipla.\n", NUM_TROCA, pilha->topo, CAPACIDADE_PILHA);
        return 0;
    }

    // A troca é feita elemento por elemento
    for (int i = 0; i < NUM_TROCA; i++) {
        // Posição na Fila: 'i' posições à frente de 'fila->frente' (circulares)
        int indice_fila = (fila->frente + i) % CAPACIDADE_FILA;
        
        // Posição na Pilha: 'i' posições a partir do topo (pilha->topo - 1 - i)
        // Note que o topo da pilha é (pilha->topo - 1), o próximo é (pilha->topo - 2), etc.
        int indice_pilha = pilha->topo - 1 - i; 

        // 1. Armazena as peças a serem trocadas
        Peca temp_fila = fila->elementos[indice_fila];
        Peca temp_pilha = pilha->elementos[indice_pilha];
        
        // 2. Realiza a troca
        fila->elementos[indice_fila] = temp_pilha;
        pilha->elementos[indice_pilha] = temp_fila;
    }

    printf("\nSUCESSO: Troca múltipla realizada!\n");
    printf("   As %d primeiras peças da fila e as %d peças da pilha foram trocadas em bloco.\n", NUM_TROCA, NUM_TROCA);
    return 1;
}


/* =========================================================================
 * 5. FUNÇÕES DE EXIBIÇÃO
 * ========================================================================= */

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
 * 6. FUNÇÃO PRINCIPAL (MAIN)
 * ========================================================================= */

int main() {
    srand(time(NULL));
    
    Fila fila_pecas;
    Pilha pilha_reserva;
    
    inicializarFila(&fila_pecas);
    inicializarPilha(&pilha_reserva);
    
    // Inicialização da fila
    printf("Inicializando o simulador Tetris Stack (Avançado)...\n");
    printf("Preenchendo a fila de peças com %d elementos iniciais...\n", NUM_PECAS_INICIAIS);
    for (int i = 0; i < NUM_PECAS_INICIAIS; i++) {
        Peca nova = gerarPeca();
        fila_pecas.elementos[fila_pecas.traseira] = nova;
        fila_pecas.traseira = (fila_pecas.traseira + 1) % CAPACIDADE_FILA;
        fila_pecas.tamanho++;
    }
    printf("Fila inicializada.\n");
    
    int opcao = -1;
    Peca peca_operacao; 
    
    do {
        exibirEstado(&fila_pecas, &pilha_reserva);
        
        // Menu de opções
        printf("\nOpções disponíveis:\n");
        printf("Código | Ação\n");
        printf("-------|---------------------------------------------------\n");
        printf("   1   | Jogar peça da frente da fila (Dequeue)\n");
        printf("   2   | Reservar peça (Fila -> Pilha)\n");
        printf("   3   | Usar peça reservada (Pop da Pilha)\n");
        printf("   4   | Trocar peça da frente da fila com o topo da pilha\n");
        printf("   5   | Trocar os 3 primeiros da fila com as 3 da pilha\n");
        printf("   0   | Sair\n");
        printf("-----------------------------------------------------------\n");
        printf("Digite o código da ação: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpção inválida. Por favor, digite um número.\n");
            while (getchar() != '\n');
            opcao = -1; 
            continue;
        }
        
        int precisa_reposicao = 0; // Flag para reposição automática da fila
        
        switch (opcao) {
            case 1: // Jogar peça (Dequeue)
                if (dequeue(&fila_pecas, &peca_operacao)) {
                    printf("\nSUCESSO: Peça [%c %d] jogada.\n", peca_operacao.nome, peca_operacao.id);
                    precisa_reposicao = 1; 
                }
                break;
                
            case 2: // Reservar peça (Fila -> Pilha)
                if (dequeue(&fila_pecas, &peca_operacao)) {
                    // Tenta fazer o push; se falhar, a peça é perdida, mas a fila precisa de reposição
                    if (!push(&pilha_reserva, peca_operacao)) {
                       printf("AVISO: Peça [%c %d] não reservada, mas removida da fila.\n", peca_operacao.nome, peca_operacao.id);
                    }
                    precisa_reposicao = 1;
                }
                break;
                
            case 3: // Usar peça reservada (Pop da Pilha)
                // Não causa reposição na fila
                pop(&pilha_reserva, &peca_operacao);
                break;
                
            case 4: // Trocar peça da frente da fila com o topo da pilha
                // Não causa remoção de peça, logo não precisa de reposição
                trocarPecaAtual(&fila_pecas, &pilha_reserva);
                break;

            case 5: // Troca múltipla (3 itens)
                // Não causa remoção de peça, logo não precisa de reposição
                trocaMultipla(&fila_pecas, &pilha_reserva);
                break;
                
            case 0: // Sair
                printf("\nEncerrando o gerenciador de peças do Tetris Stack. Até a próxima!\n");
                break;
                
            default: 
                printf("\nOpção inválida. Por favor, escolha um código válido.\n");
                break;
        }
        
        // Reposição automática: se a fila teve uma peça removida (opções 1 ou 2)
        if (precisa_reposicao && !filaCheia(&fila_pecas)) {
            Peca nova_peca = gerarPeca();
            enqueue(&fila_pecas, nova_peca);
        }
        
    } while (opcao != 0);
    
    return 0;
}