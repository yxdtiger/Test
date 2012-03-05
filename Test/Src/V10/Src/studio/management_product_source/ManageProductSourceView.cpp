// ManageProductSourceView.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/Resource.h"
#include "ManageProductSourceView.h"
#include "../main_frame/MainFrm.h"
#include <assert.h>

#include "../../environment/file.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
using namespace wmdf;

// ManageProductSourceView

IMPLEMENT_DYNCREATE(ManageProductSourceView, CFormView)

ManageProductSourceView::ManageProductSourceView()
	: CFormView(ManageProductSourceView::IDD)
{
  m_pAddDlg = new CManagmentProductSourceAdd(this);
  m_pAddDlg->Create(IDD_DIALOG_MANAGMENT_PRODUCT_SOURCE_ADD);
}

ManageProductSourceView::~ManageProductSourceView()
{
  delete m_pAddDlg;
  m_pAddDlg = NULL;
}

void ManageProductSourceView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST_PRODUCT_SOURCE, m_conListProductSource);
  DDX_Control(pDX, IDC_TREE_SOURCE, m_conProductSourceTree);
}

BEGIN_MESSAGE_MAP(ManageProductSourceView, CFormView)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BUTTON_ADD, &ManageProductSourceView::OnBnClickedButtonAdd)
  ON_MESSAGE(WM_ADD_PRODUCT_SOURCE, OnAddProductSource)
  ON_BN_CLICKED(IDC_BUTTON_MODIFY, &ManageProductSourceView::OnBnClickedButtonModify)
  ON_BN_CLICKED(IDC_BUTTON_DELETE, &ManageProductSourceView::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// ManageProductSourceView diagnostics

#ifdef _DEBUG
void ManageProductSourceView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ManageProductSourceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ManageProductSourceView message handlers

void ManageProductSourceView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pManageProductSource=NULL; // 清空窗口指针
}

void ManageProductSourceView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: Add your specialized code here and/or call the base class
  GetParentFrame()->SetWindowText("产品源管理");

  // set the style of listctrl
  m_conListProductSource.SetExtendedStyle(LVS_EX_FLATSB
    |LVS_EX_FULLROWSELECT
    |LVS_EX_HEADERDRAGDROP
    |LVS_EX_ONECLICKACTIVATE
    |LVS_EX_GRIDLINES);

  m_conListProductSource.SetExtendedStyle(LVS_EX_FLATSB
    |LVS_EX_FULLROWSELECT
    |LVS_EX_HEADERDRAGDROP
    |LVS_EX_ONECLICKACTIVATE
    |LVS_EX_GRIDLINES);

  // load user configuration from "user.xml"
  if (File::Exists(ApplicationDirectory::SourceDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::SourceDescriptionFile(),&m_productSourceManager);
  }

  // initialize CListCtrl's header column
  int column_index = 0;
  m_conListProductSource.InsertColumn(column_index++, _T("产品ID"), LVCFMT_LEFT, 100);
  m_conListProductSource.InsertColumn(column_index++, _T("产品源IP地址"), LVCFMT_LEFT, 100);
  m_conListProductSource.InsertColumn(column_index++, _T("产品源端口号"), LVCFMT_LEFT, 100);
  m_conListProductSource.InsertColumn(column_index++, _T("描述"), LVCFMT_LEFT, 100);

  // 插入产品源
  for(int product_source_index = 0; product_source_index < m_productSourceManager.product_source_count(); ++product_source_index)
  {
    m_conListProductSource.InsertProductSource(m_productSourceManager.product_sources(product_source_index));
  }
 
}

void ManageProductSourceView::OnBnClickedButtonAdd()
{
  // TODO: Add your control notification handler code here
  m_pAddDlg->ShowWindow(SW_SHOW);
}

LRESULT ManageProductSourceView::OnAddProductSource(WPARAM wParam, LPARAM lParam)
{
  ProductSource* pProductSource = reinterpret_cast<ProductSource*>(wParam);
  bool bAdd = *(reinterpret_cast<bool*>(lParam));

  if(bAdd)
    AddProductSource(*pProductSource);
  else
    ModifyProductSource(*pProductSource);

  // 保存至配置文件
  SaveToXmlFile( ApplicationDirectory::SourceDescriptionFile() );

  return TRUE;
}

void ManageProductSourceView::AddProductSource(const ProductSource& productSource)
{
  ProductSource* pProductSource = NULL;
  if( NULL == (pProductSource = m_productSourceManager.FindProductSource(productSource.product_id())) )
  {
    MessageBox("产品id已存在，添加产品源失败!", "错误", MB_ICONERROR);
    return;
  }

  pProductSource = new ProductSource;
  *pProductSource = productSource;

  m_productSourceManager.add_product_source(pProductSource);

  m_conListProductSource.InsertProductSource(pProductSource);
}

void ManageProductSourceView::ModifyProductSource(const ProductSource& productSource)
{
  ProductSource* pProductSource = new ProductSource;
  *pProductSource = productSource;

  // 先删除，后插入
  m_conListProductSource.DeleteProductSource(pProductSource->product_id());
  m_productSourceManager.delete_product_source(pProductSource->product_id());

  m_conListProductSource.InsertProductSource(pProductSource);
  m_productSourceManager.add_product_source(pProductSource);
}

void ManageProductSourceView::SaveToXmlFile(const std::string& file_path)
{
  Utils::Serialize(file_path, reinterpret_cast<Persistable*>(&m_productSourceManager));
};

void ManageProductSourceView::OnBnClickedButtonModify()
{
  // TODO: Add your control notification handler code here
  POSITION pos = m_conListProductSource.GetFirstSelectedItemPosition();
  int nItem = m_conListProductSource.GetNextSelectedItem(pos);

  // 获取改行的id
  CString str_id = m_conListProductSource.GetItemText(nItem, 0);
  int32_t product_source_id = atoi(str_id.GetBuffer(0));
  str_id.ReleaseBuffer();

  // 找到id对应的user
  ProductSource* pProductSource = NULL;
  pProductSource = m_productSourceManager.FindProductSource(product_source_id);
  if(NULL == pProductSource)
  {
    MessageBox("请选择要修改的对象", "错误", MB_ICONERROR);
    return;
  }

  // 利用pUser填充CUserModifyDlg
  CManagmentProductSourceAdd product_source_modify_dialog(this, false);
  product_source_modify_dialog.SetProductSource(*pProductSource);

  // create a modeless dialog
  // ptr_modify_dlg_->ShowWindow(SW_SHOW);

  if(IDOK == product_source_modify_dialog.DoModal())
  {

  }
  else
  {

  }
}

void ManageProductSourceView::OnBnClickedButtonDelete()
{
  // TODO: Add your control notification handler code here
  POSITION pos = m_conListProductSource.GetFirstSelectedItemPosition();
  if(NULL == pos)
  {
    MessageBox("请选择需要删除的行.");
    return;
  }

  int nItemIndex = m_conListProductSource.GetNextSelectedItem(pos);
  CString strProductSourceId = m_conListProductSource.GetItemText(nItemIndex, 0);
  CString strContext = "删除id为" + strProductSourceId + "的产品数据源?";
  if(IDOK == MessageBox(strContext, "删除", MB_OKCANCEL))
  {
    int32_t product_source_id = atoi(strProductSourceId.GetBuffer(0));
    strProductSourceId.ReleaseBuffer();
    m_conListProductSource.DeleteProductSource(product_source_id);
    m_productSourceManager.delete_product_source(product_source_id);

    // 保存至配置文件
    SaveToXmlFile( ApplicationDirectory::SourceDescriptionFile() );
  }
}