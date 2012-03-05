// ManageProductDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "ManageProductDialog.h"
#include "../main_frame/MainFrm.h"
#include "../common/studio_utility.h"
#include <sstream>
#include <assert.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

#include "../../environment/file.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
#include "../../environment/splitter.h"
#include "../../environment/server_environment.h"
using namespace wmdf;

// define a data enum which will be set as item data to m_conTreeProduct
enum PRODUCT_INNER_TYPE
{
  ROOT_DES = 0,
  PRODUCT_DES,
  VERSION_DES,
  MESSAGE_DES
};

struct source_data
{
  PRODUCT_INNER_TYPE type;
  void* ptr;
};


/////////////////////////////////////////////////////////////////////////////
// CManageProductDialog

IMPLEMENT_DYNCREATE(CManageProductDialog, CDialog)

CManageProductDialog::CManageProductDialog(UINT id /* = IDD */, CWnd* pParent /* = NULL */)
	: CDialog(id, pParent)
{
  m_flag = false;
}

CManageProductDialog::~CManageProductDialog()
{
}

void CManageProductDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TREE_PRODUCT, m_conTreeProduct);
  DDX_Control(pDX, IDC_BUTTON_ADD, m_conButton_add);
  DDX_Control(pDX, IDC_BUTTON_MODIFY, m_conButton_modify);
  DDX_Control(pDX, IDC_BUTTON_DELETE, m_conButton_delete);
  DDX_Control(pDX, IDC_BUTTON_SEARCH, m_conButton_search);
  DDX_Control(pDX, IDOK, m_conButton_ok);
  DDX_Control(pDX, IDCANCEL, m_conButton_cancel);
  DDX_Control(pDX, DEST_SERVER_ID, m_dest_server);
}


BEGIN_MESSAGE_MAP(CManageProductDialog, CDialog)
	ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BUTTON_ADD, &CManageProductDialog::OnBnClickedButtonAdd)
  ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CManageProductDialog::OnBnClickedButtonModify)
  ON_BN_CLICKED(IDC_BUTTON_DELETE, &CManageProductDialog::OnBnClickedButtonDelete)
  ON_MESSAGE(WM_ADD_MODIFY_ITEM, &CManageProductDialog::OnAddDataItem)
  ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRODUCT, &CManageProductDialog::OnTvnSelchangedTreeProduct)
  ON_BN_CLICKED(IDOK, &CManageProductDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_GET_CONFIG_BUTTON, &CManageProductDialog::OnBnClickedGetConfigButton)
  ON_BN_CLICKED(IDC_UPDATE_CONFIG_BUTTON, &CManageProductDialog::OnBnClickedUpdateConfigButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageProductDialog diagnostics

/////////////////////////////////////////////////////////////////////////////
// CManageProductDialog message handlers

void CManageProductDialog::InitTreeProduct()
{
  // delete all
  m_conTreeProduct.DeleteAllItems();

  // create root
  TVINSERTSTRUCT tvInsert;
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = _T("产品管理");
  HTREEITEM hRoot = m_conTreeProduct.InsertItem(&tvInsert);
  source_data* pSourceData = new source_data;
  pSourceData->type = ROOT_DES;
  pSourceData->ptr = NULL;
  m_conTreeProduct.SetItemData(hRoot, reinterpret_cast<DWORD_PTR>(pSourceData));

  // create ProductDescripter
  int nProductCount = reinterpret_cast<ProductManager*>(m_pDataManager)->products_count();
  for(int i = 0; i < nProductCount; ++i)
  {
    ProductDescripter* pPD = reinterpret_cast<ProductManager*>(m_pDataManager)->products(i);
    CString strContent;
    GetContentFromData(strContent, pPD);
    HTREEITEM hPDItem = m_conTreeProduct.InsertItem(strContent, hRoot, TVI_LAST);
    pSourceData = new source_data;
    pSourceData->type = PRODUCT_DES;
    pSourceData->ptr = pPD;
    m_conTreeProduct.SetItemData(hPDItem, reinterpret_cast<DWORD_PTR>(pSourceData));

    // create VersionDescription
    int nSupportVersionsCount = pPD->support_versions_count();
    for(int j = 0; j < nSupportVersionsCount; ++j)
    {
      VersionDescripter* pVD = pPD->support_versions(j);
      CString strContent;
      GetContentFromData(strContent, pVD);
      HTREEITEM hVDItem = m_conTreeProduct.InsertItem(strContent, hPDItem, TVI_LAST);
      pSourceData = new source_data;
      pSourceData->type = VERSION_DES;
      pSourceData->ptr = pVD;
      m_conTreeProduct.SetItemData(hVDItem, reinterpret_cast<DWORD_PTR>(pSourceData));

      // create support_messages
      int nSupportMessageCount = pVD->support_messages_count();
      for(int k = 0; k < nSupportMessageCount; ++k)
      {
        MessageDescripter* pMD = pVD->support_message(k);
        CString strContent;
        GetContentFromData(strContent, pMD);
        HTREEITEM hMDItem = m_conTreeProduct.InsertItem(strContent, hVDItem, TVI_LAST);
        pSourceData = new source_data;
        pSourceData->type = MESSAGE_DES;
        pSourceData->ptr = pMD;
        m_conTreeProduct.SetItemData(hMDItem, reinterpret_cast<DWORD_PTR>(pSourceData));
      }
    }
  }

  m_conTreeProduct.ExpandAll();
  m_conTreeProduct.SelectItem(hRoot);
}

void CManageProductDialog::OnBnClickedButtonAdd()
{
  // TODO: Add your control notification handler code here
  
  // 首先看当前选择的treectrl行
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  if(NULL == hSelectItem) // 未选中行
    return;

  source_data* pSourceData = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hSelectItem));
  CAddOrModifyDialog* pDlg = NULL;
  CreateAddModifyDialog(pSourceData->type, pDlg, true);
  if(NULL != pDlg)
  {     
    pDlg->DoModal(); 
    delete pDlg;
    pDlg = NULL;
  }
}

void CManageProductDialog::OnBnClickedButtonModify()
{
  // TODO: Add your control notification handler code here
  source_data* pSourceData = NULL;
  vector<void*> arrAncestor;
  GetDescripterFromSelect(pSourceData, arrAncestor);
  if(NULL == pSourceData)
    return;

  CAddOrModifyDialog* pDlg = NULL;
  CreateAddModifyDialog(pSourceData->type, pDlg, false);
  if(NULL != pDlg)
  {
    pDlg->m_pData = pSourceData->ptr;
    pDlg->DoModal();
    delete pDlg;
    pDlg = NULL;
  }
}

void CManageProductDialog::OnBnClickedButtonDelete()
{
  // TODO: Add your control notification handler code here

  // 首先弹出确认对话框
  if( IDOK != MessageBox("确认删除所选项?", "删除", MB_OKCANCEL) )
    return;
  
  // 先从m_product_manager中删除
  source_data* pSourceData = NULL;
  vector<void*> arrAncestor;
  GetDescripterFromSelect(pSourceData, arrAncestor);
  if(NULL == pSourceData)
  {
    MessageBox("未选择要删除的项!", "删除", MB_ICONWARNING);
    return;
  }

  // 从m_pDataManager中删除数据
  DeleteData(pSourceData);

  // 重画产品树
  InitTreeProduct();
  
  // 序列化
//  MementoData();
}

bool CManageProductDialog::AddProductDescripter(wmdf::ProductDescripter* pPD)
{
  // 首先查找是否有相同的id 
  ProductDescripter* pResult1 = reinterpret_cast<ProductManager*>(m_pDataManager)->FindProductDescripter(pPD->product_id());
  ProductDescripter* pResult2 = reinterpret_cast<ProductManager*>(m_pDataManager)->FindProductDescripter(pPD->product_name());
  if(NULL != pResult1)
  {
    MessageBox("产品id重复，请检查!");
    return false;
  }

  if(NULL != pResult2)
  {
    MessageBox("产品名重复，请检查!");
    return false;
  }

  reinterpret_cast<ProductManager*>(m_pDataManager)->add_products(pPD);

  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pPD);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  source_data* pSourceData = new source_data;
  pSourceData->type = PRODUCT_DES;
  pSourceData->ptr = pPD;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductDialog::AddVersionDescripter(wmdf::VersionDescripter* pVD)
{
  // 获取所属的产品名
  source_data* pSourceData = NULL;
  vector<void*> arrAncestor;
  GetDescripterFromSelect(pSourceData, arrAncestor);
  assert(NULL != pSourceData);
  ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(pSourceData->ptr);

  VersionDescripter* pResult = pPD->FindVersionDescripter(pVD->major_version(), pVD->minor_version());
  if(NULL != pResult)
  {
    MessageBox("版本重复，请检查!");
    return false;
  }

   pPD->add_support_versions(pVD);

  // 画界面
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pVD);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  pSourceData = new source_data;
  pSourceData->type = VERSION_DES;
  pSourceData->ptr = pVD;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductDialog::AddMessageDescripter(wmdf::MessageDescripter* pMD)
{
  // 获取所属版本
  source_data* pSourceData = NULL;
  vector<void*> arrAncestor;
  GetDescripterFromSelect(pSourceData, arrAncestor);
  assert(NULL != pSourceData);
  VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(pSourceData->ptr);

  MessageDescripter* pResult = pVD->FindMessageDescripter( pMD->message_id() );
  if(NULL != pResult)
  {
    MessageBox("消息id重复，请检查!");
    return false;
  }

  if( SiblingRepeat(CString(pMD->message_description().c_str())) )
  {
    MessageBox("消息名重复，请检查!");
    return false;
  }

  pVD->add_support_message(pMD);

  // 画界面
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pMD);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  pSourceData = new source_data;
  pSourceData->type = MESSAGE_DES;
  pSourceData->ptr = pMD;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductDialog::ModifyProductDescripter(wmdf::ProductDescripter* pPD)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pPD);    
  m_conTreeProduct.SetItemText(hSelectItem, strContent);
  return true;
}

bool CManageProductDialog::ModifyVersionDescripter(wmdf::VersionDescripter* pVD)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pVD);
  m_conTreeProduct.SetItemText(hSelectItem, strContent);
  return true;
}

bool CManageProductDialog::ModifyMessageDescripter(wmdf::MessageDescripter* pMD)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pMD);
  m_conTreeProduct.SetItemText(hSelectItem, strContent);
  return true;
}

void CManageProductDialog::OnTvnSelchangedTreeProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

  // TODO: Add your control notification handler code here
  RefreshContent();

  *pResult = 0;
}

void CManageProductDialog::GetDescripterFromSelect(source_data*& pSourceData, std::vector<void*>& arrAncestor)
{
  // 获取当前选择
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();  
  if(NULL == hSelectItem)
    return;

  // 获取类型
  pSourceData = reinterpret_cast<source_data*>( m_conTreeProduct.GetItemData(hSelectItem) );
}

bool CManageProductDialog::SiblingRepeat(CString strText)
{
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();  

  HTREEITEM hNextSiblingTreeItem = hSelectItem;
  while ( NULL != (hNextSiblingTreeItem = m_conTreeProduct.GetNextSiblingItem(hNextSiblingTreeItem)) )
  {
    CString strKey = m_conTreeProduct.GetItemText(hNextSiblingTreeItem);

    if(strKey == strText)
      return true;
  }

  HTREEITEM hPreSiblingTreeItem = hSelectItem;
  while ( NULL != (hPreSiblingTreeItem = m_conTreeProduct.GetPrevSiblingItem(hPreSiblingTreeItem)) )
  {
    CString strKey = m_conTreeProduct.GetItemText(hPreSiblingTreeItem);

    if(strKey == strText)
      return true;
  }

  return false;
}

void CManageProductDialog::RefreshContent()
{
  source_data* pSourceData = NULL;
  vector<void*> arrAncestor;
  GetDescripterFromSelect(pSourceData, arrAncestor);
  if(NULL == pSourceData)
    return;

  CString strShowContent;
  switch(pSourceData->type)
  {
  case PRODUCT_DES:
    {
      ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(pSourceData->ptr);

      if(NULL != pPD)
      {
        GetContentFromData(strShowContent, pPD);
      }
    }
    break;
  case VERSION_DES:
    {
      VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(pSourceData->ptr);

      if(NULL != pVD)
      {
        GetContentFromData(strShowContent, pVD);
      }
    }
    break;
  case MESSAGE_DES:
    {
      MessageDescripter* pMD = reinterpret_cast<MessageDescripter*>(pSourceData->ptr);

      if(NULL != pMD)
      {
        GetContentFromData(strShowContent, pMD);
      }
    }
    break;
  default:
    break;
  }

  GetDlgItem(IDC_STATIC_CONTENT)->SetWindowText(strShowContent);
}

BOOL CManageProductDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  // 设置窗口标题
  SetWindowText("产品管理");

  StudioUtility::InitServerList(m_dest_server);

  return TRUE;  // return TRUE unless you set the focus to a control
}

bool CManageProductDialog::LoadDataManager()
{
  m_pDataManager = ProductManager::instance();
  int server_index = m_dest_server.GetCurSel();
  return StudioUtility::GetConfigInfo(server_index, CFT_PRODUCT,m_pDataManager);
}


void CManageProductDialog::MementoData()
{
  int index = m_dest_server.GetCurSel();
  if(!StudioUtility::SetConfigInfo(index,CFT_PRODUCT,m_pDataManager))
  {
    MessageBox("修改产品信息失败！");
  }
  else
    MessageBox("修改产品信息成功！");
}

void CManageProductDialog::GetContentFromData(CString& strContent, wmdf::ProductDescripter* pData)
{
  strContent.Format("Product: ProductID=%d ProductName=%s Supplier=%s Description=%s",
    pData->product_id(), pData->product_name().c_str(), pData->supplier().c_str(), pData->description().c_str());
}

void CManageProductDialog::GetContentFromData(CString& strContent, wmdf::VersionDescripter* pData)
{
  strContent.Format("Version: Majorversion=%d Minorversion=%d",
    pData->major_version(), pData->minor_version());

  for(int32_t i = 0; i < pData->support_codecs_count(); ++i)
  {
    CString strTemp;
    strTemp.Format(" SupportCodec%d=%d", i, pData->support_codecs(i));
    strContent += strTemp;
  }
}

void CManageProductDialog::GetContentFromData(CString& strContent, wmdf::MessageDescripter* pData)
{
  strContent.Format("Message: MessageID=%d Description=%s",
    pData->message_id(), pData->message_description().c_str());
}

void CManageProductDialog::CreateAddModifyDialog(UINT type, CAddOrModifyDialog*& pDlg, bool bAdd /* = true */)
{
  if(bAdd)  // add
  {
    switch(type)
    {
    case ROOT_DES:
      pDlg = new CManageProductAddProductDescripterDialog(this, bAdd);
      pDlg->WnSetWindowText("新增产品描述");
      break;
    case PRODUCT_DES:
      pDlg = new CManageProductAddVersionDescripterDialog(this, bAdd);
      pDlg->WnSetWindowText("新增版本描述");
      break;
    case VERSION_DES:
      pDlg = new CManageProductAddMessageDescripterDialog(this, bAdd);
      pDlg->WnSetWindowText("新增消息");
      break;
    default: 
      break;
    }
  }
  else  // modify
  {
    switch(type)
    {
    case ROOT_DES:
      break;
    case PRODUCT_DES:
      pDlg = new CManageProductAddProductDescripterDialog(this, bAdd);
      pDlg->WnSetWindowText("修改产品描述");
      break;
    case VERSION_DES:
      pDlg = new CManageProductAddVersionDescripterDialog(this, bAdd);
      pDlg->WnSetWindowText("修改版本描述");
      break;
    case MESSAGE_DES:
      pDlg = new CManageProductAddMessageDescripterDialog(this, bAdd);
      pDlg->WnSetWindowText("修改消息");
      break;
    default: 
      break;
    }
  }
}

void CManageProductDialog::DeleteData(source_data* pSourceData)
{
  switch(pSourceData->type)
  {
  case ROOT_DES:
    MessageBox("根项不能删除!", "删除", MB_ICONWARNING);
    break;
  case PRODUCT_DES:
    {
      ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(pSourceData->ptr);
      reinterpret_cast<ProductManager*>(m_pDataManager)->delete_products(pPD);
    }
    break;
  case VERSION_DES:
    {
      VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(pSourceData->ptr);

      // 取得父结构
      HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
      HTREEITEM hParentItem = m_conTreeProduct.GetParentItem(hSelectItem);
      source_data* pTemp = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hParentItem));
      ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(pTemp->ptr);
      pPD->delete_version( pVD );
    }
    break;
  case MESSAGE_DES:
    {
      MessageDescripter* pMD = reinterpret_cast<MessageDescripter*>(pSourceData->ptr);

      // 取得父结构
      HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
      HTREEITEM hParentItem = m_conTreeProduct.GetParentItem(hSelectItem);
      source_data* pTemp = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hParentItem));
      VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(pTemp->ptr);
      pVD->delete_message( pMD );
    }
    break;
  default:
    break;
  }
}


LRESULT CManageProductDialog::OnAddDataItem(WPARAM wParam, LPARAM lParam)
{
  ItemParam* pIP = reinterpret_cast<ItemParam*>(wParam);

  ParseDataItem(pIP->pData, pIP->bAddDialog, pIP->type);

  // Serialization
//  MementoData();

  // refresh content
  RefreshContent();

  // refresh tree product
  m_conTreeProduct.Invalidate();

  return TRUE;
}

void CManageProductDialog::ParseDataItem(void* pData, bool bAdd, ENUM_DIALOG_TYPE type)
{
  if(bAdd)
  {
    switch(type)
    {
    case DIALOG_PRODUCT_DESCRIPTER:
      AddProductDescripter( reinterpret_cast<ProductDescripter*>(pData) );
      break;
    case DIALOG_VERSION_DESCRIPTER:
      AddVersionDescripter( reinterpret_cast<VersionDescripter*>(pData) );
      break;
    case DIALOG_MESSAGE_DESCRIPTER:
      AddMessageDescripter( reinterpret_cast<MessageDescripter*>(pData) );
      break;
    default:
      break;
    }
  }
  else
  {
    switch(type)
    {
    case DIALOG_PRODUCT_DESCRIPTER:
      ModifyProductDescripter( reinterpret_cast<ProductDescripter*>(pData) );
      break;
    case DIALOG_VERSION_DESCRIPTER:
      ModifyVersionDescripter( reinterpret_cast<VersionDescripter*>(pData) );
      break;
    case DIALOG_MESSAGE_DESCRIPTER:
      ModifyMessageDescripter( reinterpret_cast<MessageDescripter*>(pData) );
      break;
    default:
      break;
    }
  }
}
void CManageProductDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  MementoData();
  OnOK();
}

void CManageProductDialog::OnBnClickedGetConfigButton()
{
  if(LoadDataManager())
  {
    InitTreeProduct();
    m_flag = true;
  }
  else
    MessageBox("获取产品信息失败！请检查连接是否正常");
}

void CManageProductDialog::OnBnClickedUpdateConfigButton()
{
  if(m_flag)
    MementoData();
  else
    MessageBox("请先获取配置信息并修改！");
}
