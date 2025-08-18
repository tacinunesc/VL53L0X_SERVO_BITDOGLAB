#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "vl53l0x.h"
#include "servo.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_fonts.h"

#define PORTA_I2C i2c0
const uint PINO_SDA_I2C = 0;
const uint PINO_SCL_I2C = 1;

#define LED_VERDE 11
#define LED_VERMELHO 13

void exibir_oled(uint16_t distancia_cm, const char* estado_porta) {
    char buffer[32];
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("MONITOR DISTANCIA", Font_7x10, White);

    snprintf(buffer, sizeof(buffer), "DISTANCIA: %d cm", distancia_cm);
    ssd1306_SetCursor(0, 16);
    ssd1306_WriteString(buffer, Font_7x10, White);

    snprintf(buffer, sizeof(buffer), "ACESSO-AUT: %s", estado_porta);
    ssd1306_SetCursor(0, 32);
    ssd1306_WriteString(buffer, Font_7x10, White);

    ssd1306_UpdateScreen();
}

int main() {
    stdio_init_all();
    while(!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // Inicializa OLED
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();

    // Inicializa LEDs
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    // Inicializa I2C
    i2c_init(PORTA_I2C, 100 * 1000);
    gpio_set_function(PINO_SDA_I2C, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL_I2C, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA_I2C);
    gpio_pull_up(PINO_SCL_I2C);

    inicializar_pwm_servo();

    vl53l0x_dispositivo sensor;
    if (!vl53l0x_inicializar(&sensor, PORTA_I2C)) {
        printf("ERRO: Falha ao inicializar o sensor VL53L0X.\n");
        while (1);
    }
    printf("Sensor VL53L0X inicializado com sucesso.\n");

    vl53l0x_iniciar_continuo(&sensor, 0);
    printf("Sensor em modo contínuo. Coletando dados...\n");

    uint8_t ultima_posicao = 255;

    while (1) {
        uint16_t distancia_cm = vl53l0x_ler_distancia_continua_cm(&sensor);

        const char* estado_porta = "FECHADO";
        uint8_t nova_posicao;
        if (distancia_cm < 10) {
            nova_posicao = 1; // Abrir (direita)
            estado_porta = "ABERTO";
        } else {
            nova_posicao = 2; // Fechar (esquerda)
            estado_porta = "FECHADO";
        }

        exibir_oled(distancia_cm, estado_porta);

        if (distancia_cm == 65535) {
            printf("Timeout ou erro de leitura.\n");
        } else if (distancia_cm > 800) {
            printf("Fora de alcance.\n");
        } else {
            printf("Distância: %d cm\n", distancia_cm);

            if (nova_posicao != ultima_posicao) {
                servo_posicao(nova_posicao);
                sleep_ms(500);
                servo_posicao(0);
                ultima_posicao = nova_posicao;
            }

            // LEDs de status
            if (nova_posicao == 1) {
                gpio_put(LED_VERDE, 1);
                gpio_put(LED_VERMELHO, 0);
            } else {
                gpio_put(LED_VERDE, 0);
                gpio_put(LED_VERMELHO, 1);
            }
        }
        sleep_ms(200);
    }
    return 0;
}