#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <iostream>
#include <set>
using namespace std;

//设备父类
class Device
{
private:
    string ip;
    set<string> slot;
public: 
    Device();
    Device(string ip);
    string getIp();
    set<string> getSlot();
    void setIp(string ip);
    void setSlot(string slot);
    void setSlot(set<string> slot);
    bool operator ==(const Device &dev) const;
    bool operator < (const Device &dev) const;
};

#endif // DEVICE_H
