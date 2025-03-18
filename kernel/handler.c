#include <n7OS/handler.h>
#include <n7OS/irq.h>
#include <stdio.h>

extern void handler_IT_50();

void init_handlers()
{
    // init IT 50
    init_irq_entry(50, (uint32_t)handler_IT_50);
}

void handler_IT_50_C()
{
    printf("int 50 catched succcesfully");
}