#pragma once
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// CManageProductView form view

#pragma once
#include "afxwin.h"

#include "../../product/product_manager.h"
#include "ManageProductAddProductDescripterDialog.h"
#include "ManageProductAddVersionDescripterDialog.h"
#include "ManageProductAddMessageDescripterDialog.h"
#include "../common/ExpandTreeCtrl.h"

enum PRODUCT_INNER_TYPE;

class CManageProductView : public CFormView
{
  DECLARE_DYNCREATE(CManageProductView)

protected:
	CManageProductView();           // protected constructor used by dynamic creation
	virtual ~CManageProductView();

public:
	enum { IDD = IDD_MANAGE_PRODUCT };
#ifdef _DEBUG
  virtual void AssertValid() const;
#ifndef _WIN32_WCE
  virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

public:
  // 产品管理类
  wmdf::ProductManager m_product_manager;

  CExpandTreeCtrl m_conTreeProduct;

  // 添加各类描述的对话框
  CManageProductAddProductDescripterDialog* m_pAddProductDescripterDialog;
  CManageProductAddVersionDescripterDialog* m_pAddVersionDescripterDialog;
  CManageProductAddMessageDescripterDialog* m_pAddMessageDescripterDialog;

public:
  // 根据m_product_manager初始化TreeProduct控件
  void InitTreeProduct();

  // 从当前树选择，得到其对应的Descripter描述
  void* GetDescripterFromSelect(PRODUCT_INNER_TYPE& type);

  bool AddProductDescripter(wmdf::ProductDescripter* pPD);
  bool ModifyProductDescripter(wmdf::ProductDescripter* pPD);
  bool AddVersionDescripter(wmdf::VersionDescripter* pVD);
  bool ModifyVersionDescripter(wmdf::VersionDescripter* pVD);
  bool AddMessageDescripter(wmdf::MessageDescripter* pMD);
  bool ModifyMessageDescripter(wmdf::MessageDescripter* pMD);

  // 保存至xml文件中
  void SaveToXmlFile(const std::string& file_path);

  // 检查当前选择item的所有sibling是否与strText重复
  bool SiblingRepeat(CString strText);

  // 刷新static控件的显示内容
  void RefreshContent();

public:
  virtual void OnInitialUpdate();
  afx_msg void OnDestroy();

public:

  afx_msg void OnBnClickedButtonAdd();
  afx_msg void OnBnClickedButtonModify();
  afx_msg void OnBnClickedButtonDelete();

  afx_msg LRESULT OnAddProductDescripter(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnAddVersionDescripter(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnAddMessageDescripter(WPARAM wParam, LPARAM lParam);

  afx_msg void OnTvnSelchangedTreeProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
