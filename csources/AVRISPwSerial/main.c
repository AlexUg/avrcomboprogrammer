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
 *  Main source file for the AVRISP project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "AVRISP-MKII.h"
#include "USBtoSerial.h"


uint8_t activeConfig __attribute__ ((section (".noinit")));
//uint8_t activeConfig;

void
SelectConfiguration(void);


//#if (BOARD != BOARD_NONE)
///* Some board hardware definitions (e.g. the Arduino Micro) have their LEDs defined on the same pins
// as the ISP, PDI or TPI interfaces (see the accompanying project documentation). If a board other
// than NONE is selected (to enable the LED driver with the programmer) you should double-check that
// no conflicts will occur. If there is a conflict, turn off the LEDs (set BOARD to NONE in the makefile)
// or define a custom board driver (see the LUFA manual) with alternative LED mappings.
// */
//#warning Board specific drivers have been selected; make sure the board LED driver does not conflict with the programmer ISP/PDI/TPI interfaces.
//#endif

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int
main (void)
{
  SetupHardware ();

#ifdef SERIAL_ENABLE
  if (activeConfig)
    SetupSerialHardware();
  else
#endif
    V2Protocol_Init ();

  LEDs_SetAllLEDs (LEDMASK_USB_NOTREADY);
  GlobalInterruptEnable ();

  for (;;)
    {

#ifdef SERIAL_ENABLE
  if (activeConfig)
    Serial_Task();
  else
#endif
    AVRISP_Task ();

      USB_USBTask ();
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void
SetupHardware (void)
{
#if (ARCH == ARCH_AVR8)
  /* Disable watchdog if enabled by bootloader/fuses */
  MCUSR &= ~(1 << WDRF);
  wdt_disable ();

  /* Disable clock division */
  clock_prescale_set (clock_div_1);
#endif

  SelectConfiguration();

  /* Hardware Initialization */
  LEDs_Init ();

  /* USB Stack Initialization */
  USB_Init ();
}

/** Event handler for the library USB Connection event. */
void
EVENT_USB_Device_Connect (void)
{
  LEDs_SetAllLEDs (LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void
EVENT_USB_Device_Disconnect (void)
{
  LEDs_SetAllLEDs (LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void
EVENT_USB_Device_ConfigurationChanged (void)
{
  bool ConfigSuccess = true;

#ifdef SERIAL_ENABLE
  if (activeConfig)
    ConfigSuccess &= EVENT_Serial_Device_ConfigurationChanged();
  else
#endif

  /* Setup AVRISP Data OUT endpoint */
    ConfigSuccess &= EVENT_AVRISP_Device_ConfigurationChanged();

  /* Indicate endpoint configuration success or failure */
  LEDs_SetAllLEDs (ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

void
SelectConfiguration(void)
{
  activeConfig = (activeConfig + 1) % 2;
}

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 *
 *  \param[in]  wValue                 Descriptor type and index to retrieve
 *  \param[in]  wIndex                 Sub-index to retrieve (such as a localized string language)
 *  \param[out] DescriptorAddress      Address of the retrieved descriptor
 *
 *  \return Length of the retrieved descriptor in bytes, or NO_DESCRIPTOR if the descriptor was not found
 */
uint16_t
CALLBACK_USB_GetDescriptor (const uint16_t wValue, const uint16_t wIndex,
                            const void** const DescriptorAddress)
{

#ifdef SERIAL_ENABLE
  if (activeConfig)
    return Serial_GetDescriptor(wValue, wIndex, DescriptorAddress);
  else
#endif

  return AVRISP_GetDescriptor (wValue, wIndex, DescriptorAddress);
}

