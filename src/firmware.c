
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <errno.h>

#include "soc.h"

#define LEDS  ((uint32_t*) 0x40000000)
#define flash ((uint32_t*) 0x70000000)

// Memory locations defined in the linker config.
extern "C" uint32_t _stext, _etext, _sdata, _edata, _sheap, _eheap, _sstack, _estack, _ivector;

    /*
     *
     */

extern "C" void irq_handler(void)__attribute__((interrupt));;

void irq_handler(void)
{
    // check for timer interrupt
    uint32_t cause = read_mcause();
    if ((cause & 0xff) != 0x07)
    {
        return;
    }

    static uint64_t s = 0x01000000;

    s += 0x01000000;
    timer_set(s);

    static int i = 0;
    LEDS[0] = i++;
}

// banner made with : figlet "SERV Risc-V" | sed 's/\\/\\\\/g'
char banner[] = 
"\r\n"
"  ____  _____ ______     __  ____  _            __     __\r\n"
" / ___|| ____|  _ \\ \\   / / |  _ \\(_)___  ___   \\ \\   / /\r\n"
" \\___ \\|  _| | |_) \\ \\ / /  | |_) | / __|/ __|___\\ \\ / / \r\n"
"  ___) | |___|  _ < \\ V /   |  _ <| \\__ \\ (_|_____\\ V /  \r\n"
" |____/|_____|_| \\_\\ \\_/    |_| \\_\\_|___/\\___|     \\_/   \r\n"
"\r\n"
"The World's smallest RISC-V CPU. Using Bit-serial Architecture.\r\n"
"\r\n"
"https://github.com/olofk/serv\r\n\r\n";

    /*
     *
     */

void show_section(const char* text, uint32_t *start, uint32_t *end)
{
    uint32_t s = (uint32_t) start;
    uint32_t e = (uint32_t) end;

    print(text);
    print(" addr 0x");
    print_num(s, 16, 6);
    print(" size 0x");
    print_num(e - s, 16, 6);
    print("\r\n");
}

    /*
     *
     */

int main(void)
{
    *LEDS = 0;

#if 0
    print(banner);

    print("RAM ");
    print_num((uint32_t) &_estack, 10, 6);
    print(" bytes\r\n");
    print("\r\n");
    show_section("Program :", & _stext, & _etext);
    show_section("Data    :", & _sdata, & _edata);
    show_section("Heap    :", & _sheap, & _eheap);
    show_section("Stack   :", & _sstack, & _estack);
    print("\r\n");
#endif

    uint32_t v;

    timer_set(0x01000000);

    uint64_t t = timer_get();
    print("Timer 0x"); 
    print_num(t >> 32, 16, 8);
    print_num(t & 0xFFFFFFFF, 16, 8);
    print("\r\n"); 
    print("Compare 0x"); 
    t = timer_get_cmp();
    print_num(t >> 32, 16, 8);
    print_num(t & 0xFFFFFFFF, 16, 8);
    print("\r\n"); 

    // This instruction does not work!
    write_mie(0x08);

    write_mstatus(0x8);
    write_mtvec((uint32_t) irq_handler);

    int j = 0;

    while (true)
    {
        t = timer_get();
        print_num(t >> 32, 16, 8);
        print("_");
        print_num(t & 0xFFFFFFFF, 16, 8);
        
        print(" E:");
        v = read_mie();
        print_num(v, 16, 8);

        print(" S:");
        v = read_mstatus();
        print_num(v, 16, 8);

        print(" C:");
        v = read_mcause();
        print_num(v, 16, 8);

        print(" ");
        t = timer_get_cmp();
        print_num(t >> 32, 16, 8);
        print("_");
        print_num(t & 0xFFFFFFFF, 16, 8);
        print(" ");
        
        // loop
        print_num(j, 10, 2);
        j += 1;
        print("\r\n"); 

        for (int i = 0; i < 1000; i++)
        {
            v |= *LEDS;            
        }
    }

    return 0;
}

//  FIN
