/* Yaesu FT-x90R LCD replacement, based on a 16(L)F1937 PIC and a 6 digit
   14 segment display. Based on the F1 development board source code.
   X-IDE 
   Created on 16 July 2015, 3:55 PM

   TODO:    The PCB includes an RC filter and a trimpot. This could be used to
            generate CTCSS. Which leads me to
   ERRATUM: RB1 is the CTCSS pin. It should be RB3, to use the PWM function.
            See http://members.optusnet.com.au/vk3biz/ctcss-tones.pdf
            So to generate CTCSS I suspect RB1 and RB3 have to be swapped
            in hardware and software. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// See datasheet page 62
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
// MCLR/VPP is RE3
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2=M2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
// 16F1937 has VCAPEN
//#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include "lcd.h"

#define HARDWARE_VER_1_0

/* See pic16f1937.h for register names
   RB6 is B.6 */
#ifdef HARDWARE_VER_1_0
#define LED1 RB1
#define STD RB2
#define CE  RB3
#define IN0 RC0
#define IN1 RC1
#define IN2 RD1
#define IN3 RD2
#endif

// From front of FT-290R, leftmost connection is green
// R40 STD CE FC Vss R41 R42 R43 Vdd

void lcd_init(void)
{
  // Configure LCDPS
  // Wave form type A or B
  // Bias Mode
  // Prescaler
  // 1:16 - 1:1
  LCDPS = 0xa0; // WFT = 1 (B type), LCDA = 1, Prescaler 0 (1:1) 
  //WFT = 1;        // B type
  //WFT=0; // A
  //LCDA=1;
  //LCDPSbits.LP = 0x0f;

  // SEG13 and SEG14 are shared by the programming pins
  // SEG13 6A 6B 6J 6K
  // SEG14 6N 6P 6d 6E
  LCDSE0 = 0xff; // SE<7..0>
  LCDSE1 = 0xff; // SE<15..8> // 0x9f
  LCDSE2 = 0xff; // SE<23..16>

  // Configure LCDCON
  // LCD enbled
  // LCD enabled in sleep
  // LCD Clock Select - T1OSC
  // LCD Commons - 1/4
  LCDCON = 0x0b; // LCDEN = 0 for now, SLPEN = 0, CS = 10, LMUX = 11
  //SLPEN = 0;
  //WERR = 0;
  //LCDCONbits.CS = 2; // LFINTOSC
  //LCDCONbits.LMUX = 3; // 1/4 -- all 4 commons, 96 segments

  // clear ALL SEGMENT DATA
  LCDDATA0  = 0; //SEG<7..0> COM0
  LCDDATA1  = 0; //SEG<15..8> COM0
  LCDDATA2  = 0; //SEG<23..16> COM0
  LCDDATA3  = 0; //SEG<7..0> COM1
  LCDDATA4  = 0; //SEG<15..8> COM1
  LCDDATA5  = 0; //SEG<23..16> COM1
  LCDDATA6  = 0; //SEG<7..0> COM2
  LCDDATA7  = 0; //SEG<15..8> COM2
  LCDDATA8  = 0; //SEG<23..16> COM2
  LCDDATA9  = 0; //SEG<7..0> COM3
  LCDDATA10 = 0; //SEG<15..8> COM3
  LCDDATA11 = 0; //SEG<23..16> COM3

  LCDIE = 0; //  LCD Interrupt Enable in PIR2
  LCDIF = 0; // LCD Interrupt Flag in PIR in PIE2

  // Reference ladder control
  LCDRL = 0x30; // High power B

  // Configure LCDREF
  // Internal reference enabled
  // Internal Reference Source
  // Internal Reference always on
  LCDREF = 0x80; 
  //LCDIRE = 1; Internal Reference Enable
  //LCDIRS = 0; Powered by VDD
  //LCDIRI = 0;

  LCDCST = 0;     // maximum contrast
  LCDEN = 1;
}

void LCD(int digit, int value)
{
  short A=0, B=0, C=0, D=0, E=0, F=0, G=0, M=0, H=0, J=0, K=0, P=0, Q=0, N=0, DP=0;
  
  // i is 0x3x or 0xbx if decimal point is set
  if (value & 0x80)
  {
    DP=1;
  }
  value = value & 0x7f;
  switch(value)
  {
    case 0x00: // Turn all off -- test
      break;
    case 0x20:
      // Space -- all off
      break;
    case '0': 
      A=B=C=D=E=F=K=Q=1;
      break;
    case '1':
      B=C=1;
      break;
    case '2':
      A=B=G=M=E=D=1;
      break;
    case '3':
      A=B=C=D=G=M=1;
      break;
    case '4':
      B=C=F=G=M=1;
      break;
    case '5':
      A=C=D=F=G=M=1;
      break;
    case '6':
      A=C=D=E=F=G=M=1;
      break;
    case '7':
      A=B=C=1;
      break;
    case '8':
      A=B=C=D=E=F=G=M=1;
      break;
    case '9':
      A=B=C=D=F=G=M=1;
      break;
    case 0x3a:
    case 'A':
      A=B=C=E=F=G=M=1;
      break;
    case 0x3b:
    case 'B':
      C=D=E=F=G=M=1;
      break;
    case 0x3c:
    case 'C':
      A=D=E=F=1;
      break;
    case 0x3d:
    case 'D':
      B=C=D=E=G=M=1;
      break;
    case 0x3e:
    case 'E':
      A=D=E=F=G=M=1;
      break;
    case 0x3f:
    case 'F':
      A=E=F=G=M=1;
      break;
    // TODO: the rest of the alphabet
    case 'K':
      E=F=G=K=N=1;
      break;
    case 'P':
    case 0x10:
      A=B=E=F=G=M=1;
      break;
      
    case 'S':
      A=C=D=F=G=M=1;
      break;
    case 'Z':
      A=D=K=Q=1;
      break;
    case 'c':
      D=E=G=M=1;
      break;
    case 'm':
      B=F=H=K=1;
      break;      
    case 0x7f: // Turn all on -- test
      A=B=C=D=E=F=G=M=H=J=K=P=Q=N=DP=1;
      break;

    case 0x40:
      G=M=H=J=K=N=Q=P=N=1;
      break;
    default: // If we don't know the character, display an asterisk
      if (value&0x40) A=1;
      if (value&0x20) B=1;
      if (value&0x10) C=1;
      if (value&0x08) D=1;
      if (value&0x04) E=1;
      if (value&0x02) F=1;
      if (value&0x01) G=M=1;
      
  }

  switch(digit)
  {
    case 1:
      while(!WA);
      A1=A; B1=B; C1=C; D1=D; E1=E; F1=F; G1=G; H1=H;
      J1=J; K1=K; M1=M; P1=P; Q1=Q; N1=N; DP1=DP;
      break;
    case 2:
      while(!WA);
      A2=A; B2=B; C2=C; D2=D; E2=E; F2=F; G2=G; H2=H;
      J2=J; K2=K; M2=M; P2=P; Q2=Q; N2=N; DP2=DP;
      break;
    case 3:
      while(!WA);
      A3=A; B3=B; C3=C; D3=D; E3=E; F3=F; G3=G; H3=H;
      J3=J; K3=K; M3=M; P3=P; Q3=Q; N3=N; DP3=DP;
      break;
    case 4:
      while(!WA);
      A4=A; B4=B; C4=C; D4=D; E4=E; F4=F; G4=G; H4=H;
      J4=J; K4=K; M4=M; P4=P; Q4=Q; N4=N; DP4=DP;
      break;
    case 5:
      while(!WA);
      A5=A; B5=B; C5=C; D5=D; E5=E; F5=F; G5=G; H5=H;
      J5=J; K5=K; M5=M; P5=P; Q5=Q; N5=N;
      break;
    case 6:
      while(!WA);
      A6=A; B6=B; C6=C; D6=D; E6=E; F6=F; G6=G; H6=H;
      J6=J; K6=K; M6=M; P6=P; Q6=Q; N6=N;
      break;
  }
}


/*
Reverse Engineering 2015-07-10

Two clock pulses clock in two nibbles, six times

5.000.2 00110010 10110000 00110000 00110000 10110101 00100000
5.000.3 00110011 10110000 00110000 00110000 10110101 00100000
5.000.4 00110100 10110000 00110000 00110000 10110101 00100000
5.000.5 00110101 10110000 00110000 00110000 10110101 00100000
5.000.6 00110110 10110000 00110000 00110000 10110101 00100000

M                                                    00100001
-                                                    00100010
CLAR                                                 00100100
 
It's gorram ASCII
*/

/* These have to be static but not really global */
int index=0;
char disp[12]={0,0,0,0,0,0,0,0,0,0,0,0}; // 4 bits only

void interrupt isr()
{
  static int i=0;
  static int toggle = 0;
  
  PIR1=PIR2=PIR3=0;

  if (IOCIF) // STD or CE changed
  {
    if (IOCBF3==1) // CE
    {
      // Rising edge - get ready)
      if (CE == 1)
      {
        index=0;
      }
      else // falling edge - display what we got
      {
        /* The glass is your oyster */
        LCD(1,disp[8]<<4 | disp[9]);
        LCD(2,disp[6]<<4 | disp[7]);
        LCD(3,disp[4]<<4 | disp[5]);
        LCD(4,disp[2]<<4 | disp[3]);
        LCD(5,disp[0]<<4 | disp[1]);
        

        // disp[10] has CLAR - and M bits
        // We have one digit left and four bars

        if (disp[11] & 0x02) // - or F
        {
          LCD(6,'F');
        }
        else
        {
          LCD(6,0);
          if (disp[11] & 0x04) // CLAR
          {
            LCD(6,'c');
          }
          if (disp[11] & 0x01) // M
          {
            LCD(6,'m');
          }
        }

        if (disp[11] & 0x08) // not used
        {
          while(!WA);
          BAR0=1;
        } else {
          while(!WA);
          BAR0=0;
        }
      }
    }
    if (IOCBF2==1) // STD
    {
      if (index < 12)
      {
        disp[index] = (IN3<<3) |
                      (IN2<<2) |
                      (IN1<<1) |
                       IN0;
      }
      index++;
    }

  }
  IOCBF=0; // clear IOCIF via IOCBF
}

int main(int argc, char** argv)
{
  volatile long i;   
  volatile long j,k;
  
  int toggle=0;

  // peripheral configuration
  PORTC = 0; 
  PORTD = 0;
  PORTE = 0;

  TRISA=0xff; // inputs
  TRISB=0xff;
  TRISC=0xff;
  TRISD=0xff;
  TRISE=0xff;

  ANSELA=0; /* no analogue in */
  ANSELB=0;
  
  ANSELD=0;
  ANSELE=0;
  
  /* Interrupts */
  INTCON=0;
  PIE1=PIE2=PIE3=0;

  LCDIE = 1; // LCD interrupt
  PEIE = 1; // peripheral interrupt
  GIE = 0; // global interrupt
  
  /* Need for speed */
  /*
   * OSCCON : SPLLEN IRCF<3:0> - SCS<1:0>
   * SPLLEN = 1 Software PLL 4x on
   * IRCF = 1110 8 MHz or 32 MHz
   * SCS = 0 Clock determined by FOSC in config word 1
   * /
  /* FOSC<2:0>=100=INTOSC
   * SCS=00
   * IRCF=1110
   * SPLLEN = 1 */
  //FOSC=INTOSC with pragma above
  OSCCON=0xf0; /* SPPLEN IRCF - SCS */
  
  // From radio
  // make B2 and B3 inputs
  TRISB2=1;
  TRISB3=1;
  // enable falling edge on STD = RB2
  IOCBN2=1;
  IOCBP2=0;
  // enable rising edge on CE = RB3
  IOCBN3=1; // on falling update display
  IOCBP3=1;
      
  // LED output
  TRISB1=0;
  LED1=0;
  
  // Make RE3 an input with weak pullup
  TRISE3 = 1;
  WPUE3 = 1;
  
  lcd_init();
  
  // If you want a startup message, add it here
  LCD(1,'Z');
  LCD(2,'S');
  LCD(3,'1');
  LCD(4,'K');
  LCD(5,'E');
  
  
  // If RE3 is 0, enter test mode
  if (/*RE3 == */0)
  {
    i=0;
    j=0;
    k=0;
    while(1)
    {
      i++;
      if (i>500000L)
      {
        toggle=1-toggle;
        while(!WA);
        if (toggle)
        {
          LCDDATA0=LCDDATA1=LCDDATA2=LCDDATA3=LCDDATA4=LCDDATA5=
          LCDDATA6=LCDDATA7=LCDDATA8=LCDDATA9=LCDDATA10=
          LCDDATA11=0xff;
        }
        else
        {
          LCDDATA0=LCDDATA1=LCDDATA2=LCDDATA3=LCDDATA4=LCDDATA5=
          LCDDATA6=LCDDATA7=LCDDATA8=LCDDATA9=LCDDATA10=
          LCDDATA11=0;
        }

        i=0;

        //LED1=RE3; //toggle;
        LED1=toggle;
      }
    }
  } else {
    // RE3 == 1
    
    // enable interrupts
    IOCIE=1; // Interrupt on change (port B)
    GIE=1;  // Global int on
    // Wait here, interrupt does the work
    while(1)
    {
      i++;
      if (i>50000L)
      {
        toggle=1-toggle;
        LED1=toggle;
        i=0;
      }
    }
  }
  // Can't happen
  return (EXIT_SUCCESS);
}

