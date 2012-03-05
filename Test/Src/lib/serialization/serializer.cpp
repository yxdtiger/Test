#include "serializer.h"
#include "persistable.h"
#include "memento_writer.h"
#include "memento_reader.h"

using namespace wmdf;

Serializer::Serializer(void)
{
}

Serializer::~Serializer(void)
{
}

bool XmlFileSerializer::Serialization(Persistable* Persistable)
{
	XMLMementoWriter writer;
	Memento m;
	bool b = Persistable->SaveState(&m);
	writer.Write(&m, (char*)m_Path.c_str());
	return b;
}

bool XmlFileSerializer::Deserialization(Persistable* Persistable)
{
	XMLMementoReader reader;
	Memento m;
	reader.ReadFromFile(&m,(char*)m_Path.c_str());
	return Persistable->LoadState(&m);
}

XmlBufferSerializer::XmlBufferSerializer(char** pBuf)
{
	assert(pBuf != NULL);
	m_pBuff = pBuf;
	m_Buf = NULL;
	m_MaxLen = 0;
}

XmlBufferSerializer::XmlBufferSerializer(char* buf,int MaxLen)
{
	m_pBuff = NULL;
	assert(buf != NULL);
	m_Buf = buf;
	m_MaxLen = MaxLen;
}

bool XmlBufferSerializer::Serialization(Persistable* Persistable)
{
	XMLMementoWriter writer;
	Memento m;
	bool b = Persistable->SaveState(&m);
	if (m_pBuff != NULL)
	{
		*m_pBuff = writer.WriteToBuf(&m);
	}
	else
	{
		char* newBuf = writer.WriteToBuf(&m);
		strcpy(m_Buf,newBuf);
		delete [] newBuf;
	}
	return b;
}
bool XmlBufferSerializer::Deserialization(Persistable* Persistable)
{
	XMLMementoReader reader;
	Memento m;
	char* wBuf = NULL;
	if (m_pBuff != NULL)
		wBuf = *m_pBuff;
	else
		wBuf = m_Buf;
	reader.ReadFromString(&m,wBuf);
	delete []wBuf; 
	return Persistable->LoadState(&m);
}

wmdf::XmlStringSerializer::XmlStringSerializer()
{
  xml_ = "";
}

bool wmdf::XmlStringSerializer::Serialization( Persistable* persistable )
{
  XMLMementoWriter writer;
  Memento m;
  bool b = persistable->SaveState(&m);
  xml_ = writer.WriteToString(&m);  
  return b;
}

bool wmdf::XmlStringSerializer::Deserialization( Persistable* persistable )
{
  XMLMementoReader reader;
  Memento m;  
  if(xml_.empty())
  {
    return false;
  }
  else
  {
    reader.ReadFromString(&m,(char*)xml_.c_str());
    return persistable->LoadState(&m);
  }
}
