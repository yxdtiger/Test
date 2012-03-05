#include "serializer_utils.h"

#ifdef _WIN32
#include <windows.h>
#elif linux
#include <unistd.h>
#define MAX_PATH 260
#endif

wmdf::SerializerUtils::SerializerUtils(void)
{
}

wmdf::SerializerUtils::~SerializerUtils(void)
{
}

wmdf::Serializer* wmdf::SerializerUtils::CreateSerializer( const string& file_name )
{
  return new XmlFileSerializer(file_name);
}

void wmdf::SerializerUtils::Deserialize( const string& file_name,Persistable* persistable )
{
  Serializer* serializer= CreateSerializer(file_name);
  serializer->Deserialization(persistable);
  delete serializer;
  serializer = NULL;
}

void wmdf::SerializerUtils::Serialize( const string& file_name,Persistable* persistable )
{
  Serializer* serializer= CreateSerializer(file_name);
  serializer->Serialization(persistable);
  delete serializer;
  serializer = NULL;
}
