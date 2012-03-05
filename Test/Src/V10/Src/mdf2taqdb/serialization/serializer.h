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

}

#endif