#include "TaskManager.h"
#include "MqttController.h"
#include "ESPConfigurations.h"
#include "TempController.h"
#include "BrightnessController.h"
#include <LogHelper.h>

TaskManager taskManager;
MqttController mqttController(false, false);

#if TEMP_COUNT>0
  TempController tempController;
#endif

#if BRIGHTNESS_COUNT>0
  BrightnessController brightnessController;
#endif

unsigned long sentCompleteTS = 0;

void onConnectionEstablished() {}   // legacy

uint8_t finishedCountNeeded = 0;

void setup() {
#if BRIGHTNESS_COUNT>0
  finishedCountNeeded++;
#endif
#if TEMP_COUNT>0
  finishedCountNeeded++;
#endif
  
  LOG_INIT();

  taskManager.registerTask(&mqttController);
  
#if BRIGHTNESS_COUNT>0
  taskManager.registerTask(&brightnessController);
#endif

#if TEMP_COUNT>0
  taskManager.registerTask(&tempController);
#endif

  taskManager.init();
}

void sleepRestart() {
  LOG_PRINTLN(F("Restart"));

#ifndef ESP32
  wdt_disable();
#endif

  WiFi.disconnect();
  WiFi.mode( WIFI_OFF );
#ifndef ESP32
  WiFi.forceSleepBegin();
#endif
  delay(100);

#if REAL_DEEP_SLEEP==1
  ESP.deepSleep(DEEP_SLEEP_DURATION);
#else
  delay(DEEP_SLEEP_DURATION);
  ESP.restart();
#endif
}



void loop() {
  taskManager.update();

uint8_t finishedCount = 0;

#if BRIGHTNESS_COUNT>0
  if (brightnessController.isFinished()) finishedCount++;
#endif
#if TEMP_COUNT>0
  if (tempController.isFinished()) finishedCount++;
#endif

  if (finishedCount==finishedCountNeeded) {
    // still wait a bit until sent
    if (sentCompleteTS==0) {
      LOG_PRINTLN(F("All tasks complete"));
      sentCompleteTS = millis();
    }

    if (millis()>sentCompleteTS+SEND_WAIT_TIME_MS) {
      sleepRestart();
    }
  } else {
    // wait for task complete
    if (millis() > NO_CONNECTION_TIMEOUT_SEC * 1000) {
      LOG_PRINTLN(F("Connection timeout"));
      sleepRestart();
    }
  }

  //LOG_PRINTLN(WiFi.status());

  delay(10);
}
