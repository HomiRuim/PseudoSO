#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_PROCESSES 1000
#define MAX_OPERATIONS 500
#define MAX_DISK_BLOCKS 1000

typedef struct {
    int pid;
    int arrival_time;
    int priority;
    int cpu_time;
    int memory_working_set;
    int req_printer;
    int req_scanner;
    int req_modem;
    int req_sata;
    
    int page_references[256]; 
    int page_ref_count;
} Process;

typedef struct {
    int pid;
    int opcode; 
    char filename;
    int blocks; 
} FileOp;

// O 'extern' avisa ao compilador que essas variáveis existem, 
// mas só serão criadas de fato no parser.c
extern Process process_table[MAX_PROCESSES];
extern int total_processes;

extern FileOp file_operations[MAX_OPERATIONS];
extern int total_file_operations;

extern char disk_map[MAX_DISK_BLOCKS];
extern int total_disk_blocks;

void parse_processes(const char* filename);
void parse_strings(const char* filename);
void parse_files(const char* filename);

#endif