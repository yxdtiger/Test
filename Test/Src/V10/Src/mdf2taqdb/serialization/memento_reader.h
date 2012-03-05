#if !defined MEMENTOREADER_H
#define MEMENTOREADER_H

#include "memento.h"
#include "../tinyxml/tinyxml.h"

namespace wmdf{

class MementoReader
{
public:
	MementoReader(){};
	virtual ~MementoReader(){};
	virtual void ReadFromFile(Memento *pm, char *path) = 0;
	virtual void ReadFromString(Memento *pm, char *string) = 0;
};

class XMLMementoReader: public MementoReader
{
public:
	XMLMementoReader();
	~XMLMementoReader();
private:
	void ReadFromXml(Memento *pm, TiXmlDocument& doc);
	void ProcessMemento(Memento *pm, TiXmlElement *pNode);
public:
	void ReadFromFile(Memento *pm, char *path);
	void ReadFromString(Memento *pm, char *string);

};

}
#endif