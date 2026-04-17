#include<stdint.h>

int main(void)
{
uint32_t* pClkCtrlReg =   (uint32_t*)0x40023830; //RCC_AHB1ENR
uint32_t* pPortDModeReg = (uint32_t*)0x40020C00; // GPIOD_MODER
uint32_t* pPortDOutReg =  (uint32_t*)0x40020C14; // GPIOD_ODR

*pClkCtrlReg |= ( 1 << 3); // Enable GPIOD from RCC_AHB1ENR


*pPortDModeReg &= ~(( 3 << 24)| (3<<26) | (3<<28) | (3<<30)); // Clear PD12 PD13 PD14 PD15 Mode Bits before setting

*pPortDModeReg |= (( 1 << 24)|(1<<26)|(1<<28)|(1<<30)); // Output mode ->01 : bit 25=0,bit 24=1

while(1)
{

	*pPortDOutReg |= ( 1 << 12); //Set bit 12 (ON)
    for(uint32_t i=0 ; i < 300000 ; i++ ); // software delay
	*pPortDOutReg &= ~( 1 << 12); // clear bit 12 (OFF)

	*pPortDOutReg |= ( 1 << 13); //Set bit 13 (ON)
	 for(uint32_t i=0 ; i < 300000 ; i++ ); // software delay
    *pPortDOutReg &= ~( 1 << 13); // clear bit 13 (OFF)

    *pPortDOutReg |= ( 1 << 14); //Set bit 14 (ON)
     for(uint32_t i=0 ; i < 300000 ; i++ ); // software delay
    *pPortDOutReg &= ~( 1 << 14); // clear bit 14 (OFF)

    *pPortDOutReg |= ( 1 << 15); //Set bit 15 (ON)
     for(uint32_t i=0 ; i < 300000 ; i++ ); // software delay
     *pPortDOutReg &= ~( 1 << 15); // clear bit 15 (OFF)
}

}
