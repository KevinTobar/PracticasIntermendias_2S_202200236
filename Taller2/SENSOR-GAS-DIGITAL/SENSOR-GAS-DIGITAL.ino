#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- Pines ----------
const int PIN_MQ2    = A1;
const int PIN_LED    = 2;
const int PIN_BUZZER = 4;

// ---------- Configuración ----------
const int UMBRAL = 400;   // Ajusta según tu calibración (0-1023)

// Pantalla LCD 16x2 I2C (dirección 0x27; si no enciende, prueba 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  // Mensaje de calentamiento del sensor
  lcd.setCursor(0, 0);
  lcd.print("Calentando MQ-2");
  lcd.setCursor(0, 1);
  lcd.print("Espere...");
  delay(20000);   // 20 segundos (en Wokwi puedes bajarlo a 2000)
  lcd.clear();
}

void loop() {
  // 1. LEER
  int valor = analogRead(PIN_MQ2);

  // 2 y 3. MOSTRAR y COMPARAR
  lcd.setCursor(0, 0);
  if (valor > UMBRAL) {
    lcd.print("ALERTA!         ");
    // 4. ALERTAR
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_BUZZER, HIGH);
  } else {
    lcd.print("Todo correcto   ");
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
  }

  // Segunda línea: cantidad registrada
  lcd.setCursor(0, 1);
  lcd.print("Gas: ");
  lcd.print(valor);
  lcd.print("     ");   // espacios para borrar dígitos sobrantes

  delay(300);
}