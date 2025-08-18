#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "vl53l0x.h"
#include "servo.h"

// --- Configuração da Porta I2C ---
#define PORTA_I2C i2c0
const uint PINO_SDA_I2C = 0;
const uint PINO_SCL_I2C = 1;

int main() {
    stdio_init_all();
    while(!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("--- Iniciando Sensor VL53L0X (Modo Contínuo) ---\n");
    printf("Usando a porta I2C com SDA no pino %d e SCL no pino %d\n", PINO_SDA_I2C, PINO_SCL_I2C);

    // Inicializa o I2C
    i2c_init(PORTA_I2C, 100 * 1000);
    gpio_set_function(PINO_SDA_I2C, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL_I2C, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA_I2C);
    gpio_pull_up(PINO_SCL_I2C);

    // Inicializa o servo
    inicializar_pwm_servo();

    // Inicializa o sensor VL53L0X
    vl53l0x_dispositivo sensor;
    if (!vl53l0x_inicializar(&sensor, PORTA_I2C)) {
        printf("ERRO: Falha ao inicializar o sensor VL53L0X.\n");
        while (1);
    }
    printf("Sensor VL53L0X inicializado com sucesso.\n");

    vl53l0x_iniciar_continuo(&sensor, 0);
    printf("Sensor em modo contínuo. Coletando dados...\n");

    uint8_t ultima_posicao = 255; // Valor impossível para garantir atualização na primeira vez

    while (1) {
        uint16_t distancia_cm = vl53l0x_ler_distancia_continua_cm(&sensor);

        if (distancia_cm == 65535) {
            printf("Timeout ou erro de leitura.\n");
        } else if (distancia_cm > 800) {
            printf("Fora de alcance.\n");
        } else {
            printf("Distância: %d cm\n", distancia_cm);

            uint8_t nova_posicao;
            if (distancia_cm < 10) {
                nova_posicao = 1; // Abrir (direita)
            } else {
                nova_posicao = 2; // Fechar (esquerda)
            }

            // Só muda o servo se a posição mudou
            if (nova_posicao != ultima_posicao) {
                servo_posicao(nova_posicao);
                sleep_ms(500); // tempo para abrir/fechar
                servo_posicao(0); // Para o servo após o movimento
                ultima_posicao = nova_posicao;
            }
        }
        sleep_ms(200);
    }
    return 0;
}