/************************************************************************************************
Copyright 2016-2026, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnologia
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2026, Marcelo Joaquin Herrera <marceloherrera275@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

#ifndef EDU_CIAA_NXP
#error "This program can only be compiled for the EDU-CIAA-NXP board"
#endif

#include "placa.h"
#include "reloj.h"
#include <stdbool.h>
#include <string.h>

/* === Macros definitions ====================================================================== */
#define TICKS_PER_SECOND 10   // cada ciclo del lazo multiplexado toma 100ms. 10 ticks = 1 segundo real.
#define TICKS_3_SECONDS  (3 * TICKS_PER_SECOND)   // 30 ciclos para pulsación larga (3s)
#define TICKS_30_SECONDS (30 * TICKS_PER_SECOND)  // 300 ciclos para el timeout de descarte (30s)

/* === Private data type declarations ========================================================== */
typedef enum {
    MODO_SIN_AJUSTAR,
    MODO_MINUTOS,
    MODO_HORAS,
    MODO_NORMAL,
    MODO_MINUTOS_ALARMA,
    MODO_HORAS_ALARMA
} modo_sistema_t;

/* === Private variable declarations =========================================================== */
static struct board_s * placa;
static clock_t reloj;

static modo_sistema_t modo_actual = MODO_SIN_AJUSTAR;
static modo_sistema_t modo_anterior = (modo_sistema_t)0xFF; 

static uint32_t contador_f1 = 0;
static uint32_t contador_f2 = 0;
static uint32_t timeout_ajuste = 0;
static bool alarma_sonando = false;

static hora_t hora_ajuste;

/* === Private function implementations ========================================================= */

static void AlarmaHandler(clock_t reloj_instancia) {
    alarma_sonando = true;
    DigitalOutputActivate(placa->buzzer);
}

static void IncrementarAjuste(bool minutos) {
    if (minutos) {
        hora_ajuste[3]++;
        if (hora_ajuste[3] > 9) {
            hora_ajuste[3] = 0;
            hora_ajuste[2]++;
        }
        if (hora_ajuste[2] > 5) {
            hora_ajuste[2] = 0;
        }
    } else {
        hora_ajuste[1]++;
        if ((hora_ajuste[0] == 2) && (hora_ajuste[1] > 3)) {
            hora_ajuste[0] = 0;
            hora_ajuste[1] = 0;
        } else if (hora_ajuste[1] > 9) {
            hora_ajuste[1] = 0;
            hora_ajuste[0]++;
        }
    }
}

static void DecrementarAjuste(bool minutos) {
    if (minutos) {
        if (hora_ajuste[3] == 0 && hora_ajuste[2] == 0) {
            hora_ajuste[2] = 5;
            hora_ajuste[3] = 9;
        } else {
            if (hora_ajuste[3] == 0) {
                hora_ajuste[3] = 9;
                hora_ajuste[2]--;
            } else {
                hora_ajuste[3]--;
            }
        }
    } else {
        if (hora_ajuste[0] == 0 && hora_ajuste[1] == 0) {
            hora_ajuste[0] = 2;
            hora_ajuste[1] = 3;
        } else {
            if (hora_ajuste[1] == 0) {
                hora_ajuste[1] = 9;
                hora_ajuste[0]--;
            } else {
                hora_ajuste[1]--;
            }
        }
    }
}

/* === Public function implementation ========================================================== */

int main(void) {
    
    hora_t hora_pantalla;
    hora_t temp_valida;
    uint16_t divisor_un_segundo = 0;
    bool punto_segundos = false;
    
    static bool f3_anterior = false;
    static bool f4_anterior = false;
    static bool aceptar_anterior = false;
    static bool cancelar_anterior = false;

    // variables para almacenar el estado lógico real (invertido) de las teclas
    bool f1_presionado;
    bool f2_presionado;
    bool f3_presionado;
    bool f4_presionado;
    bool aceptar_presionado;
    bool cancelar_presionado;

    bool btn_f3;
    bool btn_f4;
    bool btn_aceptar;
    bool btn_cancelar;

    int index;
    volatile int delay; 

    placa = (struct board_s *)BoardCreate();
    reloj = RelojCreate(TICKS_PER_SECOND, AlarmaHandler);

    memset(hora_ajuste, 0, sizeof(hora_t));

    while (true) {
        
        f1_presionado       = !DigitalInputGetState(placa->tecla_F1);
        f2_presionado       = !DigitalInputGetState(placa->tecla_F2);
        f3_presionado       = !DigitalInputGetState(placa->tecla_F3);
        f4_presionado       = !DigitalInputGetState(placa->tecla_F4);
        aceptar_presionado  = !DigitalInputGetState(placa->tecla_aceptar);
        cancelar_presionado = !DigitalInputGetState(placa->tecla_cancelar);

       
        btn_f3       = f3_presionado && !f3_anterior;
        btn_f4       = f4_presionado && !f4_anterior;
        btn_aceptar  = aceptar_presionado && !aceptar_anterior;
        btn_cancelar = cancelar_presionado && !cancelar_anterior;

        
        f3_anterior       = f3_presionado;
        f4_anterior       = f4_presionado;
        aceptar_anterior  = aceptar_presionado;
        cancelar_anterior = cancelar_presionado;

        
        if (alarma_sonando) {
            if (btn_aceptar) {
                PostponeAlarm(reloj, 5); 
                alarma_sonando = false;
                DigitalOutputDeactivate(placa->buzzer);
            }
            if (btn_cancelar) {
                alarma_sonando = false; 
                DigitalOutputDeactivate(placa->buzzer);
            }
        } else {
            
            if (modo_actual == MODO_NORMAL || modo_actual == MODO_SIN_AJUSTAR) {
                if (btn_aceptar)  { SetAlarmEnabled(reloj, true);  }
                if (btn_cancelar) { SetAlarmEnabled(reloj, false); }
            }

            
            if (f1_presionado && (modo_actual == MODO_NORMAL || modo_actual == MODO_SIN_AJUSTAR)) {
                contador_f1++;
                if (contador_f1 >= TICKS_3_SECONDS) {
                    modo_actual = MODO_MINUTOS;
                    contador_f1 = 0;
                    timeout_ajuste = 0;
                    if (!GetCurrentTime(reloj, hora_ajuste)) {
                        memset(hora_ajuste, 0, sizeof(hora_t));
                    }
                }
            } else {
                contador_f1 = 0;
            }

            
            if (f2_presionado && modo_actual == MODO_NORMAL) {
                contador_f2++;
                if (contador_f2 >= TICKS_3_SECONDS) {
                    modo_actual = MODO_MINUTOS_ALARMA;
                    contador_f2 = 0;
                    timeout_ajuste = 0;
                    GetAlarmTime(reloj, hora_ajuste);
                }
            } else {
                contador_f2 = 0;
            }

            
            switch (modo_actual) {
                
                case MODO_MINUTOS:
                    timeout_ajuste++;
                    if (btn_f4) { IncrementarAjuste(true); timeout_ajuste = 0; }
                    if (btn_f3) { DecrementarAjuste(true); timeout_ajuste = 0; }
                    
                    if (btn_aceptar) {
                        modo_actual = MODO_HORAS; // Pasa firmemente a horas sin saltarse el menú
                        timeout_ajuste = 0;
                    }
                    if (btn_cancelar || timeout_ajuste >= TICKS_30_SECONDS) {
                        modo_actual = (GetCurrentTime(reloj, temp_valida)) ? MODO_NORMAL : MODO_SIN_AJUSTAR;
                    }
                    break;

                case MODO_HORAS:
                    timeout_ajuste++;
                    if (btn_f4) { IncrementarAjuste(false); timeout_ajuste = 0; }
                    if (btn_f3) { DecrementarAjuste(false); timeout_ajuste = 0; }
                    
                    if (btn_aceptar) {
                        SetCurrentTime(reloj, hora_ajuste); // guarda la hora definitiva
                        modo_actual = MODO_NORMAL;          // regresa al modo normal y se queda ahi sin saltarse el menú
                        timeout_ajuste = 0;
                    }
                    if (btn_cancelar || timeout_ajuste >= TICKS_30_SECONDS) {
                        modo_actual = (GetCurrentTime(reloj, temp_valida)) ? MODO_NORMAL : MODO_SIN_AJUSTAR;
                    }
                    break;

                case MODO_MINUTOS_ALARMA:
                    timeout_ajuste++;
                    if (btn_f4) { IncrementarAjuste(true); timeout_ajuste = 0; }
                    if (btn_f3) { DecrementarAjuste(true); timeout_ajuste = 0; }
                    
                    if (btn_aceptar) {
                        modo_actual = MODO_HORAS_ALARMA;
                        timeout_ajuste = 0;
                    }
                    if (btn_cancelar || timeout_ajuste >= TICKS_30_SECONDS) {
                        modo_actual = MODO_NORMAL;
                    }
                    break;

                case MODO_HORAS_ALARMA:
                    timeout_ajuste++;
                    if (btn_f4) { IncrementarAjuste(false); timeout_ajuste = 0; }
                    if (btn_f3) { DecrementarAjuste(false); timeout_ajuste = 0; }
                    
                    if (btn_aceptar) {
                        SetAlarmTime(reloj, hora_ajuste); // guarda la hora de la alarma
                        modo_actual = MODO_NORMAL;
                        timeout_ajuste = 0;
                    }
                    if (btn_cancelar || timeout_ajuste >= TICKS_30_SECONDS) {
                        modo_actual = MODO_NORMAL;
                    }
                    break;

                default:
                    break;
            }
        }

        
        if (modo_actual != modo_anterior) {
            modo_anterior = modo_actual;
            
            if (modo_actual == MODO_SIN_AJUSTAR) {
                DisplayFlashDigits(placa->display, 0, 3, 40);  // parpadea todo el display entero al arrancar
            } else if (modo_actual == MODO_MINUTOS || modo_actual == MODO_MINUTOS_ALARMA) {
                DisplayFlashDigits(placa->display, 2, 3, 40);  // parpadean unicamente los minutos
            } else if (modo_actual == MODO_HORAS || modo_actual == MODO_HORAS_ALARMA) {
                DisplayFlashDigits(placa->display, 0, 1, 40);  // parpadean unicamente las horas
            } else {
                DisplayFlashDigits(placa->display, 0, 3, 0);   // queda fijo y brillante en modo normal
            }
        }

        
        ClockTick(reloj);

        divisor_un_segundo++;
        if (divisor_un_segundo >= (TICKS_PER_SECOND / 2)) { 
            divisor_un_segundo = 0;
            punto_segundos = !punto_segundos; // Conmutación exacta a 1 Hz real para el punto
        }

        
        if (modo_actual == MODO_NORMAL || modo_actual == MODO_SIN_AJUSTAR) {
            if (!GetCurrentTime(reloj, hora_pantalla)) {
                memset(hora_pantalla, 0, sizeof(hora_t)); // muestra 00:00 limpio al iniciar
            }
        } else {
            memcpy(hora_pantalla, hora_ajuste, 4);
        }

        DisplayWriteBCD(placa->display, hora_pantalla, 4);

        
        if (punto_segundos && (modo_actual == MODO_NORMAL || modo_actual == MODO_SIN_AJUSTAR)) {
            DisplayToggleDots(placa->display, 1, 1); 
        }

        if (modo_actual == MODO_MINUTOS_ALARMA || modo_actual == MODO_HORAS_ALARMA) {
            DisplayToggleDots(placa->display, 0, 3);
        }

        
        if (IsAlarmEnabled(reloj) && (modo_actual == MODO_NORMAL || modo_actual == MODO_SIN_AJUSTAR)) {
            DisplayToggleDots(placa->display, 0, 0);
        }

        
        for (index = 0; index < 50; index++) {
            for (delay = 0; delay < 40000; delay++) {
                __asm("NOP");
            }
            DisplayRefresh(placa->display); 
        }
    }

    return 0;
}
/* === End of documentation ==================================================================== */