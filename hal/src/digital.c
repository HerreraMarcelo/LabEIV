/*********************************************************************************************************************
Copyright 2016-2026, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnologia
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2026, Marcelo Joaquin Herrera <marceloherrera275@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file digital.c
 ** @brief Implementación de la biblioteca para gestion de entradas y salidas digitales
 **/

/* === Headers files inclusions ================================================================ */

#include "digital.h"
#include "chip.h"
#include <stdlib.h>
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */
struct digital_output_s {
    uint32_t puerto;
    uint8_t terminal;
    bool invertida;
};
struct digital_input_s {
    uint32_t puerto;
    uint8_t terminal;
    bool invertida;
    bool estado_anterior;
};
/* === Private function declarations =========================================================== */

/* === Private variable definitions ============================================================ */

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

digital_output_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal, bool invertida) {
    digital_output_t self;
    self = malloc(sizeof(struct digital_output_s));
    if (self) {
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, puerto, terminal, !invertida);
        self->puerto = puerto;
        self->terminal = terminal;
        self->invertida = invertida;
        DigitalOutputDeactivate(self);
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, false);
        
    }
    return self;
}

void DigitalOutputActivate(digital_output_t self){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, !self->invertida);

}

void DigitalOutputDeactivate(digital_output_t self){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, self->invertida);
}

void DigitalOutputToggle(digital_output_t self){
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->puerto, self->terminal);
}
digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal, bool invertida){
    digital_input_t self;
    self = malloc(sizeof(struct digital_input_s));
    if (self) {
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, puerto, terminal, false);
        self->puerto = puerto;
        self->terminal = terminal;
        self->invertida = invertida;
        self->estado_anterior = DigitalInputGetState(self);
        
    }
    return self;
}
bool DigitalInputGetState(digital_input_t entrada){
    return (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, entrada->puerto, entrada->terminal) == 0) == entrada->invertida;
}
int DigitalInputHasChanged(digital_input_t entrada){
    int resultado = 0;
    bool estado_actual = DigitalInputGetState(entrada);
    if (estado_actual && !entrada->estado_anterior) {
    resultado = ACTIVATE_EVENT;
    } else if (!estado_actual && entrada->estado_anterior) {
        resultado = DEACTIVATE_EVENT;
    }
    entrada->estado_anterior = estado_actual;
    return resultado;
}
bool DigitalInputHasActivate(digital_input_t entrada){
    return DigitalInputHasChanged(entrada) == ACTIVATE_EVENT;
}

bool DigitalInputHasDeactivate(digital_input_t entrada){
    return DigitalInputHasChanged(entrada) == DEACTIVATE_EVENT;
}

/* === End of documentation ==================================================================== */