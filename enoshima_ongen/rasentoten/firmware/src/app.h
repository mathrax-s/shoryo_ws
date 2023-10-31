/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"                // SYS function prototypes
#include "pitches.h"
#include "scale_table.h"


#define PINK_SAZAE

#if defined(PINK_SAZAE)
//PINK_SAZAE
#include "wt_saw_1.h"
#include "wt_saw_2.h"
#elif 
//WHITE_SAZAE
#define WHITE_SAZAE 
#include "wt_sin_1.h"
#include "wt_sin_2.h"
#endif

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
    // DOM-IGNORE-END

    // *****************************************************************************
    // *****************************************************************************
    // Section: Type Definitions
    // *****************************************************************************
    // *****************************************************************************
#define APP_DATA_SIZE   1

    //#define POT1 9 // power of two; must match with scale_table values
    //#define POT2 12 // power of two; must match with scale_table values
    //
    //#define ENVPOT1 7
    //#define ENVPOT2 8

    //#define POT1 7 // power of two; must match with scale_table values
    //#define POT2 8 // power of two; must match with scale_table values
    //
    //#define ENVPOT1 7
    //#define ENVPOT2 6
#define MAX_S 21

#define OSCILLATOR_COUNT 24 // 16 oscillators: 25% load with -O1 (64: 90%)
#define CLIP 1016 // 127 * 8

    extern uint32_t increments_pot1[ OSCILLATOR_COUNT ];
    extern uint32_t phase_accu_pot1[ OSCILLATOR_COUNT ];
    extern uint32_t envelope_positions_envpot1[ OSCILLATOR_COUNT ];

    extern uint32_t increments_pot2[ OSCILLATOR_COUNT ];
    extern uint32_t phase_accu_pot2[ OSCILLATOR_COUNT ];
    extern uint32_t envelope_positions_envpot2[ OSCILLATOR_COUNT ];

    extern uint8_t next_osc1;
    extern uint8_t next_osc2;

    extern uint8_t my_sound_num;
    extern uint8_t last_sound_num;

    extern uint8_t data;
    extern uint8_t rxCounter;
    extern uint8_t receiveBuffer[100];


    void playSound(uint8_t note);
    void playStartup(uint8_t note);

    static const uint8_t startup_scale[] = {
        NOTE_Gb6, NOTE_Ab6, NOTE_Bb6, NOTE_Db7,
        //        NOTE_Db5, NOTE_Eb5, NOTE_Gb5, NOTE_Ab5, NOTE_Bb5, 
        //        NOTE_Db6, NOTE_Eb6, NOTE_Gb6, NOTE_Ab6, NOTE_Bb6, 
        //        NOTE_Db7, NOTE_Eb7, NOTE_Gb7, NOTE_Ab7, NOTE_Bb7, 
        //        NOTE_Db8, NOTE_Eb8, NOTE_Gb8, NOTE_Ab8, NOTE_Bb8, 
        //        NOTE_Db9, NOTE_Eb9, NOTE_Gb9, NOTE_Ab9, NOTE_Bb9, 

    };
    // for MATHRAX
    static const uint8_t scale[] = {

//                        NOTE_Db3, NOTE_Eb3, NOTE_Gb3, NOTE_Ab3, NOTE_Bb3,
//                        NOTE_Db4, NOTE_Eb4, NOTE_Gb4, NOTE_Ab4, NOTE_Bb4,

#if defined(WT_SAW_1_H) // RasenToTen : PINK_SAZAE
        NOTE_Ab5, NOTE_Bb5, NOTE_Db6, NOTE_Eb6, NOTE_Gb6, 
        NOTE_Ab6, NOTE_Bb6, NOTE_Db7, NOTE_Eb7, NOTE_Gb7, 
        NOTE_Ab7, NOTE_Bb7, NOTE_Db8, NOTE_Eb8, NOTE_Gb8, 
        NOTE_Ab8, NOTE_Bb8, NOTE_Db9, NOTE_Eb9, NOTE_Gb9, 
        NOTE_Ab9, NOTE_Bb9, NOTE_Db10, NOTE_Eb10, NOTE_Gb10, 
        NOTE_Ab10, 
        
#elif defined(WT_SIN_1_H)// RasenToTen : WHITE_SAZAE
        // 2023/8/31 ~
        NOTE_Db5, NOTE_Eb5, NOTE_Gb5, NOTE_Ab5, NOTE_Bb5,
        NOTE_Db6, NOTE_Eb6, NOTE_Gb6, NOTE_Ab6, NOTE_Bb6,
        NOTE_Db7, NOTE_Eb7, NOTE_Gb7, NOTE_Ab7, NOTE_Bb7,
        NOTE_Db8, NOTE_Eb8, NOTE_Gb8, NOTE_Ab8, NOTE_Bb8,
        NOTE_Db9, NOTE_Eb9, NOTE_Gb9, NOTE_Ab9, NOTE_Bb9,
        NOTE_Db10,
        // ~ 2023/8/31
#endif

        //        NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,
        //        NOTE_Db7, NOTE_Eb7, NOTE_Gb7, NOTE_Ab7, NOTE_Bb7, NOTE_Db8,
        //        NOTE_Db8, NOTE_Eb8, NOTE_Gb8, NOTE_Ab8, NOTE_Bb8,
        //        NOTE_Db9, NOTE_Eb9, NOTE_Gb9, NOTE_Ab9, NOTE_Bb9,
        //        NOTE_Db10, NOTE_Eb10, NOTE_Gb10, NOTE_Ab10, NOTE_Bb10,
        //        NOTE_D3, NOTE_E3, NOTE_G3, NOTE_A3, NOTE_B3,
        //        NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4,
        //        NOTE_D5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_B5,
        //        NOTE_D6, NOTE_E6, NOTE_G6, NOTE_A6, NOTE_B6,
        //        NOTE_D7, NOTE_E7, NOTE_G7, NOTE_A7, NOTE_B7,
        //        NOTE_D8, NOTE_E8, NOTE_G8, NOTE_A8, NOTE_B8,
        //        NOTE_D9, NOTE_E9, NOTE_G9, NOTE_A9, NOTE_B9,

    };

    // *****************************************************************************

    /* Application states

      Summary:
        Application states enumeration

      Description:
        This enumeration defines the valid application states.  These states
        determine the behavior of the application at various times.
     */

    typedef enum {
        /* Application's state machine's initial state. */
        APP_STATE_INIT = 0,
        APP_STATE_STARTUP,
        APP_STATE_SERVICE_TASKS,
        /* TODO: Define states used by the application state machine. */
        APP_STATE_TRANSMIT_MESSAGE,
        APP_STATE_WAIT_MESSAGE_TRANSFER_COMPLETE,
        APP_STATE_RECEIVE_DATA,
        APP_STATE_WAIT_RESPONSE,
        APP_STATE_WAIT_RECEIVE_COMPLETE,
        APP_STATE_RECEIVE_COMPLETE,
        APP_STATE_TRANSMIT_DATA,
        APP_STATE_WAIT_TRANSMIT_COMPLETE,
        APP_STATE_ERROR,
        APP_STATE_IDLE,
    } APP_STATES;


    // *****************************************************************************

    /* Application Data

      Summary:
        Holds application data

      Description:
        This structure holds the application's data.

      Remarks:
        Application strings and buffers are be defined outside this structure.
     */

    typedef struct {
        /* The application's current state */
        APP_STATES state;

        /* TODO: Define any additional data used by the application. */
    } APP_DATA;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Callback Routines
    // *****************************************************************************
    // *****************************************************************************
    /* These routines are called by drivers when certain events occur.
     */

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Initialization and State Machine Functions
    // *****************************************************************************
    // *****************************************************************************

    /*******************************************************************************
      Function:
        void APP_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the
        application in its initial state and prepares it to run so that its
        APP_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        APP_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void APP_Initialize(void);


    /*******************************************************************************
      Function:
        void APP_Tasks ( void )

      Summary:
        MPLAB Harmony Demo application tasks function

      Description:
        This routine is the Harmony Demo application's tasks function.  It
        defines the application's state machine and core logic.

      Precondition:
        The system and application initialization ("SYS_Initialize") should be
        called before calling this.

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        APP_Tasks();
        </code>

      Remarks:
        This routine must be called from SYS_Tasks() routine.
     */

    void APP_Tasks(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */

