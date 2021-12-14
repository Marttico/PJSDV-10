
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
    void updateVariables(uint8_t,uint16_t);
    void Loop();
private:
    Server sv;
    int socketID;
    bool buttonPressed;
    uint16_t drukSensor;
	bool trilStand;
    bool trilPermissie;

};

#endif /* CHAIR_H_ */
