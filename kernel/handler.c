#include <n7OS/handler.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/process.h>
#include <n7OS/keyboard.h>
#include <stdio.h>

extern void handler_IT_50();
extern void handler_IT_32();
extern void handler_IT_33();

uint32_t curr_time = 0;

// On récupère le buffer du clavier
extern uint8_t buffer_key[BUFFER_SIZE];
extern uint32_t buffer_head;
extern uint32_t buffer_tail;

void init_handlers()
{
    // init IT 50
    init_irq_entry(50, (uint32_t)handler_IT_50);
    init_irq_entry(32, (uint32_t)handler_IT_32);
    init_irq_entry(33, (uint32_t)handler_IT_33);
}

// Gestion IT 50
void handler_IT_50_C()
{
    printf("\nint 50 catched succcesfully\n");
}

// Gestion IT 32 -> Timer
void handler_IT_32_C()
{
    outb(0x20, 0x20); // ACK IRQ0
    curr_time++;

    // Affichier le timer
    if (curr_time % 1000 == 0)
    {
        console_puttime();
    }
    if (curr_time % 10 == 0)
    {
        schedule();
    }
}

int shift_pressed = 0;

// Gestion IT 33 -> Clavier
void handler_IT_33_C()
{
    // On récupère le code de la touche
    uint8_t key_code = inb(KEYB_ENCODER);

    shift_pressed = key_code == SHIFT_PRESSED ? 1 : key_code == SHIFT_RELEASED ? 0
                                                                               : shift_pressed;

    uint8_t c = shift_pressed == 1 ? scancode_map_shift[key_code] : scancode_map[key_code];

    if (key_code & 0x80)
    {
        // Si on relache une touche on ne fait rien
    }
    else
    {
        if (c != 0)
        {
            int next_pos = (buffer_tail + 1) % BUFFER_SIZE;

            if (next_pos != buffer_head)
            { // check si le buffer n'est pas plein
                buffer_key[buffer_tail] = c;
                buffer_tail = next_pos;
            }
        }
    }
    outb(0x20, 0x20); // ACK IRQ1
}
