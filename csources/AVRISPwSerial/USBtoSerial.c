/*
 LUFA Library
 Copyright (C) Dean Camera, 2019.

 dean [at] fourwalledcubicle [dot] com
 www.lufa-lib.org
 */

/*
 Copyright 2019  Dean Camera (dean [at] fourwalledcubicle [dot] com)

 Permission to use, copy, modify, distribute, and sell this
 software and its documentation for any purpose is hereby granted
 without fee, provided that the above copyright notice appear in
 all copies and that both that the copyright notice and this
 permission notice and warranty disclaimer appear in supporting
 documentation, and that the name of the author not be used in
 advertising or publicity pertaining to distribution of the
 software without specific, written prior permission.

 The author disclaims all warranties with regard to this
 software, including all implied warranties of merchantability
 and fitness.  In no event shall the author be liable for any
 special, indirect or consequential damages or any damages
 whatsoever resulting from loss of use, data or profits, whether
 in an action of contract, negligence or other tortious action,
 arising out of or in connection with the use or performance of
 this software.
 */

/** \file
 *
 *  Main source file for the USBtoSerial project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "USBtoSerial.h"

/** Circular buffer to hold data from the host before it is sent to the device via the serial port. */
static RingBuffer_t USBtoUSART_Buffer;

/** Underlying data buffer for \ref USBtoUSART_Buffer, where the stored bytes are located. */
static uint8_t USBtoUSART_Buffer_Data[128];

/** Circular buffer to hold data from the serial port before it is sent to the host. */
static RingBuffer_t USARTtoUSB_Buffer;

/** Underlying data buffer for \ref USARTtoUSB_Buffer, where the stored bytes are located. */
static uint8_t USARTtoUSB_Buffer_Data[128];

/* Pulse generation counters to keep track of the time remaining for each pulse type */
#define TX_RX_LED_PULSE_PERIOD 100
uint16_t TxLEDPulse = 0; // time remaining for Tx LED pulse
uint16_t RxLEDPulse = 0; // time remaining for Rx LED pulse

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
  { .Config =
    { .ControlInterfaceNumber = INTERFACE_ID_CDC_CCI, .DataINEndpoint =
      { .Address = CDC_TX_EPADDR, .Size = CDC_TXRX_EPSIZE, .Banks = 1, }, .DataOUTEndpoint =
      { .Address = CDC_RX_EPADDR, .Size = CDC_TXRX_EPSIZE, .Banks = 1, }, .NotificationEndpoint =
      { .Address = CDC_NOTIFICATION_EPADDR, .Size = CDC_NOTIFICATION_EPSIZE, .Banks = 1, }, }, };

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
void
SetupSerialHardware (void)
{

  /* Initialize TIMER3 to handle LED tasks.
   * With 16 MHz clock and 1/64 prescaler, timer 1 is clocked at 250 kHz
   * Our chosen compare match generates an interrupt every 1 ms.
   */
  OCR3AH = 0;
  OCR3AL = 250;
  TIMSK3 = (1 << OCIE3A);     // enable timer 1 output compare A match interrupt
  TCCR3B = ((1 << CS31) | (1 << CS30)); // 1/64 prescaler on timer 1 input
  TCCR3A = 0;

  /* Pull target /RESET line high */
  AUX_LINE_DDR |= AUX_LINE_MASK;
  AUX_LINE_PORT &= ~AUX_LINE_MASK;

  RingBuffer_InitBuffer (&USBtoUSART_Buffer, USBtoUSART_Buffer_Data, sizeof(USBtoUSART_Buffer_Data));
  RingBuffer_InitBuffer (&USARTtoUSB_Buffer, USARTtoUSB_Buffer_Data, sizeof(USARTtoUSB_Buffer_Data));

}

void
Serial_Task (void)
{
  /* Only try to read in bytes from the CDC interface if the transmit buffer is not full */
  if (!(RingBuffer_IsFull (&USBtoUSART_Buffer)))
    {
      int16_t ReceivedByte = CDC_Device_ReceiveByte (&VirtualSerial_CDC_Interface);

      /* Store received byte into the USART transmit buffer */
      if (!(ReceivedByte < 0))
        {
          LEDs_TurnOnLEDs(LEDMASK_TX);
          TxLEDPulse = TX_RX_LED_PULSE_PERIOD;
          RingBuffer_Insert (&USBtoUSART_Buffer, ReceivedByte);
        }
    }

  uint16_t BufferCount = RingBuffer_GetCount (&USARTtoUSB_Buffer);
  if (BufferCount)
    {
      LEDs_TurnOnLEDs(LEDMASK_RX);
      RxLEDPulse = TX_RX_LED_PULSE_PERIOD;

      Endpoint_SelectEndpoint (VirtualSerial_CDC_Interface.Config.DataINEndpoint.Address);

      /* Check if a packet is already enqueued to the host - if so, we shouldn't try to send more data
       * until it completes as there is a chance nothing is listening and a lengthy timeout could occur */
      if (Endpoint_IsINReady ())
        {
          /* Never send more than one bank size less one byte to the host at a time, so that we don't block
           * while a Zero Length Packet (ZLP) to terminate the transfer is sent if the host isn't listening */
          uint8_t BytesToSend = MIN(BufferCount, (CDC_TXRX_EPSIZE - 1));

          /* Read bytes from the USART receive buffer into the USB IN endpoint */
          while (BytesToSend--)
            {
              /* Try to send the next byte of data to the host, abort if there is an error without dequeuing */
              if (CDC_Device_SendByte (&VirtualSerial_CDC_Interface, RingBuffer_Peek (&USARTtoUSB_Buffer)) != ENDPOINT_READYWAIT_NoError)
                {
                  break;
                }

              /* Dequeue the already sent byte from the buffer now we have confirmed that no transmission error occurred */
              RingBuffer_Remove (&USARTtoUSB_Buffer);
            }
        }
    }

  /* Load the next byte from the USART transmit buffer into the USART if transmit buffer space is available */
  if (Serial_IsSendReady () && !(RingBuffer_IsEmpty (&USBtoUSART_Buffer)))
    Serial_SendByte (RingBuffer_Remove (&USBtoUSART_Buffer));

  CDC_Device_USBTask (&VirtualSerial_CDC_Interface);
}


/** Event handler for the library USB Configuration Changed event. */
bool
EVENT_Serial_Device_ConfigurationChanged (void)
{
  return CDC_Device_ConfigureEndpoints (&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void
EVENT_USB_Device_ControlRequest (void)
{
  CDC_Device_ProcessControlRequest (&VirtualSerial_CDC_Interface);
}

/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
ISR(USART1_RX_vect, ISR_BLOCK)
{
  uint8_t ReceivedByte = UDR1;

  if ((USB_DeviceState == DEVICE_STATE_Configured) && !(RingBuffer_IsFull (&USARTtoUSB_Buffer)))
    RingBuffer_Insert (&USARTtoUSB_Buffer, ReceivedByte);
}

/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void
EVENT_CDC_Device_LineEncodingChanged (USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
  uint8_t ConfigMask = 0;

  switch (CDCInterfaceInfo->State.LineEncoding.ParityType)
    {
    case CDC_PARITY_Odd:
      ConfigMask = ((1 << UPM11) | (1 << UPM10));
      break;
    case CDC_PARITY_Even:
      ConfigMask = (1 << UPM11);
      break;
    }

  if (CDCInterfaceInfo->State.LineEncoding.CharFormat == CDC_LINEENCODING_TwoStopBits)
    ConfigMask |= (1 << USBS1);

  switch (CDCInterfaceInfo->State.LineEncoding.DataBits)
    {
    case 6:
      ConfigMask |= (1 << UCSZ10);
      break;
    case 7:
      ConfigMask |= (1 << UCSZ11);
      break;
    case 8:
      ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
      break;
    }

  /* Keep the TX line held high (idle) while the USART is reconfigured */
  PORTD |= (1 << 3);

  /* Must turn off USART before reconfiguring it, otherwise incorrect operation may occur */
  UCSR1B = 0;
  UCSR1A = 0;
  UCSR1C = 0;

  /* Set the new baud rate before configuring the USART */
  UBRR1 = SERIAL_2X_UBBRVAL(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);

  /* Reconfigure the USART in double speed mode for a wider baud rate range at the expense of accuracy */
  UCSR1C = ConfigMask;
  UCSR1A = (1 << U2X1);
  UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));

  /* Release the TX line after the USART has been reconfigured */
  PORTD &= ~(1 << 3);
}

/** Event handler for the CDC Class driver Host-to-Device Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
 bool CurrentDTRState = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR);

 if (CurrentDTRState)
   {
     AUX_LINE_PORT |= AUX_LINE_MASK;
   }
 else
   {
     AUX_LINE_PORT &= ~AUX_LINE_MASK;
   }
}

//uint16_t ctr = 0;
ISR(TIMER3_COMPA_vect, ISR_BLOCK)
{
  /* Reset counter */
  TCNT3H = 0;
  TCNT3L = 0;

  /* Check whether the TX or RX LED one-shot period has elapsed.  if so, turn off the LED */
  if (TxLEDPulse && !(--TxLEDPulse))
    LEDs_TurnOffLEDs(LEDMASK_TX);
  if (RxLEDPulse && !(--RxLEDPulse))
    LEDs_TurnOffLEDs(LEDMASK_RX);
}

