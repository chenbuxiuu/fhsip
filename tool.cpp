#include "tool.h"

Tool::Tool()
{

}

/**
  *date:2017/12/29
  *author:xhchen
  *brief:保存信息到xml
  *parm:set<Device> 要保存的信息
  *     xml 保存的xml路径
  *     attr 要保存的属性 ip=1 ip+slot=2
*/


bool Tool::saveXML(set<Device> device, string xml, int attr)
{
    xml="info/"+xml;
    QFile *file=new QFile(QString::fromStdString(xml));
    if(!file->open(QIODevice::WriteOnly))
    {
        return false;
    }
    else if(attr==1)
    {
        QXmlStreamWriter *xmlWriter=new QXmlStreamWriter(file);
        xmlWriter->writeStartDocument();
        xmlWriter->setAutoFormatting(true);
        xmlWriter->writeStartElement("Notes");
        set<Device>::iterator it;
        it=device.begin();
        for(it;it!=device.end();it++)
        {
            xmlWriter->writeStartElement("ip");
            Device temp=*it;
            xmlWriter->writeAttribute("addr",QString::fromStdString(temp.getIp()));
            xmlWriter->writeEndElement();
        }
        xmlWriter->writeEndElement();
        file->close();
        return true;
    }
    else if(attr==2)
    {
        QXmlStreamWriter *xmlWriter=new QXmlStreamWriter(file);
        xmlWriter->writeStartDocument();
        xmlWriter->setAutoFormatting(true);
        xmlWriter->writeStartElement("Notes");
        set<Device>::iterator it;
        it=device.begin();
        for(it;it!=device.end();it++)
        {
            xmlWriter->writeStartElement("ip");
            Device temp=*it;
            xmlWriter->writeAttribute("addr",QString::fromStdString(temp.getIp()));
            set<string> slotInfo=temp.getSlot();
            set<string>::iterator it;
            it=slotInfo.begin();
            string slot="";
            for(it;it!=slotInfo.end();it++)
            {
                string s=*it;
                slot+=s+" ";
            }
            xmlWriter->writeTextElement("slot",QString::fromStdString(slot));
            xmlWriter->writeEndElement();
        }
        xmlWriter->writeEndElement();
        file->close();
        return true;
    }
    else
    {
        //do nothing
        return true;
    }
}

/**
  *date:2017/12/29
  *author:xhchen
  *brief:读取保存的设备信息
  *parm:xml 保存的xml路径
  *     attr 保存的属性 ip=1 ip+slot=2
  * 如果做成递归解析应该更有通用性，不过情况也不多，有时间再改成递归的吧
*/
set<Device> Tool::readXML(string xml, int attr)
{
    xml="info/"+xml;
    QFile *file=new QFile(QString::fromStdString(xml));
    set<Device> device;
    if(!file->open(QIODevice::ReadOnly))
    {
        //无历史信息
    }
    else if(attr==1)
    {
        QXmlStreamReader *xmlReader=new QXmlStreamReader(file);
        while(!xmlReader->hasError()&&!xmlReader->atEnd())
        {
            xmlReader->readNext();
            if(xmlReader->isStartElement())
            {

                if(xmlReader->name()=="ip")
                {
                    string ip=xmlReader->attributes().value("addr").toString().toStdString();
                    Device temp(ip);
                    device.insert(temp);
                }

            }
        }
        file->close();
    }
    if(attr==2)
    {
        QXmlStreamReader *xmlReader=new QXmlStreamReader(file);
        while(!xmlReader->hasError()&&!xmlReader->atEnd())
        {
            xmlReader->readNext();
            if(xmlReader->isStartElement())
            {

                if(xmlReader->name()=="ip")
                {
                    string ip=xmlReader->attributes().value("addr").toString().toStdString();
                    Device d(ip);
                    xmlReader->readNextStartElement();
                    string slot=xmlReader->readElementText().toStdString();
                    set<string> slot_set=stringSplit(slot,' ');
                    d.setSlot(slot_set);
                    device.insert(d);
                }
            }

        }
    }
    else
    {
        //do nothing
    }
    return device;
}
/**
  *date:2018/01/12
  *author:xhchen
  *brief:将string的内容分割组装成set，目前用于读取xml时，组装slot——set
  *parm:string s 要分割的字符串
  *     char split 分隔符
*/
set<string> Tool::stringSplit(string s, char split)
{
    set<string> sList;
    stringstream ss(s);
    string token;
    while(1)
    {
        getline(ss, token, split);
        if ( ss.fail() )
            break;
        sList.insert(token);
    }
    return sList;
}


