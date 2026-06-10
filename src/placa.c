/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2016-2025, Esteban Volentini <evolentini@herrera.unt.edu.ar>

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

/** @file placa.c
 ** @brief Definición de la placa de desarrollo
 **/

/* === Headers files inclusions ================================================================ */

#include "placa.h"
#include "chip.h"
#include "poncho.h"
#include "board.h"
/* === Macros definitions ====================================================================== */

#define LED_R_PORT 2
#define LED_R_PIN  0
#define LED_R_FUNC SCU_MODE_FUNC4
#define LED_R_GPIO 5
#define LED_R_BIT  0

#define LED_G_PORT 2
#define LED_G_PIN  1
#define LED_G_FUNC SCU_MODE_FUNC4
#define LED_G_GPIO 5
#define LED_G_BIT  1

#define LED_B_PORT 2
#define LED_B_PIN  2
#define LED_B_FUNC SCU_MODE_FUNC4
#define LED_B_GPIO 5
#define LED_B_BIT  2

#define LED_1_PORT 2
#define LED_1_PIN  10
#define LED_1_FUNC SCU_MODE_FUNC0
#define LED_1_GPIO 0
#define LED_1_BIT  14

#define LED_2_PORT 2
#define LED_2_PIN  11
#define LED_2_FUNC SCU_MODE_FUNC0
#define LED_2_GPIO 1
#define LED_2_BIT  11

#define LED_3_PORT 2
#define LED_3_PIN  12
#define LED_3_FUNC SCU_MODE_FUNC0
#define LED_3_GPIO 1
#define LED_3_BIT  12

#define TEC_1_PORT 1
#define TEC_1_PIN  0
#define TEC_1_FUNC SCU_MODE_FUNC0
#define TEC_1_GPIO 0
#define TEC_1_BIT  4

#define TEC_2_PORT 1
#define TEC_2_PIN  1
#define TEC_2_FUNC SCU_MODE_FUNC0
#define TEC_2_GPIO 0
#define TEC_2_BIT  8

#define TEC_3_PORT 1
#define TEC_3_PIN  2
#define TEC_3_FUNC SCU_MODE_FUNC0
#define TEC_3_GPIO 0
#define TEC_3_BIT  9

#define TEC_4_PORT 1
#define TEC_4_PIN  6
#define TEC_4_FUNC SCU_MODE_FUNC0
#define TEC_4_GPIO 1
#define TEC_4_BIT  9

#define KEY_F1_PORT 4
#define KEY_F1_PIN  8
#define KEY_F1_FUNC SCU_MODE_FUNC4
#define KEY_F1_GPIO 5
#define KEY_F1_BIT  12

#define KEY_F2_PORT 4
#define KEY_F2_PIN  9
#define KEY_F2_FUNC SCU_MODE_FUNC4
#define KEY_F2_GPIO 5
#define KEY_F2_BIT  13

#define KEY_F3_PORT 4
#define KEY_F3_PIN  10
#define KEY_F3_FUNC SCU_MODE_FUNC4
#define KEY_F3_GPIO 5
#define KEY_F3_BIT  14

#define KEY_F4_PORT 6
#define KEY_F4_PIN  7
#define KEY_F4_FUNC SCU_MODE_FUNC4
#define KEY_F4_GPIO 5
#define KEY_F4_BIT  15

#define KEY_ACCEPT_PIN  2
#define KEY_ACCEPT_PORT 3
#define KEY_ACCEPT_FUNC SCU_MODE_FUNC4
#define KEY_ACCEPT_GPIO 5
#define KEY_ACCEPT_BIT  9

#define KEY_CANCEL_PORT 3
#define KEY_CANCEL_PIN  1
#define KEY_CANCEL_FUNC SCU_MODE_FUNC4
#define KEY_CANCEL_GPIO 5
#define KEY_CANCEL_BIT  8

#define KEY_ACCEPT_PIN  2
#define KEY_ACCEPT_PORT 3
#define KEY_ACCEPT_FUNC SCU_MODE_FUNC4
#define KEY_ACCEPT_GPIO 5
#define KEY_ACCEPT_BIT  9

#define KEY_CANCEL_PORT 3
#define KEY_CANCEL_PIN  1
#define KEY_CANCEL_FUNC SCU_MODE_FUNC4
#define KEY_CANCEL_GPIO 5
#define KEY_CANCEL_BIT  8

/* === Private data type declarations ========================================================== */

/* === Private function declarations =========================================================== */
/**
 * @brief Function to configure pins and gpio bits used by board leds
 */
//static void ConfigureLeds(struct board_s * self);

/**
 * @brief Function to configure pins and gpio bits used by board keys
 */
static void ConfigureKeys(struct board_s * self);
static void DigitsInit(void);
static void SegmentsInit(void);
static void BuzzerInit(void);
static void UpdateSegments(uint8_t segments);
static void UpdateDigits(uint8_t digit);
/* === Private variable definitions ============================================================ */
static struct board_s board = {};
/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */
/*static void ConfigureLeds(struct board_s * self) {
    Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
    self->ledRGB_rojo = DigitalOutputCreate(LED_R_GPIO, LED_R_BIT, false);
    //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, false);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, true);

    Chip_SCU_PinMuxSet(LED_G_PORT, LED_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_G_FUNC);
    self->ledRGB_verde = DigitalOutputCreate(LED_G_GPIO, LED_G_BIT, false);
    //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, false);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, true);

    Chip_SCU_PinMuxSet(LED_B_PORT, LED_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_B_FUNC);
    self->ledRGB_azul = DigitalOutputCreate(LED_B_GPIO, LED_B_BIT, false);
    //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, false);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, true);

    /******************/
    /*Chip_SCU_PinMuxSet(LED_1_PORT, LED_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_1_FUNC);
    self->led_amarillo = DigitalOutputCreate(LED_1_GPIO, LED_1_BIT, false);
    //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, false);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, true);

    Chip_SCU_PinMuxSet(LED_2_PORT, LED_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_2_FUNC);
    self->led_rojo = DigitalOutputCreate(LED_2_GPIO, LED_2_BIT, false);
    //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, false);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, true);

    Chip_SCU_PinMuxSet(LED_3_PORT, LED_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_3_FUNC);
    self->led_verde = DigitalOutputCreate(LED_3_GPIO, LED_3_BIT, false);
    //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, false);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, true);
}*/

static void ConfigureKeys(struct board_s * self) {
    Chip_SCU_PinMuxSet(KEY_F4_PORT, KEY_F4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F4_FUNC);
    self->tecla_F4 = DigitalInputCreate(KEY_F4_GPIO, KEY_F4_BIT, true);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_1_GPIO, TEC_1_BIT, false);

    Chip_SCU_PinMuxSet(KEY_F3_PORT, KEY_F3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F3_FUNC);
    self->tecla_F3 = DigitalInputCreate(KEY_F3_GPIO, KEY_F3_BIT, true);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_2_GPIO, TEC_2_BIT, false);

    Chip_SCU_PinMuxSet(KEY_F2_PORT, KEY_F2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F2_FUNC);
    self->tecla_F2 = DigitalInputCreate(KEY_F2_GPIO, KEY_F2_BIT, true);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_3_GPIO, TEC_3_BIT, false);

    Chip_SCU_PinMuxSet(KEY_F1_PORT, KEY_F1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F1_FUNC);
    self->tecla_F1 = DigitalInputCreate(KEY_F1_GPIO, KEY_F1_BIT, true);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_4_GPIO, TEC_4_BIT, false);

    Chip_SCU_PinMuxSet(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_ACCEPT_FUNC);
    self->tecla_aceptar = DigitalInputCreate(KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT, true);

    Chip_SCU_PinMuxSet(KEY_CANCEL_PORT, KEY_CANCEL_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_CANCEL_FUNC);
    self->tecla_cancelar = DigitalInputCreate(KEY_CANCEL_GPIO, KEY_CANCEL_BIT, true);
}
static void DigitsInit(void) {
    Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, true);
}

static void SegmentsInit(void) {
    Chip_SCU_PinMuxSet(SEGMENT_A_PORT, SEGMENT_A_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_B_PORT, SEGMENT_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_B_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_C_PORT, SEGMENT_C_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_C_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_D_PORT, SEGMENT_D_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_D_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_E_PORT, SEGMENT_E_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_E_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_F_PORT, SEGMENT_F_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_F_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_G_PORT, SEGMENT_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_G_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_P_PORT, SEGMENT_P_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_P_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, true);
}

static void BuzzerInit(void) {
    Chip_SCU_PinMuxSet(BUZZER_PORT, BUZZER_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | BUZZER_FUNC);
    board.buzzer = DigitalOutputCreate(BUZZER_GPIO, BUZZER_BIT, false);
}

static void UpdateSegments(uint8_t segments) {
    if (segments == 0x00) {
        Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK);
        Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
    } else {
        Chip_GPIO_SetValue(LPC_GPIO_PORT, SEGMENTS_GPIO, segments & SEGMENTS_MASK);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (segments & SEGMENT_P));
    }
}

static void UpdateDigits(uint8_t digit) {
    Chip_GPIO_SetValue(LPC_GPIO_PORT, DIGITS_GPIO, (1 << (3 - digit)) & DIGITS_MASK);
}
/* === Public function implementation ========================================================== */
/*board_t BoardCreate() {
    static struct board_s self;
    BoardSetup();
    ConfigureLeds(&self);
    ConfigureKeys(&self);
    return &self;
}*/
board_t BoardCreate(void) {
    BoardSetup();
    DigitsInit();
    SegmentsInit();
    BuzzerInit();
    //KeysInit();
    ConfigureKeys(&board);
    board.display = DisplayCreate(4, &(struct display_driver_s){
        .UpdateDigits = UpdateDigits,
        .UpdateSegments = UpdateSegments,
    });

    return &board;
}
/* === End of documentation ==================================================================== */