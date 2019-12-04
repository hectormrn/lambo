#include <DHT.h>
#include <LiquidCrystal.h>

const int pinecho = 8;
const int pintrigger = 9;
const int pinled = 13;
//String topMessage = "";

//start constansts - Leds (Faros del auto)
const int pinFaroDelantero = 20; 
const int pinFaroTrasero = 21;
//end constants

//start constansts - sensor humedad
#define DHTPIN 19
#define DHTTYPE DHT11  // Dependiendo del tipo de sensor
DHT dht(DHTPIN, DHTTYPE); // Inicializamos el sensor DHT11
//end constanst

//Inicializamos una instancia de LiquidCrystal class
LiquidCrystal lcd(7,6,5,4,3, 2);
//end

unsigned int tiempo, distancia;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinecho, INPUT);
  pinMode(pintrigger, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(pinFaroDelantero, OUTPUT);

  //LCD code...
  lcd.begin(16, 2);
  lcd.print("- CYBER-LAMBO -");
  //end
  
  // inicializa el sensor DHT
  dht.begin();
  getTemperaturaYHumedad();
}

void loop() {
  digitalWrite(pinFaroDelantero, HIGH);
  delay(50);
  digitalWrite(pinFaroDelantero,LOW);
  delay(50);
  digitalWrite(pintrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pintrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pintrigger, LOW);

  tiempo = pulseIn(pinecho, HIGH);
  distancia = tiempo / 58;
  Serial.print(distancia);
  Serial.println(" cm ");
  delay(200);
  

  if(distancia <= 15 ) {
    Serial.println("====== Aqui debemos prender los FAROS ======");
    digitalWrite(pinFaroTrasero, HIGH);
    digitalWrite(13, HIGH);
    delay(500);  
    clearLCDLine(1);
    pintLcdMessage("Alerta! impacto!");
  }else{
    digitalWrite(pinFaroTrasero, LOW);
    digitalWrite(13, LOW);  
    clearLCDLine(1);
    pintLcdMessage("Avanza (^_^)");
  }
  
}

//Print messages LCD
void pintLcdMessage(String message) {
  lcd.setCursor(0, 1);
  lcd.print(message);
}

void clearLCDLine(int line)
{               
  lcd.setCursor(0,line);
  // 20 indicates symbols in line. For 2x16 LCD write - 16
  for(int n = 0; n < 20; n++) 
  {
     lcd.print(" ");
  }
}

//Leer temperatura y humedad
void getTemperaturaYHumedad() {
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
  
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  // Calcular el índice de calor en Fahreheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Índice de calor: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
  //topMessage = "Humedad " + h + "Temperatura " + t; 
  //return topMessage;
  
}
