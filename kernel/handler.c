#include <n7OS/handler.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
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
    curr_time++;

    // Affichier le timer
    if (curr_time % 1000 == 0)
    {
        printf("time: h:%d ", ((curr_time / 1000) / 60) / 60);

        if (((curr_time % 3600000) / 1000) / 60 >= 0 && ((curr_time % 3600000) / 1000) / 60 <= 9)
        {
            printf("m:0%d ",((curr_time % 3600000) / 1000) / 60);
        }
        else
        {
            printf("m:%d ", ((curr_time % 3600000) / 1000) / 60);
        }

        if ((curr_time % 60000) / 1000 >= 0 && (curr_time % 60000) / 1000 <= 9)
        {
            printf("s:0%d\n", (curr_time % 60000) / 1000);
        }
        else
        {
            printf("s:%d\n", (curr_time % 60000) / 1000);
        }
    }
    outb(0x20, 0x20);
}