#include <inttypes.h>
#include <n7OS/irq.h>
#include <n7OS/processor_structs.h>

void init_irq_entry(int irq_num, uint32_t addr)
{
    idt_entry_t *new_entry = (idt_entry_t *)&idt[irq_num];

    // On récupère les 16 bits de poids faible
    new_entry->offset_inf = addr & 0xffff;
    // On récupère les 16 bits de poids fort
    new_entry->offset_sup = addr >> 16;
    new_entry->zero = 0x0;
    new_entry->sel_segment = KERNEL_CS;
    // On donne le bon type
    new_entry->type_attr = PRESENT_ENTRY | DPL | INT_GATE_TRAP_GATE | INT32_GATE;
}
