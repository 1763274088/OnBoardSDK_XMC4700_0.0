/*! @file BspUsart.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Usart helper functions and ISR for board XMC4700Discovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */
#include <XMC4700.h>
#include "xmc_gpio.h"
#include "xmc_uart.h"
#include <xmc_scu.h>
#include "BspUsart.h"
#include "DJI_API.h"
#include "DJI_HardDriver.h"
#include "DJI_Flight.h"

#define USIC0_SR5          5

using namespace DJI::onboardSDK;
extern CoreAPI defaultAPI;
extern CoreAPI *coreApi;
extern Flight flight;
extern FlightData flightData;

extern unsigned char come_data;
extern unsigned char Rx_length;
extern int Rx_adr;
extern int Rx_Handle_Flag;
extern unsigned char Rx_buff[];

#define PC_UART_RX_PIN       P3_14
#define PC_UART_TX_PIN       P3_15

/* PC_UART configuration */
XMC_USIC_CH_t           *pc_uart =XMC_UART1_CH1;
XMC_UART_CH_CONFIG_t     pc_uart_config =
{
		115200U,
		8U,
		8U,
		1U,
		16U,
		XMC_USIC_CH_PARITY_MODE_NONE
};	

XMC_GPIO_CONFIG_t        pc_rx_pin_config;
XMC_GPIO_CONFIG_t        pc_tx_pin_config;

#define N3_UART_RX_PIN      P0_4
#define N3_UART_TX_PIN      P0_5

/* N3_UART configuration */
XMC_USIC_CH_t           *n3_uart = XMC_UART1_CH0;
XMC_UART_CH_CONFIG_t     n3_uart_config =
{
		230400U,
		8U,
		8U,
		1U,
		16U,
		XMC_USIC_CH_PARITY_MODE_NONE
};	

XMC_GPIO_CONFIG_t        n3_rx_pin_config;
XMC_GPIO_CONFIG_t        n3_tx_pin_config;

/*
 * PC_UART is used for receiving commands from PC and
 * printing debug information to PC
 */
void PC_UART_Config(void)
{
  XMC_UART_CH_Init(pc_uart, &pc_uart_config);
  XMC_UART_CH_SetInputSource(pc_uart, XMC_UART_CH_INPUT_RXD , USIC1_C1_DX0_P3_14);
  XMC_UART_CH_Start(pc_uart);  // uart.CCR.mode=UART-> USIC0_CH0 is switched for UART operation mode


  /*PC_UART Rx pin configuration*/
  pc_rx_pin_config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;          // Rx input with internal pull-up
  XMC_GPIO_Init(PC_UART_RX_PIN, &pc_rx_pin_config);
  /*PC_UART Tx pin configuration*/
  pc_tx_pin_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
  pc_tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;  // Tx output in ALT2
  XMC_GPIO_Init(PC_UART_TX_PIN, &pc_tx_pin_config);
}

/*
 * N3_UART is used for communicating with the DJI flight controller
 * The Baud rate needs to match the Baud rate used by the flight controller
 */
void N3_UART_Config(void)
{

  XMC_UART_CH_Init(n3_uart, &n3_uart_config);
  XMC_UART_CH_SetInputSource(n3_uart, XMC_UART_CH_INPUT_RXD , USIC1_C0_DX0_P0_4);
  XMC_UART_CH_Start(n3_uart);  // uart.CCR.mode=UART-> USIC0_CH0 is switched for UART operation mode


  /*N3_UART Rx pin configuration*/
  n3_rx_pin_config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;          // Rx input with internal pull-up
  XMC_GPIO_Init(N3_UART_RX_PIN, &n3_rx_pin_config);
  /*N3_UART Tx pin configuration*/
  n3_tx_pin_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
  n3_tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;  // Tx output in ALT2
  XMC_GPIO_Init(N3_UART_TX_PIN, &n3_tx_pin_config);
}	


void UARTxNVIC_Config()
{
	
	XMC_USIC_CH_EnableEvent(pc_uart, XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
  XMC_USIC_CH_SetInterruptNodePointer(pc_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, USIC0_SR5);             // set USIC0 SR5 for RI
  XMC_USIC_CH_EnableEvent(pc_uart, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
  XMC_USIC_CH_SetInterruptNodePointer(pc_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, USIC0_SR5);

  NVIC_SetPriority(USIC0_5_IRQn,2);   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
  NVIC_EnableIRQ(USIC0_5_IRQn);       // CMSIS function for NVIC control: enable IRQn

	XMC_USIC_CH_EnableEvent(n3_uart, XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
  XMC_USIC_CH_SetInterruptNodePointer(n3_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, USIC0_SR5);             // set USIC0 SR5 for RI
  XMC_USIC_CH_EnableEvent(n3_uart, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
  XMC_USIC_CH_SetInterruptNodePointer(n3_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, USIC0_SR5);

  NVIC_SetPriority(USIC1_5_IRQn,3);   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
  NVIC_EnableIRQ(USIC1_5_IRQn);       // CMSIS function for NVIC control: enable IRQn
	

}

void UartConfig()
{
  PC_UART_Config();
  N3_UART_Config();
  UARTxNVIC_Config();
}

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

void USIC1_5_IRQHandler(void)
{
    coreApi->byteHandler(XMC_UART_CH_GetReceivedData(n3_uart)); //Data from M100 were committed to "byteHandler"
}


#ifdef __cplusplus
}
#endif //__cplusplus
