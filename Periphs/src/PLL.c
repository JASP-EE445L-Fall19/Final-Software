#include <stdint.h>
#include "TM4C1290NCPDT.h"

#define HWREG(x)                                                              \
        (*((volatile uint32_t *)(x)))
#define SysCtlXtalCfgToIndex(a) ((a & 0x7c0) >> 6)
				
#define PLL_M_TO_REG(mi, mf)                                                  \
        ((uint32_t)mi | (uint32_t)(mf << SYSCTL_PLLFREQ0_MFRAC_S))
#define PLL_N_TO_REG(n)                                                       \
        ((uint32_t)(n - 1) << SYSCTL_PLLFREQ1_N_S)
#define PLL_Q_TO_REG(q)                                                       \
        ((uint32_t)(q - 1) << SYSCTL_PLLFREQ1_Q_S)
				
//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_MOSCCTL register.
//
//*****************************************************************************
#define SYSCTL_MOSCCTL_OSCRNG   0x00000010  // Oscillator Range
#define SYSCTL_MOSCCTL_PWRDN    0x00000008  // Power Down
#define SYSCTL_MOSCCTL_NOXTAL   0x00000004  // No Crystal Connected
#define SYSCTL_MOSCCTL_MOSCIM   0x00000002  // MOSC Failure Action
#define SYSCTL_MOSCCTL_CVAL     0x00000001  // Clock Validation for MOSC

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_RSCLKCFG
// register.
//
//*****************************************************************************
#define SYSCTL_RSCLKCFG_MEMTIMU 0x80000000  // Memory Timing Register Update
#define SYSCTL_RSCLKCFG_NEWFREQ 0x40000000  // New PLLFREQ Accept
#define SYSCTL_RSCLKCFG_ACG     0x20000000  // Auto Clock Gating
#define SYSCTL_RSCLKCFG_USEPLL  0x10000000  // Use PLL
#define SYSCTL_RSCLKCFG_PLLSRC_M                                              \
                                0x0F000000  // PLL Source
#define SYSCTL_RSCLKCFG_PLLSRC_PIOSC                                          \
                                0x00000000  // PIOSC is PLL input clock source
#define SYSCTL_RSCLKCFG_PLLSRC_MOSC                                           \
                                0x03000000  // MOSC is the PLL input clock
                                            // source
#define SYSCTL_RSCLKCFG_OSCSRC_M                                              \
                                0x00F00000  // Oscillator Source
#define SYSCTL_RSCLKCFG_OSCSRC_PIOSC                                          \
                                0x00000000  // PIOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_LFIOSC                                         \
                                0x00200000  // LFIOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_MOSC                                           \
                                0x00300000  // MOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_RTC                                            \
                                0x00400000  // Hibernation Module RTC
                                            // Oscillator (RTCOSC)
#define SYSCTL_RSCLKCFG_OSYSDIV_M                                             \
                                0x000FFC00  // Oscillator System Clock Divisor
#define SYSCTL_RSCLKCFG_PSYSDIV_M                                             \
                                0x000003FF  // PLL System Clock Divisor
#define SYSCTL_RSCLKCFG_OSYSDIV_S                                             \
                                10
#define SYSCTL_RSCLKCFG_PSYSDIV_S                                             \
                                0
																
#define SYSCTL_CFG_VCO_480      0xF1000000  // VCO is 480 MHz
																
																
//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_MEMTIM0 register.
//
//*****************************************************************************
#define SYSCTL_MEMTIM0_EBCHT_M  0x03C00000  // EEPROM Clock High Time
#define SYSCTL_MEMTIM0_EBCHT_0_5                                              \
                                0x00000000  // 1/2 system clock period
#define SYSCTL_MEMTIM0_EBCHT_1  0x00400000  // 1 system clock period
#define SYSCTL_MEMTIM0_EBCHT_1_5                                              \
                                0x00800000  // 1.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_2  0x00C00000  // 2 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_2_5                                              \
                                0x01000000  // 2.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_3  0x01400000  // 3 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_3_5                                              \
                                0x01800000  // 3.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_4  0x01C00000  // 4 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_4_5                                              \
                                0x02000000  // 4.5 system clock periods
#define SYSCTL_MEMTIM0_EBCE     0x00200000  // EEPROM Bank Clock Edge
#define SYSCTL_MEMTIM0_MB1      0x00100010  // Must be one
#define SYSCTL_MEMTIM0_EWS_M    0x000F0000  // EEPROM Wait States
#define SYSCTL_MEMTIM0_FBCHT_M  0x000003C0  // Flash Bank Clock High Time
#define SYSCTL_MEMTIM0_FBCHT_0_5                                              \
                                0x00000000  // 1/2 system clock period
#define SYSCTL_MEMTIM0_FBCHT_1  0x00000040  // 1 system clock period
#define SYSCTL_MEMTIM0_FBCHT_1_5                                              \
                                0x00000080  // 1.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_2  0x000000C0  // 2 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_2_5                                              \
                                0x00000100  // 2.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_3  0x00000140  // 3 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_3_5                                              \
                                0x00000180  // 3.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_4  0x000001C0  // 4 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_4_5                                              \
                                0x00000200  // 4.5 system clock periods
#define SYSCTL_MEMTIM0_FBCE     0x00000020  // Flash Bank Clock Edge
#define SYSCTL_MEMTIM0_FWS_M    0x0000000F  // Flash Wait State
#define SYSCTL_MEMTIM0_EWS_S    16
#define SYSCTL_MEMTIM0_FWS_S    0


//*****************************************************************************
//
// The following are defines for the System Control register addresses.
//
//*****************************************************************************
#define SYSCTL_RSCLKCFG         0x400FE0B0  // Run and Sleep Mode Configuration
#define SYSCTL_MOSCCTL          0x400FE07C  // Main Oscillator Control
#define SYSCTL_MEMTIM0          0x400FE0C0  // Memory Timing Parameter Register
#define SYSCTL_PLLFREQ0         0x400FE160  // PLL Frequency 0
#define SYSCTL_PLLFREQ1         0x400FE164  // PLL Frequency 1
#define SYSCTL_PLLSTAT          0x400FE168  // PLL Status
#define SYSCTL_RIS              0x400FE050  // Raw Interrupt Status

#define SYSCTL_RIS_MOSCPUPRIS   0x00000100  // MOSC Power Up Raw Interrupt

//*****************************************************************************
//
// The following are values that can be passed to the SysCtlClockSet() API as
// the ui32Config parameter.
//
//*****************************************************************************
#define SYSCTL_XTAL_5MHZ        0x00000240  // External crystal is 5MHz
#define SYSCTL_XTAL_10MHZ       0x00000400  // External crystal is 10 MHz
#define SYSCTL_XTAL_16MHZ       0x00000540  // External crystal is 16 MHz
#define SYSCTL_XTAL_25MHZ       0x00000680  // External crystal is 25.0 MHz
#define SYSCTL_OSC_MAIN         0x00000000  // Osc source is main osc
#define SYSCTL_OSC_INT          0x00000010  // Osc source is int. osc
#define SYSCTL_OSC_INT4         0x00000020  // Osc source is int. osc /4
#define SYSCTL_OSC_INT30        0x00000030  // Osc source is int. 30 KHz
#define SYSCTL_OSC_EXT32        0x80000038  // Osc source is ext. 32 KHz
#define SYSCTL_USE_PLL          0x00000000  // System clock is the PLL clock
#define SYSCTL_USE_OSC          0x00003800  // System clock is the osc clock

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_PLLFREQ0
// register.
//
//*****************************************************************************
#define SYSCTL_PLLFREQ0_PLLPWR  0x00800000  // PLL Power
#define SYSCTL_PLLFREQ0_MFRAC_M 0x000FFC00  // PLL M Fractional Value
#define SYSCTL_PLLFREQ0_MINT_M  0x000003FF  // PLL M Integer Value
#define SYSCTL_PLLFREQ0_MFRAC_S 10
#define SYSCTL_PLLFREQ0_MINT_S  0

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_PLLFREQ1
// register.
//
//*****************************************************************************
#define SYSCTL_PLLFREQ1_Q_M     0x00001F00  // PLL Q Value
#define SYSCTL_PLLFREQ1_N_M     0x0000001F  // PLL N Value
#define SYSCTL_PLLFREQ1_Q_S     8
#define SYSCTL_PLLFREQ1_N_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_PLLSTAT register.
//
//*****************************************************************************
#define SYSCTL_PLLSTAT_LOCK     0x00000001  // PLL Lock

//*****************************************************************************
//
// The mapping of system clock frequency to flash memory timing parameters.
//
//*****************************************************************************
static const struct
{
    uint32_t ui32Frequency;
    uint32_t ui32MemTiming;
}
g_sXTALtoMEMTIM[] =
{
    { 16000000, (SYSCTL_MEMTIM0_FBCHT_0_5 | SYSCTL_MEMTIM0_FBCE |
                 (0 << SYSCTL_MEMTIM0_FWS_S) |
                 SYSCTL_MEMTIM0_EBCHT_0_5 | SYSCTL_MEMTIM0_EBCE |
                 (0 << SYSCTL_MEMTIM0_EWS_S) |
                 SYSCTL_MEMTIM0_MB1) },
    { 40000000, (SYSCTL_MEMTIM0_FBCHT_1_5 | (1 << SYSCTL_MEMTIM0_FWS_S) |
                 SYSCTL_MEMTIM0_EBCHT_1_5 | (1 << SYSCTL_MEMTIM0_EWS_S) |
                 SYSCTL_MEMTIM0_MB1) },
    { 60000000, (SYSCTL_MEMTIM0_FBCHT_2 | (2 << SYSCTL_MEMTIM0_FWS_S) |
                 SYSCTL_MEMTIM0_EBCHT_2 | (2 << SYSCTL_MEMTIM0_EWS_S) |
                 SYSCTL_MEMTIM0_MB1) },
    { 80000000, (SYSCTL_MEMTIM0_FBCHT_2_5 | (3 << SYSCTL_MEMTIM0_FWS_S) |
                 SYSCTL_MEMTIM0_EBCHT_2_5 | (3 << SYSCTL_MEMTIM0_EWS_S) |
                 SYSCTL_MEMTIM0_MB1) },
    { 100000000, (SYSCTL_MEMTIM0_FBCHT_3 | (4 << SYSCTL_MEMTIM0_FWS_S) |
                  SYSCTL_MEMTIM0_EBCHT_3 | (4 << SYSCTL_MEMTIM0_EWS_S) |
                  SYSCTL_MEMTIM0_MB1) },
    { 120000000, (SYSCTL_MEMTIM0_FBCHT_3_5 | (5 << SYSCTL_MEMTIM0_FWS_S) |
                  SYSCTL_MEMTIM0_EBCHT_3_5 | (5 << SYSCTL_MEMTIM0_EWS_S) |
                  SYSCTL_MEMTIM0_MB1) },
};
										
//*****************************************************************************
//
// An array that maps the crystal number in RCC to a frequency.
//
//*****************************************************************************
static const uint32_t g_pui32Xtals[] =
{
    1000000,
    1843200,
    2000000,
    2457600,
    3579545,
    3686400,
    4000000,
    4096000,
    4915200,
    5000000,
    5120000,
    6000000,
    6144000,
    7372800,
    8000000,
    8192000,
    10000000,
    12000000,
    12288000,
    13560000,
    14318180,
    16000000,
    16384000,
    18000000,
    20000000,
    24000000,
    25000000
};

static uint32_t
SysCtlClockFreqSet(uint32_t ui32Config, uint32_t ui32SysClock);
				
// fVCO = (fIN * MDIV)
// fIN = fXTAL/(Q+1)(N+1) or fPIOSC/(Q+1)(N+1)
// MDIV = MINT + (MFRAC / 1024)

void PLL_Init120MHz (void) {
	
	/*
	SYSCTL->MOSCCTL = 0x14;							// PWRDN bit and NOXTAL bit must be cleared and set simultaneously
	SYSCTL->MEMTIM0 = (0x2 << 22) | (0x2 << 6) | (0x1 << 16) | (0x1 << 0) | (1 << 4) | (1 << 20);
	
	SYSCTL->RSCLKCFG |= (0x03 << 20);			// Set Oscillator source as crystal
	SYSCTL->RSCLKCFG |= (0x03 << 24);			// Set PLL source as crystal
	
	SYSCTL->PLLFREQ0 &= ~(0x3FF << 10);		// MFRAC = 0
	SYSCTL->PLLFREQ0 |= (0x60 & 0x3FF);	// MINT = 0x60
	
	SYSCTL->PLLFREQ1 &= ~(0x1F << 8);			// Q = 0
	SYSCTL->PLLFREQ1 |= 0x04;						// N = 0x4
	
	SYSCTL->MEMTIM0 = (0x6 << 22) | (0x6 << 6) | (0x5 << 16) | (0x5 << 0);
	
	SYSCTL->PLLFREQ0 |= (1 << 23);				// Power PLL
	while ((SYSCTL->PLLSTAT & 0x01) == 0x00);		// Wait until PLL stabilized
	
	SYSCTL->RSCLKCFG |= (1 << 30) | (1 << 31) | (1 << 28) | 0x04;				// Enable new PLL frequency
	
	//SYSCTL->RSCLKCFG &= ~(0xF << 20);		// Change OscSrc back to PIOSC
	*/
	
	SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);
}


//*****************************************************************************
//
// Get the correct memory timings for a given system clock value.
//
//*****************************************************************************
static uint32_t
_SysCtlMemTimingGet(uint32_t ui32SysClock)
{
    uint_fast8_t ui8Idx;

    //
    // Loop through the flash memory timings.
    //
    for(ui8Idx = 0;
        ui8Idx < (sizeof(g_sXTALtoMEMTIM) / sizeof(g_sXTALtoMEMTIM[0]));
        ui8Idx++)
    {
        //
        // See if the system clock frequency is less than the maximum frequency
        // for this flash memory timing.
        //
        if(ui32SysClock <= g_sXTALtoMEMTIM[ui8Idx].ui32Frequency)
        {
            //
            // This flash memory timing is the best choice for the system clock
            // frequency, so return it now.
            //
            return(g_sXTALtoMEMTIM[ui8Idx].ui32MemTiming);
        }
    }

    //
    // An appropriate flash memory timing could not be found, so the device is
    // being clocked too fast.  Return the default flash memory timing.
    //
    return(0);
}

//*****************************************************************************
//
// Calculate the system frequency from the register settings base on the
// oscillator input.
//
//*****************************************************************************
static uint32_t
_SysCtlFrequencyGet(uint32_t ui32Xtal)
{
    uint32_t ui32Result;
    uint_fast16_t ui16F1, ui16F2;
    uint_fast16_t ui16PInt, ui16PFract;
    uint_fast8_t ui8Q, ui8N;

    //
    // Extract all of the values from the hardware registers.
    //
    ui16PFract = ((HWREG(SYSCTL_PLLFREQ0) & SYSCTL_PLLFREQ0_MFRAC_M) >>
                  SYSCTL_PLLFREQ0_MFRAC_S);
    ui16PInt = HWREG(SYSCTL_PLLFREQ0) & SYSCTL_PLLFREQ0_MINT_M;
    ui8Q = (((HWREG(SYSCTL_PLLFREQ1) & SYSCTL_PLLFREQ1_Q_M) >>
             SYSCTL_PLLFREQ1_Q_S) + 1);
    ui8N = (((HWREG(SYSCTL_PLLFREQ1) & SYSCTL_PLLFREQ1_N_M) >>
             SYSCTL_PLLFREQ1_N_S) + 1);

    //
    // Divide the crystal value by N.
    //
    ui32Xtal /= (uint32_t)ui8N;

    //
    // Calculate the multiplier for bits 9:5.
    //
    ui16F1 = ui16PFract / 32;

    //
    // Calculate the multiplier for bits 4:0.
    //
    ui16F2 = ui16PFract - (ui16F1 * 32);

    //
    // Get the integer portion.
    //
    ui32Result = ui32Xtal * (uint32_t)ui16PInt;

    //
    // Add first fractional bits portion(9:0).
    //
    ui32Result += (ui32Xtal * (uint32_t)ui16F1) / 32;

    //
    // Add the second fractional bits portion(4:0).
    //
    ui32Result += (ui32Xtal * (uint32_t)ui16F2) / 1024;

    //
    // Divide the result by Q.
    //
    ui32Result = ui32Result / (uint32_t)ui8Q;

    //
    // Return the resulting PLL frequency.
    //
    return(ui32Result);
}

//*****************************************************************************
//
//! Configures the system clock.
//!
//! \param ui32Config is the required configuration of the device clocking.
//! \param ui32SysClock is the requested processor frequency.
//!
//! This function configures the main system clocking for the device.  The
//! input frequency, oscillator source, whether or not to enable the PLL, and
//! the system clock divider are all configured with this function.  This
//! function configures the system frequency to the closest available divisor
//! of one of the fixed PLL VCO settings provided in the \e ui32Config
//! parameter.  The caller sets the \e ui32SysClock parameter to request the
//! system clock frequency, and this function then attempts to match this using
//! the values provided in the \e ui32Config parameter.  If this function
//! cannot exactly match the requested frequency, it picks the closest
//! frequency that is lower than the requested frequency.  The \e ui32Config
//! parameter provides the remaining configuration options using a set of
//! defines that are a logical OR of several different values, many of which
//! are grouped into sets where only one of the set can be chosen.  This
//! function returns the current system frequency which may not match the
//! requested frequency.
//!
//! If the application is using an external crystal then the frequency is
//! set by using one of the following values:
//! \b SYSCTL_XTAL_5MHZ, \b SYSCTL_XTAL_6MHZ, \b SYSCTL_XTAL_8MHZ,
//! \b SYSCTL_XTAL_10MHZ, \b SYSCTL_XTAL_12MHZ, \b SYSCTL_XTAL_16MHZ,
//! \b SYSCTL_XTAL_18MHZ, \b SYSCTL_XTAL_20MHZ, \b SYSCTL_XTAL_24MHZ, or
//! \b SYSCTL_XTAL_25MHz.
//!
//! The oscillator source is chosen with one of the following values:
//!
//! - \b SYSCTL_OSC_MAIN to use an external crystal or oscillator.
//! - \b SYSCTL_OSC_INT to use the 16-MHz precision internal oscillator.
//! - \b SYSCTL_OSC_INT30 to use the internal low frequency oscillator.
//! - \b SYSCTL_OSC_EXT32 to use the hibernate modules 32.786-kHz oscillator.
//! This option is only available on devices that include the hibernation
//! module.
//!
//! The system clock source is chosen with one of the following values:
//!
//! - \b SYSCTL_USE_PLL is used to select the PLL output as the system clock.
//! - \b SYSCTL_USE_OSC is used to choose one of the oscillators as the
//! system clock.
//!
//! The PLL VCO frequency is chosen with one of the the following values:
//!
//! - \b SYSCTL_CFG_VCO_480 to set the PLL VCO output to 480-MHz
//! - \b SYSCTL_CFG_VCO_320 to set the PLL VCO output to 320-MHz
//!
//! Example: Configure the system clocking to be 40 MHz with a 320-MHz PLL
//! setting using the 16-MHz internal oscillator.
//!
//! \verbatim
//! SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320,
//!                    40000000);
//! \endverbatim
//!
//! \note This function cannot be used with TM4C123 devices.  For TM4C123
//! devices use the SysCtlClockSet() function.
//!
//! \return The actual configured system clock frequency in Hz or zero if the
//! value could not be changed due to a parameter error or PLL lock failure.
//
//*****************************************************************************
static uint32_t
SysCtlClockFreqSet(uint32_t ui32Config, uint32_t ui32SysClock)
{
    volatile int32_t i32Timeout, i32VCOIdx, i32XtalIdx;
    uint32_t ui32MOSCCTL;
    uint32_t ui32Delay;
    uint32_t ui32SysDiv, ui32Osc, ui32OscSelect, ui32RSClkConfig;

    //
    // Get the index of the crystal from the ui32Config parameter.
    //
    i32XtalIdx = SysCtlXtalCfgToIndex(ui32Config);

    if((ui32Config & 0x38) == SYSCTL_OSC_MAIN)
    {
        //
        // Bounds check the source frequency for the main oscillator.  The is
        // because the PLL tables in the g_pppui32XTALtoVCO structure range
        // from 5MHz to 25MHz.
        //
        if((i32XtalIdx > (SysCtlXtalCfgToIndex(SYSCTL_XTAL_25MHZ))) ||
           (i32XtalIdx < (SysCtlXtalCfgToIndex(SYSCTL_XTAL_5MHZ))))
        {
            return(0);
        }

        ui32Osc = g_pui32Xtals[i32XtalIdx];

        //
        // Set the PLL source select to MOSC.
        //
        ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_MOSC;
        ui32OscSelect |= SYSCTL_RSCLKCFG_PLLSRC_MOSC;

        //
        // Clear MOSC power down, high oscillator range setting, and no crystal
        // present setting.
        //
        ui32MOSCCTL = HWREG(SYSCTL_MOSCCTL) &
                      ~(SYSCTL_MOSCCTL_OSCRNG | SYSCTL_MOSCCTL_PWRDN |
                        SYSCTL_MOSCCTL_NOXTAL);

        //
        // Increase the drive strength for MOSC of 10 MHz and above.
        //
        if(i32XtalIdx >= (SysCtlXtalCfgToIndex(SYSCTL_XTAL_10MHZ) -
                          (SysCtlXtalCfgToIndex(SYSCTL_XTAL_5MHZ))))
        {
            ui32MOSCCTL |= SYSCTL_MOSCCTL_OSCRNG;
        }

        HWREG(SYSCTL_MOSCCTL) = ui32MOSCCTL;
        
        //
        // Timeout using the legacy delay value.
        //
        ui32Delay = 524288;

        while((HWREG(SYSCTL_RIS) & SYSCTL_RIS_MOSCPUPRIS) == 0)
        {
            ui32Delay--;

            if(ui32Delay == 0)
            {
                break;
            }
        }

        //
        // If the main oscillator failed to start up then do not switch to
        // it and return.
        //
        if(ui32Delay == 0)
        {
            return(0);
        }

        
    }
    else
    {
        //
        // This was an invalid request because no oscillator source was
        // indicated.
        //
        ui32Osc = 0;
        ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_PIOSC;
    }

    //
    // Check if the running with the PLL enabled was requested.
    //
    if((ui32Config & SYSCTL_USE_OSC) == SYSCTL_USE_PLL)
    {
        //
        // ui32Config must be SYSCTL_OSC_MAIN or SYSCTL_OSC_INT.
        //
        if(((ui32Config & 0x38) != SYSCTL_OSC_MAIN) &&
           ((ui32Config & 0x38) != SYSCTL_OSC_INT))
        {
            return(0);
        }

        //
        // Get the VCO index out of the ui32Config parameter.
        //
        i32VCOIdx = (ui32Config >> 24) & 7;

        //
        // Set the memory timings for the maximum external frequency since
        // this could be a switch to PIOSC or possibly to MOSC which can be
        // up to 25MHz.
        //
        HWREG(SYSCTL_MEMTIM0) = _SysCtlMemTimingGet(25000000);

        //
        // Clear the old PLL divider and source in case it was set.
        //
        ui32RSClkConfig = HWREG(SYSCTL_RSCLKCFG) &
                          ~(SYSCTL_RSCLKCFG_PSYSDIV_M |
                            SYSCTL_RSCLKCFG_OSCSRC_M |
                            SYSCTL_RSCLKCFG_PLLSRC_M | SYSCTL_RSCLKCFG_USEPLL);

        //
        // Update the memory timings to match running from PIOSC.
        //
        ui32RSClkConfig |= SYSCTL_RSCLKCFG_MEMTIMU;

        //
        // Update clock configuration to switch back to PIOSC.
        //
        HWREG(SYSCTL_RSCLKCFG) = ui32RSClkConfig;

        //
        // The table starts at 5 MHz so modify the index to match this.
        //
        i32XtalIdx -= SysCtlXtalCfgToIndex(SYSCTL_XTAL_5MHZ);

        //
        // Calculate the System divider such that we get a frequency that is
        // the closest to the requested frequency without going over.
        //
        ui32SysDiv = (240000000 + ui32SysClock - 1) /
                     ui32SysClock;

        //
        // Set the oscillator source.
        //
        HWREG(SYSCTL_RSCLKCFG) |= ui32OscSelect;

        //
        // Set the M, N and Q values provided from the table and preserve
        // the power state of the main PLL.
        //
        HWREG(SYSCTL_PLLFREQ1) = PLL_N_TO_REG(5);
        HWREG(SYSCTL_PLLFREQ1) |= PLL_Q_TO_REG(ui32SysDiv);
        HWREG(SYSCTL_PLLFREQ0) = PLL_M_TO_REG(96, 0) |
             (HWREG(SYSCTL_PLLFREQ0) & SYSCTL_PLLFREQ0_PLLPWR);

        //
        // Calculate the actual system clock as PSYSDIV is always div-by 2.
        //
        ui32SysClock = _SysCtlFrequencyGet(ui32Osc) / 2;

        //
        // Set the Flash and EEPROM timing values.
        //
        HWREG(SYSCTL_MEMTIM0) = _SysCtlMemTimingGet(ui32SysClock);

        //
        // Check if the PLL is already powered up.
        //
        if(HWREG(SYSCTL_PLLFREQ0) & SYSCTL_PLLFREQ0_PLLPWR)
        {
            //
            // Trigger the PLL to lock to the new frequency.
            //
            HWREG(SYSCTL_RSCLKCFG) |= SYSCTL_RSCLKCFG_NEWFREQ;
        }
        else
        {
            //
            // Power up the PLL.
            //
            HWREG(SYSCTL_PLLFREQ0) |= SYSCTL_PLLFREQ0_PLLPWR;
        }

        //
        // Wait until the PLL has locked.
        //
        for(i32Timeout = 32768; i32Timeout > 0; i32Timeout--)
        {
            if((HWREG(SYSCTL_PLLSTAT) & SYSCTL_PLLSTAT_LOCK))
            {
                break;
            }
        }

        //
        // If the loop above did not timeout then switch over to the PLL
        //
        if(i32Timeout)
        {
            ui32RSClkConfig = HWREG(SYSCTL_RSCLKCFG);
            ui32RSClkConfig |= (1 << SYSCTL_RSCLKCFG_PSYSDIV_S) |
                                ui32OscSelect | SYSCTL_RSCLKCFG_USEPLL;
            ui32RSClkConfig |= SYSCTL_RSCLKCFG_MEMTIMU;

            //
            // Set the new clock configuration.
            //
            HWREG(SYSCTL_RSCLKCFG) = ui32RSClkConfig;
        }
        else
        {
            ui32SysClock = 0;
        }
    }
    

    //
    // Finally change the OSCSRC back to PIOSC
    //
    HWREG(SYSCTL_RSCLKCFG) &= ~(SYSCTL_RSCLKCFG_OSCSRC_M);

    return(ui32SysClock);
}
