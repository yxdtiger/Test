// AuditingCertView.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "ManageProductView.h"
#include "../main_frame/MainFrm.h"
#include <sstream>
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
using namespace wmdf;

//#include <boost/shared_ptr.hpp>
//#include <boost/make_shared.hpp>
//using namespace boost;

// define a data enum which will be set as item data to m_conTreeProduct
enum PRODUCT_INNER_TYPE
{
  PRODUCT_DES = 0,
  VERSION_DES,
  MESSAGE_DES
};

/////////////////////////////////////////////////////////////////////////////
// CManageProductView

IMPLEMENT_DYNCREATE(CManageProductView, CFormView)

CManageProductView::CManageProductView()
	: CFormView(CManageProductView::IDD)
{
	//{{AFX_DATA_INIT(CManageProductView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT 

  m_pAddProductDescripterDialog = new CManageProductAddProductDescripterDialog(this, true);
  m_pAddProductDescripterDialog->Create(IDD_MANAGE_PRODUCT_ADD_PRODUCT_DESCRIPTER);
  m_pAddVersionDescripterDialog = new CManageProductAddVersionDescripterDialog(this, true);
  m_pAddVersionDescripterDialog->Create(IDD_MANAGE_PRODUCT_ADD_VERSION_DESCRIPTER);
  m_pAddMessageDescripterDialog = new CManageProductAddMessageDescripterDialog(this, true);
  m_pAddMessageDescripterDialog->Create(IDD_MANAGE_PRODUCT_ADD_MESSAGE_DESCRIPTER);

}

CManageProductView::~CManageProductView()
{
}

void CManageProductView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TREE_PRODUCT, m_conTreeProduct);
}


BEGIN_MESSAGE_MAP(CManageProductView, CFormView)
	ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BUTTON_ADD, &CManageProductView::OnBnClickedButtonAdd)
  ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CManageProductView::OnBnClickedButtonModify)
  ON_BN_CLICKED(IDC_BUTTON_DELETE, &CManageProductView::OnBnClickedButtonDelete)
  ON_MESSAGE(WM_ADD_PRODUCT_DESCRIPTER, OnAddProductDescripter)
  ON_MESSAGE(WM_ADD_VERSION_DESCRIPTER, OnAddVersionDescripter)
  ON_MESSAGE(WM_ADD_MESSAGE_DESCRIPTER, OnAddMessageDescripter)
  ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRODUCT, &CManageProductView::OnTvnSelchangedTreeProduct)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageProductView diagnostics

#ifdef _DEBUG
void CManageProductView::AssertValid() const
{
  CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CManageProductView::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CManageProductView message handlers

void CManageProductView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

  // 设置窗口标题
  GetParentFrame()->SetWindowText("产品管理");

  // 初始化m_product_manager
  if (File::Exists(ApplicationDirectory::ProductDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::ProductDescriptionFile(),&m_product_manager);
  }

  InitTreeProduct();

}

void CManageProductView::OnDestroy() 
{
	CFormView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_pManageProduct=NULL; // 清空窗口指针

	// TODO: Add your message handler code here
	
}

void CManageProductView::InitTreeProduct()
{
  // delete all
  m_conTreeProduct.DeleteAllItems();

  PRODUCT_INNER_TYPE productInnerType;

  // create root
  TVINSERTSTRUCT tvInsert;
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = _T("产品管理");
  HTREEITEM hRoot = m_conTreeProduct.InsertItem(&tvInsert);

  // create ProductDescripter
  int nProductCount = m_product_manager.products_count();
  for(int i = 0; i < nProductCount; ++i)
  {
    ProductDescripter* pPD = m_product_manager.products(i);
    HTREEITEM hPDItem = m_conTreeProduct.InsertItem(pPD->product_name().c_str(), hRoot, TVI_LAST);
    productInnerType = PRODUCT_DES;
    m_conTreeProduct.SetItemData(hPDItem, static_cast<DWORD_PTR>(productInnerType));

    // create VersionDescription
    int nSupportVersionsCount = pPD->support_versions_count();
    for(int j = 0; j < nSupportVersionsCount; ++j)
    {
      VersionDescripter* pVD = pPD->support_versions(j);
      stringstream ss;
      ss << static_cast<int>(pVD->major_version()) << "." << static_cast<int>(pVD->minor_version());
      HTREEITEM hVDItem = m_conTreeProduct.InsertItem(ss.str().c_str(), hPDItem, TVI_LAST);
      productInnerType = VERSION_DES;
      m_conTreeProduct.SetItemData(hVDItem, static_cast<DWORD_PTR>(productInnerType));

      // create support_messages
      int nSupportMessageCount = pVD->support_messages_count();
      for(int k = 0; k < nSupportMessageCount; ++k)
      {
        MessageDescripter* pMD = pVD->support_message(k);

        HTREEITEM hMDItem = m_conTreeProduct.InsertItem(pMD->message_description().c_str(), hVDItem, TVI_LAST);
        productInnerType = MESSAGE_DES;
        m_conTreeProduct.SetItemData(hMDItem, static_cast<DWORD_PTR>(productInnerType));
      }
    }
  }

  m_conTreeProduct.ExpandAll();
}
void CManageProductView::OnBnClickedButtonAdd()
{
  // TODO: Add your control notification handler code here
  
  // 首先看当前选择的treectrl行
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();

  // 获取child item的类型
  HTREEITEM hChildItem = m_conTreeProduct.GetChildItem(hSelectItem);

  if(NULL == hChildItem)
    return;
  
  PRODUCT_INNER_TYPE productInnerType = static_cast<PRODUCT_INNER_TYPE>(m_conTreeProduct.GetItemData(hChildItem));

  switch(productInnerType)
  {
  case PRODUCT_DES:
    m_pAddProductDescripterDialog->ShowWindow(SW_SHOW);
    break;
  case VERSION_DES:
    m_pAddVersionDescripterDialog->ShowWindow(SW_SHOW);
    break;
  case MESSAGE_DES:
    m_pAddMessageDescripterDialog->ShowWindow(SW_SHOW);
    break;
  default: 
    break;
  }
}

void CManageProductView::OnBnClickedButtonModify()
{
  // TODO: Add your control notification handler code here
  PRODUCT_INNER_TYPE productInnerType;
  void* ptr = GetDescripterFromSelect(productInnerType);
  if(NULL == ptr)
    return;

  switch(productInnerType)
  {
  case PRODUCT_DES:
    {
      CManageProductAddProductDescripterDialog dlg(this, false);
      ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(ptr);
      dlg.SetProductDescripter(*pPD);
      dlg.DoModal();
    }
    break;
  case VERSION_DES:
    {  
      CManageProductAddVersionDescripterDialog dlg(this, false);
      VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(ptr);
      dlg.SetVersionDescripter(*pVD);
      dlg.DoModal();
    }
    break;
  case MESSAGE_DES:
    { 
      CManageProductAddMessageDescripterDialog dlg(this, false);
      MessageDescripter* pMD = reinterpret_cast<MessageDescripter*>(ptr);
      dlg.SetMessageDescripter(*pMD);
      dlg.DoModal();
    }
    break;
  default: 
    break;
  }
}

void CManageProductView::OnBnClickedButtonDelete()
{
  // TODO: Add your control notification handler code here
}

LRESULT CManageProductView::OnAddProductDescripter(WPARAM wParam, LPARAM lParam)
{
  ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(wParam);
  bool bAdd = *(reinterpret_cast<bool*>(lParam));
  bool bResult = false;

  if(bAdd)
  {
    bResult = AddProductDescripter(pPD);
  }
  else
  {
    bResult = ModifyProductDescripter(pPD);
  }


  if(bResult)
  {
    // 保存至配置文件
    SaveToXmlFile( ApplicationDirectory::ProductDescriptionFile() );

    RefreshContent();
  }


  return TRUE;
}

LRESULT CManageProductView::OnAddVersionDescripter(WPARAM wParam, LPARAM lParam)
{
  VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(wParam);
  bool bAdd = *(reinterpret_cast<bool*>(lParam));
  bool bResult = false;

  if(bAdd)
  {
    bResult = AddVersionDescripter(pVD);
  }
  else
  {
    bResult = ModifyVersionDescripter(pVD);
  }

  if(bResult)
  {
     // 保存至配置文件
    SaveToXmlFile( ApplicationDirectory::ProductDescriptionFile() );

    RefreshContent();
  }

  return TRUE;
}

LRESULT CManageProductView::OnAddMessageDescripter(WPARAM wParam, LPARAM lParam)
{
  MessageDescripter* pMD = reinterpret_cast<MessageDescripter*>(wParam);
  bool bAdd = *(reinterpret_cast<bool*>(lParam));
  bool bResult = false;

  if(bAdd)
  {
    bResult = AddMessageDescripter(pMD);
  }
  else
  {
    bResult = ModifyMessageDescripter(pMD);
  }

  if(bResult)
  {
    // 保存至配置文件
    SaveToXmlFile( ApplicationDirectory::ProductDescriptionFile() );

    RefreshContent();
  }

  return TRUE;
}

bool CManageProductView::AddProductDescripter(wmdf::ProductDescripter* pPD)
{
  // 首先查找是否有相同的id 
  ProductDescripter* pResult1 = m_product_manager.FindProductDescripter(pPD->product_id());
  ProductDescripter* pResult2 = m_product_manager.FindProductDescripter(pPD->product_name());
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

  ProductDescripter* pNew = new ProductDescripter;
  *pNew = *pPD;

  m_product_manager.add_products(pNew);

  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(pPD->product_name().c_str(), hCurrentItem, TVI_LAST);
  m_conTreeProduct.SetItemData(hInsertItem, static_cast<DWORD_PTR>(PRODUCT_DES));

  return true;
}

bool CManageProductView::ModifyProductDescripter(wmdf::ProductDescripter* pPD)
{
  // 首先通过id查找product
  ProductDescripter* pResult = m_product_manager.FindProductDescripter(pPD->product_id());
  assert(NULL != pResult);

  if( SiblingRepeat(CString(pPD->product_name().c_str())) )
  {
    MessageBox("产品名重复，请检查!");
    return false;
  }
  
  pResult->set_product_name(pPD->product_name());
  pResult->set_supplier(pPD->supplier());
  pResult->set_description(pPD->description());

  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  m_conTreeProduct.SetItemText(hCurrentItem, pResult->product_name().c_str());

  return true;
}

bool CManageProductView::AddVersionDescripter(wmdf::VersionDescripter* pVD)
{
  // 获取所属的产品名
  PRODUCT_INNER_TYPE productInnerType;
  ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>( GetDescripterFromSelect(productInnerType) );
  assert(PRODUCT_DES == productInnerType && NULL != pPD);

  VersionDescripter* pResult = pPD->FindVersionDescripter(pVD->major_version(), pVD->minor_version());
  if(NULL != pResult)
  {
    MessageBox("版本重复，请检查!");
    return false;
  }

  VersionDescripter* pNew = new VersionDescripter;
  *pNew = *pVD;

  pPD->add_support_versions(pNew);

  // 画界面
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  CString strVersion;
  strVersion.Format("%d.%d", pVD->major_version(), pVD->minor_version());
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strVersion, hCurrentItem, TVI_LAST);
  m_conTreeProduct.SetItemData(hInsertItem, static_cast<DWORD_PTR>(VERSION_DES));

  return true;
}

bool CManageProductView::ModifyVersionDescripter(wmdf::VersionDescripter* pVD)
{
  PRODUCT_INNER_TYPE productInnerType;
  VersionDescripter* pResult = reinterpret_cast<VersionDescripter*>(GetDescripterFromSelect(productInnerType));
  assert(VERSION_DES == productInnerType && NULL != pResult);

  // 先清空
  pResult->clear_codecs();
  
  for(int32_t i = 0; i < pVD->support_codecs_count(); ++i)
  {
    pResult->add_support_codecs(pVD->support_codecs(i));
  }

  return true;
}

bool CManageProductView::AddMessageDescripter(wmdf::MessageDescripter* pMD)
{
  // 获取所属版本
  PRODUCT_INNER_TYPE productInnerType;
  VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>( GetDescripterFromSelect(productInnerType) );
  assert(VERSION_DES == productInnerType && NULL != pVD);

  MessageDescripter* pResult = pVD->FindMessageDescripter( pMD->message_id() );
  if(NULL != pResult)
  {
    MessageBox("消息id重复，请检查!");
    return false;
  }

  MessageDescripter* pNew = new MessageDescripter;
  *pNew = *pMD;

  pVD->add_support_message(pNew);

  // 画界面
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(pMD->message_description().c_str(), hCurrentItem, TVI_LAST);
  m_conTreeProduct.SetItemData(hInsertItem, static_cast<DWORD_PTR>(MESSAGE_DES));

  return true;
}

bool CManageProductView::ModifyMessageDescripter(wmdf::MessageDescripter* pMD)
{
  PRODUCT_INNER_TYPE productInnerType;
  MessageDescripter* pResult = reinterpret_cast<MessageDescripter*>( GetDescripterFromSelect(productInnerType) );
  assert(MESSAGE_DES == productInnerType && NULL != pMD);

  if( SiblingRepeat(CString(pMD->message_description().c_str())) )
  {
    MessageBox("消息名重复，请检查!");
    return false;
  }

  pResult->set_message_description(pMD->message_description());

  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  m_conTreeProduct.SetItemText( hCurrentItem, pResult->message_description().c_str() );

  return true;
}

void CManageProductView::OnTvnSelchangedTreeProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

  // TODO: Add your control notification handler code here
  RefreshContent();

  *pResult = 0;
}

void* CManageProductView::GetDescripterFromSelect(PRODUCT_INNER_TYPE& type)
{
  // 获取当前选择
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();  

  // 获取类型
  type = static_cast<PRODUCT_INNER_TYPE>( m_conTreeProduct.GetItemData(hSelectItem) );

  CString strText = m_conTreeProduct.GetItemText(hSelectItem);
  CString strShowContent;

  switch(type)
  {
  case PRODUCT_DES:
    {
      return m_product_manager.FindProductDescripter(strText.GetBuffer(0));
    }
    break;
  case VERSION_DES:
    {
      // 找父item
      HTREEITEM hParentItem = m_conTreeProduct.GetParentItem(hSelectItem);

      CString strProductName = m_conTreeProduct.GetItemText(hParentItem);
      ProductDescripter* pPD = m_product_manager.FindProductDescripter(strProductName.GetBuffer(0));
      strProductName.ReleaseBuffer();
      assert(NULL != pPD);

      Splitter splitter(strText.GetBuffer(0), ".");
      assert(2 == splitter.size());
      strText.ReleaseBuffer();
      int major_version, minor_version;
      major_version = atoi(splitter[0].c_str());
      minor_version = atoi(splitter[1].c_str());
      VersionDescripter* pVD = pPD->FindVersionDescripter( static_cast<int8_t>(major_version), static_cast<int8_t>(minor_version) );

      return pVD;

    }
    break;
  case MESSAGE_DES:
    {
      HTREEITEM hVersionItem = m_conTreeProduct.GetParentItem(hSelectItem);
      HTREEITEM hProductItem = m_conTreeProduct.GetParentItem(hVersionItem);
      CString strProductName = m_conTreeProduct.GetItemText(hProductItem);
      CString strVersionName = m_conTreeProduct.GetItemText(hVersionItem);

      ProductDescripter* pPD = m_product_manager.FindProductDescripter(strProductName.GetBuffer(0));
      strProductName.ReleaseBuffer();
      Splitter splitter(strVersionName.GetBuffer(0), ".");
      assert(2 == splitter.size());
      strVersionName.ReleaseBuffer();
      int major_version, minor_version;
      major_version = atoi(splitter[0].c_str());
      minor_version = atoi(splitter[1].c_str());
      VersionDescripter* pVD = pPD->FindVersionDescripter( static_cast<int8_t>(major_version), static_cast<int8_t>(minor_version) );
      strVersionName.ReleaseBuffer();
      MessageDescripter* pMD = pVD->FindMessageDescripter(strText.GetBuffer(0));
      strText.ReleaseBuffer();

      return pMD;
    }
    break;
  default:
    break;
  }

  return NULL;
}

void CManageProductView::SaveToXmlFile(const std::string& file_path)
{
  Utils::Serialize(file_path, reinterpret_cast<Persistable*>(&m_product_manager));
};

bool CManageProductView::SiblingRepeat(CString strText)
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

void CManageProductView::RefreshContent()
{
  PRODUCT_INNER_TYPE productInnerType;
  void* ptr = GetDescripterFromSelect(productInnerType);
  if(NULL == ptr)
    return;

  CString strShowContent;
  switch(productInnerType)
  {
  case PRODUCT_DES:
    {
      ProductDescripter* pPD = reinterpret_cast<ProductDescripter*>(ptr);

      if(NULL != pPD)
      {
        strShowContent.Format("ProductID=%d\nProductName=%s\nSupplier=%s\nDescription=%s",
          pPD->product_id(), pPD->product_name().c_str(), pPD->supplier().c_str(), pPD->description().c_str() );
      }
    }
    break;
  case VERSION_DES:
    {
      VersionDescripter* pVD = reinterpret_cast<VersionDescripter*>(ptr);

      if(NULL != pVD)
      {
        strShowContent.Format( "version=%d.%d\n", pVD->major_version(), pVD->minor_version());

        for(int32_t i  = 0; i < pVD->support_codecs_count(); ++i)
        {
          CString strCodec;
          strCodec.Format("SupportCodec%d=%d\n", i, pVD->support_codecs(i));

          strShowContent += strCodec;
        }
      }
    }
    break;
  case MESSAGE_DES:
    {
      MessageDescripter* pMD = reinterpret_cast<MessageDescripter*>(ptr);

      if(NULL != pMD)
      {
        strShowContent.Format( "message_id=%d\nmessage_description=%s",
          pMD->message_id(), pMD->message_description().c_str() );
      }
    }
    break;
  default:
    break;
  }

  GetDlgItem(IDC_STATIC_CONTENT)->SetWindowText(strShowContent);
}