
#include <unistd.h>
#include <errno.h>

#include "soc.h"

#define TIMER ((uint32_t*) 0xc0000000)
#define uart  ((uint32_t*) 0x50000000)

    /*
     *  _sbrk() is used by malloc() to alloc heap memory.
     */

// Memory locations defined in the linker config.
extern "C" uint32_t _sheap, _eheap;

extern "C" void *_sbrk(intptr_t increment)
{
    static void *heap = (void*) & _sheap;

    void *base = heap;

    void *next = & ((char *) base)[increment];

    if (next >= (void*) & _eheap)
    {
        errno = ENOMEM;
        return (void*) -1;
    }

    heap = next;
    return base;
}

    /*
     *  Timer
     */

#define TIMER_MTIME_LO    0
#define TIMER_MTIME_HI    1
#define TIMER_MTIMECMP_LO 2
#define TIMER_MTIMECMP_HI 3

void timer_set(uint64_t t)
{
    TIMER[TIMER_MTIMECMP_LO] = t & 0xffffffff;
    asm volatile ("" : : : "memory");
    TIMER[TIMER_MTIMECMP_HI] = t >> 32;
}

uint64_t timer_get()
{
    const uint32_t lo = TIMER[TIMER_MTIME_LO];
    asm volatile ("" : : : "memory");
    const uint32_t hi = TIMER[TIMER_MTIME_HI];

    return lo + (((uint64_t) hi) << 32);
}

uint64_t timer_get_cmp()
{
    const uint32_t lo = TIMER[TIMER_MTIMECMP_LO];
    asm volatile ("" : : : "memory");
    const uint32_t hi = TIMER[TIMER_MTIMECMP_HI];

    return lo + (((uint64_t) hi) << 32);
}

    /*
     *
     */

void soc_putc(uint8_t c)
{
    *uart = c;
}

void print(const char *text)
{
    for (const char *s = text; *s; s++)
    {
        soc_putc(*s);
    }
}

    /*
     *
     */

void print_num(uint32_t n, uint32_t base, uint32_t digits)
{
    if (digits > 1)
    {
        print_num(n / base, base, digits-1);
    }

    n %= base;
    const uint8_t c = (n > 9) ? ('a' + n - 10) : ('0' + n);
    soc_putc(c);
}

void print_dec(uint32_t n)
{
    print_num(n, 10, 8);
}

void print_hex(uint32_t n, uint32_t digits)
{
    print_num(n, 16, digits);
}

//  FIN
