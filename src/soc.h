
    /*
     *
     */

#define LEDS  ((uint32_t*) 0x40000000)
#define flash ((uint32_t*) 0x70000000)
#define IRQ   ((uint32_t*) 0x80000000)
#define TIMER ((uint32_t*) 0xc0000000)

    /*
     *  IRQ Controller
     */

#define IRQ_ENABLE 0
#define IRQ_STATE  1
#define IRQ_ACK    2
#define IRQ_SIGNAL 3
#define IRQ_SET_EN 4
#define IRQ_CLR_EN 5

inline void irq_set_enable(uint32_t d)
{
    IRQ[IRQ_SET_EN] = d;
}

inline void irq_ack(uint32_t d)
{
    IRQ[IRQ_ACK] = d;
}

inline uint32_t irq_state()
{
    return IRQ[IRQ_STATE];
}

    /*
     *  CSR access functions
     */

inline uint32_t read_mie()
{
    uint32_t value;
    __asm__ volatile ("csrr %0, mie" : "=r"(value));
    return value;
}

inline void write_mie(uint32_t value)
{
    __asm__ volatile ("csrw mie, %0" : : "r"(value));
}

inline uint32_t read_mtvec()
{
    uint32_t value;
    __asm__ volatile ("csrr %0, mtvec" : "=r"(value));
    return value;
}

inline void write_mtvec(uint32_t value)
{
    __asm__ volatile ("csrw mtvec, %0" : : "r"(value));
}

inline uint32_t read_mstatus()
{
    uint32_t value;
    __asm__ volatile ("csrr %0, mstatus" : "=r"(value));
    return value;
}

inline void write_mstatus(uint32_t value)
{
    __asm__ volatile ("csrw mstatus, %0" : : "r"(value));
}

inline uint32_t read_mcause()
{
    uint32_t value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(value));
    return value;
}

inline void write_mcause(uint32_t value)
{
    __asm__ volatile ("csrw mcause, %0" : : "r"(value));
}

    /*
     *  Timer
     */

void timer_set(uint64_t t);
uint64_t timer_get();
uint64_t timer_get_cmp();

    /*
     *
     */

void print(const char *text);
void print_num(uint32_t n, uint32_t base, uint32_t digits);
void print_dec(uint32_t n);
void print_hex(uint32_t n, uint32_t digits);

//  FIN
