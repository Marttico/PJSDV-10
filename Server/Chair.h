
#ifndef CHAIR_H_
#define CHAIR_H_
#include <string>
#include <sys/socket.h>
#include "Server.h"
using namespace std;

class Chair {
public:
	Chair(int, bool);
	~Chair();
    void loop();
    void zetLed(bool);
    void zetTril(bool);
    void zetTrilPermissie(bool);
    bool checkTril() const;
    uint16_t checkDrukSensor() const;
    bool checkKnop() const;
private:
    Server sv;
    bool buttonPressed;
    uint16_t drukSensor;
	bool trilStand;
    bool trilPermissie;

};

#endif /* CHAIR_H_ */
