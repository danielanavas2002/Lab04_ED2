//****************************************************************
// Librerías
//****************************************************************
#include <Arduino.h>
#include "driver/ledc.h"
//****************************************************************
// Definición de etiquetas
//****************************************************************
// Paso 1: selección de parámetros de la señal PWM
//#define pwmChannel 0 // 16 canales 0-15
#define ledRChannel 1
#define ledGChannel 2
#define ledBChannel 3

#define freqPWM 100 // Frecuencia en Hz
#define freqPWM1 100 // Frecuencia en Hz
#define freqPWM2 100 // Frecuencia en Hz

#define resolution 8 // 1-16 bits de resolución

#define pinLedR 19
#define pinLedG 18
#define pinLedB 5
#define pinPWM 15 // GPIO 2 para tener la salida del PWM

const int botonI = 34;  // Boton Incrementar 
const int botonC = 35;  // Boton Cambio de Led

//****************************************************************
// Prototipos de funciones
//****************************************************************
void configurarPWM(void);
//****************************************************************
// Variables Globales
//****************************************************************
int estadoC;
int estadoI;
int dutycycleR;
int dutycycleG;
int dutycycleB;
int estadoAnteriorC = 0;
int ledRGB = 0;
//****************************************************************
// ISR: Interrupciones
//****************************************************************
// void IRAM_ATTR ISR(){
//
//}
//****************************************************************
// Configuración
//****************************************************************
void setup(){
  pinMode(botonI, INPUT);
  pinMode(botonC, INPUT);
  configurarPWM(); 
  Serial.begin(115200);
}
//****************************************************************
// Loop Principal
//****************************************************************
void loop(){
  // Lee el estado de los botones
  int estadoI = digitalRead(botonI);
  int estadoC = digitalRead(botonC);

  // Pequeña pausa para evitar lecturas erráticas debido al rebote del botón
  delay(100);

  if (estadoC == 1 && estadoAnteriorC == 0) {
    ledRGB++;
    if (ledRGB > 2) {
      ledRGB = 0; // Limitamos el valor máximo a 
    }
  }
  estadoAnteriorC = estadoC;
  Serial.println(ledRGB);


  if(ledRGB == 0){ //Led Rojo
  Serial.println("LED ROJO");
    if(estadoI == 1){
      dutycycleR ++; // Incrementar el dutycycle 
      if(dutycycleR > 256){
        dutycycleR = 0;
      }
      ledcWrite(ledRChannel, dutycycleR);
      Serial.println(dutycycleR);
      delay(10);
    
    }
  }

  if(ledRGB == 1){ //Led Verde
  Serial.println("LED VERDE");
    if(estadoI == 1){
      dutycycleG ++; // Incrementar el dutycycle 
      if(dutycycleG > 256){
        dutycycleG = 0;
      }
      ledcWrite(ledGChannel, dutycycleG);
      Serial.println(dutycycleG);
      delay(10);
    }
  }

  if(ledRGB == 2){ //Led Azul
  Serial.println("LED AZUL");
    if(estadoI == 1){
      dutycycleB ++; // Incrementar el dutycycle 
      if(dutycycleB > 256){
        dutycycleB = 0;
      }
      ledcWrite(ledBChannel, dutycycleB);
      Serial.println(dutycycleB);
      delay(10);
    }
  } 
  
  

}
//****************************************************************
// Función para configurar módulo PWM
//****************************************************************
void configurarPWM(void)
{
// Paso 1: Configurar el módulo PWM
ledcSetup(ledRChannel, freqPWM1, resolution);
ledcSetup(ledGChannel, freqPWM2, resolution);
ledcSetup(ledBChannel, freqPWM, resolution);

// Paso 2: seleccionar en que GPIO tendremos nuestra señal PWM
ledcAttachPin(pinLedR, ledRChannel);
ledcAttachPin(pinLedG, ledGChannel);
ledcAttachPin(pinLedB, ledBChannel);
}



