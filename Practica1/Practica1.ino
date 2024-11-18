int indicatorPin = A0;      // Pin 8 para el indicador de encendido
int pins[] = {A1, A2, A3, A4}; // Definimos un array para los pines
int delayt = 500;
bool indicatorState = false; // Estado del pin 8 para hacer que parpadee

void setup() {
  // Configuramos el pin 8 como salida para el indicador de encendido
  pinMode(indicatorPin, OUTPUT);

  // Pin 8 encendido durante 2 segundos al inicio
  digitalWrite(indicatorPin, HIGH);
  delay(2000);

  // Configuramos los pines como salida
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  // Recorremos todas las combinaciones de 4 bits (de 0 a 15)
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 4; j++) {
      // Usamos operaciones bit a bit para obtener los estados de los pines
      int state = (i >> j) & 1; // Extraemos el bit correspondiente
      digitalWrite(pins[j], state == 1 ? HIGH : LOW); // Convertimos 1 y 0 a HIGH y LOW
    }

    // Alternamos el estado del pin 8 para que parpadee
    indicatorState = !indicatorState;
    digitalWrite(indicatorPin, indicatorState ? HIGH : LOW);

    delay(delayt);
  }
}