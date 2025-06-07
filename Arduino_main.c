#include <EmonLib.h>
#include <LiquidCrystal.h>

EnergyMonitor emon1;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Voltage of the electric
int voltage = 230;

// Pin of the SCT sensor
int sct_pin = A0;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);   
  // Pin, calibration - Current Constant = Ratio/BurdenR. 1800/62 = 29. 
  emon1.current(sct_pin, 29);
  // Initial display information
  lcd.setCursor(0,0);
  lcd.print("Current(A): ");
  lcd.setCursor(0,1);
  lcd.print("Power(W): ");
} 

void loop() {
  // Calculate the current  
  double Irms = emon1.calcIrms(1480);
  // Show the current value
  Serial.print("Current: ");
  Serial.print(Irms, 2); // Irms
  lcd.setCursor(10,0);
  lcd.print(Irms, 2);
   
  // Calculate and show the power value
  double power = Irms*voltage;
  Serial.print(" Power: ");
  Serial.print(power, 2);
  lcd.setCursor(10,1);
  lcd.print("      ");
  lcd.setCursor(10,1);
  lcd.print(power, 2);
                           
  delay(1000);
}