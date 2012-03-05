// CManageProductSourceDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/Resource.h"
#include "ManageProductSourceDialog.h"
#include "../main_frame/MainFrm.h"
#include <assert.h>
#include "ManageProductSourceSelectDialog.h"
#include "ManageProductSourceAddorModifyBackupSourceDialog.h"
#include "ManageProductSourceAddorModifyMarketTimeDialog.h"
#include "ManageProductSourceAddorModifyProductSourceDialog.h"
#include "ManageProductSourceAddorModifySubscribedProductDialog.h"

#include "../../environment/file.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
#include "../../environment/server_environment.h"
#include "../common/studio_utility.h"
using namespace wmdf;

#define MARCO_SERIALIZATION

enum PRODUCT_SOURCE_TYPE
{
  ROOT,
  PRODUCT_SOURCE,
  MARKET_TIME,
  BACKUP_SOURCE,
  SUBSCRIBED_PRODUCT
};

struct source_data
{
  PRODUCT_SOURCE_TYPE type;
  void* ptr;
};

// CManageProductSourceDialog

IMPLEMENT_DYNCREATE(CManageProductSourceDialog, CManageProductDialog)

CManageProductSourceDialog::CManageProductSourceDialog(UINT id /* = IDD */, CWnd* pParent /* = NULL */)
	: CManageProductDialog(id, pParent)
{
  m_pDataManager = new wmdf::ProductSourceManager();
}

CManageProductSourceDialog::~CManageProductSourceDialog()
{
  if(m_pDataManager!=NULL)
    delete m_pDataManager;
}

BEGIN_MESSAGE_MAP(CManageProductSourceDialog, CManageProductDialog)
  ON_WM_DESTROY()
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_GET_CONFIG_BUTTON, &CManageProductSourceDialog::OnBnClickedGetConfigButton)
  ON_BN_CLICKED(IDC_UPDATE_CONFIG_BUTTON, &CManageProductSourceDialog::OnBnClickedUpdateConfigButton)
END_MESSAGE_MAP()

// CManageProductSourceDialog diagnostics

// CManageProductSourceDialog message handlers

BOOL CManageProductSourceDialog::OnInitDialog()
{
  CManageProductDialog::OnInitDialog();

  // TODO: Add your specialized code here and/or call the base class
  SetWindowText("产品源管理");

  //SetTimer(1, 50, NULL);
  return TRUE;
}

bool CManageProductSourceDialog::AddProductSource(wmdf::ProductSource* pProductSource)
{
  ProductSource* pResult = NULL;
  if( NULL != (pResult = reinterpret_cast<ProductSourceManager*>(m_pDataManager)->FindProductSource(pProductSource->product_id())) )
  {
    MessageBox("产品id已存在，添加产品源失败!", "错误", MB_ICONERROR);
    return false;
  }

  reinterpret_cast<ProductSourceManager*>(m_pDataManager)->add_product_source(pProductSource);

  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pProductSource);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  source_data* pSourceData = new source_data;
  pSourceData->type = PRODUCT_SOURCE;
  pSourceData->ptr = pProductSource;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductSourceDialog::AddMarketTime(wmdf::MarketTime* pItem)
{
  // 首先获取ProductSource
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  source_data* pSourceData = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hCurrentItem));
  ProductSource* pProductSource = reinterpret_cast<ProductSource*>(pSourceData->ptr);
  assert(NULL != pProductSource);

  pProductSource->add_market_time(pItem);

  CString strContent;
  GetContentFromData(strContent, pItem);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  pSourceData = new source_data;
  pSourceData->type = MARKET_TIME;
  pSourceData->ptr = pItem;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductSourceDialog::AddBackupSource(wmdf::BackupSource* pItem)
{
  // 首先获取ProductSource
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  source_data* pSourceData = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hCurrentItem));
  ProductSource* pProductSource = reinterpret_cast<ProductSource*>(pSourceData->ptr);
  assert(NULL != pProductSource);

  pProductSource->add_backup_source(pItem);
  CString strContent;
  GetContentFromData(strContent, pItem);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  pSourceData = new source_data;
  pSourceData->type = BACKUP_SOURCE;
  pSourceData->ptr = pItem;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductSourceDialog::AddSubscribedProduct(wmdf::ProtocolDescription* pItem)
{
  // 首先获取ProductSource
  HTREEITEM hCurrentItem = m_conTreeProduct.GetSelectedItem();
  source_data* pSourceData = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hCurrentItem));
  ProductSource* pProductSource = reinterpret_cast<ProductSource*>(pSourceData->ptr);
  assert(NULL != pProductSource);

  pProductSource->add_subscribed_product(pItem);
  CString strContent;
  GetContentFromData(strContent, pItem);
  HTREEITEM hInsertItem = m_conTreeProduct.InsertItem(strContent, hCurrentItem, TVI_LAST);
  pSourceData = new source_data;
  pSourceData->type = SUBSCRIBED_PRODUCT;
  pSourceData->ptr = pItem;
  m_conTreeProduct.SetItemData(hInsertItem, reinterpret_cast<DWORD_PTR>(pSourceData));

  return true;
}

bool CManageProductSourceDialog::ModifyProductSource(wmdf::ProductSource* pItem)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();

  CString strContent;
  GetContentFromData(strContent, pItem);
  m_conTreeProduct.SetItemText(hSelectItem, strContent);

  return true;
}

bool CManageProductSourceDialog::ModifyMarketTime(wmdf::MarketTime* pItem)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pItem);
  m_conTreeProduct.SetItemText(hSelectItem, strContent);

  return true;
}

bool CManageProductSourceDialog::ModifyBackupSource(wmdf::BackupSource* pItem)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pItem);
  m_conTreeProduct.SetItemText(hSelectItem, strContent);

  return true;
}

bool CManageProductSourceDialog::ModifySubscribedProduct(wmdf::ProtocolDescription* pItem)
{
  // 修改树标题
  HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
  CString strContent;
  GetContentFromData(strContent, pItem);
  m_conTreeProduct.SetItemText(hSelectItem, strContent);

  return true;
}

void CManageProductSourceDialog::InitTreeProduct()
{
  // delete all
  m_conTreeProduct.DeleteAllItems();

  // create root
  TVINSERTSTRUCT tvInsert;
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT;
  tvInsert.item.pszText = _T("产品源管理");
  HTREEITEM hRoot = m_conTreeProduct.InsertItem(&tvInsert);
  source_data* pSourceData = new source_data;
  pSourceData->type = ROOT;
  pSourceData->ptr = NULL;
  m_conTreeProduct.SetItemData(hRoot, reinterpret_cast<DWORD_PTR>(pSourceData));

  // create ProductSource
  int nProductSourceCount = reinterpret_cast<ProductSourceManager*>(m_pDataManager)->product_source_count();
  for(int i = 0; i < nProductSourceCount; ++i)
  {
    ProductSource* pPS = reinterpret_cast<ProductSourceManager*>(m_pDataManager)->product_sources(i);
    CString strContent;
    GetContentFromData(strContent, pPS);
    HTREEITEM hPSItem = m_conTreeProduct.InsertItem( strContent, hRoot, TVI_LAST);
    // 设置check
    m_conTreeProduct.SetCheck(hPSItem, pPS->enabled());

    pSourceData = new source_data;
    pSourceData->type = PRODUCT_SOURCE;
    pSourceData->ptr = pPS;
    m_conTreeProduct.SetItemData(hPSItem, reinterpret_cast<DWORD_PTR>(pSourceData));

    // create MarketTime
    int nMarketTimeCount =  pPS->market_time_count();
    for(int j = 0; j < nMarketTimeCount; ++j)
    {
      MarketTime* pMT = pPS->market_times(j);
      CString strContent;
      GetContentFromData(strContent, pMT);
      HTREEITEM hMTItem = m_conTreeProduct.InsertItem(strContent, hPSItem, TVI_LAST);
      pSourceData = new source_data;
      pSourceData->type = MARKET_TIME;
      pSourceData->ptr = pMT;
      m_conTreeProduct.SetItemData(hMTItem, reinterpret_cast<DWORD_PTR>(pSourceData));  
    }
    
    // create Backup
    int nBackupCount =  pPS->backup_source_count();
    for(int j = 0; j < nBackupCount; ++j)
    {
      BackupSource* pBS = pPS->backup_sources(j);
      CString strContent;
      GetContentFromData(strContent, pBS);
      HTREEITEM hBSItem = m_conTreeProduct.InsertItem(strContent, hPSItem, TVI_LAST);
      pSourceData = new source_data;
      pSourceData->type = BACKUP_SOURCE;
      pSourceData->ptr = pBS;
      m_conTreeProduct.SetItemData(hBSItem, reinterpret_cast<DWORD_PTR>(pSourceData));  
    }

    // create SubscribedProduct
    int nSubscribedProductCount =  pPS->subscribed_products_count();
    for(int j = 0; j < nSubscribedProductCount; ++j)
    {
      ProtocolDescription* pSP = pPS->subscribed_products(j);
      CString strContent;
      GetContentFromData(strContent, pSP);
      HTREEITEM hMTItem = m_conTreeProduct.InsertItem(strContent, hPSItem, TVI_LAST);
      source_data* pSourceData = new source_data;
      pSourceData->type = SUBSCRIBED_PRODUCT;
      pSourceData->ptr = pSP;
      m_conTreeProduct.SetItemData(hMTItem, reinterpret_cast<DWORD_PTR>(pSourceData));  
    }
  }

  m_conTreeProduct.ExpandAll();
}

void CManageProductSourceDialog::RefreshContent()
{
  source_data* pSourceData = NULL;
  vector<void*> arrAncestor;
  GetDescripterFromSelect(pSourceData, arrAncestor);
  if(NULL == pSourceData)
    return;

  CString strShowContent;
  switch(pSourceData->type)
  {
  case ROOT:
    {
      strShowContent = "数据源管理";
    }
    break;
  case PRODUCT_SOURCE:
    {
      ProductSource* pPS = reinterpret_cast<ProductSource*>(pSourceData->ptr);

      if(NULL != pPS)
      {
        GetContentFromData(strShowContent, pPS);
      }
    }
    break;
  case MARKET_TIME:
    {
      MarketTime* pMT = reinterpret_cast<MarketTime*>(pSourceData->ptr);

      if(NULL != pMT)
      {
        GetContentFromData(strShowContent, pMT);
      }
    }
    break;
  case BACKUP_SOURCE:
    {
      BackupSource* pBS = reinterpret_cast<BackupSource*>(pSourceData->ptr);
      if(NULL != pBS)
      {
        GetContentFromData(strShowContent, pBS);
      }
    }
    break;
  case SUBSCRIBED_PRODUCT:
    {
      ProtocolDescription* pSP = reinterpret_cast<ProtocolDescription*>(pSourceData->ptr);
      if(NULL != pSP)
      {
        GetContentFromData(strShowContent, pSP);
      }
    }
    break;
  default:
    break;
  }

  GetDlgItem(IDC_STATIC_CONTENT)->SetWindowText(strShowContent);
}

bool CManageProductSourceDialog::LoadDataManager()
{
  int server_index = m_dest_server.GetCurSel();
  return StudioUtility::GetConfigInfo(server_index, CFT_SOURCE,m_pDataManager);
}

void CManageProductSourceDialog::GetContentFromData(CString& strContent, wmdf::ProductSource* pData)
{
  strContent.Format("ProductSource: ProductID=%d IPAddr=%s Port=%d Username=%s Password=%s Codec=%d Description=%s",
    pData->product_id(), pData->ip_address().c_str(), pData->port(),
    pData->user_name().c_str(), pData->pass_word().c_str(), pData->codec_type(),pData->description().c_str()); 
}

void CManageProductSourceDialog::GetContentFromData(CString& strContent, wmdf::MarketTime* pData)
{
  strContent.Format("MarketTime: BeginTime=%d EndTime=%d", pData->begin_time(), pData->end_time());
}

void CManageProductSourceDialog::GetContentFromData(CString& strContent, wmdf::BackupSource* pData)
{
  strContent.Format("BackupSource: BackupIPAddress=%s Port=%d Username=%s Password=%s",
    pData->ip_address().c_str(), pData->port(), pData->user_name().c_str(), pData->pass_word().c_str());
}

void CManageProductSourceDialog::GetContentFromData(CString& strContent, wmdf::ProtocolDescription* pData)
{
  strContent.Format("SubscribedProduct: ProductID=%d Majorversion=%d MinorVersion=%d Codec=%d",
    pData->product_id(), pData->major_version(), pData->minor_version(), pData->codec_type());
}

void CManageProductSourceDialog::CreateAddModifyDialog(UINT type, CAddOrModifyDialog*& pDlg, bool bAdd /* = true */)
{
  if(bAdd)  // add
  {
    switch(type)
    {
    case ROOT:
      pDlg = new CManageProductSourceAddorModifyProductSourceDialog(this, bAdd);
      break;
    case PRODUCT_SOURCE:
      {
        CManageProductSourceSelectDialog dlg;
        if(dlg.DoModal() == IDOK)
        {
          switch(dlg.m_nSelectType)
          {
          case 0: // market_time
            {
              pDlg = new CManageProductSourceAddorModifyMarketTimeDialog(this, bAdd);
            }
            break;
          case 1: // backup source
            {
              pDlg = new CManageProductSourceAddorModifyBackupSourceDialog(this, bAdd);
            }
            break;
          case 2: // subscribed product
            {
              pDlg = new CManageProductSourceAddorModifySubscribedProductDialog(this, bAdd);
            }
            break;
          default:
            break;
          }
        }
      }
      break;
    default:
      break;
    }
  }
  else  // modify
  {
    switch(type)
    {
    case ROOT:
      break;
    case PRODUCT_SOURCE:
      pDlg = new CManageProductSourceAddorModifyProductSourceDialog(this, bAdd);
      break;
    case MARKET_TIME:
      pDlg = new CManageProductSourceAddorModifyMarketTimeDialog(this, bAdd);
      break;
    case BACKUP_SOURCE:
      pDlg = new CManageProductSourceAddorModifyBackupSourceDialog(this, bAdd);
      break;
    case SUBSCRIBED_PRODUCT:
      pDlg = new CManageProductSourceAddorModifySubscribedProductDialog(this, bAdd);
      break;
    default:
      break;
    }
  }
}

void CManageProductSourceDialog::DeleteData(source_data* pSourceData)
{
  switch(pSourceData->type)
  {
  case ROOT:
    MessageBox("根项不能删除!", "删除", MB_ICONWARNING);
    break;
  case PRODUCT_SOURCE:
    {
      ProductSource* ptr = reinterpret_cast<ProductSource*>(pSourceData->ptr);
      reinterpret_cast<ProductSourceManager*>(m_pDataManager)->delete_product_source(ptr);
    }
    break;
  case MARKET_TIME:
    {
      MarketTime* ptr = reinterpret_cast<MarketTime*>(pSourceData->ptr);

      // 取得父结构
      HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
      HTREEITEM hParentItem = m_conTreeProduct.GetParentItem(hSelectItem);
      source_data* pTemp = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hParentItem));
      ProductSource* pPS = reinterpret_cast<ProductSource*>(pTemp->ptr);
      pPS->delete_market_time( ptr );

    }
    break;
  case BACKUP_SOURCE:
    {
      BackupSource* ptr = reinterpret_cast<BackupSource*>(pSourceData->ptr);

      // 取得父结构
      HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
      HTREEITEM hParentItem = m_conTreeProduct.GetParentItem(hSelectItem);
      source_data* pTemp = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hParentItem));
      ProductSource* pPS = reinterpret_cast<ProductSource*>(pTemp->ptr);
      pPS->delete_backup_source( ptr );
    }
    break;
  case SUBSCRIBED_PRODUCT:
    {
      ProtocolDescription* ptr = reinterpret_cast<ProtocolDescription*>(pSourceData->ptr);

      // 取得父结构
      HTREEITEM hSelectItem = m_conTreeProduct.GetSelectedItem();
      HTREEITEM hParentItem = m_conTreeProduct.GetParentItem(hSelectItem);
      source_data* pTemp = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hParentItem));
      ProductSource* pPS = reinterpret_cast<ProductSource*>(pTemp->ptr);
      pPS->delete_subscribed_product( ptr );
    }
    break;
  default:
    break;
  } 
}

// 保存数据
void CManageProductSourceDialog::MementoData()
{
  // 获取ProductSource的Enable状态
  HTREEITEM hRoot = m_conTreeProduct.GetRootItem();

  if (m_conTreeProduct.ItemHasChildren(hRoot))
  {
    HTREEITEM hNextItem;
    HTREEITEM hChildItem = m_conTreeProduct.GetChildItem(hRoot);

    while (hChildItem != NULL)
    {
      BOOL bCheck = m_conTreeProduct.GetCheck(hChildItem);
      source_data* pSD = reinterpret_cast<source_data*>(m_conTreeProduct.GetItemData(hChildItem));
      ProductSource* pPS = static_cast<ProductSource*>(pSD->ptr); 
      pPS->set_enabled((bool)bCheck);

      hNextItem = m_conTreeProduct.GetNextItem(hChildItem, TVGN_NEXT);
      hChildItem = hNextItem;
    }
  }

  // save to xml file
  int index = m_dest_server.GetCurSel();
  if(!StudioUtility::SetConfigInfo(index,CFT_SOURCE,m_pDataManager))
  {
    MessageBox("修改适配源信息失败！");
  }
  else
  {
    MessageBox("修改适配源信息成功！");
  }
}

void CManageProductSourceDialog::ParseDataItem(void* pData, bool bAdd, ENUM_DIALOG_TYPE type)
{
  if(bAdd)
  {
    switch(type)
    {
    case DIALOG_PRODUCT_SOURCE:
      AddProductSource( reinterpret_cast<ProductSource*>(pData) );
      break;
    case DIALOG_MARKET_TIME:
      AddMarketTime( reinterpret_cast<MarketTime*>(pData) );
      break;
    case DIALOG_BACKUP_SOURCE:
      AddBackupSource( reinterpret_cast<BackupSource*>(pData) );
      break;
    case DIALOG_SUBSCRIBED_PRODUCT:
      AddSubscribedProduct( reinterpret_cast<ProtocolDescription*>(pData) );
      break;
    default:
      break;
    }
  }
  else
  {
    switch(type)
    {
    case DIALOG_PRODUCT_SOURCE:
      ModifyProductSource( reinterpret_cast<ProductSource*>(pData) );
      break;
    case DIALOG_MARKET_TIME:
      ModifyMarketTime( reinterpret_cast<MarketTime*>(pData) );
      break;
    case DIALOG_BACKUP_SOURCE:
      ModifyBackupSource( reinterpret_cast<BackupSource*>(pData) );
      break;
    case DIALOG_SUBSCRIBED_PRODUCT:
      ModifySubscribedProduct( reinterpret_cast<ProtocolDescription*>(pData) );
      break;
    default:
      break;
    }
  }
}
void CManageProductSourceDialog::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  InitTreeProduct();
  KillTimer(1);

  CManageProductDialog::OnTimer(nIDEvent);
}
void CManageProductSourceDialog::OnBnClickedGetConfigButton()
{
  if(LoadDataManager())
  {
    InitTreeProduct();
    m_flag = true;
  }
  else
    MessageBox("获取适配源信息失败！请检查连接是否正常");
}

void CManageProductSourceDialog::OnBnClickedUpdateConfigButton()
{
  if(m_flag)
    MementoData();
  else
    MessageBox("请先获取配置信息并修改！");
}
