void FirebaseUpdate(float temperature, float humidity, float current, float voltage, float power, int LDRValue, int gasValue,int rainAnalog) {
  String pathTemperature = prefix + "temperature";
  if (Firebase.setFloat(firebaseData, pathTemperature, temperature)) {
 //   Serial.println("Temperature: " + String(temperature));
  } else {
 //  Serial.println("Failed to write temperature to Firebase");
  }

  String pathHumidity = prefix + "humidity";
  if (Firebase.setFloat(firebaseData, pathHumidity, humidity)) {
 //   Serial.println("Humidity: " + String(humidity));
  } else {
 //   Serial.println("Failed to write humidity to Firebase");
  }

  float currentA = current / 1000.0;
  currentA = round(currentA * 100) / 100.0;
  String pathCurrent = prefix + "/current";
  if (Firebase.setFloat(firebaseData, pathCurrent, currentA)) {
  //  Serial.print("Current (divided by 1000): ");
  //  Serial.print(currentA);
  //  Serial.println(" A");
  } else {
  //  Serial.println("Failed to write current to Firebase");
  }

  String pathVoltage = prefix + "/voltage";
  if (Firebase.setFloat(firebaseData, pathVoltage, voltage)) {
  //  Serial.println("Voltage: " + String(voltage));
  } else {
  //  Serial.println("Failed to write voltage to Firebase");
  }

  String pathPower = prefix + "/power";
  if (Firebase.setFloat(firebaseData, pathPower, power)) {
  //  Serial.println("Power: " + String(power));
  } else {
  //  Serial.println("Failed to write power to Firebase");
  }
   
    String pathLDR = prefix + "/irradiance";
    if (Firebase.setInt(firebaseData, pathLDR, LDRValue)) {
  //    Serial.println("LDR: " + String(LDRValue));
    } else {
  //    Serial.println("Failed to write irradiance to Firebase");
    }
    
      String pathGas = prefix + "gas_sensor/gas";
  if (Firebase.setFloat(firebaseData, pathGas, gasValue)) {
    //Serial.print("Gas: ");
    //Serial.println(gasValue);
    String pathGasNotice = prefix + "gas_sensor/gas_notice";
    if (gasValue > 0) {
      if (Firebase.setString(firebaseData, pathGasNotice, "Gas detected")) {
     //   Serial.println("Gas detected");
      } else {
     //   Serial.println("Failed to write gas notice to Firebase");
      }
    } else {
      if (Firebase.setString(firebaseData, pathGasNotice, "No gas")) {
     //   Serial.println("No gas");
      } else {
     //   Serial.println("Failed to write gas notice to Firebase");
      }
    }
  } else {
    // Serial.println("Failed to write gas to Firebase");
  }
  String pathRainAnalog = prefix + "/rain_sensor/rain_analog";
    //  String pathRainDigital = prefix + "/rain_sensor/rain_digital";
    if (Firebase.setFloat(firebaseData, pathRainAnalog, rainAnalog)) {
      Serial.println("Rain analog: " + String(rainAnalog));
    } else {
      Serial.println("Failed to write rain analog to Firebase");
    }
    //  if (Firebase.setFloat(firebaseData, pathRainDigital, rainDigitalVal)) {
    //    Serial.println("Rain digital: " + String(rainDigitalVal));
    //  } else {
    //    Serial.println("Failed to write rain digital to Firebase");
    //  }

    String pathRainCondition = prefix + "/rain_sensor/rain_condition";

    if (rainAnalog >= 4000) {
      Serial.println("Tidak ada air hujan");
      Firebase.setString(firebaseData,  pathRainCondition , "Tidak ada air hujan");
    }
    else if (rainAnalog < 4000) {
      Serial.println("Terjadi gerimis");
      Firebase.setString(firebaseData,  pathRainCondition , "Terjadi gerimis");
    }
    else if (rainAnalog < 2000) {
      Serial.println("Terjadi hujan");
      Firebase.setString(firebaseData,  pathRainCondition , "Terjadi hujan");
    }
}
