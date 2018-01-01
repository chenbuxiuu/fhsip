#include "device.h"

Device::Device()
{

}

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

set<string> Device::getSlot()
{
    return this->slot;
}

void Device::setIp(string ip)
{
    this->ip=ip;
}

void Device::setSlot(string slot)
{
    this->slot.insert(slot);
}

void Device::setSlot(set<string> slot)
{
    set<string>::iterator it;
    it=slot.begin();
    for(it;it!=slot.end();it++)
    {
        this->slot.insert(*it);
    }
}

