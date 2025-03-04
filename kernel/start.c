#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <stdio.h>

void kernel_start(void)
{
    init_kheap();
    init_console();
    printf("\f");
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);

    // lancement des interruptions
    sti();
    
    initialise_paging();

    
    printf("Hello world!");


    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
