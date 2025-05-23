#include <n7OS/timer.h>
#include <n7OS/cpu.h>
#include <n7OS/handler.h>
#include <stdio.h>

extern uint32_t curr_time;

void init_timer()
{
    outb(0x0, CMOS_ADDRESS);
    unsigned char second = inb(CMOS_DATA);
    outb(0x2, CMOS_ADDRESS);
    unsigned char minute = inb(CMOS_DATA);
    outb(0x4, CMOS_ADDRESS);
    unsigned char hour = inb(CMOS_DATA);
    second = (second & 0x0F) + ((second / 16) * 10);
    minute = (minute & 0x0F) + ((minute / 16) * 10);
    hour = (hour & 0x0F) + ((hour / 16) * 10);

    curr_time = curr_time + hour * 60 * 60 * 1000 + minute * 60 * 1000 + second * 1000;

    uint32_t frequence = 1190;

    // Configure le timer
    outb(0x34, 0x43);

    // Configure la fréquence du timer
    outb(frequence & 0xFF, 0x40);
    outb(frequence >> 8, 0x40);

    // lancement de l'IT timer
    outb(inb(0x21) & 0xfe, 0x21);
}
