#include <Wire.h>
#include <Servo.h>
Servo xservo;
Servo yservo;
int mpu=0x68;
int c=0;
int errLoop=500;
float prevtime;
float currtime;
float elapsedtime;
int Accxhreg=0x3B, Accxlreg=0x3C;
int Accyhreg=0x3D, Accylreg=0x3E;
int Acczhreg=0x3F, Acczlreg=0x40;
int Accxhval, Accxlval;
int Accyhval, Accylval;
int Acczhval, Acczlval;
float Accx, Accy, Accz;
float AccErrorX, AccErrorY;
float AccAnglex, AccAngley;

int Gyroxhreg=0x43, Gyroxlreg=0x44;
int Gyroyhreg=0x45, Gyroylreg=0x46;
int Gyrozhreg=0x47,Gyrozlreg=0x48;
int Gyroxhval, Gyroxlval;
int Gyroyhval, Gyroylval;
int Gyrozhval, Gyrozlval;
float Gyrox, Gyroy,Gyroz;
float GyroErrorX, GyroErrorY,GyroErrorZ;
float GAngleX, GAngleY, GAngleZ;
float roll, pitch, yaw;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Wire.begin();
  Wire.beginTransmission(mpu);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
 
  
  while(c<errLoop){
    Wire.beginTransmission(mpu);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(mpu,6,true);
    Accx=(Wire.read() << 8 | Wire.read())/16384.0;
    Accy=(Wire.read() << 8 | Wire.read())/16384.0;
    Accz=(Wire.read() << 8 | Wire.read())/16384.0;
//    Serial.println(Accx);
//    Serial.println(Accy);
//    Serial.println(Accz);
   // delay(1000);
    AccErrorX = AccErrorX + ((atan((Accy) / sqrt(pow((Accx), 2) + pow((Accz), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (Accx) / sqrt(pow((Accy), 2) + pow((Accz), 2))) * 180 / PI));
    c++;
  }
  AccErrorX=AccErrorX/errLoop;
  AccErrorY=AccErrorY/errLoop;
  
  c=0;
  while (c < errLoop) {
    Wire.beginTransmission(mpu);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(mpu, 6, true);
    Gyrox = (Wire.read() << 8 | Wire.read())/131.0;
    Gyroy = (Wire.read() << 8 | Wire.read())/131.0;
    Gyroz = (Wire.read() << 8 | Wire.read())/131.0;
    
    GyroErrorX = GyroErrorX + Gyrox;
    GyroErrorY = GyroErrorY + Gyroy;
    GyroErrorZ = GyroErrorZ + Gyroz ;
    c++;
  }
  GyroErrorX = GyroErrorX / errLoop;
  GyroErrorY = GyroErrorY / errLoop;
  GyroErrorZ = GyroErrorZ / errLoop;
  Serial.print("GyroErrorX = ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY = ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ = ");
  Serial.println(GyroErrorZ);
  Serial.print("AccErrorX = ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY = ");
  Serial.println(AccErrorY);
  //delay(10000);

  xservo.attach(2);
  yservo.attach(11);
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(mpu);
  Wire.write(Accxlreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Accxlval=Wire.read();
  Wire.beginTransmission(mpu);
  Wire.write(Accxhreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Accxhval=Wire.read();
  Accx= ((Accxhval << 8) | Accxlval) / 16384.0;
  AccAnglex=((atan((Accy) / sqrt(pow((Accx), 2) + pow((Accz), 2))) * 180 / PI)) - AccErrorX + 1.75;
  
  Wire.beginTransmission(mpu);
  Wire.write(Accylreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Accylval=Wire.read();
  Wire.beginTransmission(mpu);
  Wire.write(Accyhreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Accyhval=Wire.read();
  Accy= ((Accyhval << 8) | Accylval)/16384;
  AccAngley= ((atan(-1 * (Accx) / sqrt(pow((Accy), 2) + pow((Accz), 2))) * 180 / PI)) - AccErrorY - 0.04;
  
  Wire.beginTransmission(mpu);
  Wire.write(Acczlreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Acczlval=Wire.read();
  Wire.beginTransmission(mpu);
  Wire.write(Acczhreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Acczhval=Wire.read();
  Accz= ((Acczhval << 8) | Acczlval)/16384;

 
  Wire.beginTransmission(mpu);
  Wire.write(Gyroxlreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Gyroxlval=Wire.read();
  Wire.beginTransmission(mpu);
  Wire.write(Gyroxhreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Gyroxhval=Wire.read();
  Gyrox= ((Gyroxhval << 8) | Gyroxlval)/131.0;
  
   Wire.beginTransmission(mpu);
  Wire.write(Gyroylreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Gyroylval=Wire.read();
  Wire.beginTransmission(mpu);
  Wire.write(Gyroyhreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Gyroyhval=Wire.read();
  Gyroy= ((Gyroyhval << 8) | Gyroylval)/131.0;
  
    Wire.beginTransmission(mpu);
  Wire.write(Gyrozlreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Gyrozlval=Wire.read();
  Wire.beginTransmission(mpu);
  Wire.write(Gyrozhreg);
  Wire.endTransmission();
  Wire.requestFrom(mpu,1);
  Gyrozhval=Wire.read();
  Gyroz= ((Gyrozhval << 8) | Gyrozlval)/131.0;
  
   prevtime=currtime;
   currtime=millis();
   elapsedtime=(currtime-prevtime)/1000;
  
  Gyrox = (Gyrox - GyroErrorX);
  GAngleX = GAngleX + Gyrox * elapsedtime;
  
  Gyroy = (Gyroy - GyroErrorY);
  GAngleY = GAngleY + Gyroy * elapsedtime;
  
  Gyroz = (Gyroz - GyroErrorZ);
  GAngleZ = GAngleZ + Gyroz * elapsedtime;
  
  roll = 0.96*GAngleX + 0.04*AccAnglex;
  pitch = 0.96*GAngleY + 0.04*AccAngley;
  yaw = GAngleZ;
  Serial.print(roll);
  Serial.print(" / ");
  Serial.print(pitch);
  Serial.print("/");
  Serial.println(yaw);
  xservo.write((int)roll + 90);
  yservo.write((int)pitch + 90);
}
