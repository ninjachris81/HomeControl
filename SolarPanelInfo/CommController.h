#ifndef COMMCONTROLLER_H
#define COMMCONTROLLER_H

#include <AbstractIntervalTask.h>

#define UPDATE_INTERVAL_MS 10000

class CommController : public AbstractIntervalTask {
  public:
    CommController();
  
    void init();

    void update();

    void sendVoltage(uint16_t mamps);

    void sendTemp(uint8_t temp);
    void sendHumidity(uint8_t humidity);

    void sendError(String error);

};

#endif
