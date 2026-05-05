         /////////////////////////////////////////////  
        //     AI-driven Web-based Ancillary       //
       //     Lab Assistant | UNO Q & Gemini      //
      //             ---------------             //
     //             (Arduino UNO Q)             //           
    //             by Kutluhan Aktar           // 
   //                                         //
  /////////////////////////////////////////////

//
// Based on Arduino UNO Q, this lab assistant is able to show real-time sensor readings, identify users by fingerprint, detect lab equipment via object detection, generate AI lessons via Gemini, and talk through its full-fledged web dashboard.
//
// For more information:
// https://www.hackster.io/kutluhan-aktar/ai-driven-web-based-ancillary-lab-assistant-uno-q-gemini-b6d539
//
//
// Connections
// Arduino UNO Q :  
//                                Capacitive Fingerprint Sensor (UART)
// 3.3V    ------------------------ VIN
// GND     ------------------------ GND
// D1 / USART1_TX ----------------- RX
// D0 / USART1_RX ----------------- TX
// 3.3V    ------------------------ 3V3
//                                Gravity: Electrochemical Alcohol Sensor
// 3.3V    ------------------------ +
// GND     ------------------------ -
// SCL     ------------------------ C/R
// SDA     ------------------------ D/T
//                                Gravity: 1Kg Weight Sensor Kit - HX711
// 3.3V    ------------------------ VCC
// GND     ------------------------ GND
// SCL     ------------------------ SCL
// SDA     ------------------------ SDA
//                                Gravity: Geiger Counter Module - Ionizing Radiation Detector
// GND     ------------------------ -
// 3.3V    ------------------------ +
// D2      ------------------------ D
//                                Gravity: Electrochemical Nitrogen Dioxide Sensor - NO2
// 3.3V    ------------------------ +
// GND     ------------------------ -
// SCL     ------------------------ C/R
// SDA     ------------------------ D/T
//                                Grove - Integrated Pressure Sensor Kit - MPX5700AP
// GND     ------------------------ GND
// 3.3V    ------------------------ VCC
// A0      ------------------------ SIG
//                                Grove - Water Atomization Sensor - Ultrasonic
// GND     ------------------------ GND
// 5V      ------------------------ VCC
// D4      ------------------------ EN
//                                Gravity: GNSS Positioning Module
// 3.3V    ------------------------ +
// GND     ------------------------ -
// SCL     ------------------------ C/R
// SDA     ------------------------ D/T
//                                Waveshare - 1.28inch Round LCD Display Module
// 3.3V    ------------------------ VCC
// GND     ------------------------ GND
// D11     ------------------------ DIN
// D13     ------------------------ CLK
// D10     ------------------------ CS
// D7      ------------------------ DC
// D8      ------------------------ RST
// D9      ------------------------ BL
//                                Control Button (A)
// A1      ------------------------ +
//                                Control Button (B)
// A2      ------------------------ +
//                                Control Button (C)
// A3      ------------------------ +
//                                Control Button (D)
// A4      ------------------------ +
//                                5mm Common Anode RGB LED
// D3      ------------------------ R
// D5      ------------------------ G
// D6      ------------------------ B


// Include the required libraries:
#include <Arduino_RouterBridge.h>
#include <DFRobot_ID809.h>
#include "DFRobot_Alcohol.h"
#include <DFRobot_HX711_I2C.h>
#include "DFRobot_MultiGasSensor.h"
#include <DFRobot_Geiger.h>
#include "DFRobot_GNSS.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

// Import the custom menu (interface) logos — icons.
#include "logo.h"

// Import the custom color theme.
#include "color_theme.h"

// Define the round LCD (GC9A01) screen configurations and declare the GC9A01 class instance.
#define SCREEN_WIDTH   240
#define SCREEN_HEIGHT  240
#define TFT_DC         D7
#define TFT_CS         D10
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

// Define the class instance for the capacitive fingerprint sensor (UART).
DFRobot_ID809 fingerprint;

// Define the configurations and the class instance for the Geiger counter module (ionizing radiation).
#define geiger_detect_pin D2
DFRobot_Geiger geiger(geiger_detect_pin);

// Define the configurations and the class instance for the electrochemical alcohol sensor.
/*
  1) The available collection range is between 1 and 100. The sensor generates the final result as the average of the given number (collection range) of the latest data collection array items.
 
  2) The available I2C addresses are as follows. Please use the onboard the DIP switch to change the default I2C address. 
                        | A0 | A1 |  
    ALCOHOL_ADDRESS_0   | 0  | 0  |   0x72
    ALCOHOL_ADDRESS_1   | 1  | 0  |   0x73
    ALCOHOL_ADDRESS_2   | 0  | 1  |   0x74
    ALCOHOL_ADDRESS_3   | 1  | 1  |   0x75 (Default)
*/
#define alcohol_collect_num  10
DFRobot_Alcohol_I2C alcohol_sensor(&Wire, ALCOHOL_ADDRESS_3);

// Define the configurations and the class instance for the electrochemical nitrogen dioxide (NO2) sensor.
/*
  1) The available I2C addresses are as follows. Please use the onboard the DIP switch to change the default I2C address.
    | A0 | A1 |  
    | 0  | 0  |   0x74 (Default)
    | 0  | 1  |   0x75
    | 1  | 0  |   0x76
    | 1  | 1  |   0x77    
*/
DFRobot_GAS_I2C no2_gas_sensor(&Wire, 0x74);

// Define the configurations and the class instance for the HX711 weight sensor.
/*
  1) The available I2C addresses are as follows. Please use the onboard the DIP switch to change the default I2C address.
    | A0 | A1 |  
    | 0  | 0  |   0x64 (Default)
    | 1  | 0  |   0x65
    | 0  | 1  |   0x66
    | 1  | 1  |   0x67 
*/
DFRobot_HX711_I2C weight_sensor(&Wire,/*addr=*/0x64);

// Define the configurations and the class instance for the GNSS positioning module
/*
  1) The default I2C address is 0x20.
  2) Once the module acquires a GPS signal successfully, the onboard LED should turn from red to green.
*/
DFRobot_GNSS_I2C gnss_sensor(&Wire ,GNSS_DEVICE_ADDR);

// Declare the required variables to calculate the pressure applied to the integrated pressure sensor (MPX5700AP) by creating a struct — pressure_sensor.
struct pressure_sensor {
  int c_pin = A0;
  int raw_value; 
  const int collection_range = 10;
  const int offset = 410; // zero pressure adjust
  const int full_scale = 9630; // max pressure (span) adjust
};

// Declare the switch pin of the ultrasonic water atomization sensor.
#define water_atomization_pin    D4

// Define the control button pin configurations.
#define control_button_A         A1
#define control_button_B         A2
#define control_button_C         A3
#define control_button_D         A4

// Define the RGB LED pin configurations.
#define red_pin                  D3
#define green_pin                D5
#define blue_pin                 D6

// If you need to print sensor readings and system notifications on the App Lab monitor for debugging, change the __debug_monitor value to true.
volatile boolean __debug_monitor = false;

// Declare the necessary parameters for the sensor readings by creating a struct — sensor_readings. 
struct sensor_readings {
  unsigned long latest_read_time, read_offset = 1000000;
  float pressure;
  float alcohol_concentration;
  float weight;
  struct _no2{ float concentration; int board_temp; }; struct _no2 _no2;
  struct _geiger{ int cpm, nsvh, usvh; }; struct _geiger _geiger;
  struct _gnss{ String date, utc; char lat_dir, lon_dir; double latitude, longitude, altitude, sog, cog; }; struct _gnss _gnss;
  String water_atomization = "OFF";
};

// Define data holders:
struct pressure_sensor pressure_sensor;
struct sensor_readings sensor_readings;
volatile boolean shown_screen_sensor = false;
int current_sensor_screen = 0, total_sensor_screen = 16;
volatile boolean shown_screen_home = false, activated_screen_home = true;
volatile boolean shown_screen_fingerprint = false, activated_screen_fingerprint = false;
String ongoing_fingerprint_task = "check_id";
int provided_user_id = 0;
volatile boolean deleteAllScans = false;


void setup(){
  /*
    Initiate the Arduino Router background Linux service to borrow and run functions between Qualcomm MPU and STM32 MCU interchangeably.
  */
  Bridge.begin();

  // Initiate the integrated App Lab monitor for debugging if necessary.
  //Monitor.begin();

  // Employ the Arduino Router background Linux service to enable Qualcomm MPU to borrow and run these functions on STM32 MCU.
  Bridge.provide("interface_web_control", interface_web_control);
  
  // Initiate the hardware serial port to communicate with the capacitive fingerprint sensor.
  Serial.begin(115200);
  delay(1000);
  
  // Register pin configurations.
  pinMode(control_button_A, INPUT_PULLUP); pinMode(control_button_B, INPUT_PULLUP); pinMode(control_button_C, INPUT_PULLUP); pinMode(control_button_D, INPUT_PULLUP);
  pinMode(red_pin, OUTPUT); pinMode(green_pin, OUTPUT); pinMode(blue_pin, OUTPUT); adjustColor(0,0,0);
  pinMode(water_atomization_pin, OUTPUT); digitalWrite(water_atomization_pin, LOW);
  
  // Initiate the GC9A01 round LCD screen.
  tft.begin();

  // Initiate the fingerprint sensor on the hardware serial port.
  fingerprint.begin(Serial);
  // Check the fingerprint sensor connection status.
  while(fingerprint.isConnected() == false){
    //if(__debug_monitor) Monitor.println("Error => Fingerprint sensor connection failed!");
    show_err_screen("Fingerprint", "Connection Failed!", "UART");
    delay(1000);
  }
  // After conducting experiments with the fingerprint sensor, you can clear the whole fingerprint library — ID (1-80).
  if(deleteAllScans){ fingerprint.delFingerprint(DELALL); delay(1000); }

  // Initiate the electrochemical alcohol sensor and check its connection status.
  while(!alcohol_sensor.begin()){
    //if(__debug_monitor) Monitor.println("Error => Alcohol sensor connection failed!");
    show_err_screen("Alcohol", "Connection Failed!", "I2C");
    delay(1000);
  }
  // Once the alcohol sensor starts, declare its data reading modes: MEASURE_MODE_AUTOMATIC (default) or MEASURE_MODE_PASSIVE.
  alcohol_sensor.setModes(MEASURE_MODE_AUTOMATIC);

  // Initiate the HX711 weight sensor and check its connection status.
  while (!weight_sensor.begin()) {
    //if(__debug_monitor) Monitor.println("Error => Weight sensor connection failed!");
    show_err_screen("Weight", "Connection Failed!", "I2C");
    delay(1000);
  }
  // Once the weight sensor starts, assign the calibration weight value (g) and the calibration trigger threshold value (g) for automatic calibration.
  weight_sensor.setCalWeight(50);
  weight_sensor.setThreshold(30);
  weight_sensor.setCalibration(weight_sensor.getCalibration());
  delay(1000);

  // Initiate the electrochemical nitrogen dioxide (NO2) gas sensor and check its connection status.
  while(!no2_gas_sensor.begin()){
    //if(__debug_monitor) Monitor.println("Error => NO2 gas sensor connection failed!");
    show_err_screen("NO2", "Connection Failed!", "I2C");
    delay(1000);
  }
  // Once the NO2 gas sensor starts, declare its data reading mode: INITIATIVE or PASSIVITY.
  no2_gas_sensor.changeAcquireMode(no2_gas_sensor.PASSIVITY); delay(1000);
  no2_gas_sensor.setTempCompensation(no2_gas_sensor.OFF); // ON or OFF
   
  // Initiate the GNSS positioning module and check its connection status.
  while(!gnss_sensor.begin()){
    //if(__debug_monitor) Monitor.println("Error => GNSS module connection failed!");
    show_err_screen("GNSS", "Connection Failed!", "I2C");
    delay(1000);
  }
  // Once the GNSS module starts, define its essential configurations.
  gnss_sensor.enablePower();
  /*
    1) Available combinations to select GNSS bands to acquire information are as follows.
      eGPS              use gps
      eBeiDou           use beidou
      eGPS_BeiDou       use gps + beidou
      eGLONASS          use glonass
      eGPS_GLONASS      use gps + glonass
      eBeiDou_GLONASS   use beidou +glonass
      eGPS_BeiDou_GLONASS use gps + beidou + glonass    
  */
  gnss_sensor.setGnss(eGPS_BeiDou_GLONASS);
  gnss_sensor.setRgbOn(); // gnss_sensor.setRgbOff(); 

  // Initiate the Geiger counter module.
  geiger.start();

  // After successfully setting up all sensors, define the current time (microseconds) to perform precise subsequent readings.
  sensor_readings.latest_read_time = micros();

}

void loop(){
  // According to their specific data generation intervals, obtain sensor readings from each sensor without suspending code flow.
  obtain_sensor_readings(sensor_readings.read_offset);

  // Update the current sensor data screen number once the control button A or the control button B is pressed.
  if(!digitalRead(control_button_A)){
    current_sensor_screen++;
    if(current_sensor_screen >= total_sensor_screen) current_sensor_screen = 0;   
    shown_screen_sensor = false;
    activated_screen_home = false; shown_screen_home = false;
    activated_screen_fingerprint = false; shown_screen_fingerprint = false;
    delay(500);
  }

  if(!digitalRead(control_button_B)){
    current_sensor_screen--;
    if(current_sensor_screen < 0) current_sensor_screen = total_sensor_screen-1;   
    shown_screen_sensor = false;
    activated_screen_home = false; shown_screen_home = false;
    activated_screen_fingerprint = false; shown_screen_fingerprint = false;     
    delay(500);
  }  

  // If the home interface or the fingerprint ID test interface are not activated, display the requested sensor data screen.
  if(!activated_screen_home && !activated_screen_fingerprint){
    switch(current_sensor_screen){
      case 0:
        show_sensor_screen("NO2", "Concentration", String(sensor_readings._no2.concentration), "PPM", 0);
      break;
      case 1:
        show_sensor_screen("NO2", "Board Temp.", String(sensor_readings._no2.board_temp), "C", 0);
      break;
      case 2:
        show_sensor_screen("Alcohol", "Concentration", String(sensor_readings.alcohol_concentration), "PPM", 1);
      break; 
      case 3:
        show_sensor_screen("Weight", "Estimation", String(sensor_readings.weight), "G (g)", 2);
      break; 
      case 4:
        show_sensor_screen("Geiger", "Ionizing", String(sensor_readings._geiger.cpm), "CPM", 3);
      break; 
      case 5:
        show_sensor_screen("Geiger", "Ionizing", String(sensor_readings._geiger.nsvh), "nSv/h", 3);
      break;
      case 6:
        show_sensor_screen("Geiger", "Ionizing", String(sensor_readings._geiger.usvh), "uSv/h", 3);
      break; 
      case 7:
        show_sensor_screen("Pressure", "Integrated", String(sensor_readings.pressure), "kPa", 4);
      break;
      case 8:
        show_sensor_screen("Water", "Atomization", sensor_readings.water_atomization, "V", 5);
      break; 
      case 9:
        show_sensor_screen("GNSS", "Date", sensor_readings._gnss.date, "Y/M/D", 6);
      break; 
      case 10:
        show_sensor_screen("GNSS", "UTC", sensor_readings._gnss.utc, "H_M_S", 6);
      break;       
      case 11:
        show_sensor_screen("GNSS", "Latitude", String(sensor_readings._gnss.latitude), "Degrees", 6);
      break; 
      case 12:
        show_sensor_screen("GNSS", "Longitude", String(sensor_readings._gnss.longitude), "Degrees", 6);
      break; 
      case 13:
        show_sensor_screen("GNSS", "Altitude", String(sensor_readings._gnss.altitude), "M (m)", 6);
      break;
      case 14:
        show_sensor_screen("GNSS", "Speed Over Ground", String(sensor_readings._gnss.sog), "SOG", 6);
      break;  
      case 15:
        show_sensor_screen("GNSS", "Course Over Ground", String(sensor_readings._gnss.cog), "COG", 6);
      break;        
    }
  }

  // If the home (main) interface is activated, show it until the user requests to review a sensor data screen or initiate a fingerprint sensor task.
  if(activated_screen_home) show_home_screen();

  // Once the control button D is pressed, return to the home (main) interface.
  if(!digitalRead(control_button_D)) return_home();

  // If a fingerprint sensor task is initiated, show its respective interface until the sensor completes the given task and returns to the home interface.
  while(activated_screen_fingerprint){
    // Start the requested fingerprint sensor task.
    manage_fingerprint_task(ongoing_fingerprint_task, (uint8_t)provided_user_id);    
  }
  
  // If the control button C is pressed, initiate the 'check_id' fingerprint task manually.
  if(!digitalRead(control_button_C)){
    shown_screen_sensor = false;
    activated_screen_home = false; shown_screen_home = false;
    activated_screen_fingerprint = true; shown_screen_fingerprint = false;
    ongoing_fingerprint_task = "check_id";
    delay(500);
  } 
  
}

void obtain_sensor_readings(unsigned long read_offset){
  if(micros() - sensor_readings.latest_read_time >= read_offset){
    pressure_sensor.raw_value = 0;
    for(int x = 0; x < pressure_sensor.collection_range; x++) pressure_sensor.raw_value = pressure_sensor.raw_value + analogRead(pressure_sensor.c_pin);
    sensor_readings.pressure = (pressure_sensor.raw_value - pressure_sensor.offset) * 700.0 / (pressure_sensor.full_scale - pressure_sensor.offset);
    //if(__debug_monitor){Monitor.print("Pressure sensor raw value (A/D) is  "); Monitor.print(pressure_sensor.raw_value); Monitor.print("\nEstimated pressure is "); Monitor.print(sensor_readings.pressure);  Monitor.println(" kPa\n");}    
  }
  if(micros() - sensor_readings.latest_read_time >= 2*read_offset){
    sensor_readings.alcohol_concentration = alcohol_sensor.readAlcoholData(alcohol_collect_num);
    if(sensor_readings.alcohol_concentration == ERROR) sensor_readings.alcohol_concentration = -1;
    //if(__debug_monitor){ Monitor.print("Alcohol concentration is "); Monitor.print(sensor_readings.alcohol_concentration); Monitor.println(" PPM.\n"); }
  }
  if(micros() - sensor_readings.latest_read_time >= 3*read_offset){
    sensor_readings.weight = weight_sensor.readWeight();
    if(sensor_readings.weight < 0.5) sensor_readings.weight = 0;
    //if(__debug_monitor){ Monitor.print("Estimated weight is "); Monitor.print(sensor_readings.weight); Monitor.println(" g.\n"); }
    sensor_readings._geiger.cpm = geiger.getCPM();
    sensor_readings._geiger.nsvh = geiger.getnSvh();
    sensor_readings._geiger.usvh = geiger.getuSvh();
    //if(__debug_monitor){Monitor.print("CPM: "); Monitor.println(sensor_readings._geiger.cpm); Monitor.print("nSv/h: "); Monitor.println(sensor_readings._geiger.nsvh); Monitor.print("μSv/h "); Monitor.println(sensor_readings._geiger.usvh);}
  }
  if(micros() - sensor_readings.latest_read_time >= 4*read_offset){
    sensor_readings._no2.concentration = no2_gas_sensor.readGasConcentrationPPM();
    sensor_readings._no2.board_temp = no2_gas_sensor.readTempC();
    //if(__debug_monitor){ Monitor.print("NO2 concentration is: "); Monitor.print(sensor_readings._no2.concentration); Monitor.println(" PPM\n"); Monitor.print("NO2 sensor board temperature is: "); Monitor.print(sensor_readings._no2.board_temp); Monitor.println(" ℃\n"); }
  }
  if(micros() - sensor_readings.latest_read_time >= 5*read_offset){
    sTim_t utc = gnss_sensor.getUTC();
    sTim_t date = gnss_sensor.getDate();
    sLonLat_t lat = gnss_sensor.getLat();
    sLonLat_t lon = gnss_sensor.getLon();
    sensor_readings._gnss.date = String(date.year) + "/" + String(date.month) + "/" + String(date.date); sensor_readings._gnss.utc = String(utc.hour) + "_" + String(utc.minute) + "_" + String(utc.second);
    sensor_readings._gnss.lat_dir = (char)lat.latDirection; sensor_readings._gnss.lon_dir = (char)lon.lonDirection;
    sensor_readings._gnss.latitude = lat.latitudeDegree; sensor_readings._gnss.longitude = lon.lonitudeDegree;
    sensor_readings._gnss.altitude = gnss_sensor.getAlt();
    sensor_readings._gnss.sog = gnss_sensor.getSog(); // Speed Over Ground
    sensor_readings._gnss.cog = gnss_sensor.getCog(); // Course Over Ground
    //if(__debug_monitor){ Monitor.print("GNSS (latitude): ");  Monitor.print(sensor_readings._gnss.latitude); Monitor.print("GNSS (longitude): ");  Monitor.print(sensor_readings._gnss.longitude); Monitor.print("GNSS (altitude): ");  Monitor.print(sensor_readings._gnss.altitude); }
  }
  if(micros() - sensor_readings.latest_read_time >= 6*read_offset){
    // After collecting all sensor variables, invoke the borrowed Python function via the Arduino Router using MessagePack RPC.
    Bridge.call("update_sensor_on_app", sensor_readings.pressure, sensor_readings.alcohol_concentration, sensor_readings.weight, sensor_readings._no2.concentration, sensor_readings._no2.board_temp, sensor_readings._geiger.cpm, sensor_readings._geiger.nsvh, sensor_readings._geiger.usvh, sensor_readings._gnss.date, sensor_readings._gnss.utc, sensor_readings._gnss.lat_dir, sensor_readings._gnss.lon_dir, sensor_readings._gnss.latitude, sensor_readings._gnss.longitude, sensor_readings._gnss.altitude, sensor_readings._gnss.sog, sensor_readings._gnss.cog);
    // Restart the sensor reading timer.
    sensor_readings.latest_read_time = micros();    
  }
}

void show_sensor_screen(String title, String title_exp, String sensor_value, String sensor_unit, int _theme){
  int l_1_s = 6, l_2_s = 14, l_sp = 5;
  int divider_w = SCREEN_WIDTH, divider_h = SCREEN_HEIGHT/4;
  int title_w = (divider_w/5)*3, title_h = (divider_h/3)*2;
  int logo_r = 40;
  int panel_w = SCREEN_WIDTH-logo_r-(4*l_sp), panel_h = (2*logo_r)-(4*l_sp);
  int inner_panel_w = panel_w-logo_r, inner_panel_h = panel_h-(2*l_sp);
  int t_x_s = (logo_r+(2*l_sp)+logo_r-l_sp) + inner_panel_w/2;
  int t_h_s = (SCREEN_HEIGHT/2)+(1.5*l_sp)-(l_2_s/2);
  if(!shown_screen_sensor){
    adjustColor(1,0,1);
    tft.fillScreen(Q_teal);
    tft.fillRect(0, 0, divider_w, divider_h, Q_grey);
    tft.fillRoundRect((divider_w-title_w)/2, (divider_h/3)*2, title_w, title_h, 5, Q_golden);
    tft.setTextSize(2); tft.setTextColor(Q_light_grey);
    tft.setCursor((SCREEN_WIDTH-(title.length()*l_2_s))/2, ((divider_h/3)*2)+l_sp);
    tft.print(title);
    tft.setTextSize(1);
    tft.setCursor((SCREEN_WIDTH-(title_exp.length()*l_1_s))/2, ((divider_h/3)*2)+title_h-l_1_s-l_sp);
    tft.print(title_exp);
    tft.fillCircle(logo_r+(2*l_sp), (SCREEN_HEIGHT/2)+(1.5*l_sp), logo_r, Q_primary);
    tft.fillRoundRect(logo_r+(2*l_sp), (SCREEN_HEIGHT/2)+(1.5*l_sp)-(panel_h/2), panel_w, panel_h, 5, Q_primary);
    tft.drawBitmap(logo_r+(2*l_sp)-(sensor_logo_w[_theme]/2), (SCREEN_HEIGHT/2)+(1.5*l_sp)-(sensor_logo_h[_theme]/2), sensor_logo_bit[_theme], sensor_logo_w[_theme], sensor_logo_h[_theme], Q_white);
    tft.fillRect(logo_r+(2*l_sp)+logo_r-l_sp, (SCREEN_HEIGHT/2)+(1.5*l_sp)-(inner_panel_h/2), inner_panel_w, inner_panel_h, Q_cyan);
    tft.setTextSize(2); tft.setTextColor(Q_white);
    tft.setCursor(t_x_s-((sensor_value.length()*l_2_s)/2), t_h_s);
    tft.print(sensor_value);
    tft.fillRect(0, SCREEN_HEIGHT-divider_h, divider_w, divider_h, Q_grey);
    tft.setTextSize(2); tft.setTextColor(Q_cyan);
    tft.setCursor((SCREEN_WIDTH-(sensor_unit.length()*l_2_s))/2, SCREEN_HEIGHT-(divider_h/2)-(l_2_s/2));
    tft.print(sensor_unit);
  }else{
    tft.fillRect(logo_r+(2*l_sp)+logo_r-l_sp, (SCREEN_HEIGHT/2)+(1.5*l_sp)-(inner_panel_h/2), inner_panel_w, inner_panel_h, Q_cyan);
    tft.setTextSize(2); tft.setTextColor(Q_white);
    tft.setCursor(t_x_s-((sensor_value.length()*l_2_s)/2), t_h_s);
    tft.print(sensor_value);    
  }
  // Avoid flickering due to drawing the same interface consecutively.
  shown_screen_sensor = true;
}

void manage_fingerprint_task(String task, uint8_t requested_id){
  uint8_t result = 0;
  if(task == "check_id"){
    shown_screen_fingerprint = false;
    show_fingerprint_task_screen("Check ID", "Please scan finger!", Q_cyan, Q_primary);
    // Once the user places a finger onto the capacitive fingerprint sensor, capture the fingerprint image.
    if(fingerprint.collectionFingerprint(/*timeout=*/0) != ERR_ID809){
      // Then, notify the user that the fingerprint image captured successfully via the respective task interface.
      shown_screen_fingerprint = false;
      show_fingerprint_task_screen("Captured", "Remove finger!", Q_golden, Q_primary);
      // Wait until the user removes the captured finger.
      while(fingerprint.detectFinger());
      // Then, obtain the ID of the captured fingerprint if registered in the sensor's fingerprint library - ID(1-80).
      result = fingerprint.search();
      if(result != 0){
        // If the captured fingerprint is registered (enrolled):
        shown_screen_fingerprint = false;
        show_fingerprint_task_screen("ID: "+String(result), "Successful!", Q_green, Q_primary);
        delay(2000);
        // Return to the home interface.
        return_home();
      }else{
        // Otherwise, notify the user accordingly:
        shown_screen_fingerprint = false;
        show_fingerprint_task_screen("ID: N", "Not registered!", Q_magenta, Q_white);
        delay(2000);
        // Return to the home interface.
        return_home();        
      }
    }else{
      // If the sensor cannot capture fingerprints precisely, notify the user accordingly.
      shown_screen_fingerprint = false;
      show_fingerprint_task_screen("Error", "Cannot capture!", Q_red, Q_white);
      delay(2000);
      // Return to the home interface.
      return_home();        
    }
  }
  else if(task == "register_id"){
    uint8_t register_ID;
    int fingerprint_sampling_number = 3, current_sample = 0;
    shown_screen_fingerprint = false;
    show_fingerprint_task_screen("Register", "Please scan finger!", Q_cyan, Q_primary);
    // Obtain an available fingerprint ID from the sensor's fingerprint library - ID(1-80) - for registering the new fingerprint.
    register_ID = fingerprint.getEmptyID();
    if(register_ID != ERR_ID809){
      // Up to the given sampling number,capture fingerprint images consecutively.
      while(current_sample < fingerprint_sampling_number){
        // Once the user places a finger onto the capacitive fingerprint sensor, capture the fingerprint image.
        if(fingerprint.collectionFingerprint(/*timeout=*/0) != ERR_ID809){
          // Then, notify the user that the fingerprint image sample captured successfully via the respective task interface.
          shown_screen_fingerprint = false;
          show_fingerprint_task_screen("Captured ["+String(current_sample+1)+"]", "Remove finger!", Q_golden, Q_primary);
          // Proceed to capturing the following fingerprint image sample.
          current_sample++;
          // Wait until the user removes the captured finger.
          while(fingerprint.detectFinger());          
        }else{
          // If the sensor cannot capture fingerprint image samples precisely, notify the user accordingly.
          shown_screen_fingerprint = false;
          show_fingerprint_task_screen("Error", "Please reposition!", Q_red, Q_white);
          delay(2000);          
        }
      }
      // After capturing fingerprint image samples successfully, record the new fingerprint to the provided unregistered ID.
      if(fingerprint.storeFingerprint(/*Empty ID = */register_ID) != ERR_ID809){
        shown_screen_fingerprint = false;
        show_fingerprint_task_screen("Success ["+String(register_ID)+"]", "Registered!", Q_green, Q_primary);
        delay(2000);
        // Notify the Python backend accordingly via the borrowed function.
        Bridge.call("manage_account_actions_on_stm32", "signup", register_ID);        
        // Return to the home interface.
        return_home();
      }else{
        // If the sensor cannot save the new fingerprint to the provided ID, notify the user accordingly.
        shown_screen_fingerprint = false;
        show_fingerprint_task_screen("Error", "Cannot register!", Q_red, Q_white);
        delay(2000);
        // Notify the Python backend accordingly via the borrowed function.
        Bridge.call("manage_account_actions_on_stm32", "signup", -1);
        // Return to the home interface.
        return_home();          
      }
    }else{
      // If the sensor cannot produce an unregistered ID, notify the user accordingly.
      shown_screen_fingerprint = false;
      show_fingerprint_task_screen("Error", "Cannot find ID!", Q_red, Q_white);
      delay(2000);
      // Notify the Python backend accordingly via the borrowed function.
      Bridge.call("manage_account_actions_on_stm32", "signup", -2);      
      // Return to the home interface.
      return_home();       
    }
  }
  else if(task == "verify_id"){
    shown_screen_fingerprint = false;
    show_fingerprint_task_screen("Verify User", "Please scan finger!", Q_cyan, Q_primary);
    // Once the user places a finger onto the capacitive fingerprint sensor, capture the fingerprint image.
    if(fingerprint.collectionFingerprint(/*timeout=*/0) != ERR_ID809){
      // Then, notify the user that the fingerprint image captured successfully via the respective task interface.
      shown_screen_fingerprint = false;
      show_fingerprint_task_screen("Captured", "Remove finger!", Q_golden, Q_primary);
      // Wait until the user removes the captured finger.
      while(fingerprint.detectFinger());
      // Then, obtain the ID of the captured fingerprint if registered in the sensor's fingerprint library - ID(1-80).
      result = fingerprint.search();
      if(result != 0 && result == requested_id){
        // If the captured fingerprint is registered (enrolled) and its ID corresponds with the requested ID, verify the user to utilize the web application (dashboard).
        shown_screen_fingerprint = false;
        show_fingerprint_task_screen("Matched!", "User verified!", Q_green, Q_primary);
        delay(2000);
        // Notify the Python backend accordingly via the borrowed function.
        Bridge.call("manage_account_actions_on_stm32", "signin", result);        
        // Return to the home interface.
        return_home();
      }else{
        // Otherwise, notify the user accordingly and wait until the user scans the accurate fingerprint:
        shown_screen_fingerprint = false;
        show_fingerprint_task_screen("Try again!", "Not verified!", Q_magenta, Q_white);
        delay(2000);
        // Notify the Python backend accordingly via the borrowed function.
        Bridge.call("manage_account_actions_on_stm32", "signin", -1);
        // Return to the home interface.
        return_home();        
      }
    }else{
      // If the sensor cannot capture fingerprints precisely, notify the user accordingly and wait until the user scans the accurate fingerprint.
      shown_screen_fingerprint = false;
      show_fingerprint_task_screen("Error", "Cannot capture!", Q_red, Q_white);
      delay(2000);
      // Notify the Python backend accordingly via the borrowed function.
      Bridge.call("manage_account_actions_on_stm32", "signin", -2);       
    }    
  }
}

void show_fingerprint_task_screen(String title, String title_exp, uint16_t bg_color, uint16_t t_color){
  int l_1_s = 6, l_2_s = 14, l_sp = 5;
  int divider_w = SCREEN_WIDTH, divider_h = SCREEN_HEIGHT-fingerprint_h-(5*l_sp);
  if(!shown_screen_fingerprint){
    adjustColor(1,1,1);
    tft.fillScreen(Q_primary);
    tft.drawBitmap((SCREEN_WIDTH-fingerprint_w)/2, 2*l_sp, fingerprint_bits, fingerprint_w, fingerprint_h, bg_color);
    tft.fillRect(0, SCREEN_HEIGHT-divider_h, divider_w, divider_h, bg_color);
    tft.setTextSize(2); tft.setTextColor(t_color);
    tft.setCursor(((SCREEN_WIDTH-(title.length()*l_2_s))/2)+(2*l_sp), SCREEN_HEIGHT-divider_h+l_sp);
    tft.print(title);
    tft.setTextSize(1);
    tft.setCursor((SCREEN_WIDTH-(title_exp.length()*l_1_s))/2, SCREEN_HEIGHT-(5*l_sp));
    tft.print(title_exp);
  }
  // Avoid flickering due to drawing the same interface consecutively.
  shown_screen_fingerprint = true;
  
}

void show_home_screen(){
  if(!shown_screen_home){
    adjustColor(0,0,1);
    tft.fillScreen(Q_white);
    tft.fillCircle(SCREEN_WIDTH/2, (SCREEN_HEIGHT/2), (home_w/2)+20, Q_primary);
    tft.drawBitmap((SCREEN_WIDTH-home_w)/2, (SCREEN_HEIGHT-home_h)/2, home_bits, home_w, home_h, Q_white);
  }
  // Avoid flickering due to drawing the same interface consecutively.
  shown_screen_home = true;
}

void return_home(){
  shown_screen_sensor = false;
  activated_screen_home = true; shown_screen_home = false;
  activated_screen_fingerprint = false; shown_screen_fingerprint = false;
  delay(500);  
}

void show_err_screen(String title, String title_exp, String err_description){
  int l_1_s = 6, l_2_s = 14, l_sp = 5;
  int divider_w = SCREEN_WIDTH, divider_h = SCREEN_HEIGHT/4;
  int title_w = (divider_w/5)*3, title_h = (divider_h/3)*2;
  int logo_r = 36;
  tft.fillScreen(Q_teal);
  tft.fillRect(0, 0, divider_w, divider_h, Q_red);
  tft.fillRoundRect((divider_w-title_w)/2, (divider_h/3)*2, title_w, title_h, 5, Q_golden);
  tft.setTextSize(2); tft.setTextColor(Q_light_grey);
  tft.setCursor((SCREEN_WIDTH-(title.length()*l_2_s))/2, ((divider_h/3)*2)+l_sp);
  tft.print(title);
  tft.setTextSize(1);
  tft.setCursor((SCREEN_WIDTH-(title_exp.length()*l_1_s))/2, ((divider_h/3)*2)+title_h-l_1_s-l_sp);
  tft.print(title_exp);
  tft.fillCircle(SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+(1.5*l_sp), logo_r, Q_red);
  tft.drawBitmap((SCREEN_WIDTH-error_w)/2, ((SCREEN_HEIGHT-error_h)/2)+(1.5*l_sp), error_bits, error_w, error_h, Q_white);
  tft.fillRect(0, SCREEN_HEIGHT-divider_h, divider_w, divider_h, Q_red);
  tft.setTextSize(2); tft.setTextColor(Q_white);
  tft.setCursor((SCREEN_WIDTH-(err_description.length()*l_2_s))/2, SCREEN_HEIGHT-(divider_h/2)-(l_2_s/2));
  tft.print(err_description);
}

void adjustColor(int r, int g, int b){
  digitalWrite(red_pin, 1-r);
  digitalWrite(green_pin, 1-g);
  digitalWrite(blue_pin, 1-b);
}

/*
  Functions specific to communicate with the application backend in Python via the integrated Arduino Router background Linux service.
*/

void interface_web_control(String command, int interface_num){
  // Update the lab assistant interface (onboard) according to the provided user selection.
  if(command == "update_interface" || command == "update_water_on" || command == "update_water_off"){
    // Change the water atomization sensor state once requested by the user.
    if(command == "update_water_on"){ sensor_readings.water_atomization = "ON"; digitalWrite(water_atomization_pin, HIGH); }
    else if(command == "update_water_off"){ sensor_readings.water_atomization = "OFF"; digitalWrite(water_atomization_pin, LOW); } 
    if(interface_num != -1){
      current_sensor_screen = interface_num;
      shown_screen_sensor = false;
      activated_screen_home = false; shown_screen_home = false;
      activated_screen_fingerprint = false; shown_screen_fingerprint = false;
      delay(500);
    }else{
      return_home();
    }    
  }else{
     shown_screen_sensor = false;
     activated_screen_home = false; shown_screen_home = false;
     activated_screen_fingerprint = true; shown_screen_fingerprint = false;
     ongoing_fingerprint_task = command;
     provided_user_id = interface_num;
     delay(500);
   }
}

