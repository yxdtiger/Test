#if !defined MEMENTOWRITER_H
#define MEMENTOWRITER_H

#include "memento.h"
#include <string>
#include "../tinyxml/tinyxml.h"

namespace wmdf
{

class MementoWriter
{
public:
	MementoWriter(){};
	virtual ~MementoWriter(){};
public:
	virtual void Write(Memento *pm, char *path) = 0;
	virtual char* WriteToBuf(Memento *pm) = 0;
  virtual std::string WriteToString(Memento *pm) = 0;
};

class XMLMementoWriter :public MementoWriter
{
public:
	XMLMementoWriter(){};
	virtual ~XMLMementoWriter(){};
private:
	void ProcessMemento(Memento *pm, TiXmlElement *pNode, TiXmlDocument &doc);
public:
	void Write(Memento *pm, TiXmlDocument& doc);
	virtual void Write(Memento *pm, char *path);
	virtual char* WriteToBuf(Memento *pm) ;
  virtual std::string WriteToString(Memento *pm);
};

}

#endif
