

/**
 * Proyecto TivaWare
 * Electrónica Digital 2
 * Gerardo Paz Fuentes 20173
 *
 * Fecha de creación: 24/11/2022
 */

/* ------ LIBRERÍAS ------ */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"

/* ------ CONSTANTES Y VARIABLES ------ */

#define XTAL 16000000
#define STATE1 0
#define STATE2 4
#define STATE3 8
#define STATE4 12

#define button1 64
#define button2 128

uint8_t charIn;
char seg7_char[], seg7_timer_char[]; // para enviar datos por UART
uint8_t led1 = 0, led2 = 0, led3 = 0, led4 = 0, seg7 = 0, state = 0, cont = 0, seg7_timer = 0;
uint8_t animacion1 = 46, animacion2 = 16;
uint32_t ui32Period, PeriodB; // TMR0 y TMR1
uint32_t valADC; // Potenciómetro
uint8_t mapADC, decenas = 0, unidades = 0; // ADC conversiones

/* ------ PROTOTIPOS DE FUNCIÓN ------ */

void config(void);
void TMR0_config(void);
void TMR1_config(void);
void ADC_config(void);
void InitGPIOs(void);
void configUART(void);
void buttonHandler(void);
void UART0ReadIntHandler(void);
void Timer0IntHandler(void);
void Timer1IntHandler(void);

void imprimir(uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4);
unsigned short map(uint32_t val, uint32_t in_min, uint32_t in_max, unsigned short out_min, unsigned short out_max);

/* ------ CÓDIGO PRINCIPAL ------ */

int main (void){
    config();

    while(1){
        state = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3); // estado DIPs

        if(state == STATE1){ // Estado 1
            switch(charIn){
                case '1':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0000110);
                    break;
                case '2':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1011011);
                    break;
                case '3':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1001111);
                    break;
                case '4':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1100110);
                    break;
                case '5':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1101101);
                    break;
                case '6':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111101);
                    break;
                case '7':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0000111);
                    break;
                case '8':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111111);
                    break;
                case '9':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1101111);
                    break;
                case '0':
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0111111);
                    break;
                case 'q':
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, led1);
                    break;
                case 'w':
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, led2);
                    break;
                case 'e':
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, led3);
                    break;
                case 'r':
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, led4);
                default:
                    break;
            }

        }
        else if(state == STATE2){ // Estado 2
            // Mostrar contador, solo se enciende el pin que es
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, cont);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, cont);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, cont);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, cont);

            switch(seg7_timer){
                case 0:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0111111);
                    break;
                case 1:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0000110);
                    break;
                case 2:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1011011);
                    break;
                case 3:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1001111);
                    break;
                case 4:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1100110);
                    break;
                case 5:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1101101);
                    break;
                case 6:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111101);
                    break;
                case 7:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0000111);
                    break;
                case 8:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111111);
                    break;
                case 9:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1101111);
                    break;
                case 10:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1110111);
                    break;
                case 11:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111100);
                    break;
                case 12:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1011000);
                    break;
                case 13:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1011110);
                    break;
                case 14:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111011);
                    break;
                case 15:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1110001);
                    break;
            }
        }
        else if(state ==STATE3){ // Estado 3
            switch(animacion1){
                case 46:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000001);
                    break;
                case 37:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000010);
                    break;
                case 29:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01000000);
                    break;
                case 22:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00010000);
                    break;
                case 16:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00001000);
                    break;
                case 11:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000100);
                    break;
                case 7:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01000000);
                    break;
                case 4:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00100000);
                    break;
                case 2:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b10000000);
                    break;
            }

            switch(animacion2){
                case 16:
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0x00);
                    break;
                case 8:
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0x00);
                    break;
                case 4:
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0xFF);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0x00);
                    break;
                case 2:
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
                    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0xFF);
                    break;
            }
        }
        else if(state == STATE4){ // Estado 4
            ADCProcessorTrigger(ADC0_BASE, 3); // iniciar conversión
            while(!ADCIntStatus(ADC0_BASE, 3, false)){} // esperar a terminar la conversión
            ADCIntClear(ADC0_BASE, 3);  // limpiar interrupción
            ADCSequenceDataGet(ADC0_BASE, 3, &valADC);

            mapADC = map(valADC, 0, 4096, 0, 100);  // mapeo de valores

            decenas = mapADC/10;
            unidades = mapADC%10;



            switch(unidades){
                case 1:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0000110);
                    break;
                case 2:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1011011);
                    break;
                case 3:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1001111);
                    break;
                case 4:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1100110);
                    break;
                case 5:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1101101);
                    break;
                case 6:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111101);
                    break;
                case 7:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0000111);
                    break;
                case 8:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1111111);
                    break;
                case 9:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b1101111);
                    break;
                case 0:
                    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b0111111);
                    break;
                default:
                    break;
            }

            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, decenas);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, decenas);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, decenas);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, decenas);

        }

    }
}


/* ------ CONFIGURACIONES ------ */

void config(void){

    // Oscilador
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    IntPrioritySet(INT_GPIOA, 0); // prioridad de interrupción puerto A (para UART)

    InitGPIOs();
    configUART();
    TMR0_config();
    TMR1_config();
    ADC_config();

    // Interrupciones gobales
    IntMasterEnable();

}

void InitGPIOs(void){
    // Habilitar puertos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Leds (out)
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // 7 segmentos (out)
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);

    // pushbuttons (in)
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // Weak pull up / corriente baja 2mA

    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_6|GPIO_INT_PIN_7); // interrupción de cada botón
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_INT_PIN_6|GPIO_INT_PIN_7, GPIO_LOW_LEVEL); // disparo de bajo nivel
    IntRegister(INT_GPIOA, buttonHandler); // llamada función de interrupción botones en PORTA
    IntEnable(INT_GPIOA); // Interrupción botones

    // DIPs (in)
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD); // corriente baja 2mA / push pull


}

void buttonHandler(void){
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_6|GPIO_INT_PIN_7); // limpiar interrupciónes
    int bot = GPIOIntStatus(GPIO_PORTA_BASE, true);

    if(state == STATE1){
        switch(bot){
            case button1:
                if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0){
                    while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0){} // Antirrebotes

                    imprimir(led1, led2, led3, led4);

                    UARTCharPut(UART0_BASE, '7');
                    UARTCharPut(UART0_BASE, 'S');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'G');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, seg7_char[0]);
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }
                break;
            case button2:
                    if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0){
                        while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0){} // Antirrebotes

                        imprimir(led1, led2, led3, led4);

                        UARTCharPut(UART0_BASE, '7');
                        UARTCharPut(UART0_BASE, 'S');
                        UARTCharPut(UART0_BASE, 'E');
                        UARTCharPut(UART0_BASE, 'G');
                        UARTCharPut(UART0_BASE, ':');
                        UARTCharPut(UART0_BASE, ' ');
                        UARTCharPut(UART0_BASE, seg7_char[0]);
                        UARTCharPut(UART0_BASE, ' ');
                        UARTCharPut(UART0_BASE, 10);
                        UARTCharPut(UART0_BASE, 13);
                    }
                    break;
            }
    }
    else if(state == STATE2){
        switch(bot){
            case button1:
                if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0){
                    while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0){} // Antirrebotes
                    if(cont > 0)
                        cont--;
                    else
                        cont = 15;
                }
                break;
            case button2:
                if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0){
                    while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0){} // Antirrebotes
                    if(cont<15)
                        cont++;
                    else
                        cont = 0;
                }
                break;

        }
        led1 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0);
    }

}

void configUART(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTStdioConfig(0, 9600, 16000000);

    GPIOPinConfigure(GPIO_PA0_U0RX); // Puertos
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // Pines UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Interrupción
    IntEnable(INT_UART0);

    UARTFIFODisable(UART0_BASE);
    UARTIntEnable(UART0_BASE, UART_INT_RX);
}

// Interrupción UART
void UART0ReadIntHandler(void) {
    UARTIntClear(UART0_BASE, UART_INT_RX);  // limpiar interrupción

    charIn = UARTCharGet(UART0_BASE); // guardar CHAR

    UARTCharPut(UART0_BASE, charIn);
    UARTCharPut(UART0_BASE, 10);
    UARTCharPut(UART0_BASE, 13);

    switch(charIn){
        case '1':
            seg7 = 1;
            seg7_char[0] = '1';
            break;
        case '2':
            seg7 = 2;
            seg7_char[0] = '2';
            break;
        case '3':
            seg7 = 3;
            seg7_char[0] = '3';
            break;
        case '4':
            seg7 = 4;
            seg7_char[0] = '4';
            break;
        case '5':
            seg7 = 5;
            seg7_char[0] = '5';
            break;
        case '6':
            seg7 = 6;
            seg7_char[0] = '6';
            break;
        case '7':
            seg7 = 7;
            seg7_char[0] = '7';
            break;
        case '8':
            seg7 = 8;
            seg7_char[0] = '8';
            break;
        case '9':
            seg7 = 9;
            seg7_char[0] = '9';
            break;
        case '0':
            seg7 = 0;
            seg7_char[0] = '0';
            break;
        case 'q':
            led1 = ~led1;
            break;
        case 'w':
            led2 = ~led2;
            break;
        case 'e':
            led3 = ~led3;
            break;
        case 'r':
            led4 = ~led4;
            break;
        default:
            charIn = 'x';
            break;
    }
}

void TMR1_config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // habilitar TMR1
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    PeriodB = (SysCtlClockGet() / 10);
    TimerLoadSet(TIMER1_BASE, TIMER_A, PeriodB - 1);

    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer1IntHandler(void){
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // limpiar interrupción
    if(state == STATE3){
        animacion1--;
        animacion2--;

        if(animacion1 == 0)
            animacion1 = 46;
        if(animacion2 == 0)
            animacion2 = 16;
    }

}

void TMR0_config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // se habilita timer0
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ui32Period = (SysCtlClockGet()) / 4; // 0.25s
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    IntEnable(INT_TIMER0A); // interrupción TMR0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A); // Habilitar TMR0A
}

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // limpiar interrupción
    if(state == STATE2){
        if(seg7_timer<15)
            seg7_timer++;
        else
            seg7_timer = 0;

        switch(seg7_timer){
            case 0:
                seg7_timer_char[0] = '0';
                break;
            case 1:
                seg7_timer_char[0] = '1';
                break;
            case 2:
                seg7_timer_char[0] = '2';
                break;
            case 3:
                seg7_timer_char[0] = '3';
                break;
            case 4:
                seg7_timer_char[0] = '4';
                break;
            case 5:
                seg7_timer_char[0] = '5';
                break;
            case 6:
                seg7_timer_char[0] = '6';
                break;
            case 7:
                seg7_timer_char[0] = '7';
                break;
            case 8:
                seg7_timer_char[0] = '8';
                break;
            case 9:
                seg7_timer_char[0] = '9';
                break;
            case 10:
                seg7_timer_char[0] = 'a';
                break;
            case 11:
                seg7_timer_char[0] = 'b';
                break;
            case 12:
                seg7_timer_char[0] = 'c';
                break;
            case 13:
                seg7_timer_char[0] = 'd';
                break;
            case 14:
                seg7_timer_char[0] = 'e';
                break;
            case 15:
                seg7_timer_char[0] = 'f';
                break;
            }

        switch(cont){
            case 0:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '0');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 1:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 2:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '2');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 3:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '3');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 4:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '4');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 5:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '5');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 6:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '6');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 7:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '7');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 8:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '8');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 9:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '9');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 10:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, '0');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 11:
                seg7_timer_char[0] = 'b';
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 12:
                seg7_timer_char[0] = 'c';
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, '2');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 13:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, '3');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 14:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, '4');
                UARTCharPut(UART0_BASE, ' ');
                break;
            case 15:
                UARTCharPut(UART0_BASE, 'C');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, '1');
                UARTCharPut(UART0_BASE, '5');
                UARTCharPut(UART0_BASE, ' ');
                break;
            }

        // envío de datos con UART

        UARTCharPut(UART0_BASE, '7');
        UARTCharPut(UART0_BASE, 'S');
        UARTCharPut(UART0_BASE, 'E');
        UARTCharPut(UART0_BASE, 'G');
        UARTCharPut(UART0_BASE, ':');
        UARTCharPut(UART0_BASE, ' ');
        UARTCharPut(UART0_BASE, seg7_timer_char[0]);
        UARTCharPut(UART0_BASE, ' ');
        UARTCharPut(UART0_BASE, 10);
        UARTCharPut(UART0_BASE, 13);

    }

}

void ADC_config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // habilitar ADC
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // entrada analógica
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);  // limpiar interrupción

    IntEnable(INT_GPIOE);
}

void imprimir(uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4){
    UARTCharPut(UART0_BASE, 'L');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, '1');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    if(l1 == 0xFF)
        UARTCharPut(UART0_BASE, '1');
    else if(l1 == 0)
        UARTCharPut(UART0_BASE, '0');
    UARTCharPut(UART0_BASE, ' ');

    UARTCharPut(UART0_BASE, 'L');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, '2');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    if(l2 == 0xFF)
        UARTCharPut(UART0_BASE, '1');
    else if(l2 == 0)
        UARTCharPut(UART0_BASE, '0');
    UARTCharPut(UART0_BASE, ' ');

    UARTCharPut(UART0_BASE, 'L');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, '3');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    if(l3 == 0xFF)
        UARTCharPut(UART0_BASE, '1');
    else if(l3 == 0)
        UARTCharPut(UART0_BASE, '0');
    UARTCharPut(UART0_BASE, ' ');

    UARTCharPut(UART0_BASE, 'L');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, '4');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    if(l4 == 0xFF)
        UARTCharPut(UART0_BASE, '1');
    else if(l4 == 0)
        UARTCharPut(UART0_BASE, '0');
    UARTCharPut(UART0_BASE, ' ');
}

// Mapeo
unsigned short map(uint32_t x, uint32_t x0, uint32_t x1, unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}
