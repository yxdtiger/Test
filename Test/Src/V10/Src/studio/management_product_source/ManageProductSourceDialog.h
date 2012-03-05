#pragma once

#include "../../product/product_source_manager.h"
#include "../management_product/ManageProductDialog.h"

// CManageProductSourceDialog form view
struct source_data;

class CManageProductSourceDialog : public CManageProductDialog
{
	DECLARE_DYNCREATE(CManageProductSourceDialog)

public:
	CManageProductSourceDialog(UINT id = IDD, CWnd* pParent = NULL);           // protected constructor used by dynamic creation
	virtual ~CManageProductSourceDialog();

public:
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE };

protected:

private:

  // 根据m_productSourceManager重画m_conProductSourceTree
  virtual void InitTreeProduct();

  // 刷新static控件的显示内容
  virtual void RefreshContent();

  bool AddProductSource(wmdf::ProductSource* pItem);
  bool AddMarketTime(wmdf::MarketTime* pItem);
  bool AddBackupSource(wmdf::BackupSource* pItem);
  bool AddSubscribedProduct(wmdf::ProtocolDescription* pItem);
  bool ModifyProductSource(wmdf::ProductSource* pItem);
  bool ModifyMarketTime(wmdf::MarketTime* pItem);
  bool ModifyBackupSource(wmdf::BackupSource* pItem);
  bool ModifySubscribedProduct(wmdf::ProtocolDescription* pItem);

  // 载入数据管理器
  virtual bool LoadDataManager();

  void GetContentFromData(CString& strContent, wmdf::ProductSource* pData);
  void GetContentFromData(CString& strContent, wmdf::MarketTime* pData);
  void GetContentFromData(CString& strContent, wmdf::BackupSource* pData);
  void GetContentFromData(CString& strContent, wmdf::ProtocolDescription* pData);

  // 虚函数，根据pSourceData的指定类型，创建不同的CAddOrModifyDialog
  virtual void CreateAddModifyDialog(UINT type, CAddOrModifyDialog*& pDlg, bool bAdd = true);

  // 虚函数，从数据管理器中删除数据
  virtual void DeleteData(source_data* pSourceData);

  // 保存数据
  virtual void MementoData();

  virtual void ParseDataItem(void* pData, bool bAdd, ENUM_DIALOG_TYPE type);

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedGetConfigButton();
  afx_msg void OnBnClickedUpdateConfigButton();
};