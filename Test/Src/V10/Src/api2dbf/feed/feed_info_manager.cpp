#include "feed_info_manager.h"

wmdf::FeedInfoManager::FeedInfoManager(void)
{
}

wmdf::FeedInfoManager::~FeedInfoManager(void)
{
}

bool wmdf::FeedInfoManager::LoadState( Memento* m )
{
  int child_num=m->GetChildCount();
  for(int i=0;i<child_num;i++)
  {
    Memento* sub_m=m->GetChild(i);
    FeedInfo* pt=new FeedInfo();
    pt->LoadState(sub_m);
    feed_infos_.push_back(pt);
  }
  return true;
}

bool wmdf::FeedInfoManager::SaveState( Memento* m )
{
  m->SetName("ProductTargetManager");
  for(uint32_t i=0;i<feed_infos_.size();i++)
  {
    Memento* sub_m=m->CreateChild("ProductTarget");
    feed_infos_[i]->SaveState(sub_m);
  }
  return true;
}





//wmdf::ProductTarget* wmdf::FeedInfoManager::FindTargetInfo(int32_t product_id)
//{
//  wmdf::ProductTarget* ptr_product_target = NULL;
//  for(int i = 0; i < feed_info_count(); ++i)
//  {
//    for(int j=0;j<feed_infos(i)->target_info_count();j++)
//    {
//      if(product_id == feed_infos(i)->targets(j)->product_id())
//      {
//        ptr_product_target = feed_infos(i)->targets(j);
//        break;
//      }
//    }
//    
//  }
//  return ptr_product_target;
//}
