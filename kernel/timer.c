#include <n7OS/timer.h>
#include <n7OS/cpu.h>


void init_timer()
{
    uint32_t frequence = 1190;

    // Configure le timer
    outb(0x34, 0x43);

    // Configure la fréquence du timer
    outb(frequence&0xFF, 0x40);
    outb(frequence>>8, 0x40);

    //lancement de l'IT timer
    outb(inb(0x21)&0xfe, 0x21);
}