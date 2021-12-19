
#include "Server.h"
#ifndef WEMOS_H_
#define WEMOS_H_
#define senddelay 6000
using namespace std;

class Wemos {
public:
    Wemos(int);
    ~Wemos();
    int writeWemos(char*);
    int readWemos(char*);
    bool isConnected() const;
private:
    int port;
    Server wemosServer;
};

#endif
