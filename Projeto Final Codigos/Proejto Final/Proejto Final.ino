#include <DHT.h>   
#include <SoftwareSerial.h>
#define DHTPIN 3
#define DHTTYPE DHT11 
DHT dht (DHTPIN, DHTTYPE);
SoftwareSerial mySerial(5, 6);

void setup() {
  pinMode(3, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(100);
}

void loop() {
  delay(400);
  int valorLuz = analogRead(A0);
  int valorUmidTerra = analogRead(A1);
  int valorChuva = analogRead(A2);
  int valorCalor = dht.readTemperature();
  int valorUmidade = dht.readHumidity();
  int alertar = 0;
  int valorFinal = 0;
  String mensagem;

  alertar = (2 * medirCalor(valorCalor, valorUmidade)) + medirLuz(valorLuz) + medirUmidadeTerra(valorUmidTerra) + choveu(valorChuva);

  if(alertar <= 25 || alertar >= 17)
  {
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    valorFinal = 2;
  }
  else{
    if(alertar <= 16 || alertar >= 10)
    {
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      valorFinal = 1;
    }
    else{
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(9, HIGH);
      valorFinal = 0;
    }
  }

  if (Serial.available()>0)
   switch(valorFinal)
  {
    case 2:
      SendMessageRed();
      break;
    case 1: 
      SendMessageYw();
      break;
  }
  delay(3000000);
}

 void SendMessageRed()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Alerta! O local em que a SPIA está corre um grande risco de incêndio.");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SendMessageYw()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Os riscos de incêndio são medianos, mesmo assim continue tomando cuidado.");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
int medirLuz(int luz)
{
  int sol = 0;
  if(luz > 1000)
  {
    sol = 3;
  }
  else
  {
    if(luz <= 1000 || luz >= 700)
    {
      sol = 2;
    }
    else{
      sol = 0;
    }
  }
  return sol;
}

int medirUmidadeTerra(int umidade)
{
  int umid = 0;
  int faixa = map(umidade, 0, 1023, 1, 4);
  switch (faixa){
    case 1:
      umid = 2;
    break;
    case 2:
      umid = 4;
    break;
    case 3:
      umid = 6;
    break;
    case 4:
      umid = 8;
    break;
  }
  return umid;
}

int choveu(int chuva)
{
  int tempo;
  int faixa = map(chuva, 0, 1023, 0 , 100);
  if(faixa > 2)
  {
    tempo = 0;
  }
  else{
    tempo = 4;
  }
  return tempo;
}

int medirCalor(int calor, int umidade)
{
  int risco = 0;
  int resultado = 0;
  if(umidade <= 30)
  {
    risco =+ 5;
  }
  else
  {
    if(umidade >= 30 || umidade <= 50)
    {
      risco =+ 3;
    }
    else
    {
      if(umidade >= 50 || umidade <= 60){
        risco =+ 1;
      }
      else{
        if(umidade > 60){
          risco =- 3;
        }
      }
    }
  }
  
  if(calor >= 35)
  {
    risco =+ 5;
  }
  else{
    if(calor <= 35 || calor >= 30)
    {
      risco =+ 3;
    }
    else{
      if(calor <= 30 || calor >= 25)
      {
        risco =+ 1;
      }
      else
      {
        risco =- 3;
      }
    }
  }

  if(risco >= 8)
  {
    resultado = 5;
  }
  else{
    if(risco <= 7 || risco >= 5){
      resultado = 4;
    }
    else{
      if(risco <= 4 || risco >= 2){
        resultado = 3;
      }
      else{
        resultado = 1;
      }
    }
  }

  return resultado;
}