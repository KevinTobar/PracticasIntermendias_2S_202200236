// Control de LED con pulsador: enciende, parpadea, apaga
const int botonPin = 2;
const int ledPin = 13;

int estadoLed = 0;
int lecturaAnterior = HIGH;
unsigned long ultimoCambio = 0;

bool parpadeo = false;
unsigned long ultimoParpadeo = 0;

void setup() {
  pinMode(botonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int lectura = digitalRead(botonPin);

  if (lectura == LOW && lecturaAnterior == HIGH) {
    if (millis() - ultimoCambio > 50) {
      estadoLed = (estadoLed + 1) % 3;
      ultimoCambio = millis();
    }
  }
  lecturaAnterior = lectura;

  switch (estadoLed) {
    case 0:
      digitalWrite(ledPin, LOW);
      break;
    case 1:
      digitalWrite(ledPin, HIGH);
      break;
    case 2:
      if (millis() - ultimoParpadeo >= 300) {
        parpadeo = !parpadeo;
        digitalWrite(ledPin, parpadeo);
        ultimoParpadeo = millis();
      }
      break;
  }
}

