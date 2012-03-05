#pragma once

#include "ManageProductSourceListCtrl.h"
#include "../../product/product_source_manager.h"
#include "ManagmentProductSourceAdd.h"
#include "afxcmn.h"

// ManageProductSourceView form view

class ManageProductSourceView : public CFormView
{
	DECLARE_DYNCREATE(ManageProductSourceView)

protected:
	ManageProductSourceView();           // protected constructor used by dynamic creation
	virtual ~ManageProductSourceView();

public:
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
  ManageProductSourceListCtrl m_conListProductSource;

  wmdf::ProductSourceManager m_productSourceManager;

  CManagmentProductSourceAdd* m_pAddDlg;

private:
  // 在界面上添加user
  void AddProductSource(const wmdf::ProductSource& user);

  // 在界面上添加user
  void ModifyProductSource(const wmdf::ProductSource& user);

  // 保存至xml文件中
  void SaveToXmlFile(const std::string& file_path);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnDestroy();
  virtual void OnInitialUpdate();


  afx_msg void OnBnClickedButtonAdd();

  afx_msg LRESULT OnAddProductSource(WPARAM wParam, LPARAM lParam);
  afx_msg void OnBnClickedButtonModify();
  afx_msg void OnBnClickedButtonDelete();
  CTreeCtrl m_conProductSourceTree;
};


