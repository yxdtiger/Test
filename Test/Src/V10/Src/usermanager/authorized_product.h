#ifndef AUTHORIZED_PRODUCT_H_
#define AUTHORIZED_PRODUCT_H_

#include <string>
#include <vector>
#include "persistable.h"
#include "memento.h"
#include "../include/wmdf_api.h"

namespace wmdf{

class AuthorizedProduct : public Persistable
{
public:
	AuthorizedProduct();
  virtual ~AuthorizedProduct();
	virtual bool SaveState(Memento* m);
	virtual bool LoadState(Memento* m);
	int16_t product_id() const {return product_id_;}
	void set_product_id(int16_t product_id){product_id_=product_id;}
  int32_t message_count() const {return (int32_t)messages_.size();}
	int16_t message_id(int32_t index) const {return messages_[index];}
	void add_message_id(int16_t id){messages_.push_back(id);}
  bool contains (int16_t id);
public:
  AuthorizedProduct& operator= (const AuthorizedProduct& rhs);

private:
	int16_t product_id_;
	std::vector<int16_t> messages_;
};

}

#endif
