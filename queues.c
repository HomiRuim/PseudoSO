#include <stdlib.h>
#include "queues.h"

void init_queue(Queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

void enqueue(Queue* q, Process* p) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->process = p;
    new_node->next = NULL;

    if (q->tail == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
}

Process* dequeue(Queue* q) {
    if (q->head == NULL) {
        return NULL; // Fila vazia
    }

    Node* temp = q->head;
    Process* p = temp->process;
    
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }

    free(temp);
    return p;
}

bool is_empty(Queue* q) {
    return q->head == NULL;
}