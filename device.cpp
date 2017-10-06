#include "device.h"

Device::Device()
{}

Device::Device(string ip)
{
    this->ip=ip;
    //cout<<ip<<endl;
}

bool Device::operator ==(const Device &dev) const
{
    return this->ip==dev.ip;
}

bool Device::operator <(const Device &dev) const
{
    return this->ip!=dev.ip;
}

string Device::getIp()
{
    return this->ip;
}

void Device::setIp(string ip)
{
    this->ip=ip;
}
