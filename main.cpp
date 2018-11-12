#include "mbed.h"
#include "mbed_error.h"
#include "mbed_fault_handler.h"

mbed_error_ctx error_ctx;
mbed_fault_context_t fault_ctx;
void generate_bus_fault_unaligned_access();

static int reboot_error_happened = 0;
static bool continuous_reboot_test = false;

void mbed_error_reboot_callback(mbed_error_ctx *error_context) {
    printf("Reboor error callback received");
    reboot_error_happened = 1;
    mbed_get_reboot_error_info(&error_ctx);
    mbed_reset_reboot_error_info();
}

// main() runs in its own thread in the OS
int main() {
  
    printf("\nMbed-OS crash reporting test\n");
    if((reboot_error_happened == 0) || continuous_reboot_test) {
        printf("\nForcing exception\n");
        generate_bus_fault_unaligned_access();
        printf("\nForcing exception failed\n");
    } else {
        if(error_ctx.error_status < 0) {
            printf("\nSuccessfully read reboot info ==> \n");
            printf("\n  error status: 0x%08lX", (uint32_t)error_ctx.error_status);
            printf("\n  error value: 0x%08lX", (uint32_t)error_ctx.error_value);
            printf("\n  error address: 0x%08lX", (uint32_t)error_ctx.error_address);
            printf("\n  error reboot count: 0x%08lX", (uint32_t)error_ctx.error_reboot_count);
            printf("\n  error crc: 0x%08lX\n", (uint32_t)error_ctx.crc_error_ctx);
            
            //Read fault context
            if(error_ctx.error_status == MBED_ERROR_HARDFAULT_EXCEPTION) {
                mbed_get_reboot_fault_context(&fault_ctx);
                            
                printf("\nCrash Report data captured:\n");
                for(int i=0;i<16;i++) {
                    printf("[%d]: 0x%08X\n", i, ((uint32_t *)(&fault_ctx))[i]);
                }
            }
        }
    }
    
    printf("\nMbed-OS crash reporting test completed\n");
}

void generate_bus_fault_unaligned_access()
{
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
    uint32_t inv_addr = 0xAAA3;
    uint32_t val = *(uint32_t *)inv_addr;
    
    printf("\nval= %X", val);
    
    return;

}
