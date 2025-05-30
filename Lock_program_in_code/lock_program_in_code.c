#include "stm32f1xx_hal.h"

void Protect_Code_From_Read(void) {
    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();

    // چک کن فقط اگه RDP در سطح 0 هست، ببر به سطح 1
    if((FLASH->OBR & FLASH_OBR_RDPRT) == 0x00) {
        FLASH_OBProgramInitTypeDef OBInit = {0};
        OBInit.OptionType = OPTIONBYTE_RDP;
        OBInit.RDPLevel = OB_RDP_LEVEL_1;
        HAL_FLASHEx_OBProgram(&OBInit);
        HAL_FLASH_OB_Launch();  // باعث ریست میشه
    }

    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();
}


int main(void) {
    HAL_Init();
    SystemClock_Config();
    Protect_Code_From_Read();

    while (1) {
        // کد اصلی
    }
}


