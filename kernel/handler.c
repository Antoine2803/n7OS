#include <n7OS/handler.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/process.h>
#include <stdio.h>

extern void handler_IT_50();
extern void handler_IT_32();

uint32_t curr_time = 0;

void init_handlers()
{
    // init IT 50
    init_irq_entry(50, (uint32_t)handler_IT_50);
    init_irq_entry(32, (uint32_t)handler_IT_32);
}

void handler_IT_50_C()
{
    printf("\nint 50 catched succcesfully\n");
}

void handler_IT_32_C()
{
    outb(0x20, 0x20);
    curr_time++;

    // Affichier le timer
    if (curr_time % 1000 == 0)
    {
        console_puttime();
    }

    if (curr_time % 5000 == 0)
    {
        schedule();
    }
}