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
#include <n7OS/keyboard.h>

extern void processus1();

void idle()
{
    printf("idle\n");
    while (1)
    {
        hlt();
    }
}

void proc1()
{
    while (1)
    {
        uint8_t c = kgetch();
        if (c != 0x0)
            printf("%c", c);
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

    init_keyboard();

    init_proc(idle);
    create_proc("clavier", proc1);
    idle();

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
