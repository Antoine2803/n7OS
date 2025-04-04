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

void kernel_start(void)
{
    init_kheap();

    initialise_paging();

    // lancement des intéruptions
    sti();

    init_timer();
    init_handlers();

    init_console();

    __asm__("int $50");

    print_mem();

    init_syscall();

    alloc_page_entry(0xa000FFF8, 1, 1);
    uint32_t *ptr = (uint32_t *)0xa000FFFc;
    uint32_t test = *ptr;
    test++;

    

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
