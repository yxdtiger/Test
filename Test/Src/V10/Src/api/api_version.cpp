#include "api_version.h"
#include "../environment/strutils.h"

wmdf::ApiVersion::ApiVersion(void)
{
}

wmdf::ApiVersion::~ApiVersion(void)
{
}


int wmdf::ApiVersion::GetMajor()
{
  return API_VERSION_MAJOR;
}

int wmdf::ApiVersion::GetMinor()
{
  return API_VERSION_MINOR;
}

int wmdf::ApiVersion::GetPatch()
{
  return API_VERSION_PATCH;
}

std::string wmdf::ApiVersion::GetVersion()
{
  return StrUtils::formatstring(5,"%d.%d.%d",API_VERSION_MAJOR,API_VERSION_MINOR,API_VERSION_PATCH);
}

bool wmdf::ApiVersion::IsAtLeast( int major, int minor, int patch )
{
  if (API_VERSION_MAJOR > major)
  {
    return true;
  }
  else if (API_VERSION_MAJOR == major)
  {
    if (API_VERSION_MINOR > minor)
    {
      return true;
    }
    else if (API_VERSION_MINOR == minor)
    {
      return API_VERSION_PATCH > patch;
    }
  }
  return false;
}

