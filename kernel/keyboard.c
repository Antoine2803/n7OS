#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>
#include <stdio.h>

// Le buffer pour enregistrer
uint8_t buffer_key[BUFFER_SIZE];
uint32_t buffer_head = 0;
uint32_t buffer_tail = 0;

void init_keyboard()
{
    // Mettre en place les IT clavier
    outb(inb(0x21) & (~0x02), 0x21);

    // Reset le buffer
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer_key[i] = 0x0;
    }
}

char kgetch()
{
    if (buffer_head != buffer_tail)
    {
        uint8_t c = buffer_key[buffer_head];
        buffer_head = (buffer_head + 1) % BUFFER_SIZE;
        return c;
    }
    return 0x0;
}