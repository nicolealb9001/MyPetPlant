#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 3      // Pin al que está conectado el sensor DHT
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#define BUZZER_PIN 5  // Pin al que está conectado el buzzer
#define bomba 2   // Pin al que está conectado el relé
#define higrometro A0

// Umbrales de humedad del suelo
//int humMin = 1023;     // Valor para suelo muy seco
//int humMax = 0;     // Valor para suelo muy húmedo

// Umbral de temperatura para mostrar carita de calor
float thresholdHot = 40;  // Ajusta este valor según tus necesidades

// Umbrales para controlar el riego
// int irrigationStart = 600;  // Valor para iniciar el riego
// Declaración de las funciones que se van a utilizar

// Función para dibujar una carita feliz

enum Face {HAPPY, NEUTRAL, SAD, ANGRY};
Face currentFace = HAPPY;

void drawHappyFace() {
  display.clearDisplay();
  
  display.fillCircle(44, 20, 4, SSD1306_WHITE);  // Ojo izquierdo
  display.fillCircle(84, 20, 4, SSD1306_WHITE);  // Ojo derecho
  
  for (int i = -20; i <= 20; i++) {
    display.drawPixel(64 + i, 42 - (i * i) / 40, SSD1306_WHITE);
  }
  
  display.display();
}

// Función para dibujar una carita neutra
void drawNeutralFace() {
  display.clearDisplay();
  
  display.fillCircle(44, 20, 4, SSD1306_WHITE);  // Ojo izquierdo
  display.fillCircle(84, 20, 4, SSD1306_WHITE);  // Ojo derecho
  display.drawLine(44, 42, 84, 42, SSD1306_WHITE);  // Boca recta
  
  display.display();
}

// Función para dibujar una carita triste
void drawSadFace() {
  display.clearDisplay();
  
  display.fillCircle(44, 20, 4, SSD1306_WHITE);  // Ojo izquierdo
  display.fillCircle(84, 20, 4, SSD1306_WHITE);  // Ojo derecho
  
  for (int i = -20; i <= 20; i++) {
    display.drawPixel(64 + i, 42 + (i * i) / 40, SSD1306_WHITE);
  }
  
  display.display();
}

// Función para dibujar una carita súper triste
void drawSuperSadFace() {
  display.clearDisplay();
  
  display.fillCircle(44, 20, 4, SSD1306_WHITE);  // Ojo izquierdo
  display.fillCircle(84, 20, 4, SSD1306_WHITE);  // Ojo derecho
  
  for (int i = -20; i <= 20; i++) {
    display.drawPixel(64 + i, 48 + (i * i) / 20, SSD1306_WHITE);
  }
  
  display.display();
}

// Función para dibujar una carita enojada
void drawAngryFace() {
  display.clearDisplay();
  
  display.drawLine(40, 18, 48, 22, SSD1306_WHITE);  // Ceja izquierda
  display.drawLine(80, 18, 88, 22, SSD1306_WHITE);  // Ceja derecha
  display.fillCircle(44, 26, 4, SSD1306_WHITE);     // Ojo izquierdo
  display.fillCircle(84, 26, 4, SSD1306_WHITE);     // Ojo derecho
  
  for (int i = -20; i <= 20; i++) {
    display.drawPixel(64 + i, 42 + (i * i) / 40, SSD1306_WHITE);
  }
  
  display.display();
}

// Configuración inicial
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(higrometro, INPUT);
  pinMode(bomba, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C es la dirección I2C común para SSD1306
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

// Bucle principal
void loop() {
 // Leer el valor del sensor de humedad del suelo
 int valorHumedad = analogRead(higrometro);
 Serial.print("Soil Moisture: ");
 Serial.println(valorHumedad);
 delay(2000);

// Leer la temperatura del sensor DHT11
   float temperatura = dht.readTemperature();
   Serial.print("Temperature: ");
   Serial.println(temperatura);
   delay(2000);

   Face newFace;
  // Verificar los niveles de humedad del suelo y mostrar la cara correspondiente
  if (valorHumedad <= 613,9 && valorHumedad >=716.1) {
   newFace = NEUTRAL;
  }else if (valorHumedad <= 716.2 && valorHumedad >= 818.4){ 
   newFace = SAD;
  }else if (valorHumedad >=818.5 && temperatura>=27){
   newFace = ANGRY;
  }else{
       newFace = HAPPY;
  }
  

  if (newFace != currentFace) {
    currentFace = newFace;
    switch (currentFace) {
      case HAPPY:
        drawHappyFace();
        tone(BUZZER_PIN, 1000, 500);  // Emitir tono de 1000 Hz por 500 ms
        digitalWrite(bomba, LOW);  // Apagar la bomba
        break;
      case NEUTRAL:
        drawNeutralFace();
        tone(BUZZER_PIN, 1500, 500);  // Emitir tono de 1500 Hz por 500 ms
        digitalWrite(bomba, LOW);  // Apagar la bomba
        break;
      case SAD:
          drawSuperSadFace();
         tone(BUZZER_PIN, 2000, 500);  // Emitir tono de 2000 Hz por 500 ms
        digitalWrite(bomba, LOW);  // Apagar la bomba
        break;
      case ANGRY:
        drawAngryFace();
        tone(BUZZER_PIN, 500, 500);  // Emitir tono de 500 Hz por 500 ms
        digitalWrite(bomba, HIGH);  // Encender la bomba
        break;
    }
  }
}
