/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_debug_console.h"
#include "mc_periph_init.h"
#include "m1_sm_snsless.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "peripherals.h"
#include "fsl_lpuart.h"
#include "mid_sm_states.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (0)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

#define BOARD_USER_BUTTON_PRIORITY 4

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val
      
static void BOARD_Init(void);
static void BOARD_InitSysTick(void);
static void DemoSpeedStimulator(void);
static void init_freemaster_lpuart(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "FRDM-MCXA153", /* board id */
    "pmsm",       /* motor type */
    MCRSP_VER,    /* sw version */
};

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

uint32_t ctimer0_isr_cnt        =0u;
uint32_t adc0_isr_cnt           =0u;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void main(void)
{
    /*Accessing ID structure to prevent optimization*/
    //g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();
    
    /* Disable demo mode after reset */
    bDemoMode    = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /* Init board hardware. */
    BOARD_Init();

    LED_RED_OFF();
    LED_BLUE_OFF();
    LED_GREEN_OFF();
    
    /* Initialize peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();
    
    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    
    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* FreeMASTER driver initialization */
    FMSTR_Init();

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

static void BOARD_Init(void)
{
    /* Initialize clock configuration */
    BOARD_InitBootClocks();
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();

    /* attach FRO 12M to LPUART0 (debug console) */
    RESET_PeripheralReset(kLPUART0_RST_SHIFT_RSTn);

    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClockDiv(kCLOCK_DivLPUART0, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_EnableClock(kCLOCK_GateLPUART0);

    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();

}

static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/*!
 * @brief   ADC ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *           - demo mode if enabled
 *
 * @param   void
 *
 * @return  none
 */
void ADC0_IRQHandler(void)
{
    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();
    
    GPIO1->PSOR |= 1<<10U;
    adc0_isr_cnt++;

    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    /* M1 state machine */    
    SM_StateMachineFast(&g_sM1Ctrl);
    
    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Clear the TCOMP INT flag */
    ADC0->STAT |= (uint32_t)(1U << 9);
    
    GPIO1->PCOR |= 1<<10U;
    
    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}
  
/*!
 * @brief   Slow loop interrupt handler (1ms period)
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
void CTIMER0_IRQHandler(void)
{
    static int16_t ui16i = 0;
    ctimer0_isr_cnt++;

    GPIO1->PSOR |= 1<<12U;

    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);
    
        /* If in STOP state turn on RED */
    if (M1_GetAppState() == 2)
    {
        LED_RED_ON();
        LED_GREEN_OFF();
    }

    /* If in FAULT state RED blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (ui16i-- < 0)
        {
            LED_RED_TOGGLE();
            bDemoMode= FALSE;
            ui16i = 125;
        }
        LED_GREEN_OFF();
    }

    /* If in RUN or INIT state turn on green */
    else
    {
        LED_RED_OFF();
        LED_GREEN_ON();
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear the match interrupt flag. */
    CTIMER0->IR |= CTIMER_IR_MR0INT(1U);
    GPIO1->PCOR |= 1<<12U;

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   Port interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
void GPIO1_IRQHandler(void)
{
    if (GPIO1->ISFR[0] & GPIO_ISFR_ISF7_MASK)
    {
        /* clear the flag */
        GPIO1->ISFR[0] |= GPIO_ISFR_ISF7_MASK;

        /* proceed only if pressing longer than timeout */
        if (ui32ButtonFilter > 200U)
        {
            ui32ButtonFilter = 0U;
            if (bDemoMode)
            {
                M1_SetSpeed(0);
                M1_SetAppSwitch(FALSE);
                bDemoMode = FALSE;
            }
            else
            {
                M1_SetAppSwitch(TRUE);
                bDemoMode              = TRUE;
                ui32SpeedStimulatorCnt = 0U;
            }
        }
    }

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   DemoSpeedStimulator
 *           - When demo mode is enabled it changes the required speed according
 *             to predefined profile
 *
 * @param   void
 *
 * @return  none
 */
static void DemoSpeedStimulator(void)
{
    if (ui32ButtonFilter < 1000)
      ui32ButtonFilter++;
  
    if (bDemoMode)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 10:
                g_sM1Drive.eControl                  = kControlMode_SpeedFOC;
                break;
            case 100:
                M1_SetSpeed(FRAC16(1000.0F/M1_N_MAX));
                break;
            case 3000:
                M1_SetSpeed(FRAC16(2000.0F/M1_N_MAX));
                break;
            case 6000:
                M1_SetSpeed(FRAC16(4000.0F/M1_N_MAX));
                break;
            case 9000:
                M1_SetSpeed(FRAC16(-1000.0F/M1_N_MAX));
                break;
            case 12000:
                M1_SetSpeed(FRAC16(-2000.0F/M1_N_MAX));
                break;
            case 15000:
                M1_SetSpeed(FRAC16(-4000.0F/M1_N_MAX));
                break;
            case 18000:
                M1_SetSpeed(FRAC16(800.0F/M1_N_MAX));
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
            	;
                break;
        }
    }
    else
    {
        ui32SpeedStimulatorCnt = 0U;
    }
}

/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR);

}
