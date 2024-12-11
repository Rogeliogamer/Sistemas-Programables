#include <SoftwareSerial.h>

// Configuración del módulo Bluetooth
SoftwareSerial bluetooth(0, 1); // RX = 0, TX = 1

// Definición de pines para las conexiones del motor de pasos
const int IN1 = 8; // Pin de control IN1 conectado al pin 8 de Arduino
const int IN2 = 9; // Pin de control IN2 conectado al pin 9 de Arduino
const int IN3 = 10; // Pin de control IN3 conectado al pin 10 de Arduino
const int IN4 = 11; // Pin de control IN4 conectado al pin 11 de Arduino

// Variables para la consola
String command = ""; // Variable para almacenar el comando recibido por Bluetooth
int stepDelay = 10; // Tiempo de espera entre pasos (en milisegundos), ajustado a 10ms

// Secuencia de pasos completa para el motor (8 pasos)
int stepsFull[8][4] = 
{
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

// Secuencia de pasos para modo de torque aumentado (4 pasos)
int stepsTorque[4][4] =
{
  {1, 0, 0, 1},
  {0, 1, 0, 1},
  {0, 1, 1, 0},
  {1, 0, 1, 0}
};

// Variables para controlar el modo, dirección y estado del motor
bool fullStepMode = true; // Modo de pasos completos (true) o torque aumentado (false)
bool clockwise = true; // Dirección de movimiento (true = horario, false = antihorario)
bool isMoving = true; // Estado del motor (true = en movimiento, false = detenido)

void setup()
{
  // Configuración de los pines como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Iniciar la comunicación serial a 9600 baudios para el módulo Bluetooth HC-05
  bluetooth.begin(9600);
  // Mensaje inicial en la consola para indicar los comandos disponibles
  bluetooth.println("Ingrese:");
  bluetooth.println("'1. Horario PasoCompleto'");
  bluetooth.println("'2. Antihorario PasoCompleto'");
  bluetooth.println("'3. Horario Torque'");
  bluetooth.println("'4. Antihorario Torque'");
  bluetooth.println("'5. Combinado Horario'");
  bluetooth.println("'6. Combinado Antihorario'");
  bluetooth.println("'7. Parar'");
  bluetooth.println("'8. Continuar'");
}

void loop()
{
  // Comprobar si hay datos recibidos por el módulo Bluetooth
  if (bluetooth.available())
  {
    command = bluetooth.readString(); // Leer el comando enviado desde la app
    command.trim(); // Eliminar espacios en blanco al principio y al final
    handleCommand(command); // Llamar a la función para manejar el comando
  }

  // Ejecutar el movimiento solo si el motor está en movimiento
  if (isMoving)
  {
    // Mover el motor según el modo de operación
    moveMotor(fullStepMode ? stepsFull : stepsTorque, fullStepMode ? 8 : 4);
  }
}

// Función para manejar los comandos recibidos
void handleCommand(String cmd)
{
  if (cmd == "1")
  {
    clockwise = true; // Dirección horaria
    fullStepMode = true; // Paso completo
    bluetooth.println("Modo: Paso Completo, Dirección: Horario");
  }
  else if (cmd == "2")
  {
    clockwise = false; // Dirección antihoraria
    fullStepMode = true; // Paso completo
    bluetooth.println("Modo: Paso Completo, Dirección: Antihorario");
  }
  else if (cmd == "3")
  {
    clockwise = true; // Dirección horaria
    fullStepMode = false; // Torque aumentado
    bluetooth.println("Modo: Torque aumentado, Dirección: Horario");
  }
  else if (cmd == "4")
  {
    clockwise = false; // Dirección antihoraria
    fullStepMode = false; // Torque aumentado
    bluetooth.println("Modo: Torque aumentado, Dirección: Antihorario");
  }
  else if (cmd == "5")
  {
    clockwise = true; // Dirección horaria
    fullStepMode = false; // Modo combinado
    bluetooth.println("Modo: Combinado, Dirección: Horario");
  }
  else if (cmd == "6")
  {
    clockwise = false; // Dirección antihoraria
    fullStepMode = false; // Modo combinado
    bluetooth.println("Modo: Combinado, Dirección: Antihorario");
  }
  else if (cmd == "7")
  {
    isMoving = false; // Detener el movimiento
    bluetooth.println("Motor detenido");
  }
  else if (cmd == "8")
  {
    isMoving = true; // Continuar el movimiento
    bluetooth.println("Motor en movimiento");
  }
  else
  {
    bluetooth.println("Comando no reconocido"); // Mensaje si el comando no coincide
  }
}

// Función para mover el motor según la secuencia de pasos
void moveMotor(int steps[][4], int stepCount)
{
  // Iterar sobre la cantidad de pasos especificada
  for (int i = 0; i < stepCount; i++)
  {
    // Calcular el índice según la dirección del movimiento
    int idx = clockwise ? i : (stepCount - 1 - i);
    
    // Establecer los pines de salida según la secuencia de pasos
    digitalWrite(IN1, steps[idx][0]);
    digitalWrite(IN2, steps[idx][1]);
    digitalWrite(IN3, steps[idx][2]);
    digitalWrite(IN4, steps[idx][3]);
    
    // Esperar un tiempo determinado antes de continuar al siguiente paso
    delay(stepDelay);
    
    // Comprobar si hay un nuevo comando antes de completar el ciclo de pasos
    if (bluetooth.available())
    {
      command = bluetooth.readString(); // Leer el nuevo comando
      command.trim(); // Eliminar espacios en blanco
      handleCommand(command); // Manejar el nuevo comando
      break; // Salir del bucle si hay un nuevo comando
    }
  }
}
