#include <Wire.h>
#include "Adafruit_TCS34725.h"

//Calibração do Branco (em %)
float calibR = 0.29;
float calibG = 0.35;
float calibB = 0.31;

#define margem 0.01

String descCor[] = { "Branco", "Vermelho", "Verde", "Azul", "Azul Claro", "Amarelo", "Roxo" };

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Sensor Ativo");
  } else {
    Serial.println("Sensor TCS34725 nao encontrado ... verifique as conexoes");
    while (1)
      ;
  }
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;
  float rp, gp, bp;

  // leitura de dados de tons de cores e luminosidade
  tcs.getRawData(&r, &g, &b, &c);

  // cálculo dos níveis de cores
  colorTemp = tcs.calculateColorTemperature(r, g, b);

  // cálculo de nível de luminosidade
  lux = tcs.calculateLux(r, g, b);

  rp = (float)r / (float)c;
  gp = (float)g / (float)c;
  bp = (float)b / (float)c;

  Serial.print("Red: ");
  Serial.print(r, DEC);
  Serial.print(" ");
  Serial.print(rp);
  Serial.print("% ");

  Serial.print("Green: ");
  Serial.print(g, DEC);
  Serial.print(" ");
  Serial.print(gp);
  Serial.print("% ");

  Serial.print("Blue: ");
  Serial.print(b, DEC);
  Serial.print(" ");
  Serial.print(bp);
  Serial.print("% ");

  Serial.print("Color: ");
  Serial.print(c, DEC);
  Serial.print(" ");

 
  int cor;
  if (rp <= (calibR + margem) && gp <= (calibG + margem) && bp <= (calibB + margem)) {
    cor = 0;
  } else {
    if (rp > (calibR + margem) && gp <= (calibG + margem) && bp <= (calibB + margem)) {
      cor = 1; //Vermelho
    } 
    if (rp <= (calibR + margem) && gp > (calibG + margem) && bp <= (calibB + margem)) {
      cor = 2; //Verde
    }
    if (rp <= (calibR + margem) && gp <= (calibG + margem) && bp > (calibB + margem)) {
      cor = 3; //Azul
    }
    if (rp <= (calibR + margem) && gp > (calibG + margem) && bp > (calibB + margem)) {
      cor = 4; //Azul Claro
    }
    if (rp > (calibR + margem) && gp > (calibG + margem) && bp <= (calibB + margem)) {
      cor = 5; //Amarelo
    }
    if (rp > (calibR + margem) && gp <= (calibG + margem) && bp > (calibB + margem)) {
      cor = 6; //Roxo
    }
  }

  Serial.println(descCor[cor]);
  delay(100);
}