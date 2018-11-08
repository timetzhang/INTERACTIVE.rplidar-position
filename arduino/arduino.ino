
#include <Keyboard.h>
#include <RPLidar.h>

#define BUTTON 12 //
// You need to create an driver instance
RPLidar lidar;
uint8_t keys[] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
long lastKeyTime;

int operation;
#define RPLIDAR_MOTOR 3 // The PWM pin   for   ontrol the speed of RPLIDAR's motor.
// This pin should connected with the RPLIDAR's MOTOCTRL signal


void setup() {
  // bind the RPLIDAR driver to the arduino hardware serial
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(BUTTON, INPUT);

  Keyboard.begin();
  lidar.begin(Serial1);

  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON) == HIGH) {
    if (IS_OK(lidar.waitPoint())) {
      float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
      float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
      bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
      byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

      //右边1/4半球度数处理
      if (angle >= 270) {
        angle = 360 - angle;
        operation = 1;
      }
      else {
        operation = -1; //左边为-x
      }

      int x = operation * sin(angle * (PI / 180)) * distance;
      int y = cos(angle * (PI / 180)) * distance;

      if (x != 0 && y != 0 && x > -1700 && x < 1700 && y < 1300 && ((angle >= 0 && angle <= 90) || (angle >= 270 && angle <= 360))) {
        //      Serial.print("distance:");
        //      Serial.print(distance);
        //      Serial.print("/");


        //将角度转成弧度 angle * (PI / 180)
        //      Serial.print("angle:");
        //      Serial.print(angle);
        //      Serial.print("/");
        //      Serial.print("sin(angle):");
        //      Serial.print(sin(angle * (PI / 180)));
        //      Serial.print("/");
        //      Serial.print("cos(angle):");
        //      Serial.print(cos(angle * (PI / 180)));
        //      Serial.print("/");

        if (millis() > lastKeyTime + 100) {
          for (int i = 0; i < 8; i++) {
            Keyboard.release(keys[i]);
          }
          Keyboard.press(keys[int(map(x, -1000, 1000, 0, 9))]);
          lastKeyTime = millis();
        }

        Serial.print(x);
        Serial.print(",");
        Serial.println(y);
        delay(50);
      }
    } else {
      analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

      // try to detect RPLIDAR...
      rplidar_response_device_info_t info;
      if (IS_OK(lidar.getDeviceInfo(info, 100))) {
        // detected...
        lidar.startScan();

        // start motor rotating at max allowed speed
        analogWrite(RPLIDAR_MOTOR, 190);
        delay(1000);
      }
    }
  }
  if (digitalRead(BUTTON) == LOW) {
    for (int i = 0; i < 8; i++) {
      Keyboard.release(keys[i]);
    }
    analogWrite(RPLIDAR_MOTOR, 0);
    delay(1000);
  }
}
