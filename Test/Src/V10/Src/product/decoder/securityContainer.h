#ifndef SECURITY_CONTAINER_H
#define SECURITY_CONTAINER_H

#include <string>
#include <vector>
#include "../../include/wmdf_api.h"
#include "../../include/wmdf_structs.h"

namespace wmdf
{
  class IndexBase;

  class SecurityContainer
  {
  public:
    SecurityContainer();
    ~SecurityContainer();
    std::string windcode(){return windcode_;}
    void set_windcode(std::string windcode){windcode_=windcode;}
    int32_t indexes_count();
    void add_index(IndexBase* index);
    IndexBase* index(int32_t i){return indexes_[i];}
  private:
    std::string windcode_;
    std::vector<IndexBase*> indexes_;
  };

  class SecurityContainerManager
  {
  public:
    SecurityContainerManager();
    ~SecurityContainerManager();
	  void reset();

    void l2marketsnapshot_plus();
    void l2orderqueue_plus();
    void l2orders_plus();
    void l2transaction_plus();

    int32_t l2marketsnapshot_count();
    int32_t l2orderqueue_count();
    int32_t l2orders_count();
    int32_t l2transaction_count();

    void add_securitycontainer(SecurityContainer* securitycontainer);

    L2OrderQueue* TransferToSZSEL2OrderQueue();
    L2Transaction* TransferToSZSEL2Transactions();
    SZSEL2Order* TransferToSZSEL2Orders();
    L2MarketSnapshot* TransferToSZSEL2MarketSnapshot();


    L2OrderQueue* TransferToSSEL2OrderQueue();
    L2MarketSnapshot* TranseferToSSEL2MarketSnapshot();
    L2Transaction* TransferToSSEL2Transactions();

    L1MarketSnapshot* TransferToSZSEL1MarketSnapshot();
		L1MarketSnapshot* TransferToSSEL1MarketSnapshot();

   
  private:
    std::vector<SecurityContainer*> security_containers_;
    int32_t num_of_l2marketsnapshot_;
    int32_t num_of_l2orderqueue_;
    int32_t num_of_l2orders_;
    int32_t num_of_l2transaction_;
  };
}


#endif