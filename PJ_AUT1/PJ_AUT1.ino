#include <SoftwareSerial.h>
SoftwareSerial mySerial(A8, A9); // RX, TX
#include <Wire.h>
#include <Keypad.h>

#include <RealTimeClockDS1307.h>
int x = 0;
//จอ LDC 20*4 จอแสดงผล
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); //ถ้าจอไม่แสดงผลให้เปลี่ยน 0x27 เป็น0x3F
int read_num(int nmax, int x, int y);

bool stateph1 = 0;
bool stateph2 = 0;
bool stateph3 = 0;
bool stateTime1 =  0;
bool stateTime2 =  0;
bool stateTime3 =  0;
const byte ROWS = 4; //four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void read_time();
void readKeypad();
char key;


bool stateph  = false;
int starttimeph ;


int Moisture;
int SoilStartINT = 0;
int SoilENDINT = 0;
int xx1, xx2;

float PH;
int PHStartInt;
int PHStartfloat;
int PHEndInt;
int PHEndfloat;

float PHstart;
float PHstop;

int t1 = 0;

int pump1 = 14;
int pump2 = 15;
int pump3 = 16;
int pump4 = 17;

//เซ็นเซอร์วัดความชื้นในดิน Soil Moisture Sensor
int MoistureSensor = A1;

//เซ็นเซอร์วัดค่า PH ในดิน
int PHSensor = A2;




typedef struct  {  // the time record
  int hour;      // hour
  int min;       // minute
  int sec;       // second
} TIMER;
TIMER realTime;
TIMER timeStart1;
TIMER timeEnd1;
TIMER timeStart2;
TIMER timeEnd2;
TIMER timeStart3;
TIMER timeEnd3;
TIMER timePHStart1;
TIMER timePHEnd1;
TIMER timePHStart2;
TIMER timePHEnd2;
TIMER timePHStart3;
TIMER timePHEnd3;

typedef struct  {  // the time record
  String hour;      // hour
  String min;       // minute
  String sec;       // second
} TIME;
TIME RealTime;
TIME SetTimestart1;
TIME SetTimeEnd1;
TIME SetTimestart2;
TIME SetTimeEnd2;
TIME SetTimestart3;
TIME SetTimeEnd3;
TIME SetTimephstart1;
TIME SetTimephEnd1;
TIME SetTimephstart2;
TIME SetTimephEnd2;
TIME SetTimephstart3;
TIME SetTimephEnd3;


//ทำ map ให้เป็นจุดทศนิยม
double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Show_Time1() {     // time pump //
  lcd.setCursor(2, 1);

  lcd.print(timeStart1.hour);
  lcd.print(":");
  lcd.print(timeStart1.min);


  lcd.setCursor(2, 3);

  lcd.print(timeEnd1.hour);
  lcd.print(":");
  lcd.print(timeEnd1.min);


}


void Show_Time2() {     // time pump //
  lcd.setCursor(2, 1);

  lcd.print(timeStart2.hour);
  lcd.print(":");
  lcd.print(timeStart2.min);


  lcd.setCursor(2, 3);

  lcd.print(timeEnd2.hour);
  lcd.print(":");
  lcd.print(timeEnd2.min);


}

void Show_Time3() {     // time pump //
  lcd.setCursor(2, 1);


  lcd.print(timeStart3.hour);
  lcd.print(":");
  lcd.print(timeStart3.min);

  lcd.setCursor(2, 3);


  lcd.print(timeEnd3.hour);
  lcd.print(":");
  lcd.print(timeEnd3.min);


}



/////  time ph /////
void Show_TimePH1() {
  lcd.setCursor(2, 1);

  lcd.print(timePHStart1.hour);
  lcd.print(":");
  lcd.print(timePHStart1.min);



  lcd.setCursor(2, 3);


  lcd.print(timePHEnd1.hour);
  lcd.print(":");
  lcd.print(timePHEnd1.min);


}

void Show_TimePH2() {
  lcd.setCursor(2, 1);


  lcd.print(timePHStart2.hour);
  lcd.print(":");
  lcd.print(timePHStart2.min);

  lcd.setCursor(2, 3);

  lcd.print(timePHEnd2.hour);
  lcd.print(":");
  lcd.print(timePHEnd2.min);


}

void Show_TimePH3() {
  lcd.setCursor(2, 1);


  lcd.print(timePHStart3.hour);
  lcd.print(":");
  lcd.print(timePHStart3.min);


  lcd.setCursor(2, 3);


  lcd.print(timePHEnd3.hour);
  lcd.print(":");
  lcd.print(timePHEnd3.min);


}

////////////////////////////////////   ตั้งเวลา น้ำ /////////////////////////////////////////////////
void setTimeStart1(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_Time1();
  lcd.cursor();
  timeStart1.hour = read_num(23, x, y);
  timeStart1.min = read_num(59, x + 3, y);

  lcd.noCursor();
  if (timeStart1.hour < 10) {
    SetTimestart1.hour = (String)"0" + timeStart1.hour;
  }
  if (timeStart1.min < 10) {
    SetTimestart1.min = (String)"0" + timeStart1.min;
  }

  else {
    SetTimestart1.hour = (String)timeStart1.hour;
    SetTimestart1.min = (String)timeStart1.min;

  }

}
void setTimeEnd1(int x, int y) {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_Time1();
  lcd.cursor();
  timeEnd1.hour = read_num(23, x, y);
  timeEnd1.min = read_num(59, x + 3, y );

  lcd.noCursor();
  if (timeEnd1.hour < 10) {
    SetTimeEnd1.hour = (String)"0" + timeEnd1.hour;
  }
  if (timeEnd1.min < 10) {
    SetTimeEnd1.min = (String)"0" + timeEnd1.min;
  }

  else {
    SetTimeEnd1.hour = (String)timeEnd1.hour;
    SetTimeEnd1.min = (String)timeEnd1.min;

  }
}
void setTimeStart2(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_Time2();
  lcd.cursor();
  timeStart2.hour = read_num(23, x, y);
  timeStart2.min = read_num(59, x + 3, y);

  lcd.noCursor();

  if (timeStart2.hour < 10) {
    SetTimestart1.hour = (String)"0" + timeStart1.hour;
  }
  if (timeStart2.min < 10) {
    SetTimestart2.min = (String)"0" + timeStart1.min;
  }

  else {
    SetTimestart2.hour = (String)timeStart2.hour;
    SetTimestart2.min = (String)timeStart2.min;

  }

}
void setTimeEnd2(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_Time2();
  lcd.cursor();
  timeEnd2.hour = read_num(23, x, y);
  timeEnd2.min = read_num(59, x + 3, y);

  lcd.noCursor();
  if (timeEnd2.hour < 10) {
    SetTimeEnd2.hour = (String)"0" + timeEnd2.hour;
  }
  if (timeEnd2.min < 10) {
    SetTimeEnd2.min = (String)"0" + timeEnd2.min;
  }

  else {
    SetTimeEnd2.hour = (String)timeEnd2.hour;
    SetTimeEnd2.min = (String)timeEnd2.min;

  }
}
void setTimeStart3(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_Time3();
  lcd.cursor();
  timeStart3.hour = read_num(23, x, y);
  timeStart3.min = read_num(59, x + 3, y);
  lcd.noCursor();
  if (timeStart3.hour < 10) {
    SetTimestart3.hour = (String)"0" + timeStart1.hour;
  }
  if (timeStart3.min < 10) {
    SetTimestart3.min = (String)"0" + timeStart1.min;
  }

  else {
    SetTimestart3.hour = (String)timeStart3.hour;
    SetTimestart3.min = (String)timeStart3.min;

  }

}
void setTimeEnd3(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_Time3();
  lcd.cursor();
  timeEnd3.hour = read_num(23, x, y);
  timeEnd3.min = read_num(59, x + 3, y);

  lcd.noCursor();
  if (timeEnd3.hour < 10) {
    SetTimeEnd3.hour = (String)"0" + timeEnd3.hour;
  }
  if (timeEnd3.min < 10) {
    SetTimeEnd3.min = (String)"0" + timeEnd3.min;
  }
  if (timeEnd3.sec < 10) {
    SetTimeEnd3.sec = (String)"0" + timeEnd3.sec;
  }
  else {
    SetTimeEnd3.hour = (String)timeEnd3.hour;
    SetTimeEnd3.min = (String)timeEnd3.min;


  }
}
//////////////////////////////////   ตั้งเวลา PH /////////////////////////////////////////////////
void setTimePHStart1(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_TimePH1();
  lcd.cursor();
  timePHStart1.hour = read_num(23, x, y);
  timePHStart1.min = read_num(59, x + 3, y);

  lcd.noCursor();
  if (timePHStart1.hour < 10) {
    SetTimephstart1.hour = (String)"0" + timePHStart1.hour;
  }
  if (timePHStart1.min < 10) {
    SetTimephstart1.min = (String)"0" + timePHStart1.min;
  }

  else {
    SetTimephstart1.hour = (String)timePHStart1.hour;
    SetTimephstart1.min = (String)timePHStart1.min;

  }

}
void setTimePHEnd1(int x, int y) {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_TimePH1();
  lcd.cursor();
  timePHEnd1.hour = read_num(23, x, y);
  timePHEnd1.min = read_num(59, x + 3, y );

  lcd.noCursor();
  if (timePHEnd1.hour < 10) {
    SetTimephEnd1.hour = (String)"0" + timePHEnd1.hour;
  }
  if (timePHEnd1.min < 10) {
    SetTimephEnd1.min = (String)"0" + timePHEnd1.min;
  }

  else {
    SetTimephEnd1.hour = (String)timePHEnd1.hour;
    SetTimephEnd1.min = (String)timePHEnd1.min;

  }
}
void setTimePHStart2(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_TimePH2();
  lcd.cursor();
  timePHStart2.hour = read_num(23, x, y);
  timePHStart2.min = read_num(59, x + 3, y);

  lcd.noCursor();
  if (timePHStart2.hour < 10) {
    SetTimephstart2.hour = (String)"0" + timePHStart2.hour;
  }
  if (timePHStart1.min < 10) {
    SetTimephstart2.min = (String)"0" + timePHStart2.min;
  }

  else {
    SetTimephstart2.hour = (String)timePHStart2.hour;
    SetTimephstart2.min = (String)timePHStart2.min;

  }

}

void setTimePHEnd2(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_TimePH2();
  lcd.cursor();
  timePHEnd2.hour = read_num(23, x, y);
  timePHEnd2.min = read_num(59, x + 3, y);
  timePHEnd2.sec = read_num(59, x + 6, y);
  lcd.noCursor();
  if (timePHEnd2.hour < 10) {
    SetTimephEnd2.hour = (String)"0" + timePHEnd2.hour;
  }
  if (timePHEnd2.min < 10) {
    SetTimephEnd2.min = (String)"0" + timePHEnd2.min;
  }
  if (timePHEnd2.sec < 10) {
    SetTimephEnd2.sec = (String)"0" + timePHEnd2.sec;
  }
  else {
    SetTimephEnd2.hour = (String)timePHEnd2.hour;
    SetTimephEnd2.min = (String)timePHEnd2.min;
    SetTimephEnd2.sec = (String)timePHEnd2.sec;
  }
}
void setTimePHStart3(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_TimePH3();
  lcd.cursor();
  timePHStart3.hour = read_num(23, x, y);
  timePHStart3.min = read_num(59, x + 3, y);
  timePHStart3.sec = read_num(59, x + 6, y);
  lcd.noCursor();
  if (timePHStart3.hour < 10) {
    SetTimephstart3.hour = (String)"0" + timePHStart3.hour;
  }
  if (timePHStart3.min < 10) {
    SetTimephstart3.min = (String)"0" + timePHStart3.min;
  }
  if (timePHStart3.sec < 10) {
    SetTimephstart3.sec = (String)"0" + timePHStart3.sec;
  }
  else {
    SetTimephstart3.hour = (String)timePHStart3.hour;
    SetTimephstart3.min = (String)timePHStart3.min;
    SetTimephstart3.sec = (String)timePHStart3.sec;
  }
}
void setTimePHEnd3(int x, int y)  {
  RTC.readClock();
  lcd.setCursor(x, y);
  Show_TimePH3();
  lcd.cursor();
  timePHEnd3.hour = read_num(23, x, y);
  timePHEnd3.min = read_num(59, x + 3, y);
  timePHEnd3.sec = read_num(59, x + 6, y);
  lcd.noCursor();
  if (timePHEnd3.hour < 10) {
    SetTimephEnd3.hour = (String)"0" + timePHEnd3.hour;
  }
  if (timePHEnd2.min < 10) {
    SetTimephEnd3.min = (String)"0" + timePHEnd3.min;
  }
  if (timePHEnd2.sec < 10) {
    SetTimephEnd3.sec = (String)"0" + timePHEnd3.sec;
  }
  else {
    SetTimephEnd3.hour = (String)timePHEnd3.hour;
    SetTimephEnd3.min = (String)timePHEnd3.min;
    SetTimephEnd3.sec = (String)timePHEnd3.sec;
  }
}

void setTime() {
  RTC.setHours(23);
  RTC.setMinutes(3);
  RTC.setSeconds(15);
  RTC.setClock();
}
void show_time(void)  {

  lcd.print(RealTime.hour);
  lcd.print(":");
  lcd.print(RealTime.min);
  lcd.print(":");
  lcd.print(RealTime.sec);
  //  Serial.print(RealTime.hour);
  //  Serial.print(RealTime.min);
  //  Serial.println(RealTime.sec);

  //  Serial.print(realTime.hour);
  //  Serial.print(realTime.min);
  //  Serial.println(realTime.sec);



}

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.setTimeout(100);
  mySerial.setTimeout(100);

  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  pinMode(pump4, OUTPUT);

  lcd.init();
  lcd.backlight();

  //  pinMode(reLayMotorR, OUTPUT);
  //  pinMode(reLayMotorL, OUTPUT);

  //setTime();
}

void loop() {

  //////// รับค่าจาก nodemcu ///////////
  if (mySerial.available()) {
    String received = mySerial.readStringUntil('\n');



    if (!received.indexOf("temp=")) {    ///ph
      String str = received.substring(5);

      Serial.println(received);

    }
    else if (!received.indexOf("humi=")) {

      Serial.println(received);
    }
    else if (!received.indexOf("timephstart1=")) {


      received = received.substring(13);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimephstart1.hour = (String) part1;
      SetTimephstart1.min = (String) part2;



    }
    else  if (!received.indexOf("timephEnd1=")) {
      received = received.substring(11);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimephEnd1.hour = part1;
      SetTimephEnd1.min = part2;

      Serial.print(SetTimephEnd1.hour);
      Serial.println(SetTimephEnd1.min);

    }
    else  if (!received.indexOf("timephstart2=")) {
      received = received.substring(13);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimephstart2.hour = (String) part1;
      SetTimephstart2.min = (String) part2;



    }
    else if (!received.indexOf("timephEnd2=")) {
      received = received.substring(11);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimephEnd2.hour = part1;
      SetTimephEnd2.min = part2;



    }
    else if (!received.indexOf("timephstart3=")) {
      received = received.substring(13);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimephstart3.hour = (String) part1;
      SetTimephstart3.min = (String) part2;


    }
    else  if (!received.indexOf("timephEnd3=")) {
      received = received.substring(11);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimephEnd3.hour = part1;
      SetTimephEnd3.min = part2;


    }
    else if (!received.indexOf("timestart1=")) {
      received = received.substring(11);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimestart1.hour = part1;
      SetTimestart1.min = part2;

      Serial.print( SetTimestart1.hour);
      Serial.print( SetTimestart1.min);


    }
    else if (!received.indexOf("timeEnd1=")) {
      received = received.substring(8);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimeEnd1.hour = part1;
      SetTimeEnd1.min = part2;
      Serial.print( SetTimeEnd1.hour);
      Serial.print( SetTimeEnd1.min);

    }
    else if (!received.indexOf("timestart2=")) {
      received = received.substring(11);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimestart2.hour = part1;
      SetTimestart2.min = part2;


    }
    else if (!received.indexOf("timeEnd2=")) {
      received = received.substring(8);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimeEnd2.hour = part1;
      SetTimeEnd2.min = part2;


    }
    else if (!received.indexOf("timestart3=")) {
      received = received.substring(11);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimestart3.hour = part1;
      SetTimestart3.min = part2;



    }
    else if (!received.indexOf("timeEnd3=")) {
      received = received.substring(8);
      String part1 = splitOfstrng(received, ':', 0);
      String part2 = splitOfstrng(received, ':', 1);
      SetTimeEnd2.hour = part1;
      SetTimeEnd2.min = part2;


    }












  }
  readKeypad();
}
//set time
int read_num(int nmax, int x, int y) {
  int num, i;
  lcd.setCursor(x, y);
  num = 0, i = 0;
  do {
    do {
      key = keypad.getKey();

    } while (!key);
    if (key == '*' && i > 0)  { //check key ''
      x = x - 1;
      lcd.setCursor(x, y);
      lcd.print(" ");
      lcd.setCursor(x, y);
      num = num / 10;
      i = i - 1;
    }

    else if (key != '#' && key != '*') {
      if ((num * 10 + (key - 0x30)) <= nmax) {
        num = num * 10 + (key - 0x30);
        lcd.print(key);
        x = x + 1;
        i = i + 1;
      }
    }
  } while (key != '#');         //&& num !=0);
  return (num);

}


void readKeypad() {
  key = keypad.getKey();



  if (key == 'A'  )   {
    key = keypad.getKey();
    while (1) {
      lcd.setCursor(0, 0);
      lcd.print("               ");
      lcd.setCursor(2, 0);
      lcd.print("1-3 SET TIME SOI");
      lcd.setCursor(2, 1);
      lcd.print("4-6 SET TIME PH");
      lcd.setCursor(2, 2);
      lcd.print(" 7  SET SOI");
      lcd.setCursor(2, 3);
      lcd.print(" 8  SET PH");
      key = keypad.getKey();
      if (key == '1') {
        x = 1;
        break;
      }
      if (key == '2') {
        x = 2;
        break;
      }
      if (key == '3') {
        x = 3;
        break;
      }
      if (key == '4') {
        x = 4;
        break;
      }
      if (key == '5') {
        x = 5;
        break;
      }
      if (key == '6') {
        x = 6;
        break;
      }
      if (key == '7') {
        x = 7;
        break;
      }
      if (key == '8') {
        x = 8;
        break;
      }

    }


    if (key == '1' || x == 1  ) {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Time Start 1");
      lcd.setCursor(2, 2);
      lcd.print("Time End 1");
      setTimeStart1(2, 1);
      lcd.print("               ");
      setTimeEnd1(2, 3);
      lcd.print("               ");
      //break;


    }
    else if (key == '2' || x == 2  ) {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Time Start 2");
      lcd.setCursor(2, 2);
      lcd.print("Time End 2");
      setTimeStart2(2, 1);
      setTimeEnd2(2, 3);

    }
    else if (key == '3' || x == 3  ) {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Time Start 3");
      lcd.setCursor(2, 2);
      lcd.print("Time End 3");
      setTimeStart3(2, 1);
      setTimeEnd3(2, 3);

    }
    else if (key == '4' || x == 4  )   {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Time PH Start 1");
      lcd.setCursor(2, 2);
      lcd.print("Time PH End 1");
      setTimePHStart1(2, 1);
      setTimePHEnd1(2, 3);
    }
    else if (key == '5' || x == 5)   {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Time PH Start 2");
      lcd.setCursor(2, 2);
      lcd.print("Time PH End 2");
      setTimePHStart2(2, 1);
      setTimePHEnd2(2, 3);
    }
    else if (key == '6' || x == 6 )   {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Time PH Start 3");
      lcd.setCursor(2, 2);
      lcd.print("Time PH End 3");
      setTimePHStart3(2, 1);
      setTimePHEnd3(2, 3);
    }
    else if (key == '7' || x == 7 )   {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Set Start Moisture");
      lcd.setCursor(2, 2);
      lcd.print("Set End Moisture");
      SoilStartINT = read_num(100, 2, 1);
      SoilENDINT = read_num(100, 2, 3);
    }
    else if (key == '8' || x == 8 )   {
      lcd.setCursor(0, 0);
      lcd.print("                  ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
      lcd.setCursor(2, 0);
      lcd.print("Set Start PH");
      lcd.setCursor(2, 2);
      lcd.print("Set End PH");
      lcd.setCursor(2, 1);
      lcd.print("0.0");
      lcd.setCursor(2, 3);
      lcd.print("0.0");
      PHStartInt = read_num(9, 2, 1);
      PHStartfloat = read_num(9, 4, 1);
      String phstart  = (String)PHStartInt + "." + PHStartfloat;
      PHstart = phstart.toFloat();


      PHEndInt = read_num(9, 2, 3);
      PHEndfloat = read_num(9, 4, 3);
      String phend  = (String)PHEndInt + "." + PHEndfloat;
      PHstop = phend.toFloat();


    }


  }

  else {

    //    Serial.print(SetTimephstart1.hour);
    //    Serial.print(":");
    //    Serial.print(SetTimephstart1.min);
    //    Serial.print(" ");
    //    Serial.print(RealTime.hour);
    //    Serial.print(":");
    //    Serial.println(RealTime.min);



    if (RealTime.hour.toInt() == SetTimeEnd1.hour.toInt() && RealTime.min.toInt() == SetTimeEnd1.min.toInt()) {
      stateTime1 = 0;
      Serial.println("stop 1");
      digitalWrite(pump1, LOW);
    } else if (RealTime.hour.toInt() == SetTimestart1.hour.toInt() && RealTime.min.toInt() == SetTimestart1.min.toInt() ) {
      stateTime1 = 1;
      Serial.println("start 1");
      digitalWrite(pump1, HIGH);
    }
    if (RealTime.hour.toInt() == SetTimeEnd2.hour.toInt() && RealTime.min.toInt() == SetTimeEnd2.min.toInt() ) {
      stateTime2 = 0;
      Serial.println("stop 2");
      digitalWrite(pump1, LOW);
    } else if (RealTime.hour.toInt() == SetTimestart2.hour.toInt() && RealTime.min.toInt() == SetTimestart2.min.toInt() ) {
      stateTime2 = 1;
      Serial.println("start 2");
      digitalWrite(pump1, HIGH);
    }
    if (RealTime.hour.toInt() == SetTimeEnd3.hour.toInt() && RealTime.min.toInt() == SetTimeEnd3.min.toInt() ) {
      stateTime3 = 0;
      Serial.println("stop 3");
      digitalWrite(pump1, LOW);
    } else if ( RealTime.hour.toInt() == SetTimestart3.hour.toInt() && RealTime.min.toInt() == SetTimestart3.min.toInt()) {
      stateTime3 = 1;
      Serial.println("start 3");
      digitalWrite(pump1, HIGH);
    }
    if (RealTime.hour.toInt() == SetTimephEnd1.hour.toInt() && RealTime.min.toInt()  == SetTimephEnd1.min.toInt() ) {
      stateph1 = 0;
      Serial.println("stop PH 1");
      digitalWrite(pump3, LOW);
      digitalWrite(pump4, LOW);

    } else if (RealTime.hour.toInt()  == SetTimephstart1.hour.toInt() && RealTime.min.toInt()  == SetTimephstart1.min.toInt() ) {
      stateph1 = 1;
      Serial.println("start PH 1");
      digitalWrite(pump3, HIGH);
      digitalWrite(pump4, HIGH);
    }
    if (RealTime.hour.toInt() == SetTimephEnd2.hour.toInt() && RealTime.min.toInt() == SetTimephEnd2.min.toInt() ) {
      stateph2 = 0;
      Serial.println("stop PH 2");
      digitalWrite(pump3, LOW);
      digitalWrite(pump4, LOW);

    } else if (RealTime.hour.toInt() == SetTimephstart2.hour.toInt() && RealTime.min.toInt() == SetTimephstart2.min.toInt() ) {
      stateph2 = 1;
      Serial.println("start PH 2");
      digitalWrite(pump3, HIGH);
      digitalWrite(pump4, HIGH);

    }
    if (RealTime.hour.toInt() == SetTimephEnd3.hour.toInt() && RealTime.min.toInt() == SetTimephEnd3.min.toInt() ) {
      stateph3 = 0;
      Serial.println("stop PH 3");
      digitalWrite(pump3, LOW);
      digitalWrite(pump4, LOW);
    } else if ((RealTime.hour.toInt() == SetTimephstart3.hour.toInt() && RealTime.min.toInt() == SetTimephstart3.min.toInt() )) {
      stateph3 = 1;
      Serial.println("start PH 3");
      digitalWrite(pump3, HIGH);
      digitalWrite(pump4, HIGH);
    }
    Set_Moisture();
    lcd.clear();
    lcd.setCursor(6, 0);
    read_time();
    show_time();
    show_MoistureSensor();
    show_PH();
    Set_PH();


  }
}

void read_time(void)  {
  RTC.readClock();
  realTime.hour = RTC.getHours();
  realTime.min = RTC.getMinutes();
  realTime.sec = RTC.getSeconds();

  RealTime.hour = (String)realTime.hour;
  RealTime.min = (String)realTime.min;
  RealTime.sec = (String)realTime.sec;
  if (realTime.hour < 10) {
    RealTime.hour = "0" + RealTime.hour;
  }
  if (realTime.min < 10) {
    RealTime.min = "0" + RealTime.min;
  }
  if (realTime.sec < 10) {
    RealTime.sec = "0" + RealTime.sec;
  }




}

void Set_Moisture () {
  unsigned long currentMillis = millis();
  long previousMillis = 0;
  Moisture = show_MoistureSensor();
  if (currentMillis - previousMillis > 6000) {
    previousMillis = currentMillis;
    mySerial.println((String)"Moisture=" + Moisture);
  }
  if (Moisture < SoilStartINT ) {
    digitalWrite(pump1, HIGH);
    Serial.print("Start pump1 " );
  }


  if (Moisture > SoilENDINT && SoilENDINT > 0  && stateTime1 == 0 && stateTime2 == 0  && stateTime3 == 0) {
    digitalWrite(pump1, LOW);
    Serial.print("END pump1 " );
  }

}

void Set_PH() {
  PH = show_PH();
  unsigned long currentMillis = millis();
  long previousMillis = 0;
  if (currentMillis - previousMillis > 5000) {
    previousMillis = currentMillis;
    mySerial.println((String)"PH=" + PH);

  }


  if (realTime.sec > 58) {
    starttimeph++;

  }
  if (starttimeph >= 2) {
    stateph = 1;
  } else if (starttimeph >= 3) {
    stateph = 0;
    starttimeph = 0;
  }




  //    Serial.print(PHstart);
  //    Serial.println(PHstop);

  if (stateph == 1) {
    if (PHstart < PHstop && PHstop > 0 && PHstart > 0 ) {

      if (PH < PHstart ) {
        digitalWrite(pump2, HIGH);
        Serial.print("Start  pump2 " );
      }

      if (PH > PHstop ) {
        digitalWrite(pump3, HIGH);
        digitalWrite(pump4, HIGH);
        Serial.print("Start pump3 + pump4 " );
      }
      if (PH > PHstart && PH < PHstop &&  PHstart > 0 && PHstop > 0 && stateph1  == 0 && stateph1 == 0 &&  stateph1  == 0 ) {

        digitalWrite(pump2, 0);
        digitalWrite(pump3, 0);
        digitalWrite(pump4, 0);
      }
    }

  } else {
    digitalWrite(pump2, 0);
    digitalWrite(pump3, 0);
    digitalWrite(pump4, 0);
  }

}

String splitOfstrng(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
