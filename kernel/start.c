#include <n7OS/cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/handler.h>
#include <n7OS/timer.h>
#include <unistd.h>
#include <n7OS/sys.h>
#include <n7OS/process.h>

extern void processus1();

void idle()
{
    printf("idle\n");
    while (1)
    {
        hlt();
    }
}

void proc2()
{
    while (1)
    {
        printf("proc2\n");
        // schedule();
    }
}

void proc1()
{
    while (1)
    {
        printf("proc1\n");
        // schedule();
    }
}

void kernel_start(void)
{
    init_kheap();

    initialise_paging();

    // lancement des intéruptions
    sti();

    init_timer();
    init_handlers();
    init_syscall();

    init_console();

    __asm__("int $50");

    print_mem();

    init_proc(idle);
    create_proc("proc1", proc1);
    create_proc("proc2", proc2);

    idle();

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
