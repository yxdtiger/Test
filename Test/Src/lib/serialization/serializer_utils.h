#ifndef SERIALIZER_UTILS_H
#define SERIALIZER_UTILS_H
#include "serializer.h"

namespace wmdf
{
  class SerializerUtils
  {
  public:
    SerializerUtils(void);
    virtual ~SerializerUtils(void);
    static Serializer* CreateSerializer(const string& file_name);
    static void Deserialize(const string& file_name,Persistable* persistable);
    static void Serialize(const string& file_name,Persistable* persistable);
  };
}

#endif
