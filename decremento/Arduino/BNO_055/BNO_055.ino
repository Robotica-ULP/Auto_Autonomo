#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>

Servo servo;
/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3-5V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
   2015/AUG/27  - Added calibration and system status helpers
*/

/* Set the delay between fresh samples */
#define delay_frec_muestreo (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

int pos_servo(int angle)
{
  int pos = 90;
  if (angle > 180 && angle < 358)
  {
    pos -= (angle - 360) * 3.2; /////DERECHA
    if (pos > 165)
    {
      servo.write(165);
      Serial.print("lim  ");
    }
    else
    {
      servo.write(pos);
    }
  }
  else if (angle > 0 && angle <= 180)
  {
    pos -= (angle) * 3.2; ////IZQUIERDA
    if (pos < 20)
    {
      servo.write(20);
      Serial.print("lim  ");
    }
    else
    {
      servo.write(pos);
    }
  }
  else
  {
    servo.write(pos);
  }
  Serial.println(pos);
}
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void displaySensorStatus(void)
{
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  servo.attach(6);
  servo.write(180);
  delay(700);
  servo.write(90);
  delay(9000);
  //el servo de la direccion va desde 20 a 165
  int i = 100, b = 10;
  while (i != 90)
  {
    servo.write(i);
    delay(30);
    servo.write(i - b);
    delay(30);
    i--;
    b--;
  }
  /*servo.write(100);
    delay(500);
    servo.write(80);
    delay(50);
    servo.write(90);
    delay(30);
  */
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);

  /* Display some basic information on this sensor */

  //displaySensorDetails();

  /* Optional: Display current status */

  //displaySensorStatus();
  servo.write(85);
  delay(15);
  servo.write(95);
  delay(15);
  servo.write(90);
  delay(15);
  bno.setExtCrystalUse(true);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  int pos = 90;
  sensors_event_t event;
  bno.getEvent(&event);

  /* Display the floating point data */


  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("       ");
  /*
    Serial.print("\tY: ");
    Serial.print(event.orientation.y, 4);
    Serial.print("\tZ: ");
    Serial.print(event.orientation.z, 4);
  */


  pos_servo(event.orientation.x);
  delay(100);
  /* Optional: Display calibration status */

  //displayCalStatus();

  /* Optional: Display sensor status (debug only) */

  //displaySensorStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting nex data */
  //delay(delay_frec_muestreo);
}
