#include <stdint.h>
#include <stdio.h>

/* ---------------- C LINKAGE (VERY IMPORTANT) ---------------- */
extern "C" void UsageFault_Handler(void);
extern "C" void HardFault_Handler(void);
extern "C" void MemManage_Handler(void);
extern "C" void BusFault_Handler(void);
extern "C" void UsageFault_Handler_c(uint32_t *pMSP);

/* ---------------- DIVIDE FUNCTION ---------------- */
int fun_divide(int x , int y)
{
    return x / y;
}

/* ---------------- MAIN ---------------- */
int main(void)
{
    /* Enable configurable faults */
    uint32_t *pSHCSR = (uint32_t*)0xE000ED24; // SHCSR from ARM CORTEX Ref manual
    //printf("Bus Fault trigerred\n");

    //*pSHCSR |= (1 << 16); // MemManage
    *pSHCSR |= (1 << 17); // BusFault
    //*pSHCSR |= (1 << 18); // UsageFault ENABLED

    /* Enable divide-by-zero trap */
    uint32_t *pCCR = (uint32_t*)0xE000ED14;
    *pCCR |= (1 << 4);// enable DIV_0 TRAP

    /* Force runtime division (avoid optimization) */
    //volatile int a = 10;
    //volatile int b = 0;
   uint32_t *pMM=(uint32_t*)0x12345678;
   *pMM=10;
   /*
    volatile uint32_t *ptr=(uint32_t*)0x00000000;
    *ptr=20;*/
    //fun_divide(a, b);   //  UsageFault

    while(1);
}

/* ---------------- FAULT HANDLERS ---------------- */

extern "C" void HardFault_Handler(void)
{
	 __asm volatile(
	        "MRS r0, MSP        \n"  // Pass MSP to C handler
	        "B HardFault_Handler_c \n"
	    );
}

extern "C" void MemManage_Handler(void)
{
    __asm volatile(
            "MRS r0, MSP        \n"  // Pass MSP to C handler
            "B MemManage_Handler_c \n"
        );
}

extern "C" void BusFault_Handler(void)
{
	__asm volatile(
		        "MRS r0, MSP        \n"  // Pass MSP to C handler
		        "B BusFault_Handler_c \n"
		    );
}

/* ---------------- NAKED USAGE FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void UsageFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"  // Pass MSP to C handler
        "B UsageFault_Handler_c \n"
    );
}

/* ---------------- C HANDLER ---------------- */
extern "C" void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pUFSR = (uint32_t*)0xE000ED2A;//Usage fault status register (UFSR)

    printf("\n--- UsageFault Occurred ---\n");
    printf("UFSR = %lx\n", (*pUFSR) & 0xFFFF); // 1111 1111 1111 1111
   // uint8_t status= (*pUFSR & (1<<9));

   // printf("status = %d\n",status);
    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}
extern "C" void BusFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pBFSR = (uint32_t*)0xE000ED29;//Usage fault status register (UFSR)

    printf("\n--- BusFault Occurred ---\n");
    printf("BFSR = %lx\n", (*pBFSR) & 0xFFFF); // 1111 1111 1111 1111
   // uint8_t status= (*pUFSR & (1<<9));

   // printf("status = %d\n",status);
    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}
extern "C" void HardFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pHFSR = (uint32_t*)0xE000ED2C;//Usage fault status register (UFSR)

    printf("\n--- HardFault Occurred ---\n");
    printf("HFSR = %lx\n", (*pHFSR) & 0xFFFF); // 1111 1111 1111 1111
   // uint8_t status= (*pUFSR & (1<<9));

   // printf("status = %d\n",status);
    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}
extern "C" void MemManage_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pMMFSR = (uint32_t*)0xE000ED28;//Usage fault status register (UFSR)

    printf("\n--- MemManageFault Occurred ---\n");
    printf("MFSR = %lx\n", (*pMMFSR) & 0xFFFF); // 1111 1111 1111 1111
   // uint8_t status= (*pUFSR & (1<<9));

   // printf("status = %d\n",status);
    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}
