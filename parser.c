#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Aqui as variáveis são criadas de verdade (sem o extern)
Process process_table[MAX_PROCESSES];
int total_processes = 0;

FileOp file_operations[MAX_OPERATIONS];
int total_file_operations = 0;

char disk_map[MAX_DISK_BLOCKS];
int total_disk_blocks = 0;

void parse_processes(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir processes.txt");
        exit(EXIT_FAILURE);
    }

    char line[256];
    total_processes = 0;

    while (fgets(line, sizeof(line), file) && total_processes < MAX_PROCESSES) {
        Process* p = &process_table[total_processes];
        p->pid = total_processes; 
        
        sscanf(line, "%d, %d, %d, %d, %d, %d, %d, %d",
               &p->arrival_time,
               &p->priority,
               &p->cpu_time,
               &p->memory_working_set,
               &p->req_printer,
               &p->req_scanner,
               &p->req_modem,
               &p->req_sata);
               
        p->page_ref_count = 0;
        p->remaining_time = p->cpu_time;
        p->is_started = false;
        total_processes++;
    }
    fclose(file);
}

void parse_strings(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir string.txt");
        exit(EXIT_FAILURE);
    }

    char line[1024]; 
    int current_pid = 0;

    while (current_pid < total_processes && fgets(line, sizeof(line), file)) {
        Process* p = &process_table[current_pid];
        
        char* token = strtok(line, ",\n");
        while (token != NULL) {
            p->page_references[p->page_ref_count++] = atoi(token);
            token = strtok(NULL, ",\n");
        }
        current_pid++;
    }
    fclose(file);
}

void parse_files(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir files.txt");
        exit(EXIT_FAILURE);
    }

    char line[256];

    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d", &total_disk_blocks);
    }

    for (int i = 0; i < total_disk_blocks; i++) {
        disk_map[i] = '0';
    }

    int occupied_segments = 0;
    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d", &occupied_segments);
    }

    for (int i = 0; i < occupied_segments; i++) {
        if (fgets(line, sizeof(line), file)) {
            char file_name;
            int first_block, num_blocks;
            sscanf(line, " %c, %d, %d", &file_name, &first_block, &num_blocks);
            
            for (int b = first_block; b < first_block + num_blocks; b++) {
                disk_map[b] = file_name;
            }
        }
    }

    while (fgets(line, sizeof(line), file) && total_file_operations < MAX_OPERATIONS) {
        FileOp* op = &file_operations[total_file_operations];
        
        int args_read = sscanf(line, "%d, %d, %c, %d", 
                               &op->pid, &op->opcode, &op->filename, &op->blocks);
        
        if (op->opcode == 1 || args_read == 3) {
            op->blocks = 0; 
        }
        
        total_file_operations++;
    }
    fclose(file);
}