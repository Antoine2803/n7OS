#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <stdio.h>
#include <n7OS/handler.h>

void kernel_start(void)
{
    init_kheap();
    init_console();
    
    initialise_paging();

    //lancement des intéruptions
    sti();
    
    init_handlers();
    
    __asm__("int $50");
    
    print_mem();

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
