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


/** Event handler for the library USB Configuration Changed event. */
bool
EVENT_AVRISP_Device_ConfigurationChanged (void)
{
  bool ConfigSuccess = true;

  /* Setup AVRISP Data OUT endpoint */
  ConfigSuccess &= Endpoint_ConfigureEndpoint (AVRISP_DATA_OUT_EPADDR,
                                               EP_TYPE_BULK, AVRISP_DATA_EPSIZE,
                                               1);

  /* Setup AVRISP Data IN endpoint if it is using a physically different endpoint */
  if ((AVRISP_DATA_IN_EPADDR & ENDPOINT_EPNUM_MASK)
      != (AVRISP_DATA_OUT_EPADDR & ENDPOINT_EPNUM_MASK))
    ConfigSuccess &= Endpoint_ConfigureEndpoint (AVRISP_DATA_IN_EPADDR,
                                                 EP_TYPE_BULK,
                                                 AVRISP_DATA_EPSIZE, 1);

  /* Indicate endpoint configuration success or failure */
  return ConfigSuccess;
}

/** Processes incoming V2 Protocol commands from the host, returning a response when required. */
void
AVRISP_Task (void)
{
  /* Device must be connected and configured for the task to run */
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  V2Params_UpdateParamValues ();

  Endpoint_SelectEndpoint (AVRISP_DATA_OUT_EPADDR);

  /* Check to see if a V2 Protocol command has been received */
  if (Endpoint_IsOUTReceived ())
    {
      LEDs_SetAllLEDs (LEDMASK_BUSY);

      /* Pass off processing of the V2 Protocol command to the V2 Protocol handler */
      V2Protocol_ProcessCommand ();

      LEDs_SetAllLEDs (LEDMASK_USB_READY);
    }
}


