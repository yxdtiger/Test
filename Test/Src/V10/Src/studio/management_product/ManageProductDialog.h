#pragma once
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// CManageProductDialog form view

#pragma once
#include "afxwin.h"

#include "../../product/product_manager.h"
#include "ManageProductAddProductDescripterDialog.h"
#include "ManageProductAddVersionDescripterDialog.h"
#include "ManageProductAddMessageDescripterDialog.h"
#include "../common/ExpandTreeCtrl.h"
#include <vector>
#include "../common/BeautifulButton.h"

struct source_data;

class CManageProductDialog : public CDialog
{
  DECLARE_DYNCREATE(CManageProductDialog)

public:
	CManageProductDialog(UINT id = IDD, CWnd* pParent = NULL);           // protected constructor used by dynamic creation
	virtual ~CManageProductDialog();

public:
	enum { IDD = IDD_MANAGE_PRODUCT };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

public:
  // 产品管理类
  // wmdf::ProductManager m_product_manager;

  wmdf::Persistable* m_pDataManager;

  CExpandTreeCtrl m_conTreeProduct;

public:
  // 根据m_product_manager初始化TreeProduct控件
  virtual void InitTreeProduct();

  // 从当前树选择，得到其对应的Descripter描述
  virtual void GetDescripterFromSelect(source_data*& pSourceData, std::vector<void*>& arrAncestor);

  bool AddProductDescripter(wmdf::ProductDescripter* pPD);
  bool AddVersionDescripter(wmdf::VersionDescripter* pVD);
  bool AddMessageDescripter(wmdf::MessageDescripter* pMD);
  bool ModifyProductDescripter(wmdf::ProductDescripter* pPD);
  bool ModifyVersionDescripter(wmdf::VersionDescripter* pVD);
  bool ModifyMessageDescripter(wmdf::MessageDescripter* pMD);

  // 检查当前选择item的所有sibling是否与strText重复
  bool SiblingRepeat(CString strText);

  // 刷新static控件的显示内容
  virtual void RefreshContent();

  void GetContentFromData(CString& strContent, wmdf::ProductDescripter* pData);
  void GetContentFromData(CString& strContent, wmdf::VersionDescripter* pData);
  void GetContentFromData(CString& strContent, wmdf::MessageDescripter* pData);

  // 虚函数，根据pSourceData的指定类型，创建不同的CAddOrModifyDialog
  virtual void CreateAddModifyDialog(UINT type, CAddOrModifyDialog*& pDlg, bool bAdd = true);

  // 虚函数，从数据管理器中删除数据
  virtual void DeleteData(source_data* pSourceData);
  virtual bool LoadDataManager();
  // 保存数据
  virtual void MementoData();

  virtual void ParseDataItem(void* pData, bool bAdd, ENUM_DIALOG_TYPE type);

public:

  virtual afx_msg void OnBnClickedButtonAdd();
  virtual afx_msg void OnBnClickedButtonModify();
  virtual afx_msg void OnBnClickedButtonDelete();

  afx_msg LRESULT OnAddDataItem(WPARAM wParam, LPARAM lParam);

  virtual afx_msg void OnTvnSelchangedTreeProduct(NMHDR *pNMHDR, LRESULT *pResult);
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedOk();
  CButton m_conButton_add;
  CButton m_conButton_modify;
  CButton m_conButton_delete;
  CButton m_conButton_search;
  CButton m_conButton_ok;
  CButton m_conButton_cancel;
  CComboBox m_dest_server;
  bool m_flag;
  afx_msg void OnBnClickedGetConfigButton();
  afx_msg void OnBnClickedUpdateConfigButton();
  
};
