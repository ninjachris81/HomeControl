#ifndef COMMCONTROLLER_H
#define COMMCONTROLLER_H

#include <AbstractIntervalTask.h>

#define UPDATE_INTERVAL_MS 10000

class CommController : public AbstractIntervalTask {
  public:
    CommController();
  
    void init();

    void update();

    void sendCurrent(uint16_t mamps);

    void sendTemp(float temp);
    void sendHumidity(float humidity);

    void sendError(String error);

};

#endif
