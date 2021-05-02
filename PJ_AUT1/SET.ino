 

  int show_MoistureSensor(){
  xx1 = 0;
  for (int i = 0; i<50;i++){
    xx2 = analogRead(MoistureSensor);
    xx1 = xx1 +xx2 ;
  }
  lcd.setCursor(1, 2);
  lcd.print("Moisture = ");
  Moisture = (xx1/1000)*1000;
  Moisture = map(Moisture, 13000, 26000,100,0);
  lcd.setCursor(12, 2);
  lcd.print(Moisture);
  lcd.print("%           ");
  lcd.clear();
  return Moisture;
  }
  


float show_PH(){
  lcd.setCursor(3, 3);
  lcd.print("PH     = ");
  PH = analogRead(PHSensor);
  PH = mapf(PH ,500,0,0,7);
  lcd.print(PH);
  lcd.print("                   ");
  lcd.clear();
  return PH;
}
