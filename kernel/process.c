#include <n7OS/cpu.h>
#include <n7OS/process.h>
#include <malloc.h>
#include <stdio.h>

extern void idle();

uint32_t proc_bitmap_table[SIZE_BITMAP_TABLE];

process_t process_table[NB_PROC_MAX];

uint32_t current_pid;

extern void ctx_sw(void *ctx_old, void *ctx_new);

uint32_t find_pid()
{
    for (uint32_t proc_index = 0; proc_index < NB_PROC_MAX + 1; proc_index++)
    {
        uint32_t index = proc_index / 32;
        uint32_t offset = proc_index % 32;
        uint32_t current = proc_bitmap_table[index];
        if (!(current & (1 << (offset))))
        {
            proc_bitmap_table[index] |= 1 << offset;
            return proc_index;
        }
    }

    return -1;
}

uint32_t is_proc(uint32_t proc_index)
{
    uint32_t index = proc_index/32;
    uint32_t offset = proc_index % 32;
    uint32_t res = (proc_bitmap_table[index] & (1 << offset)) >> offset == 1;
    return res;
}

void clear_pid(uint32_t proc_index)
{
    uint32_t index = proc_index;
    uint32_t offset = index % 32;

    proc_bitmap_table[index / 32] &= ~(1 << offset);
}

uint32_t get_pid()
{
    return current_pid;
}

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
        printf("switching from %s (%d) to %s (%d)\n", process_table[current].name, current, process_table[next_proc].name, next_proc);
        sti();
        ctx_sw(process_table[current].regs, process_table[next_proc].regs);
    }
}

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

void terminate_proc(PROCESS_ID pid)
{
    if (is_proc(pid))
    {
        free((void *)process_table[pid].regs[1]);
        clear_pid(pid);
    }
}

void stop_proc(PROCESS_ID pid)
{
    if (is_proc(pid))
    {
        process_table[pid].state = PRET;
    }
}

void block_proc(PROCESS_ID pid);

void unblock_proc(PROCESS_ID pid);