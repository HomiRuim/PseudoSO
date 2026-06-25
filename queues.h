#ifndef QUEUES_H
#define QUEUES_H

#include "parser.h"

// Nó da lista encadeada que guarda um ponteiro para o processo
typedef struct Node {
    Process* process;
    struct Node* next;
} Node;

// Estrutura da Fila
typedef struct {
    Node* head;
    Node* tail;
} Queue;

// Funções do módulo de filas
void init_queue(Queue* q);
void enqueue(Queue* q, Process* p);
Process* dequeue(Queue* q);
bool is_empty(Queue* q);

#endif