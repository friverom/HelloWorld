/*
 * File:   newmain.c
 * Author: Federico
 *
 * Created on July 4, 2016, 4:37 PM
 */

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTRCCLK  // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 8000000
#define Counter_Data 4

unsigned char TMR_Counter; // Variable global para control del interrupt

/* Rutina de Interrupt */

void interrupt Timer_int(void)
{
    if (INTCONbits.T0IE && INTCONbits.T0IF) {
        --TMR_Counter;
        INTCONbits.T0IF=0;
        return;
    }
}
// process other interrupt sources here, if required

void main(void) {
    
    int i;
    unsigned char DATA,Timer;
    
    
   union ADC {
       int Result;
       struct{
           char LSB;
           char MSB;
       };
   } ADC_Result;  
   
   
   /* Inicializacion de Puertos */
    TRISC=0x00; // Puerto C salida
    TRISA=0x01; //AN0 como input
    ANSEL=0x01; //AN0 como analog Input
    OSCCON=0x71; // Internal Osc set to 8MHz
    OPTION_REG=0x07; //TMR0 Prescaler =256
    TMR0=0x01; //
    TMR_Counter=Counter_Data;
    
    
    INTCONbits.T0IF=0; //Interrupt Flag=0
    INTCONbits.T0IE=1; //Habilitar Interrupt TMR0
    INTCONbits.GIE=1;   //Habiltar Interrupt General
    
    ADCON0=0x81;    //Left Justified & Habilitar ADC
    ADCON1=0x50;    //Conversor Freq = Fosc/16
    
    DATA=0;
    PORTC=DATA;
    Timer=Counter_Data;
    
    while(1)
    {
        if(TMR_Counter==0){
            DATA++;
            PORTC=DATA;
            TMR_Counter=Timer;
        }
        
        ADCON0bits.CHS=0;   //Select AN0
        __delay_us(15);
        ADCON0bits.GO_DONE=1;   //Start ADC
        
        while(ADCON0bits.GO_DONE==1);
        ADC_Result.LSB=ADRESL;
        ADC_Result.MSB=ADRESH; 
        
        // ADC_Result.Result=8;
        
        Timer=Counter_Data+ADC_Result.MSB;
        
        
        
        
    }
    
    
    return;
}
