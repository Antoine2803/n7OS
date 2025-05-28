#include <n7OS/cpu.h>
#include <n7OS/process.h>
#include <malloc.h>
#include <stdio.h>

extern void ctx_sw(void *ctx_old, void *ctx_new);

/**
 * Bitmap qui retient les emplacements des processus dans la table des processus
 */
uint32_t proc_bitmap_table[SIZE_BITMAP_TABLE];

/**
 * Table des processus
 */
process_t process_table[NB_PROC_MAX];

/**
 * PID du processus en cours d'execution
 */
uint32_t current_pid;

/**
 * Récupère le premier pid dans la bitmap qui est libre et l'alloue
 */
uint32_t find_pid()
{
    for (uint32_t pid = 0; pid < NB_PROC_MAX + 1; pid++)
    {
        uint32_t index = pid / 32;
        uint32_t offset = pid % 32;
        uint32_t current = proc_bitmap_table[index];
        if (!(current & (1 << (offset))))
        {
            proc_bitmap_table[index] |= 1 << offset;
            return pid;
        }
    }
    return -1;
}

/**
 * Teste si il y a un processus à l'adresse pid dans la table des processus
 */
uint32_t is_proc(uint32_t pid)
{
    uint32_t index = pid / 32;
    uint32_t offset = pid % 32;
    uint32_t res = (proc_bitmap_table[index] & (1 << offset)) >> offset == 1;
    return res;
}

/**
 * Affiche les processus de la table des processus
 */
void print_process()
{
    printf("  PID\t\t\tState \t\t\tName\n");

    for (int i = 0; i < NB_PROC_MAX; i++)
    {
        if (is_proc(i))
        {
            if (process_table[i].state == ELU)
            {
                printf(" - %d\t\t\t ELU   \t\t\t%s\n", i, process_table[i].name);
            }
            else if (process_table[i].state == PRET)
            {
                printf(" - %d\t\t\t PRET  \t\t\t%s\n", i, process_table[i].name);
            }
            else if (process_table[i].state == BLOQUE)
            {
                printf(" - %d\t\t\t BLOQUE\t\t\t%s\n", i, process_table[i].name);
            }
        }
    }
}

/**
 * Libère le processus de pid pid
 */
void clear_pid(uint32_t pid)
{
    uint32_t index = pid;
    uint32_t offset = index % 32;

    proc_bitmap_table[index / 32] &= ~(1 << offset);
}

/**
 * Renvoie le pid de la fonction en cours d'execution
 */
uint32_t get_pid()
{
    return current_pid;
}

/**
 * Prépare la bitmap et met le processus 0 (idle) à ELU
 */
void init_proc(void *main)
{
    for (int i = 0; i < SIZE_BITMAP_TABLE; i++)
    {
        proc_bitmap_table[i] = 0;
    }
    uint32_t pid = create_proc("idle", main);
    process_table[pid].state = ELU;
    current_pid = pid;
}

/**
 * Fonction qui permet de context switch entre les process
 * Simple round robin sur les process de la process table
 */
void schedule()
{
    uint32_t current = get_pid();
    uint32_t next_proc = (current + 1) % NB_PROC_MAX;
    int it = 1;
    while ((it < NB_PROC_MAX) && !is_proc(next_proc) && process_table[next_proc].state != PRET)
    {
        next_proc = (next_proc + 1) % NB_PROC_MAX;
        it++;
    }
    if (it < NB_PROC_MAX && process_table[next_proc].state == PRET)
    {
        stop_proc(current);
        process_table[next_proc].state = ELU;
        current_pid = next_proc;
        // printf("switching from %s (%d) to %s (%d)\n", process_table[current].name, current, process_table[next_proc].name, next_proc);
        sti();
        ctx_sw(process_table[current].regs, process_table[next_proc].regs);
    }
}

/**
 * Crée un processus de nom name et d'adresse main
 */
uint32_t create_proc(char *name, void *main)
{
    PROCESS_ID pid = find_pid();
    process_table[pid].name = name;
    process_table[pid].pid = pid;
    process_table[pid].stack = (uint32_t *)malloc(STACK_SIZE * sizeof(uint32_t));
    process_table[pid].stack[STACK_SIZE - 1] = (uint32_t)main;
    process_table[pid].state = PRET;
    for (int i = 0; i < 5; i++)
        process_table[pid].regs[i] = 0;

    process_table[pid].regs[1] = (uint32_t)&process_table[pid].stack[STACK_SIZE - 1];
    return pid;
}

/**
 * Termine un processus
 */
void terminate_proc(PROCESS_ID pid)
{
    if (pid == get_pid())
    {
        printf("Cannot kill current process\n");
    }
    else if (is_proc(pid))
    {
        free((void *)process_table[pid].regs[1]);
        clear_pid(pid);
    }
}

/**
 * Stoppe le processus de pid pid
 */
void stop_proc(PROCESS_ID pid)
{
    if (is_proc(pid) && process_table[pid].state == ELU)
    {
        process_table[pid].state = PRET;
    }
}

/**
 * Bloque le processus de pid pid
 */
void block_proc(PROCESS_ID pid)
{
    if (is_proc(pid) && process_table[pid].state == PRET)
    {
        process_table[pid].state = BLOQUE;
    }
}

/**
 * Débloque le processus de pid pid
 */
void unblock_proc(PROCESS_ID pid)
{
    if (is_proc(pid) && process_table[pid].state == BLOQUE)
    {
        process_table[pid].state = PRET;
    }
}