void PrediksiHujan(float lastTemperature, float lastHumidity, int LDRValue) {
  // Rain predict
  float Light = LDRValue;
  int possibility = 0; // Initialize possibility

  if (lastHumidity > 60.0) {
    if (lastTemperature >= 20 && lastTemperature < 30) {
      if (Light < 500)
        possibility = 5;
      else if (Light < 1250)
        possibility = 3;
      else if (Light < 1500)
        possibility = 2;
      else
        possibility = 1;
    } else if (lastTemperature >= 30) {
      if (Light < 500)
        possibility = 5;
      else if (Light < 1250)
        possibility = 3;
      else
        possibility = 1;
    }
  } else if (lastHumidity <= 60.0) {
    possibility = 1;
  }

  String pathPossibility = prefix + "prediksi_hujan";

  switch (possibility) {
    case 1:
      Serial.println("Kemungkinan sangat kecil terjadi hujan");
      // Implement the updateFirebase() function to update Firebase with the prediction
      Firebase.setString(firebaseData, pathPossibility, "Kemungkinan sangat kecil terjadi hujan");
      break;
    case 2:
      Serial.println("Kemungkinan kecil terjadi hujan");
      // Implement the updateFirebase() function to update Firebase with the prediction
      Firebase.setString(firebaseData, pathPossibility, "Kemungkinan kecil terjadi hujan");
      break;
    case 3:
      Serial.println("Kemungkinan sedang terjadi hujan");
      // Implement the updateFirebase() function to update Firebase with the prediction
      Firebase.setString(firebaseData, pathPossibility, "Kemungkinan sedang terjadi hujan");
      break;
    case 4:
      Serial.println("Kemungkinan besar terjadi hujan");
      // Implement the updateFirebase() function to update Firebase with the prediction
      Firebase.setString(firebaseData, pathPossibility, "Kemungkinan besar terjadi hujan");
      break;
    case 5:
      Serial.println("Kemungkinan sangat besar terjadi hujan");
      // Implement the updateFirebase() function to update Firebase with the prediction
      Firebase.setString(firebaseData, pathPossibility, "Kemungkinan sangat besar terjadi hujan");
      break;
    default:
      Serial.println("Tidak dapat memprediksi hujan");
      // Implement the updateFirebase() function to handle other cases or errors
  }
}
