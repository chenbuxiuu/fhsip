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
    string type;
    set<string> slot;
public: 
    Device();
    Device(string ip);
    string getIp();
    string getType();
    set<string> getSlot(bool isPrint=false);
    void setType(string type);
    void setIp(string ip);
    void setSlot(string slot);
    void setSlot(set<string> slot);
    void addSlot(string slot);
    void addSlot(set<string> slot);
    void clearSolt();
    bool operator ==(const Device &dev) const;
    bool operator < (const Device &dev) const;
};

#endif // DEVICE_H
