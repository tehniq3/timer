/*
 * clepsydra (timer) with relay and stepper motor
 * sketch by nicu FLORICA (niq_ro)
 * 
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

#define avans 2  // pin for increase time
#define shtart 3  // pin for begin the operation
#define releu 4  // pin for relay

// stepper part: https://create.arduino.cc/projecthub/debanshudas23/getting-started-with-stepper-motor-28byj-48-3de8c9
#define A 5
#define B 6
#define C 7
#define D 8

#define repaus HIGH
#define atras LOW
// or
/*
#define repaus LOW
#define atras HIGH
*/

int timp_min = 0;  // time in minutes
int timp_sec = 0;  // time in seconds
int timp_pas = 60;  // 300 sec = 5 min
int timp_max = 600;  // 1800 sec = 30 min
long timp_activare = 0; // time for store pushing
long timp_ramas = 0;  // remaining time
int timp_ramas_min = 0;
int timp_ramas_sec = 0;
byte activare = 0;  // variable for acctivate/dezactivate the timer

void setup()
{
//  Serial.begin(9600);
	lcd.begin(); // initialize the LCD
  // lcd.init();  // for other libraries
  lcd.clear();  // clear display
pinMode(avans, INPUT);  // pin as input
pinMode(shtart, INPUT);
digitalWrite(avans, HIGH);  // activate internal pullup resistor for pin
digitalWrite(shtart, HIGH);
pinMode(A,OUTPUT);  // pin as output
pinMode(B,OUTPUT);
pinMode(C,OUTPUT);
pinMode(D,OUTPUT);
digitalWrite(A,0);  // pin as 0
digitalWrite(B,0);
digitalWrite(C,0);
digitalWrite(D,0);	
pinMode(releu, OUTPUT);
digitalWrite(releu,repaus);  	// releu in stand-by
	
	lcd.backlight();  // Turn on the blacklight and print a message.
	lcd.setCursor(0,0);  // se cursor in left upside
	lcd.print("Clepsydra ver. 1");
  lcd.setCursor(0,1);  // se cursor in left downside
  lcd.print("by Nicu Florica ");
  delay(2000);  // small pause
  lcd.setCursor(0,1);  // se cursor in left downside
  lcd.print("by niq_ro       ");
  delay(2000);  // small pause
  lcd.clear();
}

void write(int a,int b,int c,int d){
digitalWrite(A,a);
digitalWrite(B,b);
digitalWrite(C,c);
digitalWrite(D,d);
}

void onestep(){
write(1,0,0,0);
delay(5);
write(1,1,0,0);
delay(5);
write(0,1,0,0);
delay(5);
write(0,1,1,0);
delay(5);
write(0,0,1,0);
delay(5);
write(0,0,1,1);
delay(5);
write(0,0,0,1);
delay(5);
write(1,0,0,1);
delay(5);
}


void loop()
{

if ((digitalRead(avans) == LOW) and (activare == 0)) // if button is pushed
{
  timp_sec = timp_sec + timp_pas;
  if (timp_sec > timp_max) timp_sec = 0;
  delay(500);
}
  timp_min = timp_sec/60;  // convert time in minutes
  lcd.setCursor(9,0);
  lcd.print("[");
  if (timp_min < 10) lcd.print("0");
  lcd.print(timp_min);
  lcd.print("min]");
if (digitalRead(shtart) == LOW)  // if START button is pushed
{
  activare = 1;
  timp_activare = millis()/1000;
}

if (activare == 0)
{
  lcd.setCursor(0,0);
  lcd.print("--:--");
  lcd.setCursor(0,1);
  lcd.print("Power Off !     ");
  write(0,0,0,0);  // motor stop
  digitalWrite(releu, repaus);
}

if (activare == 1)
{
  lcd.setCursor(0,1);
  lcd.print("On work !         ");
  if (timp_sec == 0)
  {
   activare = 0;
   lcd.setCursor(0,1);
   lcd.print("Timer not set!  ");
   delay(3000);
  }
  timp_ramas = timp_sec + timp_activare - millis()/1000;
//  Serial.println(timp_ramas);
//  if ((timp_ramas < 0) or (timp_ramas > timp_max))
  if (timp_ramas < 0)
  {
    activare = 0;
    lcd.setCursor(0,0);
    lcd.print("--:--");
    lcd.setCursor(0,1);
    lcd.print("The End         ");
    digitalWrite(releu, repaus);
    write(0,0,0,0);  // motor stop
    delay(3000);

  }
  else
  {
  timp_ramas_min = timp_ramas/60;
  timp_ramas_sec = timp_ramas%60;
  lcd.setCursor(0,0);
  if (timp_ramas_min < 10) lcd.print(" ");
  lcd.print(timp_ramas_min);
  lcd.print(":");
  if (timp_ramas_sec < 10) lcd.print("0");
  lcd.print(timp_ramas_sec);
  onestep();
  digitalWrite(releu, atras);
  }
   if (digitalRead(avans) == LOW)
  {
   activare = 0;
   digitalWrite(releu, repaus);
   write(0,0,0,0);  // motor stop
   lcd.setCursor(0,1);
   lcd.print("Manual STOP     ");
   lcd.setCursor(0,0);
   lcd.print("--:--");
   delay(3000);
  }

}

} // end main loop
