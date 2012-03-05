#include "dispatching_center.h"
#include "../product/adapter/iproduct_adapter.h"

wmdf::DispatchingCenter::DispatchingCenter(void)
{
}

wmdf::DispatchingCenter::~DispatchingCenter(void)
{
  for(uint32_t index=0;index<adapters_.size();index++)
  {
    delete adapters_[index];
  }
  adapters_.clear();
}

void wmdf::DispatchingCenter::AddAdapter( IProductAdapter* adapter )
{
  adapters_.push_back(adapter);
}

void wmdf::DispatchingCenter::RemoveAdapter( IProductAdapter* adapter )
{
  std::vector<IProductAdapter*>::iterator iter=adapters_.begin(); 
  while (iter!=adapters_.end())
  {  
    //to do(aza.tang) to be checked...
    if(*iter == adapter)  
    {  
        delete *iter;
        *iter = NULL;
        adapters_.erase(iter);  
        return;
    }  
    iter++; 
}    
}

wmdf::IProductAdapter* wmdf::DispatchingCenter::adapters( int index )
{
  return adapters_[index];
}

int32_t wmdf::DispatchingCenter::adapters_count()
{
  return (int32_t)adapters_.size();
}

void wmdf::DispatchingCenter::StartAllAdapters()
{
  for (int32_t index=0;index< adapters_count();index++)
  {
    adapters_[index]->Start();
  }
}

void wmdf::DispatchingCenter::StopAllAdapters()
{
  for (int32_t index=0;index< adapters_count();index++)
  {
    adapters_[index]->Stop();
  }
}

void wmdf::DispatchingCenter::StartAllLoggers()
{
  for (int32_t index=0;index< adapters_count();index++)
  {
    adapters_[index]->StartLogger();
  }
}

void wmdf::DispatchingCenter::StopAllLoggers()
{
  for (int32_t index=0;index< adapters_count();index++)
  {
    adapters_[index]->StopLogger();
  }
}