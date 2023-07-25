/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


uint32_t increments_pot1[ OSCILLATOR_COUNT ];
uint32_t phase_accu_pot1[ OSCILLATOR_COUNT ];
uint32_t envelope_positions_envpot1[ OSCILLATOR_COUNT ];

uint8_t next_osc1 = 0;

uint32_t increments_pot2[ OSCILLATOR_COUNT ];
uint32_t phase_accu_pot2[ OSCILLATOR_COUNT ];
uint32_t envelope_positions_envpot2[ OSCILLATOR_COUNT ];

uint8_t next_osc2 = 0;

const uint32_t sizeof_wt_sustain_pot1 = ((uint32_t)sizeof (wt_sustain1) << POT1);
const uint32_t sizeof_wt_attack_pot1 = ((uint32_t)sizeof (wt_attak1) << POT1);
const uint32_t sizeof_wt_pot1 = ((uint32_t)sizeof (wt_attak1) << POT1)+((uint32_t)sizeof (wt_sustain1) << POT1);
const uint32_t sizeof_envelope_table_envpot1 = ((uint32_t)sizeof (envelope_table1) << ENVPOT1);

const uint32_t sizeof_wt_sustain_pot2 = ((uint32_t)sizeof (wt_sustain2) << POT2);
const uint32_t sizeof_wt_attack_pot2 = ((uint32_t)sizeof (wt_attak2) << POT2);
const uint32_t sizeof_wt_pot2 = ((uint32_t)sizeof (wt_attak2) << POT2)+((uint32_t)sizeof (wt_sustain2) << POT2);
const uint32_t sizeof_envelope_table_envpot2 = ((uint32_t)sizeof (envelope_table2) << ENVPOT2);

uint8_t my_sound_num = 0;
uint8_t last_sound_num = 0;

void playSound(uint8_t note) {
    int d = 0;
//    d= rand()%4;
    increments_pot1[ next_osc1 ] = scale_table[ note ]+ (6.586368)*0;
    phase_accu_pot1[ next_osc1 ] = 0;
    envelope_positions_envpot1[ next_osc1 ] = 0;
    next_osc1++;
    if (next_osc1 >= OSCILLATOR_COUNT) {
        next_osc1 = 0;
    }
    d= rand()%4;
    increments_pot1[ next_osc1 ] = scale_table[ note ] + (6.586368)*d;
    phase_accu_pot1[ next_osc1 ] = 0;
    envelope_positions_envpot1[ next_osc1 ] = 0;
    next_osc1++;
    if (next_osc1 >= OSCILLATOR_COUNT) {
        next_osc1 = 0;
    }

//    d= rand()%4;
    increments_pot2[ next_osc2 ] = scale_table[ note ] + (6.586368)*0;
    phase_accu_pot2[ next_osc2 ] = 0;
    envelope_positions_envpot2[ next_osc2 ] = 0;
    next_osc2++;
    if (next_osc2 >= OSCILLATOR_COUNT) {
        next_osc2 = 0;
    }
    d= rand()%4;
    increments_pot2[ next_osc2 ] = scale_table[ note ] + (6.586368)*d;
    phase_accu_pot2[ next_osc2 ] = 0;
    envelope_positions_envpot2[ next_osc2 ] = 0;
    next_osc2++;
    if (next_osc2 >= OSCILLATOR_COUNT) {
        next_osc2 = 0;
    }
}

static void tmr3EventHandler(uint32_t intCause, uintptr_t context) {

    int32_t value = 0;
    int32_t value1 = 0;
    int32_t value2 = 0;

    for (uint8_t osc = 0; osc < OSCILLATOR_COUNT; osc++) {
        phase_accu_pot1[ osc ] += increments_pot1[ osc ];

        while (phase_accu_pot1[ osc ] >= (sizeof_wt_attack_pot1 + sizeof_wt_sustain_pot1)) {
            phase_accu_pot1[ osc ] -= sizeof_wt_sustain_pot1;
        }
        uint16_t phase_accu = 0;
        int8_t accu = 0;
        if (phase_accu_pot1[ osc ] < sizeof_wt_attack_pot1) {
            phase_accu = (phase_accu_pot1[ osc ] >> POT1);
            accu = (wt_attak1[phase_accu]);
        } else {
            phase_accu = (phase_accu_pot1[ osc ] - sizeof_wt_attack_pot1) >> POT1;
            accu = (wt_sustain1[phase_accu]);
        }
        value1 += envelope_table1[ envelope_positions_envpot1[ osc ] >> ENVPOT1 ] * accu;

        if (phase_accu_pot1[ osc ] >= sizeof_wt_attack_pot1 &&
                envelope_positions_envpot1[ osc ] < sizeof_envelope_table_envpot1 - 1) {
            ++envelope_positions_envpot1[ osc ];
        }
    }
    value1 >>= 8; // envelope_table resolution
    //    value = value1;
    for (uint8_t osc = 0; osc < OSCILLATOR_COUNT; osc++) {
        phase_accu_pot2[ osc ] += increments_pot2[ osc ];

        while (phase_accu_pot2[ osc ] >= (sizeof_wt_attack_pot2 + sizeof_wt_sustain_pot2)) {
            phase_accu_pot2[ osc ] -= sizeof_wt_sustain_pot2;
        }
        uint16_t phase_accu = 0;
        int8_t accu = 0;
        if (phase_accu_pot2[ osc ] < sizeof_wt_attack_pot2) {
            phase_accu = (phase_accu_pot2[ osc ] >> POT2);
            accu = (wt_attak2[phase_accu]);
        } else {
            phase_accu = (phase_accu_pot2[ osc ] - sizeof_wt_attack_pot2) >> POT2;
            accu = (wt_sustain2[phase_accu]);
        }
        value2 += envelope_table2[ envelope_positions_envpot2[ osc ] >> ENVPOT2 ] * accu;

        if (phase_accu_pot2[ osc ] >= sizeof_wt_attack_pot2 &&
                envelope_positions_envpot2[ osc ] < sizeof_envelope_table_envpot2 - 1) {
            ++envelope_positions_envpot2[ osc ];
        }
    }
    value2 >>= 8; // envelope_table resolution
    value = (value1 + value2) >> 1;


    if (value > CLIP) {
        value = CLIP;
    } else if (value < -CLIP) {
        value = -CLIP;
    }
    OC1RS = (1024 + value);
    OC2RS = (1024 + value);


}
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);

    // AMP ENABLE
    GPIO_RB7_Clear();

    for (uint8_t osc = 0; osc < OSCILLATOR_COUNT; osc++) {
        increments_pot1[ osc ] = 0;
        phase_accu_pot1[ osc ] = 0;
        envelope_positions_envpot1[ osc ] = 0;

        increments_pot2[ osc ] = 0;
        phase_accu_pot2[ osc ] = 0;
        envelope_positions_envpot2[ osc ] = 0;
    }

    // PWM
    OCMP1_Enable();
    OCMP1_CompareSecondaryValueSet(0);
    OCMP2_Enable();
    OCMP2_CompareSecondaryValueSet(0);
    TMR2_Start();

    TMR3_CallbackRegister(tmr3EventHandler, 0);
    TMR3_Start();

    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */

