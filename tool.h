#ifndef TOOL_H
#define TOOL_H

#include <QXmlStreamReader>
#include <QFile>
#include <QIODevice>
#include <QXmlStreamWriter>
#include <set>
#include <string>
#include <sstream>
#include "device.h"

class Tool
{
public:
    Tool();
    /**
      *date:2017/12/29
      *author:xhchen
      *brief:保存信息到xml
      *parm:set<Device> 要保存的信息
      *     xml 保存的xml路径
      *     attr 要保存的属性 ip=1 ip+slot=2
    */
    static bool saveXML(set<Device> device,string xml,int attr);
    /**
      *date:2017/12/29
      *author:xhchen
      *brief:读取保存的设备信息
      *parm:xml 保存的xml路径
      *     attr 保存的属性 ip=1 ip+slot=2
      * 如果做成递归解析应该更有通用性，不过情况也不多，有时间再改成递归的吧
    */
    static set<Device> readXML(string xml,int attr);
    /**
      *date:2018/01/12
      *author:xhchen
      *brief:将string的内容分割组装成set，目前用于读取xml时，组装slot——set
      *parm:string s 要分割的字符串
      *     char split 分隔符
    */
    static set<string> stringSplit(string s, char split);
};

#endif // TOOL_H
