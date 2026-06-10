/************************************************************************************************
Copyright (c) 2022-2023, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología, Universidad Nacional de Tucumán
https://www.microprocesadores.unt.edu.ar/

Copyright (c) 2022-2023, Esteban Volentini <evolentini@herrera.unt.edu.ar>

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

/** \brief EDU-CIAA-NXP board sample application
 **
 ** \addtogroup samples Samples
 ** \brief Samples applications with MUJU Framwork
 ** @{ */

/* === Headers files inclusions =============================================================== */

#ifndef EDU_CIAA_NXP
#error "This program can only be compiled for the EDU-CIAA-NXP board"
#endif


#include "placa.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/**
 * @brief Enumeration with color sequence of RGB led
 */
typedef enum rgb_color_e {
    LED_RED_ON = 0,
    LED_RED_OFF,
    LED_GREEN_ON,
    LED_GREEN_OFF,
    LED_BLUE_ON,
    LED_BLUE_OFF,
} rgb_color_t;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/**
 * @brief Function to configure pins and gpio bits used by board leds
 */
//static void ConfigureLeds(void);

/**
 * @brief Function to configure pins and gpio bits used by board keys
 */
//static void ConfigureKeys(void);

/**
 * @brief Function to flash RGB led in sequence
 */
//static void FlashLed(board_t placa);

/**
 * @brief Function to switch on and off a led with two keys
 */
//static void SwitchLed(board_t placa);

/**
 * @brief Function to switch on and off a led with a single key
 */
//static void ToggleLed(board_t placa);

/**
 * @brief Function to turn on a led while a key is pressed
 */
//static void TestLed(board_t placa);

/**
 * @brief Function to generate a delay of approximately 100 ms
 */
//static void Delay(void);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

//static board_t board;

/* === Private function implementation ========================================================= */


/* === Public function implementation ========================================================== */

int main(void) {
    uint8_t entrada[4] = {4, 2, 3, 1};
    uint16_t frecuencia = 0;

    board_t placa = BoardCreate();

    DisplayWriteBCD(placa->display, entrada, sizeof(entrada));

    while (true) {
        
        if (DigitalInputHasActivate(placa->tecla_aceptar)) {
            if (frecuencia == 0) {
                frecuencia = 100;
            } else if (frecuencia == 100) {
                frecuencia = 250;
            } else {
                frecuencia = 0;
            }
            DisplayFlashDigits(placa->display, 0, 3, frecuencia);
        }

        if (DigitalInputHasActivate(placa->tecla_cancelar)) {
            DisplayToggleDots(placa->display, 0, 3);
        }

        if (DigitalInputHasActivate(placa->tecla_F1)) {
            entrada[3] = (entrada[3] + 1) % 10;
            DisplayWriteBCD(placa->display, entrada, sizeof(entrada));
        }

        if (DigitalInputHasActivate(placa->tecla_F2)) {
            entrada[2] = (entrada[2] + 1) % 10;
            DisplayWriteBCD(placa->display, entrada, sizeof(entrada));
        }

        if (DigitalInputHasActivate(placa->tecla_F3)) {
            entrada[1] = (entrada[1] + 1) % 10;
            DisplayWriteBCD(placa->display, entrada, sizeof(entrada));
        }

        if (DigitalInputHasActivate(placa->tecla_F4)) {
            entrada[0] = (entrada[0] + 1) % 10;
            DisplayWriteBCD(placa->display, entrada, sizeof(entrada));
        }

        // Bucle de delay y refresco del display
        for (int index = 0; index < 50; index++) {
            for (int delay = 0; delay < 1000; delay++) {
                __asm("NOP");
            }
            DisplayRefresh(placa->display);
        }
    }

    return 0;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */