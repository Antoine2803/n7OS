#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <inttypes.h>
#include <n7OS/mem.h>

#define STACK_SIZE PAGE_SIZE

#define NB_PROC_MAX 255

#define BITMAP_SIZE (NB_PROC_MAX + 1) / 32

#define SLEEP_TIME 5

typedef enum
{
    ELU,
    PRET,
    BLOQUE
} PROCESS_STATE;

typedef uint32_t PROCESS_ID;

typedef struct
{
    uint32_t *stack;
    char *name;
    PROCESS_STATE state;
    PROCESS_ID pid;
    uint32_t regs[5];
} process_t;

uint32_t is_proc();

uint32_t get_pid();

void init_proc(void *main);

void schedule();

uint32_t create_proc(char *name, void *main);

void terminate_proc(PROCESS_ID pid);

void stop_proc(PROCESS_ID pid);

void block_proc(PROCESS_ID pid);

void unblock_proc(PROCESS_ID pid);

#endif