#include <n7OS/cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/handler.h>
#include <n7OS/timer.h>
#include <unistd.h>
#include <n7OS/sys.h>
#include <n7OS/minishell.h>
#include <n7OS/process.h>
#include <n7OS/keyboard.h>

extern void processus1();

// Le process 0
void idle()
{
    printf("Process 0 launched\n");
    while (1)
    {
        hlt();
    }
}

// Un process pour tester le clavier
void simple_writer()
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

    init_timer();
    init_keyboard();
    init_handlers();
    // lancement des intéruptions
    sti();

    init_syscall();

    init_console();

    // Test du handler IT 50
    __asm__("int $50");

    // Initialisation des processus
    init_proc(idle);
    create_proc("minishell", minishell);
    // create_proc("clavier", simple_writer);

    // Fin de l'initialisation des processus
    idle();

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
