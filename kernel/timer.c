#include <n7OS/timer.h>
#include <n7OS/cpu.h>
#include <n7OS/handler.h>

extern uint32_t curr_time;


void init_timer()
{
    // outb(0x0, CMOS_ADDRESS);
    // int second = inl(CMOS_DATA);
    // outb(0x2, CMOS_ADDRESS);
    // int minute = inl(CMOS_DATA);
    // outb(0x4, CMOS_ADDRESS);
    // int hour = inl(CMOS_DATA);
    // curr_time = curr_time + hour*60*60*1000 + minute*60*1000 + second*1000;

    uint32_t frequence = 1190;

    // Configure le timer
    outb(0x34, 0x43);

    // Configure la fréquence du timer
    outb(frequence&0xFF, 0x40);
    outb(frequence>>8, 0x40);

    //lancement de l'IT timer
    outb(inb(0x21)&0xfe, 0x21);
}