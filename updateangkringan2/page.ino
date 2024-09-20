void Page1() {
  lcd.setCursor(0, 0); 
  String dayOfWeek = getDayOfWeek(timeClient.getDay());
  lcd.print(" " + dayOfWeek + " " + timeClient.getFormattedTime() + "  ");
  
  lcd.setCursor(0, 1);
  lcd.write(byte(2));
  lcd.setCursor(1, 1);
  float temperature = dht.readTemperature();
  lcd.print(temperature);
  lcd.setCursor(5, 1);
  lcd.print((char)223); 
  lcd.print("C ");
  
  lcd.setCursor(8, 1);
  lcd.write(byte(3));
  lcd.setCursor(9, 1);
  float humidity = dht.readHumidity();
  lcd.print(humidity);
  lcd.setCursor(13, 1);
  lcd.print("% ");
}

void Page2(float voltage, float current, float power) {

  lcd.setCursor(0, 0);
  lcd.write(byte(6));
  lcd.print(voltage, 2); 
  lcd.print("v ");
//----------------------------------
  lcd.setCursor (0, 1);
  lcd.write(byte(5));
  lcd.print(" ");
  lcd.print(current,0); 
  lcd.setCursor (5, 1);
  lcd.print(" mA");
  
//----------------------------------

  lcd.setCursor (8, 0);
  lcd.write(byte(7));
  lcd.print(power, 2);
  lcd.print("w ");

  lcd.setCursor (8, 1);
  lcd.print("      ");
}

void Page3() {
  lcd.setCursor(0, 0);
  lcd.print( " e05a1b6b9970  ");

  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(WiFi.localIP());
  lcd.setCursor(13, 1);
  lcd.print("  ");
}
