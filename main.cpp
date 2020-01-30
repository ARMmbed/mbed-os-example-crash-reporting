/* Copyright (c) 2020 Arm Limited
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "mbed.h"
#include "mbed_error.h"
#include "mbed_fault_handler.h"

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
mbed_error_status_t err_status;
mbed_fault_context_t fault_ctx;
static bool reboot_error_happened = false;

void generate_bus_fault_unaligned_access()
{
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
    uint32_t inv_addr = 0xAAA3;
    uint32_t val = *(uint32_t *)inv_addr;
    printf("Now generate the fault: val= %X\n", (unsigned int)val);
}

// Application callback function for reporting error context during boot up.
void mbed_error_reboot_callback(mbed_error_ctx *error_context)
{
    reboot_error_happened = true;
    err_status = error_context->error_status;
    printf("\n\n(before main) mbed_error_reboot_callback invoked with the following error context:\n");
    printf("    Status      : 0x%lX\n", (uint32_t)error_context->error_status);
    printf("    Value       : 0x%lX\n", (uint32_t)error_context->error_value);
    printf("    Address     : 0x%lX\n", (uint32_t)error_context->error_address);
    printf("    Reboot count: 0x%lX\n", (uint32_t)error_context->error_reboot_count);
    printf("    CRC         : 0x%lX\n", (uint32_t)error_context->crc_error_ctx);
    mbed_reset_reboot_error_info();
}
#endif

int main()
{
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    printf("\nThis is the crash reporting Mbed OS example\n");
    if (!reboot_error_happened) {
        printf("1st pass: Inject the fault exception\n");
        generate_bus_fault_unaligned_access();
    } else {
        if (err_status < 0) {
            printf("2nd pass: Retrieve the fault context using mbed_get_reboot_fault_context\n");
            if (err_status == MBED_ERROR_HARDFAULT_EXCEPTION) {
                mbed_get_reboot_fault_context(&fault_ctx);
                for (int i = 0; i < 13; i++) {
                    printf("    R%d   : 0x%X\n", i, ((unsigned int *)(&fault_ctx))[i]);
                }
                printf("    SP   : 0x%X\n"
                       "    LR   : 0x%X\n"
                       "    PC   : 0x%X\n"
                       "    xPSR : 0x%X\n"
                       "    PSP  : 0x%X\n"
                       "    MSP  : 0x%X\n", (unsigned int)fault_ctx.SP_reg, (unsigned int)fault_ctx.LR_reg, (unsigned int)fault_ctx.PC_reg,
                       (unsigned int)fault_ctx.xPSR, (unsigned int)fault_ctx.PSP, (unsigned int)fault_ctx.MSP);
            } else {
                printf("ERROR: Unexpected error status %x\n", err_status);
            }
        }
    }
    printf("\nMbed OS crash reporting example completed\n");
#else
    printf("Mbed OS crash reporting is not enabled\n");
#endif
}


