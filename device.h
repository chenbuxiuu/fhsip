#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <iostream>
using namespace std;

//设备父类
class Device
{
private:
    string ip;
public: 
    Device();
    Device(string ip);
    string getIp();
    void setIp(string ip);
    bool operator ==(const Device &dev) const;
    bool operator < (const Device &dev) const;
};

#endif // DEVICE_H
