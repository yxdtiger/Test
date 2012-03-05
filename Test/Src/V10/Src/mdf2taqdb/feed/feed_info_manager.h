#ifndef TARGET_DB_MANAGER_H
#define TARGET_DB_MANAGER_H

#include "feed_info.h"
#include "../serialization/persistable.h"
#include "../serialization/memento.h"

namespace wmdf
{
  class FeedInfoManager:public Persistable
  {
  public:
    FeedInfoManager(void);
    virtual ~FeedInfoManager(void);

    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    FeedInfo* feed_infos(int32_t index){return feed_infos_[index];}
    void add_feed_info(FeedInfo* feed_info){feed_infos_.push_back(feed_info);}
    int32_t feed_info_count(){return (int32_t)feed_infos_.size();}
  private:
    std::vector<FeedInfo*> feed_infos_;
  };
}

#endif
