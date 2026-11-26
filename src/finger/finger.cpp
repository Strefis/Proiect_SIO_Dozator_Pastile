#include "finger.h"

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

#include "../controls/controls.h"
#include "../display/display.h"

namespace {
const uint8_t sensor_rx_pin = 10;
const uint8_t sensor_tx_pin = 11;

SoftwareSerial fingerSerial(sensor_rx_pin, sensor_tx_pin); // RX, TX pentru senzorul de amprente
Adafruit_Fingerprint finger(&fingerSerial); // Obiect pentru senzorul de amprente
bool sensorReady = false;

void waitForFingerImage() { // asteapta pana cand senzorul detecteaza o amprenta
  while (true) {
    uint8_t result = finger.getImage(); // incearca sa obtina o imagine a amprentei
    if (result == FINGERPRINT_OK) {
      return; // imaginea a fost obtinuta cu succes
    }
    if (result != FINGERPRINT_NOFINGER) {
      delay(100); // daca a aparut o eroare, asteapta putin inainte de a incerca din nou
    }
  }
}

bool convertImage(uint8_t slot) {
  return finger.image2Tz(slot) == FINGERPRINT_OK; // converteste imaginea amprentei in date digitale si le stocheaza in slot-ul specificat
}

enum class SlotSearchResult { // rezultatele posibile ale cautarii unui slot disponibil pentru stocarea unei amprente
  Available,
  Full,
  SensorError
};

SlotSearchResult findAvailableId(uint8_t& id) { // alege urmatorul ID liber pe baza numarului de amprente salvate
  if (finger.getTemplateCount() != FINGERPRINT_OK) {
    return SlotSearchResult::SensorError;
  }
  if (finger.templateCount >= 127) {
    return SlotSearchResult::Full;
  }
  id = static_cast<uint8_t>(finger.templateCount + 1);
  return SlotSearchResult::Available;
}

bool enrollFingerprint(uint8_t id) { // inroleaza o amprenta noua si o stocheaza in slot-ul specificat
  displayModule::showFingerprintEnrollment("Place finger");
  waitForFingerImage();
  if (!convertImage(1)) {
    return false;
  }

  displayModule::showFingerprintEnrollment("Remove finger");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(50);
  }

  displayModule::showFingerprintEnrollment("Place same finger");
  waitForFingerImage();
  if (!convertImage(2) || finger.createModel() != FINGERPRINT_OK) {
    return false;
  }

  return finger.storeModel(id) == FINGERPRINT_OK;
}

void handleEnrollment() { // gestioneaza procesul de inrolare a unei amprente noi
  uint8_t id;
  SlotSearchResult slotResult = findAvailableId(id); // cauta un slot disponibil pentru stocarea amprentei si returneaza rezultatul cautarii
  if (slotResult != SlotSearchResult::Available) {
    displayModule::showFingerprintEnrollment(slotResult == SlotSearchResult::Full ? "No free slot" : "Sensor error");
    delay(2000);
    displayModule::clearFingerprintState();
    return;
  }

  if (enrollFingerprint(id)) {
    displayModule::showFingerprintEnrollment("Saved successfully");
  } else {
    displayModule::showFingerprintEnrollment("Enrollment failed");
  }
  delay(2000);
  displayModule::clearFingerprintState();
}

void checkFingerprint() { // verifica daca o amprenta este prezenta si daca este recunoscuta de senzor
  uint8_t result = finger.getImage();
  if (result == FINGERPRINT_NOFINGER) {
    return;
  }
  if (result != FINGERPRINT_OK || !convertImage(1)) {
    return;
  }

  result = finger.fingerFastSearch(); // cauta rapid amprenta in baza de date a senzorului
  if (result == FINGERPRINT_OK) {
    displayModule::showFingerprintRecognized(finger.fingerID);
  } else if (result == FINGERPRINT_NOTFOUND) {
    displayModule::showFingerprintUnrecognized();
  }
}
}

namespace fingerModule {
    void begin() {
    finger.begin(57600);
    sensorReady = finger.verifyPassword();
    Serial.println(sensorReady ? F("Finger sensor OK") : F("Finger sensor NOT found"));

    if (sensorReady && finger.getTemplateCount() == FINGERPRINT_OK) { // afiseaza cate amprente sunt deja salvate
      Serial.print(F("Templates stored: "));
      Serial.println(finger.templateCount);
    }
    }

    void update() {
    if (!sensorReady) {
        return;
    }

    if (controlsModule::consumeSetLongPress()) {
        handleEnrollment();
        return;
    }

    checkFingerprint();
    }
}