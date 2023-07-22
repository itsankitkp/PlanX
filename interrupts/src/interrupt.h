struct cpu_state {
unsigned int eax;
unsigned int ebx;
unsigned int ecx;
unsigned int edx;
unsigned int esi;
unsigned int edi;
unsigned int ebp;
unsigned int esp;
} __attribute__((packed));

struct stack_state {
unsigned int error_code;
unsigned int eip;
unsigned int cs;
unsigned int eflags;
} __attribute__((packed));

void interrupt_handler(unsigned int interrupt);