#include "cy_pdl.h"
#include "cybsp.h"
#include "cycfg.h"
#include "cycfg_capsense.h"
#include "cy_gpio.h"

#define CAPSENSE_INTR_PRIORITY    (3u)
#define CY_ASSERT_FAILED          (0u)
//#define SLIDER_NUM_TOUCH          (1u)

cy_stc_scb_uart_context_t CYBSP_UART_context;

static void initialize_capsense(void);
static void capsense_isr(void);

#define MAX_S 20
//uint8_t touch[11] = {0};
//uint32_t prevSliderPos = 0u;
uint8_t touch[MAX_S] = { 0u };
uint8_t last_touch[MAX_S] = { 0 };
uint8_t led = 0;
void handle_error(void) {
	/* Disable all interrupts */
	__disable_irq();
	while (1) {
	}
}

uint8_t btn_status = 0;
uint8_t sensor_dummy = 0;

float mapRange(float value, float a, float b, float c, float d) {
	value = (value - a) / (b - a);
	return c + value * (d - c);
}

int main(void) {
	cy_rslt_t result;
	cy_en_scb_uart_status_t initstatus;
	//	uint32_t currSliderPos;

	/* Initialize the device and board peripherals */
	result = cybsp_init();
	if (result != CY_RSLT_SUCCESS) {
		CY_ASSERT(0);
	}
	/* Initialize the UART */
	initstatus = Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config,
			&CYBSP_UART_context);
	/* Initialization failed. Handle error */
	if (initstatus != CY_SCB_UART_SUCCESS) {
		handle_error();
	}
	/* Enable global interrupts */
	__enable_irq();

	/* UART */
	Cy_SCB_UART_Enable(CYBSP_UART_HW);

	/* CapSense */
	initialize_capsense();
	Cy_CapSense_ScanAllWidgets(&cy_capsense_context);

	/* StartUp LED */
	for (int i = 0; i < 10; i++) {
		Cy_GPIO_Write(P1_2_PORT, P1_2_NUM, 1);
		Cy_SysLib_Delay(50);
		Cy_GPIO_Write(P1_2_PORT, P1_2_NUM, 0);
		Cy_SysLib_Delay(50);
	}

	for (;;) {
		if (CY_CAPSENSE_NOT_BUSY == Cy_CapSense_IsBusy(&cy_capsense_context)) {
			/* Process all widgets */
			Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);
			for (int i = 0; i < MAX_S; i++) {
				/* Turning ON/OFF based on button status */
				if (0 != Cy_CapSense_IsWidgetActive(i, &cy_capsense_context)) {

					touch[i] = '1';
				} else {
					touch[i] = '0';
				}
			}
			Cy_CapSense_ScanAllWidgets(&cy_capsense_context);
		}
		led = 0;
		for (int i = 0; i < MAX_S; i++) {
		 if(touch[i] == '1'){
			 led = 1;
		 }
		}
		if(led==1){
			Cy_GPIO_Write(P1_2_PORT, P1_2_NUM, 1);
		}else{
			Cy_GPIO_Write(P1_2_PORT, P1_2_NUM, 0);
		}
		uint8_t uartTxHeader[] = { 0xFFu };
		uint8_t uartTxTail[] = { '\n' };
		Cy_SCB_UART_PutArrayBlocking(CYBSP_UART_HW, &(uartTxHeader[0u]),
				sizeof(uartTxHeader));
		Cy_SCB_UART_PutArrayBlocking(CYBSP_UART_HW, (uint8_t*) &(touch[0u]),
				sizeof(touch));
		Cy_SCB_UART_PutArrayBlocking(CYBSP_UART_HW, uartTxTail,
				sizeof(uartTxTail));

		Cy_SysLib_Delay(5);

		// BUTTON Input
		if (1UL != Cy_GPIO_Read(P0_6_PORT, P0_6_NUM)) {
			// ON
			/* StartUp LED */
			for (int i = 0; i < 4; i++) {
				Cy_CapSense_CalibrateAllWidgets(&cy_capsense_context);
				Cy_GPIO_Write(P1_2_PORT, P1_2_NUM, 1);
				Cy_SysLib_Delay(250);
				Cy_GPIO_Write(P1_2_PORT, P1_2_NUM, 0);
				Cy_SysLib_Delay(250);
			}


		}
	}

}

// CAPSENSE
static void initialize_capsense(void) {
	cy_capsense_status_t status = CY_CAPSENSE_STATUS_SUCCESS;
	const cy_stc_sysint_t capsense_interrupt_config = {
			.intrSrc = CYBSP_CSD_IRQ, .intrPriority = CAPSENSE_INTR_PRIORITY, };

	status = Cy_CapSense_Init(&cy_capsense_context);

	if (CY_CAPSENSE_STATUS_SUCCESS == status) {
		Cy_SysInt_Init(&capsense_interrupt_config, capsense_isr);
		NVIC_ClearPendingIRQ(capsense_interrupt_config.intrSrc);
		NVIC_EnableIRQ(capsense_interrupt_config.intrSrc);
		status = Cy_CapSense_Enable(&cy_capsense_context);
	}
}

static void capsense_isr(void) {
	Cy_CapSense_InterruptHandler(CYBSP_CSD_HW, &cy_capsense_context);
}

