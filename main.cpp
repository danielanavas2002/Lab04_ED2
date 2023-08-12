//****************************************************************
// Librerías
//****************************************************************
#include <Arduino.h>
#include "driver/ledc.h"
//****************************************************************
// Definición de etiquetas
//****************************************************************
//Definicion de Canales
#define servoChannel 0 // 16 canales 0-15
#define ledRChannel 1
#define ledGChannel 2
#define ledBChannel 3
//Definicion de Frecuencias
#define freqLED 1000 // Frecuencia en Hz
#define freqServo 50 // Frecuencia en Hz
//Definicion de Resoluciones
#define resolutionLED 8 // 1-16 bits de resolución
#define resolutionServo 10 // 1-16 bits de resolución
//Definicion Pines de Salida
#define pinLedR 19
#define pinLedG 18
#define pinLedB 5
#define pinServo 23
//Definicion Pines de Botones
#define botonLED_I 34 //Boton Incremento brillo de Led
#define botonC 35 //Boton Cambio de Modo
#define botonServo_I 32 //Boton Incremento Servo
#define botonServo_D 33 //Boton Decremento Servo
//****************************************************************
// Prototipos de funciones
//****************************************************************
void configurarPWM(void);
//****************************************************************
// Variables Globales
//****************************************************************
int estadoC;
int estadoLED_I;
int estadoServo_I;
int estadoServo_D;
int dutycycleR;
int dutycycleG;
int dutycycleB;
int dutycycleServo;
int estadoAnteriorC = 0;
int ledRGB = 0;
int modoRGBS = 0;
//****************************************************************
// Configuración
//****************************************************************
void setup(){
  //Definicion de botones como entradas
  pinMode(botonLED_I, INPUT);
  pinMode(botonC, INPUT);
  pinMode(botonServo_I, INPUT);
  pinMode(botonServo_D, INPUT);
  configurarPWM(); //Configuracion PWM
  //Inicializar Variables en Posicion 0 
  dutycycleR = 0;
  dutycycleG = 0;
  dutycycleB = 0;
  dutycycleServo = 5;
  ledcWrite(servoChannel, dutycycleServo);
  Serial.begin(115200);
}
//****************************************************************
// Loop Principal
//****************************************************************
void loop(){
  // Lee el estado de los botones
  estadoLED_I = digitalRead(botonLED_I);
  estadoC = digitalRead(botonC);
  estadoServo_I = digitalRead(botonServo_I);
  estadoServo_D = digitalRead(botonServo_D);
  delay(100);

  if (estadoC == 1 && estadoAnteriorC == 0) {
    modoRGBS++;
    if (modoRGBS > 3) {
      modoRGBS = 0; // Limitamos el valor máximo a 3 
    }
  }
  estadoAnteriorC = estadoC;
  
  if(modoRGBS == 0){ //Led Rojo
    ledcWrite(ledRChannel, dutycycleR);
    ledcWrite(ledGChannel, dutycycleG);
    ledcWrite(ledBChannel, dutycycleB);
    Serial.println("LED ROJO");
    if(estadoLED_I == 1){
      dutycycleR ++; // Incrementar el dutycycle 
      if(dutycycleR > 120){
        dutycycleR = 0;
      }
      ledcWrite(ledRChannel, dutycycleR);
      Serial.println(dutycycleR);
      delay(10);
    }
  }

  if(modoRGBS == 1){ //Led Verde
    Serial.println("LED VERDE");
    if(estadoLED_I == 1){
      dutycycleG ++; // Incrementar el dutycycle 
      if(dutycycleG > 60){
        dutycycleG = 0;
      }
      ledcWrite(ledGChannel, dutycycleG);
      Serial.println(dutycycleG);
      delay(10);
    }
  }

  if(modoRGBS == 2){ //Led Azul
    Serial.println("LED AZUL");
    if(estadoLED_I == 1){
      dutycycleB ++; // Incrementar el dutycycle 
      if(dutycycleB > 60){
        dutycycleB = 0;
      }
      ledcWrite(ledBChannel, dutycycleB);
      Serial.println(dutycycleB);
      delay(10);
    }
  }

  if(modoRGBS == 3){ //Servo 
    Serial.println("SERVO");
    if(estadoServo_I == 1){
      dutycycleServo ++; // Incrementar el dutycycle 
      if(dutycycleServo > 33){
        dutycycleServo = 33;
      }
      ledcWrite(servoChannel, dutycycleServo);
      Serial.println(dutycycleServo);
      delay(10);
    }

    if(estadoServo_D == 1){
      dutycycleServo --; // Decrementar el dutycycle 
      if(dutycycleServo < 6){
        dutycycleServo = 6;
      }
      ledcWrite(servoChannel, dutycycleServo);
      Serial.println(dutycycleServo);
      delay(10);
    }
    //Funcionamiento de los Leds con el Servo
    if (5 < dutycycleServo && dutycycleServo < 15){
    ledcWrite(ledRChannel, map(dutycycleServo, 6, 14, 0, 120));
    ledcWrite(ledGChannel, 0);
    ledcWrite(ledBChannel, 0);
    } 

    if (15 < dutycycleServo && dutycycleServo < 24){
        ledcWrite(ledRChannel, 0);
        ledcWrite(ledGChannel, map(dutycycleServo, 15, 23, 0, 60));
        ledcWrite(ledBChannel, 0);
    }

    if (25 < dutycycleServo && dutycycleServo < 34){
        ledcWrite(ledRChannel, 0);
        ledcWrite(ledGChannel, 0);
        ledcWrite(ledBChannel, map(dutycycleServo, 24, 33, 0, 60));
    }
  }  
}
//****************************************************************
// Función para configurar módulo PWM
//****************************************************************
void configurarPWM(void){
// Paso 1: Configurar el módulo PWM
ledcSetup(ledRChannel, freqLED, resolutionLED);
ledcSetup(ledGChannel, freqLED, resolutionLED);
ledcSetup(ledBChannel, freqLED, resolutionLED);
ledcSetup(servoChannel, freqServo, resolutionLED);

// Paso 2: seleccionar en que GPIO tendremos nuestra señal PWM
ledcAttachPin(pinLedR, ledRChannel);
ledcAttachPin(pinLedG, ledGChannel);
ledcAttachPin(pinLedB, ledBChannel);
ledcAttachPin(pinServo, servoChannel);
}