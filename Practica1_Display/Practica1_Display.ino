// Pines para los LEDs
int indicatorPin = A0;     // Pin 8 para el indicador de encendido
int leds[] = {A1, A2, A3, A4}; // Definimos un array para los pines de los LEDs
int delayt = 500;           // Velocidad de los LEDs
bool indicatorState = false; // Estado del pin 8 para hacer que parpadee

// Pines para los segmentos del display
int a = 12;
int b = 8;
int c = 4;
int d = 6;
int e = 7;
int f = 11;
int g = 3;

// Pines para los dígitos del display
int dg1 = 2;
int dg2 = 9;
int dg3 = 10;
int dg4 = 13;

// Velocidad de multiplexación
int vel = 5; // Velocidad del display para que no parpadee

// Unidades y decenas para el número en el display
int dec, uni;

// Segmentos (a-g) para los dígitos del display
byte segmentos[7] = {a, b, c, d, e, f, g};
byte orden_digitos[4] = {dg1, dg2, dg3, dg4};

// Configuración para los dígitos (0-9) en un display de 7 segmentos
byte siete_segmentos_digitos[10][7] = { 
  { 1, 1, 1, 1, 1, 1, 0 },  // = 0
  { 0, 1, 1, 0, 0, 0, 0 },  // = 1
  { 1, 1, 0, 1, 1, 0, 1 },  // = 2
  { 1, 1, 1, 1, 0, 0, 1 },  // = 3
  { 0, 1, 1, 0, 0, 1, 1 },  // = 4
  { 1, 0, 1, 1, 0, 1, 1 },  // = 5
  { 1, 0, 1, 1, 1, 1, 1 },  // = 6
  { 1, 1, 1, 0, 0, 0, 0 },  // = 7
  { 1, 1, 1, 1, 1, 1, 1 },  // = 8
  { 1, 1, 1, 0, 0, 1, 1 }   // = 9
};

void setup() {
  // Configuramos el pin para el indicador de encendido
  pinMode(indicatorPin, OUTPUT);

  // Encendemos el pin indicador durante 2 segundos
  digitalWrite(indicatorPin, HIGH);
  delay(2000);

  // Configuramos los pines de los LEDs como salidas
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Inicializamos los pines del display como salidas
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // Recorremos los números del 0 al 15
  for (int i = 0; i < 16; i++) {
    // Configuramos los LEDs para representar el número actual
    for (int j = 0; j < 4; j++) {
      int state = (i >> j) & 1; // Extraemos el bit correspondiente
      digitalWrite(leds[j], state == 1 ? HIGH : LOW); // Convertimos 1 y 0 a HIGH y LOW
    }

    // Calculamos las unidades y decenas para el display
    uni = i % 10; // Unidad (0-9)
    dec = i / 10; // Decenas (0 o 1 para el rango 0-15)

    // Mostramos el número en el display
    EscribeDigito(2, dec);  // Decena
    delay(vel);
    EscribeDigito(1, uni);  // Unidad
    delay(vel);
    
    // Mil y Cien siempre deben mostrar 0 sin parpadeos
    //EscribeDigito(3, 0);    // Centenas
    //delay(vel);
    //EscribeDigito(4, 0);    // Millares
    //delay(vel);

    // Alternamos el estado del pin indicador para que parpadee
    indicatorState = !indicatorState;
    digitalWrite(indicatorPin, indicatorState ? HIGH : LOW);

    // Esperamos el tiempo definido para los LEDs (500 ms)
    delay(delayt);
  }
}

// Función para escribir un número en el dígito correspondiente
void EscribeDigito(byte digito, byte numero) {
  // Limpiamos los segmentos antes de escribir
  LimpiarSegmentos();

  // Activamos el dígito correspondiente
  ActivarDigito(digito);

  // Escribimos el número en el dígito activo
  for (byte i = 0; i < 7; i++) {
    digitalWrite(segmentos[i], siete_segmentos_digitos[numero][i]);
  }
}

// Función para activar un solo dígito a la vez
void ActivarDigito(int x) {
  // Desactivar todos los dígitos
  digitalWrite(dg1, HIGH);
  digitalWrite(dg2, HIGH);
  digitalWrite(dg3, HIGH);
  digitalWrite(dg4, HIGH);

  // Activar el dígito especificado
  switch (x) {
    case 1:
      digitalWrite(dg1, LOW);
      break;
    case 2:
      digitalWrite(dg2, LOW);
      break;
    case 3:
      digitalWrite(dg3, LOW);
      break;
    default:
      digitalWrite(dg4, LOW);
      break;
  }
}

// Función para limpiar los segmentos antes de escribir un nuevo número
void LimpiarSegmentos() {
  for (byte i = 0; i < 7; i++) {
    digitalWrite(segmentos[i], LOW);
  }
}
