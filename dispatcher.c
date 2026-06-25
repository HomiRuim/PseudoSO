#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "queues.h"

Queue global_queue;
Queue rt_queue;
Queue user_q1;
Queue user_q2;
Queue user_q3;

int system_time = 0; 

// Função que imprime o bloco de informações exatamente como a especificação pede
void print_dispatch_info(Process* p) {
    printf("dispatcher =>\n");
    printf("PID: %d\n", p->pid);
    printf("frames: %d\n", p->memory_working_set);
    printf("priority: %d\n", p->priority);
    printf("time: %d\n", p->cpu_time);
    printf("printers: %d\n", p->req_printer);
    printf("scanners: %d\n", p->req_scanner);
    printf("modems: %d\n", p->req_modem);
    printf("drives: %d\n", p->req_sata);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: ./dispatcher <processes.txt> <files.txt> <string.txt>\n");
        return 1;
    }

    parse_processes(argv[1]);
    parse_strings(argv[3]); 
    parse_files(argv[2]);

    init_queue(&global_queue);
    init_queue(&rt_queue);
    init_queue(&user_q1);
    init_queue(&user_q2);
    init_queue(&user_q3);

    for (int i = 0; i < total_processes; i++) {
        enqueue(&global_queue, &process_table[i]);
    }

    int processes_completed = 0;
    Process* running_process = NULL;

    // Loop Principal da CPU
    while (processes_completed < total_processes) {
        
        // 1. Verifica se novos processos chegaram no tempo atual
        int elements_in_global = 0;
        Node* curr = global_queue.head;
        while(curr != NULL) { elements_in_global++; curr = curr->next; }

        for (int i = 0; i < elements_in_global; i++) {
            Process* p = dequeue(&global_queue);
            
            if (p->arrival_time <= system_time) {
                if (p->priority == 0) enqueue(&rt_queue, p);
                else if (p->priority == 1) enqueue(&user_q1, p);
                else if (p->priority == 2) enqueue(&user_q2, p);
                else if (p->priority == 3) enqueue(&user_q3, p);
            } else {
                enqueue(&global_queue, p);
            }
        }

        // 2. Se a CPU estiver livre, busca o próximo processo mais prioritário
        if (running_process == NULL) {
            if (!is_empty(&rt_queue)) running_process = dequeue(&rt_queue);
            else if (!is_empty(&user_q1)) running_process = dequeue(&user_q1);
            else if (!is_empty(&user_q2)) running_process = dequeue(&user_q2);
            else if (!is_empty(&user_q3)) running_process = dequeue(&user_q3);
        }

        // 3. Executa o processo que está na CPU
        if (running_process != NULL) {
            // Imprime as credenciais apenas na primeira vez que ele ganha a CPU
            if (!running_process->is_started) {
                print_dispatch_info(running_process);
                printf("process %d =>\n", running_process->pid);
                printf("P%d STARTED\n", running_process->pid);
                running_process->is_started = true;
            }
            
            // Simula a execução de 1 instrução (1ms / 1 ciclo)
            int current_instruction = (running_process->cpu_time - running_process->remaining_time) + 1;
            printf("P%d instruction %d\n", running_process->pid, current_instruction);
            
            running_process->remaining_time--;
            system_time++; // O tempo avança
            
            // Verifica se o processo terminou
            if (running_process->remaining_time <= 0) {
                printf("P%d return SIGINT\n\n", running_process->pid);
                processes_completed++;
                running_process = NULL; // Libera a CPU
            } else {
                // Lógica de Preempção e Realimentação
                if (running_process->priority != 0) {
                    // Processos de usuário (Prioridades 1, 2, 3) sofrem preempção após 1ms
                    if (running_process->priority == 1) {
                        running_process->priority = 2; // Sofre aging/penalidade
                        enqueue(&user_q2, running_process);
                    } else if (running_process->priority == 2) {
                        running_process->priority = 3;
                        enqueue(&user_q3, running_process);
                    } else if (running_process->priority == 3) {
                        enqueue(&user_q3, running_process);
                    }
                    running_process = NULL; // Libera a CPU para forçar o escalonador a reavaliar as filas
                }
                // Se for Tempo Real (prioridade 0), não faz nada. O running_process continua na CPU.
            }
        } else {
            // Nenhum processo pronto para rodar. CPU Ociosa.
            system_time++; 
        }
    }

    return 0;
}