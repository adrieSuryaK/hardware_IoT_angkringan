void calculateVoltageCurrentPower(float& voltage, float& current, float& power) {
  float voltageValue = analogRead(Sensorpin);
  voltage = (voltageValue / 1023.0) * 3.3 * voltageScaleFactor;
  voltage = round(voltage * 100) / 100.0;
  
  unsigned int x = 0;
  float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0;
  current = 0.0;
  
  for (int x = 0; x < 150; x++) {
    AcsValue = analogRead(ACS712pin);
    Samples = Samples + AcsValue;
    delay(3);
  }

  AvgAcs = Samples / 150.0;
  current = ((V_offset_actual - (AvgAcs * (5.0 / 4095.0))) / 0.100) * 1000;

  if (current < 0) {
    current = 0;
  }
  current = round(current * 100) / 100.0;
  power = voltage * (current / 1000);
  power = round(power * 100.0) / 100.0;
  
}
