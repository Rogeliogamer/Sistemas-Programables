//pines de led
int ledV = 4;
int ledA = 5;
int ledR = 6;

//variables
int valorAng = 0;
float voltaje = 0;
float voltajeAnterior = 0;

//niveles
float cargado = 1.5;
float medio = 1.3;
float minimo = 0.3;

void setup()
{
  Serial.begin(9600);
  pinMode(ledV,OUTPUT);
  pinMode(ledA,OUTPUT);
  pinMode(ledR,OUTPUT);
}

void loop()
{
  //Leemos entrada analogica
  valorAng = analogRead(A0);

  //obtenemos el voltaje
  voltaje = 0.0048 * valorAng;

  //Serial.print("Voltaje: " + voltaje);
  // Solo imprimimos si hay un cambio significativo en el voltaje
  if (abs(voltaje - voltajeAnterior) > 0.01)
  {
    Serial.print("Voltaje: ");
    Serial.println(voltaje);
    voltajeAnterior = voltaje; // Actualizamos el valor anterior
  }

  if(voltaje >= cargado)
  {
    digitalWrite(ledV,HIGH);
    delay(1000);
  }
  else if(voltaje < cargado && voltaje >= medio)
  {
    digitalWrite(ledA,HIGH);
    delay(1000);
  }
  else if(voltaje < medio && voltaje >= minimo)
  {
    digitalWrite(ledR,HIGH);
    delay(1000);
  }
  else 
  {
    digitalWrite(ledR,LOW);
    digitalWrite(ledA,LOW);
    digitalWrite(ledV,LOW);
  }

  delay(500);
}