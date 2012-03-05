#ifndef USER_MANAGER_H_
#define USER_MANAGER_H_

#include <string>
#include <vector>
#include "persistable.h"
#include "memento.h"
#include "authorized_product.h"
#include "user.h"
#include "../include/wmdf_api.h"
#include <boost/thread/mutex.hpp>


namespace wmdf{

  class UserManager : public Persistable
  {
  public:
    UserManager();
    virtual ~UserManager();
    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    User* FindUser(const string& userName);
    User* FindUser(int32_t user_id);
    int32_t max_userid();
    User* users(int32_t index) const {return users_[index];}
    int32_t users_count() const {return (int32_t)users_.size();}
    void add_users(User* user);
    int32_t delete_users(int32_t user_id);
  private:
    void Clear();
  private:
    vector<User*> users_;
    boost::mutex mutex_;
  };


}


#endif
