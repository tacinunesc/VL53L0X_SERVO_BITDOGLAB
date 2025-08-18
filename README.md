# 🗺 Sensor de Distancia o VL53L0X + Servo Motor atuando com Pico W (BitDogLab)
![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico-purple.svg)
![Sensor](https://img.shields.io/badge/Sensor-VL53L0X-red.svg)
![Sensor](https://img.shields.io/badge/Servo-Motor-green.svg)

## Sobre o Projeto
Este projeto visa criar um firmware para controlar e vizualizar a distancia com o sensor (VL53L0X), utilizando em conjunto um servo motor, LEDs RGB e um
display Oled inclusos na placa Raspberry Pi Pico W (BItDogLab).

<div align="center">
  <img src="IMG_20250818_141958373.jpg "  alt="Controle" width="30%">
  <img src="IMG_20250818_142034357_MFNR_HDR.jpg"  alt="Controle" width="30%">
</div>

## 🛠️ Estrutura do projeto
- Detector.c – Programa principal em C que faz leitura de presença, com base nesta informação utiliza o servo motor girar para direita caso haja presença detectada a menos de 10cm e para a esquerda de 10cm ou mais e essa informação é exibida no porta serial e no visor oled da BitDogLab
- vl53l0x.c - Onde fica as definições do sensor de distancia
- servo.c - Onde fica as definições do atuador servo motor
- Pasta inc - Onde esta localizada as informações da oled
- CMakeLists.txt – Configuração do build usando o Pico SDK

- ## 🔌 Requisitos
Hardware:

- Raspberry Pi Pico W
- Sensor de Distancia (VL53L0X)
- Servo Motor 9g
- Adaptador do Servo Motor
- LEDs

## ⚙️ Como usar
1- Clone o repositorio

2- Deploy no Pico W
 - Segure o botão BOOTSEL do Pico W e conecte-o ao PC
 - Clique no botão run no Vscode ou arraste o arquivo .u2 para dentro do disco removível que aparecer
 - O Pico irá reiniciar executando o firmware
   
## 🔧 Funcionamento do Código
O programa realiza as seguintes ações:

1. Inicialização dos periféricos
- Configura o barramento I2C0 (pinos GPIO 0 e 1)
- Inicializa o sensor MPU6500 e o display OLED SSD1306
- Configura os pinos dos LEDs RGB
- Inicializa o sinal PWM para controle do servo motor

2. Leitura da luminosidade
A cada segundo, o sistema:

- Detecta a presença com o sensor de distancia VL53L0X
- Exibe o valor no display OLED, com a distancia detectada e acesso autorizado
- Com isso, ele acende um LED correspondente ao nível de aceleração:

   * 🟢 Verde: acesso autorizado, com distancia detectada menor que 10cm, ou seja, está aberto

   * 🔴 Vermelho: acesso autorizado, com distancia detectada maior ou igual a 10cm, ou seja, está fechado
 
3. Controle do servo motor
- Porta abre (servo gira para a direita) se a distância for menor que 10 cm.
- - Porta fecha (servo gira para a esquerda) se a distância for maior ou igual a 10 cm.

## 📦 Dependências

vl53l0x.h para o sensor de distancia

ssd1306.h e ssd1306_fonts.h para o display OLED

servo.h para atuador servo motor
