#include "user_manager.h"

using namespace std;
using namespace wmdf;

UserManager::UserManager()
{

}
UserManager::~UserManager()
{
  Clear();
}

void UserManager::add_users(User* user)
{
  boost::mutex::scoped_lock lock(mutex_);
  users_.push_back(user);
}

int32_t UserManager::delete_users(int32_t user_id)
{
  boost::mutex::scoped_lock lock(mutex_);
  for(vector<User*>::iterator it = users_.begin(); it != users_.end(); it++)
  {
    if(user_id == (*it)->user_id())
    {
      delete (*it);
      (*it) = NULL;
      it = users_.erase(it);
      return 0;
    }
  }
  return -1;
}

bool UserManager::SaveState(Memento* m)
{
  boost::mutex::scoped_lock lock(mutex_);
  m->SetName("UserManager");
  for(uint32_t i=0;i<users_.size();i++)
  {
    Memento* users_m=m->CreateChild("User");
    users_[i]->SaveState(users_m);
  }
  return true;
}

bool UserManager::LoadState(Memento* m)
{
  boost::mutex::scoped_lock lock(mutex_);
  Clear();
  int32_t userCount=m->GetChildCount();
  for(int32_t i=0;i<userCount;i++)
  {
    Memento* user_m=m->GetChild(i);
    User* pUser=new User();
    pUser->LoadState(user_m);
    users_.push_back(pUser);
  }
  return true;
}


User* UserManager::FindUser(const string& userName)
{
  boost::mutex::scoped_lock lock(mutex_);
  for(uint32_t i=0;i<users_.size();i++)
  {
    if(users_[i]->user_name()==userName)
    {
      return users_[i];
    }
  }
  return NULL;
}

User* UserManager::FindUser(int32_t user_id)
{
  boost::mutex::scoped_lock lock(mutex_);
  for(uint32_t i=0; i<users_.size(); ++i)
  {
    if(user_id == users_[i]->user_id())
    {
      return users_[i];
    }
  }
  return NULL; 
}

void wmdf::UserManager::Clear()
{
  for(uint32_t i=0;i<users_.size();i++)
  {
    delete users_[i];
  }
  users_.clear();
}

int32_t wmdf::UserManager::max_userid()
{
  int32_t user_id = 0;
  for(uint32_t i=0;i<users_.size();i++)
  {
    user_id = max(users_[i]->user_id(),user_id);
  }
  return user_id;
}