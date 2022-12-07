/*
 * File:   newmain.c
 * Author: Nicolas
 *
 * Created on 25 de Novembro de 2022, 20:57
 */

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = XT_XT // Oscillator Selection bits (Internal oscillator, XT used by USB (INTXT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#include "lcd.h"
#include <stdio.h>

#define _XTAL_FREQ 4000000

int fi;  // flag de inicio de jogo
int fix; // flag auxiliar
int T;   // timer

void putch(char data)
{
  escreve_lcd(data);   
}

void main(void) {
    // Inicializa��o
    fi = 0;
    fix = 0;
    T = 0;
    
    TRISC = 0;  
    PORTC = 0;
    TRISD = 0;  
    PORTD = 0;
    
    INTCONbits.INT0IE = 1;    // Habilita interrup��o INT0
    INTCON2bits.INTEDG0 = 0;  // Ativar na descida de voltagem
    INTCON3bits.INT1IE = 1;   // Habilita interrup��o INT1
    INTCON2bits.INTEDG1 = 0;  // Ativar na descida de voltagem
    INTCON3bits.INT2IE = 1;   // Habilita interrup��o INT2
    INTCON2bits.INTEDG2 = 0;  // Ativar na descida de voltagem
    
    INTCONbits.GIE = 1;       // Habilita interrup��es globalmente
    INTCONbits.PEIE = 1;      // Habilita int. dos perif�ricos
    TMR2IE = 1;               // Ativa interrup��o do timer 2
    
    T2CONbits.T2CKPS = 1;     // PRESCALER
    T2CONbits.TOUTPS = 0;     // POSTSCALER
    PR2 = 250;                // Valor do estouro do timer
    
    // De acordo com o site http://eng-serve.com/pic/pic_timer.html com essas 
    // Configura��es cada interrup��o do timer � aproximadamente 1ms
    
    inicializa_lcd();
    limpa_lcd();
    
    caracter_inicio(1,5);  
    printf("pronto");  
    
    while(1) {
        
        if(fix){ // Liga o buzzer e desliga
        fix = 0;
        PORTCbits.RC0 = 1;
        __delay_ms(500);
        PORTCbits.RC0 = 0;
        }
    }
}

void __interrupt() isr(void){
    
    if (INTCONbits.INT0F && !fi) { // Bot�o do mediador ele inicia o modo de jogo e come�a o timer
        T = 0;
        fi = 1;
        fix = 1;
        INTCON3bits.INT2F = 0;
        INTCON3bits.INT1F = 0;
        PORTCbits.RC1 = 0;
        PORTCbits.RC2 = 0;
        INTCONbits.INT0F = 0;
        T2CONbits.TMR2ON = 1;
        limpa_lcd();
        caracter_inicio(1,4);  
        printf("comecem!"); 
    } else if (INTCON3bits.INT1F && fi) { // Bot�o do primeiro jogador 
        T2CONbits.TMR2ON = 0; // Para o timer
        INTCONbits.INT0F = 0; // Reseta as flags
        fi = 0;
        INTCON3bits.INT1F = 0;
        PORTCbits.RC1 = 1;    // Acende o led
        caracter_inicio(1,3);  
        printf("jogador 1!!"); 
        caracter_inicio(2,0);
        printf("%d ms",T);
    } else if (INTCON3bits.INT2F && fi) { // Bot�o do segundo jogador 
        T2CONbits.TMR2ON = 0; // Para o timer
        INTCONbits.INT0F = 0; // Reseta as flags
        fi = 0;
        INTCON3bits.INT2F = 0;
        PORTCbits.RC2 = 1;    // Acende o led
        caracter_inicio(1,3);  
        printf("jogador 2!!"); 
        caracter_inicio(2,0);
        printf("%d ms",T);
    } else if (TMR2IF){ // Conta o tempo em ms
        TMR2IF = 0;
        T++; 
   } 
}