// lcd.h

/*********************************************************************
* Software License Agreement:
h*
* The software supplied herewith by Microchip Technology Incorporated
* (the "Company") for its PICmicro® Microcontroller is intended and
* supplied to you, the Company's customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*********************************************************************/
#ifndef __LCD_H
#define __LCD_H
/*
#include "GenericTypeDefs.h"

typedef union
{
    UINT16    val;
    struct
    {
        unsigned    digit0 : 4;
        unsigned    digit1 : 4;
        unsigned    digit2 : 4;
        unsigned    digit3 : 4;
    };
} BCD_TYPE;

void    lcd_init(void);
BOOL    lcd_display_digits(BCD_TYPE);
void    lcd_display_on(void);
void    lcd_display_off(void);
*/


// LCD name LCD pin PIC pin         
// LCDSEG0 1 SEG9 1N 1P 1D 1E
#define N1 SEG9COM3
#define P1 SEG9COM2
#define D1 SEG9COM1
#define E1 SEG9COM0
// LCDSEG1 2 SEG10 1DP 1COLON 1C 1M
#define DP1 SEG10COM3
#define COLON1 SEG10COM2
#define C1 SEG10COM1
#define M1 SEG10COM0
// LCDSEG2 3 SEG11 2N 2P 2D 2E
#define N2 SEG11COM3
#define P2 SEG11COM2
#define D2 SEG11COM1
#define E2 SEG11COM0
// LCDSEG3 4 SEG16 2DP 2COLON 2C 2M
#define DP2 SEG16COM3
#define COLON2 SEG16COM2
#define C2 SEG16COM1
#define M2 SEG16COM0
// LCDSEG4 5 SEG6 3N 3P 3D 3E
#define N3 SEG6COM3
#define P3 SEG6COM2
#define D3 SEG6COM1
#define E3 SEG6COM0
// LCDSEG5 6 SEG3 3DP 3COLON 3C 3M
#define DP3 SEG3COM3
#define COLON3 SEG3COM2
#define C3 SEG3COM1
#define M3 SEG3COM0
// LCDSEG6 7 SEG1 4N 4P 4D 4E
#define N4 SEG1COM3
#define P4 SEG1COM2
#define D4 SEG1COM1
#define E4 SEG1COM0
// LCDSEG7 8 SEG2 4DP 4COLON 4C 4M
#define DP4 SEG2COM3
#define COLON4 SEG2COM2
#define C4 SEG2COM1
#define M4 SEG2COM0
// LCDSEG8 9 SEG23 5N 5P 5D 5E
#define N5 SEG23COM3
#define P5 SEG23COM2
#define D5 SEG23COM1
#define E5 SEG23COM0
// LCDSEG9 10 SEG22 BAR2 BAR3 5C 5M
#define BAR2 SEG22COM3
#define BAR3 SEG22COM2
#define C5 SEG22COM1
#define M5 SEG22COM0
// LCDSEG10 11 SEG14 6N 6P 6D 6E via resistor
#define N6 SEG14COM3
#define P6 SEG14COM2
#define D6 SEG14COM1
#define E6 SEG14COM0
// LCDSEG11 12 SEG21 BAR0 BAR1 6C 6M
#define BAR0 SEG21COM3
#define BAR1 SEG21COM2
#define C6 SEG21COM1
#define M6 SEG21COM0
// LCDSEG12 17 SEG13 6J 6K 6A 6B via resistor
#define J6 SEG13COM3
#define K6 SEG13COM2
#define A6 SEG13COM1
#define B6 SEG13COM0
// LCDSEG13 18 SEG5 6H 6Q 6F 6G
#define H6 SEG5COM3
#define Q6 SEG5COM2
#define F6 SEG5COM1
#define G6 SEG5COM0
// LCDSEG14 19 SEG4 5J 5K 5A 5B
#define J5 SEG4COM3
#define K5 SEG4COM2
#define A5 SEG4COM1
#define B5 SEG4COM0
// LCDSEG15 20 SEG7 5H 5Q 5F 5G
#define H5 SEG7COM3
#define Q5 SEG7COM2
#define F5 SEG7COM1
#define G5 SEG7COM0
// LCDSEG16 21 SEG12 4J 4K 4A 4B
#define J4 SEG12COM3
#define K4 SEG12COM2
#define A4 SEG12COM1
#define B4 SEG12COM0
// LCDSEG17 22 SEG15 4H 4Q 4F 4G
#define H4 SEG15COM3
#define Q4 SEG15COM2
#define F4 SEG15COM1
#define G4 SEG15COM0
// LCDSEG18 23 SEG0 3J 3K 3A 3B
#define J3 SEG0COM3
#define K3 SEG0COM2
#define A3 SEG0COM1
#define B3 SEG0COM0
// LCDSEG19 24 SEG20 3H 3Q 3F 3G
#define H3 SEG20COM3
#define Q3 SEG20COM2
#define F3 SEG20COM1
#define G3 SEG20COM0
// LCDSEG20 25 SEG19 2J 2K 2A 2B
#define J2 SEG19COM3
#define K2 SEG19COM2
#define A2 SEG19COM1
#define B2 SEG19COM0
// LCDSEG21 26 SEG18 2H 2Q 2F 2G
#define H2 SEG18COM3
#define Q2 SEG18COM2
#define F2 SEG18COM1
#define G2 SEG18COM0
// LCDSEG22 27 SEG17 1J 1K 1A 1B
#define J1 SEG17COM3
#define K1 SEG17COM2
#define A1 SEG17COM1
#define B1 SEG17COM0
// LCDSEG23 28 SEG8 1H 1Q 1F 1G
#define H1 SEG8COM3
#define Q1 SEG8COM2
#define F1 SEG8COM1
#define G1 SEG8COM0

#endif // __LCD_H
