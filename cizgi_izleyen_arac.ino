#include <QTRSensors.h>

#define Kp 0.4 // Kendi deneyimlerinizle bulmanız gerekli küçük bir değer ile başlayıp, büyütebilirsiniz en kararlı Kp değerini bulana kadar.. 0.4
#define Kd 2 // Bunu da küçük bir değerden başlatın ve deneyimleyerek büyütün. ( Not: Kp < Kd) 2.0
#define rightMaxSpeed 90
#define leftMaxSpeed 89
#define rightBaseSpeed 60 // robot için kp ve kd değerlerini tutturduysanız şayet motorların dönmesi gereken hız budur
#define leftBaseSpeed 61 // yukarıdaki değer ile aynıdır


#define rightMotor1 7
#define rightMotor2 8
#define rightMotorPWM 9
#define leftMotor1 4
#define leftMotor2 5
#define leftMotorPWM 3


QTRSensors qtr;

const uint8_t SensorCount = 6;
unsigned int sensorValues[SensorCount];

int rightMotorSpeed = 0;
int leftMotorSpeed = 0;

void setup()
{
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, SensorCount);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); 

  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
    
  delay(1000); // Ana döngüye girmeden önce botu konumlandırmak için 1 saniye bekleyin
}
int lastError = 0;
void loop()
{
  unsigned int position = qtr.readLineWhite(sensorValues);
  
  Serial.println(position);
  /*
  for (uint8_t i = 0; i < SensorCount; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
    Serial.println(position);
  */
  
  int error = position - 2500;
  int motorSpeed = Kp * error + Kd * (error - lastError);
  if((abs(error-lastError)) >= 300)
    {
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      delay(200);
      /*
      if(motorSpeed > 0)
      {
        digitalWrite(leftMotor1, LOW);
        digitalWrite(leftMotor2, HIGH);
        delay(1000);
      }
      if(motorSpeed < 0)
      {
        digitalWrite(rightMotor1, LOW);
        digitalWrite(rightMotor2, HIGH);
        delay(1000);
      }
      */
    }
  
  rightMotorSpeed = rightBaseSpeed + motorSpeed;
  leftMotorSpeed = leftBaseSpeed - motorSpeed;
  if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed;
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed;
  if (rightMotorSpeed < 0) rightMotorSpeed = 0;
  if (leftMotorSpeed < 0) leftMotorSpeed = 0;
  lastError = error;
  
  
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  analogWrite(rightMotorPWM, rightMotorSpeed);

  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  analogWrite(leftMotorPWM, leftMotorSpeed);
  
}
