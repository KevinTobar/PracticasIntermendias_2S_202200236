// ============================================================
// Control de LED con un pulsador
// Cada vez que presionas el botón, el LED cambia de modo:
//   Modo 0: apagado -> Modo 1: encendido -> Modo 2: parpadeando
//   y luego vuelve al Modo 0 (es un ciclo).
// ============================================================

// ----- VARIABLES Y CONSTANTES (la "memoria" del programa) -----

// "const int" = un número entero que NO cambiará nunca (como un valor fijo de diseño).
// Aquí definimos a qué pin del Arduino conectamos cada componente.
const int botonPin = 2;    // El pulsador va conectado al pin digital 2
const int ledPin = 13;     // El LED va conectado al pin digital 13

// "int" = número entero que SÍ puede cambiar mientras corre el programa.
int estadoLed = 0;         // Modo actual: 0 = apagado, 1 = encendido, 2 = parpadeo

// Guarda cómo estaba el botón en la lectura anterior.
// HIGH = 5 V (nivel alto, botón sin presionar)
// LOW  = 0 V (nivel bajo, botón presionado)
int lecturaAnterior = HIGH;

// "unsigned long" = número entero grande y sin signo, ideal para guardar tiempo.
// Guarda en qué momento (en milisegundos) se aceptó la última pulsación.
unsigned long ultimoCambio = 0;

// "bool" = variable que solo vale verdadero (true) o falso (false), como un bit.
bool parpadeo = false;              // Estado del LED durante el parpadeo (true = prendido)
unsigned long ultimoParpadeo = 0;   // Momento del último cambio de parpadeo


// ============================================================
// setup(): se ejecuta UNA sola vez al energizar o reiniciar
// la placa. Sirve para configurar los pines.
// ============================================================
void setup() {
  // INPUT_PULLUP activa una resistencia interna que conecta el pin a 5 V.
  // Efecto: el pin lee HIGH cuando el botón está suelto, y al presionarlo
  // (el botón conecta el pin a tierra/GND) lee LOW.
  // Ventaja: no necesitas soldar una resistencia pull-up externa.
  pinMode(botonPin, INPUT_PULLUP);

  // Configura el pin del LED como salida: el Arduino podrá poner 5 V o 0 V en él.
  pinMode(ledPin, OUTPUT);
}


// ============================================================
// loop(): se repite infinitamente, miles de veces por segundo,
// mientras la placa tenga energía. Es el "corazón" del programa.
// ============================================================
void loop() {

  // ----- PARTE 1: LEER EL BOTÓN Y DETECTAR UNA PULSACIÓN -----

  // Lee el voltaje del pin del botón: devuelve HIGH (5 V) o LOW (0 V).
  int lectura = digitalRead(botonPin);

  // Queremos detectar el MOMENTO EXACTO en que el botón pasa de suelto a presionado
  // (un flanco de bajada). Por eso comparamos con la lectura anterior:
  //   - Ahora está LOW (presionado) Y antes estaba HIGH (suelto)
  // Sin esto, mantener el botón presionado cambiaría de modo cientos de veces.
  if (lectura == LOW && lecturaAnterior == HIGH) {

    // ANTI-REBOTE (debounce):
    // Los pulsadores mecánicos "rebotan": al presionarlos, el contacto vibra
    // y genera muchos cambios rápidos de 0 V a 5 V durante unos milisegundos.
    // Aquí ignoramos cualquier pulsación que ocurra menos de 50 ms después
    // de la anterior. millis() devuelve los milisegundos transcurridos
    // desde que se encendió la placa (es como un cronómetro interno).
    if (millis() - ultimoCambio > 50) {

      // Avanza al siguiente modo: 0 -> 1 -> 2 -> 0 -> 1 ...
      // El símbolo % es el "módulo" (residuo de la división).
      // Ejemplo: (2 + 1) % 3 = 0, así que después del modo 2 vuelve al 0.
      estadoLed = (estadoLed + 1) % 3;

      // Anota el momento de esta pulsación para el anti-rebote.
      ultimoCambio = millis();
    }
  }

  // Guarda la lectura actual para compararla en la siguiente vuelta del loop.
  lecturaAnterior = lectura;


  // ----- PARTE 2: ACTUAR SEGÚN EL MODO ACTUAL -----

  // "switch" funciona como un selector: revisa el valor de estadoLed
  // y ejecuta solo el bloque (case) que corresponda.
  switch (estadoLed) {

    case 0:  // MODO 0: LED apagado
      digitalWrite(ledPin, LOW);   // Pone 0 V en el pin: el LED se apaga
      break;                       // "break" = terminar este caso y salir del switch

    case 1:  // MODO 1: LED encendido fijo
      digitalWrite(ledPin, HIGH);  // Pone 5 V en el pin: el LED se enciende
      break;

    case 2:  // MODO 2: LED parpadeando
      // Queremos parpadear SIN detener el programa (si usáramos una pausa,
      // el Arduino no podría leer el botón mientras espera).
      // Solución: revisar si ya pasaron 300 ms desde el último cambio.
      if (millis() - ultimoParpadeo >= 300) {

        parpadeo = !parpadeo;              // El "!" invierte el valor: true -> false, false -> true
        digitalWrite(ledPin, parpadeo);    // true = 5 V (prendido), false = 0 V (apagado)
        ultimoParpadeo = millis();         // Reinicia el "cronómetro" del parpadeo
      }
      break;
  }
}