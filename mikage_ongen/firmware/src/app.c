/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint8_t data = 0;
uint8_t rxCounter = 0;
uint8_t receiveBuffer[100] = {0};
uint8_t sensorData[100] = {0};
uint8_t sensorStatus[100] = {0};
uint8_t start_count = 0;
uint32_t start_timer = 0;
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    //    last_sound_num = 0;
    //    my_sound_num = (sizeof (scale)) / 4 * 3;
    start_count = 0;
    start_timer = 0;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void) {

    /* Check the application's current state. */
    switch (appData.state) {
            /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            if (appInitialized) {
                start_timer = 0;
                for (int i = 0; i < 2; i++) {
                    GPIO_RB5_Clear();
                    CORETIMER_DelayMs(250);
                    GPIO_RB5_Set();
                    CORETIMER_DelayMs(250);
                }
                appData.state = APP_STATE_STARTUP;
            }
            break;
        }
        case APP_STATE_STARTUP:
        {

            playSound(startup_scale[start_count]);
            start_count++;
            if (start_count >= sizeof (startup_scale)) {
                start_count = 0;
                appData.state = APP_STATE_RECEIVE_DATA;
            } else {
                CORETIMER_DelayMs(125);
            }
            break;
        }
        case APP_STATE_WAIT_RESPONSE:
        {
            //            while (UART1_ReceiverIsReady() == false) {
            //                UART1_WriteByte('A');
            //            }
            appData.state = APP_STATE_RECEIVE_DATA;

            break;
        }
        case APP_STATE_RECEIVE_DATA:
        {
            if (UART1_ReceiverIsReady() == true) {
                if (UART1_ErrorGet() == UART_ERROR_NONE) {
                    UART1_Read(&data, 1);
                    if (data == 0xFF) {
                        rxCounter = 0;
                    } else if (data == '\n') {
                        uint8_t _idx = 0;
                        for(int i = 0; i<rxCounter; i++){
//                            if(i%2 != 0){
                                sensorData[_idx++] = receiveBuffer[i];
//                            }
                        }
                        appData.state = APP_STATE_RECEIVE_COMPLETE;
                    } else {
                        receiveBuffer[rxCounter] = data;
                        rxCounter++;
                    }
                }
            } else {
//                UART1_WriteByte('A');
            }
            break;
        }
        case APP_STATE_RECEIVE_COMPLETE:
        {
            for (int i = 0; i < MAX_S; i++) {
                if (sensorData[i] == '1') {
                    if (sensorStatus[i] == 0) {
                        sensorStatus[i] = 1;
                        playSound(scale[i]);
                    }
                } else {
                    sensorStatus[i] = 0;
                }
            }
            appData.state = APP_STATE_RECEIVE_DATA;
            break;
        }
        case APP_STATE_IDLE:
        default:
            break;
    }
}


/*******************************************************************************
 End of File
 */
