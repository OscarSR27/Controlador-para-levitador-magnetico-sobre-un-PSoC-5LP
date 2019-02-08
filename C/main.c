/* ========================================
 *
 * Programador: Mariana Guzman, Oscar Soto y Daniel Tristan
 * Proyecto: Levitador magnetico
 * Curso: Laboratorio de control automatico.
 *
 * En este programa se implementa el controlador PID para el control de un levitador magnetico utilizando el PSoC 5LP
 * ========================================
*/
#include <project.h>
#include <math.h>
#include <stdio.h>

CY_ISR_PROTO(Interrupcion);//Declarar el proceso que atiende la interrupcion



    int64 cuentas;
    char str1[25];
    float64 Volts;
    char str2[25];

//Declaracion de variables
    float Error;
    float Derivada;
    float Integral;
    float OutputF;
    float OutputPD;
    float Acondicionar;
    float x ;

//Constantes
    float k = 7.37779*0.00001 ;
    float g = 9.77;
    float m = 0.00585;
    float R = 2.818;
    float ue = 1.258;
    float yo = 3.5;
    float beta = 0.000275;
    float Tmuestreo = 0.001;
    float Referencia = 0;
    float E0 = 0;

//Parametros del controlador PID
    float KP = -49.9;
    float KD =-0.000795;
    float KI = -0.4;

//Interrupcion ADC
void Mi_Interrupcion(void)
{
    if(ADC_IsEndConversion(ADC_RETURN_STATUS))
            {
                MyPin_Write(1);//El pin se pone en alto para indicar el inicio del algoritmo y comenzar a tomar tiempo de ejecucion

                //Leer posicion
                cuentas = ADC_GetResult32();
                Volts = (cuentas*4.46)/65536;
                OutputF=((Volts-yo)*pow((k*ue),(3/2))) / (-2*beta*pow((g*m*R),(3/2)));

                //Obtener error
                Error = Referencia - OutputF;

                //Calcular PID
                Integral = Integral + (Error*Tmuestreo);
                Derivada = (Error - E0)/Tmuestreo;
                OutputPD = (KP * Error) + (KD * Derivada) + (KI * Integral);

                //Saturar PD
                if (OutputPD > 5){
                    OutputPD = 5;
                }
                else if(OutputPD < 0){
                    OutputPD = 0;
                }

                //Enviar PWM
                Acondicionar = (OutputPD*2399)/5;
                PWM_WriteCompare(Acondicionar);

                //Actualizar error
                E0 = Error;
                MyPin_Write(0);//El pin se pone en bajo para indicar la finalizacion del algoritmo y terminar de tomar tiempo de ejecucion
            }
}

CY_ISR(Interrupcion){
    Mi_Interrupcion();
}

int main()
{
    CyGlobalIntEnable;
    //Se llaman los componentes fisicos utilizados en el PSoC 5LP
    LCD_Start();//LCD
    PWM_Start();//PWM
    ADC_Start();//ADC
    isr_StartEx(Mi_Interrupcion);//Interrupcion
    Timer_Start();//Pin

    //Formato para informacion de salida de la LCD
    LCD_Position(0u, 0u);
    LCD_PrintString("Cuentas:");

    LCD_Position(0u, 8u);
    LCD_PrintString("Tension:");

    //ADC
    ADC_StartConvert();

    //Datos LCD
    for(;;)
    {
        sprintf(str1,"%li",cuentas);
        LCD_Position(1u, 0u);
        LCD_PrintString(str1);

        LCD_Position(1u, 8u);
        sprintf(str2,"%5.2f",Acondicionar);
        LCD_PrintString(str2);
    }
}
