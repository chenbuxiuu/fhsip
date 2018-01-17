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

set<string> Device::getSlot(bool isPrint)
{
    if(isPrint==true)
    {
        set<string>::iterator it;
        it=slot.begin();
        for(it;it!=slot.end();it++)
        {
            cout<<(*it)<<"  ";
        }
        cout<<endl;
    }

    return this->slot;

}

void Device::setIp(string ip)
{
    this->ip=ip;
}

void Device::setSlot(string slot)
{
    this->slot.clear();
    this->slot.insert(slot);
}

void Device::addSlot(string slot)
{
    this->slot.clear();
    this->slot.insert(slot);
}

void Device::setSlot(set<string> slot)
{
    this->slot=slot;
}

void Device::addSlot(set<string> slot)
{
    set<string>::iterator it;
    it=slot.begin();
    for(it;it!=slot.end();it++)
    {
        this->slot.insert(*it);
    }
}

string Device::getType()
{
    return this->type;
}

void Device::setType(string type)
{
    this->type=type;
}

/**
  *date:2018/01/12
  *author:xhchen
  *brief:清空slot set
  *parm:
*/
void Device::clearSolt()
{
//    this->slot.clear();
//    this->slot.insert("");
}

