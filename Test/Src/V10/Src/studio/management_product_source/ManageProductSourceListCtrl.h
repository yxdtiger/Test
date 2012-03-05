#pragma once

#include "../../product/product_source.h"
#include "../common/MyListCtrl.h"

// ManageProductSourceListCtrl

class ManageProductSourceListCtrl : public CMyListCtrl
{
	DECLARE_DYNAMIC(ManageProductSourceListCtrl)

public:
	ManageProductSourceListCtrl();
	virtual ~ManageProductSourceListCtrl();

public:
  void InsertProductSource(wmdf::ProductSource* pProductSource);
  void DeleteProductSource(int32_t product_id);

protected:
	DECLARE_MESSAGE_MAP()
};


