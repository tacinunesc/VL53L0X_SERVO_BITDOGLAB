/**
 * Configurações gerais da biblioteca e hardware.
 * Vantagens Desta Estrutura:
 
Portabilidade: Configure para diferentes microcontroladores sem alterar o código-fonte.

Otimização: Remova recursos não usados (ex: fontes) para economizar espaço.

Manutenção: Ajuste configurações em um único arquivo.

Clareza: Separa hardware (I2C/SPI) de dados (fontes).

Propósito: Habilita as fontes que serão usadas no projeto.
 
 * Private configuration file for the SSD1306 library.
 * This example is configured for Raspberry Pico W, I2C and including all fonts.
 */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

// Choose a bus
#define SSD1306_USE_I2C 
//#define SSD1306_USE_SPI

// I2C Configuration
#define SSD1306_I2C_PORT        i2c1
#define SSD1306_I2C_ADDR        0x3C //(0x3C << 1)

// Mirror the screen if needed
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
// # define SSD1306_INVERSE_COLOR

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26

#define SSD1306_INCLUDE_FONT_16x24

#define SSD1306_INCLUDE_FONT_16x15

// The width of the screen can be set using this
// define. The default value is 128.
 #define SSD1306_WIDTH           128

// If your screen horizontal axis does not start
// in column 0 you can use this define to
// adjust the horizontal offset
// #define SSD1306_X_OFFSET

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
 #define SSD1306_HEIGHT          64

#endif /* __SSD1306_CONF_H__ */