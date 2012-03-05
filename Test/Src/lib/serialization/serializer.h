#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include "persistable.h"
#include "memento.h"

using namespace std;

namespace wmdf{

class Persistable;
class Memento;

class  Serializer
{
public:
	Serializer(void);
	virtual ~Serializer(void);
public:
	virtual bool Serialization(Persistable* Persistable) = 0;
	virtual bool Deserialization(Persistable* Persistable) = 0;
};

class  XmlFileSerializer: public Serializer
{
public:
	XmlFileSerializer(const string& Path){m_Path = Path;}
public:
	virtual bool Serialization(Persistable* Persistable);
	virtual bool Deserialization(Persistable* Persistable);
private:
	string m_Path;
};

class  XmlBufferSerializer: public Serializer
{
public:
	XmlBufferSerializer(char** pBuf);
	XmlBufferSerializer(char* buf,int MaxLen);
public:
	virtual bool Serialization(Persistable* Persistable);
	virtual bool Deserialization(Persistable* Persistable);
private:
	char** m_pBuff;
	char* m_Buf;
	int m_MaxLen;
};

class  XmlStringSerializer: public Serializer
{
public:
  XmlStringSerializer();
  std::string GetXml(){return xml_;}
  void SetXml(std::string xml){xml_ = xml;}
public:
  virtual bool Serialization(Persistable* persistable);
  virtual bool Deserialization(Persistable* persistable);
private:
  std::string xml_;
};

}

#endif
