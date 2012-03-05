#ifndef WARNING_CENTER_H
#define WARNING_CENTER_H
#include <map>
#include "../common/studio_utility.h"

namespace wmdf
{
  class WarningCenter
  {
  public:
    WarningCenter(void);
    virtual ~WarningCenter(void);

    bool is_sound_enable() const {return is_sound_enable_;}
    void set_is_sound_enable(bool flag){is_sound_enable_=flag;} 

    WarningInfo* GetWarningInfo(std::string key);
    bool NeedAlarm();
    void AddWarningInfo(WarningInfo* waringInfo);
    void DeleteWarningInfo(std::string key,int point);
    void DeleteAllWarnings();

  private:
    std::map<std::string, WarningInfo*> map_warnings_;
    bool is_sound_enable_;
  };
}

#endif
