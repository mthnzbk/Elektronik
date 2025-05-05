#include <avr/sleep.h>
#include <avr/power.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <MPU6050.h>
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

const int MPU_addr1 = 0x68;
float xa, ya, za, roll, pitch;
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//MPU6050 Gyro(0x68);


int ivmeX, ivmeY, ivmeZ, gyroX, gyroY, gyroZ;
String msg;


unsigned long lastTime; // Son zaman kaydı
float P = 1, Q = 0.01, R = 0.1; // Kalman filtre parametreleri
//float roll, pitch; // Açılar
void setup() {
  
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(MPU_addr1);                 //begin, send the slave adress (in this case 68)
  Wire.write(0x6B);                                  //make the reset (place a 0 into the 6B register)
  Wire.write(0);
  Wire.endTransmission(true); 

  mpu6050.begin();
  //mpu6050.calcGyroOffsets(true);
  mpu6050.setGyroOffsets(0.02, -0.08, -0.16);

  // put your setup code here, to run once:
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);

  // // Harici kesmeyi ayarla (D2 pini, INT0 kesmesi)
  // attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);

  // //uyku modu
  // set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // sleep_enable();
  // sleep_cpu();
  // Gyro.initialize();
  // Gyro.setXGyroOffset(0);
  // Gyro.setYGyroOffset(0);
  // Gyro.setZGyroOffset(0);
  // Gyro.setXAccelOffset(0);
  // Gyro.setYAccelOffset(0);
  // Gyro.setZAccelOffset(0);

  // Gyro.CalibrateAccel(6);
  // Gyro.CalibrateGyro(6);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Merhaba, Metehan!");
  lcd.setCursor(0,1);
  lcd.print("Havalar Nasil!");

  lastTime = millis();
}

void loop() {
  Wire.beginTransmission(MPU_addr1);
  Wire.write(0x3B);  //send starting register address, accelerometer high byte
  Wire.endTransmission(false); //restart for read
  Wire.requestFrom(MPU_addr1, 6); //get six bytes accelerometer data
  int t = Wire.read();
  xa = (t << 8) | Wire.read();
  t = Wire.read();
  ya = (t << 8) | Wire.read();
  t = Wire.read();
  za = (t << 8) | Wire.read();
// formula from https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
  roll = atan2(ya , za) * 180.0 / PI;
  pitch = atan2(-xa , sqrt(ya * ya + za * za)) * 180.0 / PI; //account for roll already applied

  Serial.print("roll = ");
  Serial.print(roll,1);
  Serial.print(", pitch = ");
  Serial.println(pitch,1);
  delay(400);

  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());
  Serial.print("\ttemp : ");
  Serial.println(mpu6050.getTemp());
  // put your main code here, to run repeatedly:
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(2000);
  // digitalWrite(LED_BUILTIN, LOW);

  // sleep_enable();
  // sleep_cpu();
  // Gyro.getAcceleration(&ivmeX, &ivmeY, &ivmeZ);
  // Gyro.getRotation(&gyroX, &gyroY, &gyroZ);

  float Ax = ivmeX / 17000.0;
  float Ay = ivmeY / 17000.0;
  float Az = ivmeZ / 17000.0;

  float Gx = gyroX / 131.0;
  float Gy = gyroY / 131.0;

  // İvmeölçer ile ham Roll ve Pitch açılarını hesapla
  float accelRoll  = atan2(Ay, sqrt(Ax * Ax + Az * Az)) * 180 / PI;
  float accelPitch = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * 180 / PI;

  // Zaman farkını hesapla
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // ms --> saniye
  lastTime = currentTime;


  // Kalman Filtresi - 1. Tahmin Aşaması
  float roll_pred  = roll + Gx * dt;
  float pitch_pred = pitch + Gy * dt;
  P = P + Q;

  // Kalman Filtresi - 2. Güncelleme Aşaması
  float K = P / (P + R);
  roll  = roll_pred + K * (accelRoll - roll_pred);
  pitch = pitch_pred + K * (accelPitch - pitch_pred);
  P = (1 - K) * P;


  // Sonuçları yazdır
  // Serial.print("Roll: "); Serial.print(roll);
  // Serial.print(" | Pitch: "); Serial.println(pitch);

  delay(1000);

  // Serial.print("İvme: ");
  // Serial.print(Gyro.getAccelerationX());
  // Serial.print(" ");
  // Serial.print(Gyro.getAccelerationY());
  // Serial.print(" ");
  // Serial.println(Gyro.getAccelerationZ());

  // Serial.print("Sıcaklık: ");
  // Serial.println(Gyro.getTemperature());

  // Serial.print("Gyro: ");
  // Serial.print(gyroX);
  // Serial.print(" ");
  // Serial.print(gyroY);
  // Serial.print(" ");
  // Serial.println(gyroZ);

  // delay(5000);

  if (Serial.available()) {
    delay(100);
    lcd.clear();
    while (Serial.available() > 0) {
      msg = Serial.readString();
      //msg += data;
      lcd.setCursor(0, 0);
      lcd.print(msg);
      lcd.setCursor(0, 1);
      lcd.print(msg);
      //delay(5);
    }
  }
}

// void wakeUp() {
//   // Uyandırma
//   sleep_disable();
//   detachInterrupt(digitalPinToInterrupt(2));
// }
