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
 *  \brief Board specific LED driver header for the AVRISP mk II with Serial.
 *  \copydetails Group_LEDs_UNO
 *
 *  \note This file should not be included directly. It is automatically included as needed by the LEDs driver
 *        dispatch header located in LUFA/Drivers/Board/LEDs.h.
 */

/** \ingroup Group_LEDs
 *  \brief Board specific LED driver header for the AVRISP mk II with Serial.
 *
 *  Board specific LED driver header for the AVRISP mk II with Serial.
 *
 *  <table>
 *    <tr><th>Name</th><th>Color</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
 *    <tr><td>LEDS_LED1</td><td>Yellow</td><td>RX</td><td>Low</td><td>PORTD.5</td></tr>
 *    <tr><td>LEDS_LED2</td><td>Yellow</td><td>TX</td><td>Low</td><td>PORTD.4</td></tr>
 *  </table>
 *
 *  @{
 */

#ifndef __LEDS_AVRISP_SERIAL_H__
#define __LEDS_AVRISP_SERIAL_H__

/* Includes: */
#include <LUFA/Common/Common.h>

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
  {
#endif

/* Preprocessor Checks: */
#if !defined(__INCLUDE_FROM_LEDS_H)
#error Do not include this file directly. Include LUFA/Drivers/Board/LEDS.h instead.
#endif

/* Public Interface - May be used in end-application: */

#define LEDS_NOT_RDY         (1 << 3)

#define LEDS_RDY             (1 << 2)

#define LEDS_ERR             (1 << 1)

#define LEDS_ACT             (1 << 0)

#define LEDS_RX              (1 << 5)

#define LEDS_TX              (1 << 4)

/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
#define LEDMASK_USB_NOTREADY      LEDS_NOT_RDY

/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
#define LEDMASK_USB_ENUMERATING  (LEDS_NOT_RDY | LEDS_ACT)

/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
#define LEDMASK_USB_READY         LEDS_RDY

/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
#define LEDMASK_USB_ERROR         LEDS_ERR

/** LED mask for the library LED driver, to indicate that the USB interface is busy. */
#define LEDMASK_BUSY             (LEDS_RDY | LEDS_ACT)

#define LEDMASK_RX                (LEDS_RX | LEDS_ACT)

#define LEDMASK_TX                (LEDS_TX | LEDS_ACT)

#define PORTF_MASK                (LEDS_NOT_RDY | LEDS_RDY | LEDS_ERR | LEDS_ACT)

#define PORTB_MASK                (LEDS_RX | LEDS_TX)

#define ALL_MASK                  (PORTF_MASK | PORTB_MASK)

#define PORTF_ALL_PORTS           (PORTF_MASK << 4)

#define PORTB_ALL_PORTS           PORTB_MASK

/* Inline Functions: */
#if !defined(__DOXYGEN__)
static inline void
LEDs_Init (void)
{
  DDRF |= PORTF_ALL_PORTS;
  PORTF &= ~PORTF_ALL_PORTS;
  DDRB |= PORTB_ALL_PORTS;
  PORTB &= ~PORTB_ALL_PORTS;
}

static inline void
LEDs_Disable (void)
{
  DDRF &= ~PORTF_ALL_PORTS;
  PORTF &= ~PORTF_ALL_PORTS;
  DDRB &= ~PORTB_ALL_PORTS;
  PORTB &= ~PORTB_ALL_PORTS;
}

static inline void
LEDs_TurnOnLEDs (const uint8_t LEDMask)
{
  PORTF |= ((LEDMask & PORTF_MASK) << 4);
  PORTB |= LEDMask & PORTB_MASK;
}

static inline void
LEDs_TurnOffLEDs (const uint8_t LEDMask)
{
  PORTF &= ~((LEDMask & PORTF_MASK) << 4);
  PORTB &= ~(LEDMask & PORTB_MASK);
}

static inline void
LEDs_SetAllLEDs (const uint8_t LEDMask)
{
  PORTF &= ~PORTF_ALL_PORTS;
  PORTF |= ((LEDMask & PORTF_MASK) << 4);
  PORTB &= ~PORTB_ALL_PORTS;
  PORTB |= LEDMask & PORTB_MASK;
}

static inline void
LEDs_ChangeLEDs (const uint8_t LEDMask, const uint8_t ActiveMask)
{
  PORTF &= ~((LEDMask & PORTF_MASK) << 4);
  PORTF |= ((ActiveMask & PORTF_MASK) << 4);
  PORTB &= ~(LEDMask & PORTB_MASK);
  PORTB |= ActiveMask & PORTB_MASK;
}

static inline void
LEDs_ToggleLEDs (const uint8_t LEDMask)
{
  PINF = (LEDMask & PORTF_MASK) << 4;
  PINB = LEDMask & PORTB_MASK;
}

static inline uint8_t
LEDs_GetLEDs (void)
ATTR_WARN_UNUSED_RESULT;
static inline uint8_t
LEDs_GetLEDs (void)
{
//				return (~PORTD & LEDS_ALL_LEDS);
  return 0;
}
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif

/** @} */

