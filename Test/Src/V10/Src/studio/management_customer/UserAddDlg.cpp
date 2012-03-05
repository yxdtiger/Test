// UserAddDlg.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "UserAddDlg.h"
#include <sstream>
#include <assert.h>
#include <algorithm>

#include "../../usermanager/authorized_product.h"
#include "../../include/date_time.h"
#include "../common/UIUilties.h"
#include "../../environment/file.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
#include "../../environment/splitter.h"
using namespace wmdf;

using namespace std;
using namespace wmdf;

// CUserAddDlg dialog

IMPLEMENT_DYNAMIC(CUserAddDlg, CAddOrModifyDialog)

CUserAddDlg::CUserAddDlg(CWnd* pParent /*=NULL*/, bool bDialogType /*=true*/)
: CAddOrModifyDialog(CUserAddDlg::IDD, pParent, bDialogType)
  , str_user_name_(_T(""))
  , str_password_(_T(""))
  , str_company_(_T(""))
  , str_contact_(_T(""))
  , str_phone_number_(_T(""))
  , str_email_(_T(""))
  , customer_type_int_(0)
  , ip_addr_(0)
  , m_is_ip_limited(TRUE)
{
  m_pUser = NULL;
  m_pProductManager = NULL;
}

CUserAddDlg::~CUserAddDlg()
{
}

void CUserAddDlg::DoDataExchange(CDataExchange* pDX)
{
  CAddOrModifyDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TREE_PRODUCT, m_con_treectrl);
  DDX_Control(pDX, IDC_IPADDRESS, con_ip_addr_);
  DDX_Text(pDX, IDC_EDIT_USER_NAME, str_user_name_);
  DDX_Text(pDX, IDC_EDIT_PASSWORD, str_password_);
  DDX_Text(pDX, IDC_EDIT_COMPANY, str_company_);
  DDX_Text(pDX, IDC_EDIT_CONTACT, str_contact_);
  DDX_Text(pDX, IDC_EDIT_PHONE_NUMBER, str_phone_number_);
  DDX_Text(pDX, IDC_EDIT_EMAIL, str_email_);
  DDX_Control(pDX, IDC_RADIO_FORMAL, con_customer_type_);
  DDX_Radio(pDX, IDC_RADIO_FORMAL, customer_type_int_);
  DDX_Control(pDX, IDC_DATETIMEPICKER, m_conAuthDate);
  DDX_IPAddress(pDX, IDC_IPADDRESS, ip_addr_);
  DDX_Check(pDX, IDC_IS_IP_LIMITED, m_is_ip_limited);
}


BEGIN_MESSAGE_MAP(CUserAddDlg, CAddOrModifyDialog)
END_MESSAGE_MAP()


// CUserAddDlg message handlers

BOOL CUserAddDlg::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here

  // 利用ProductManager对三态树初始化
  m_imgState.Create(IDB_BITMAP_STATE, 13, 1, RGB(255,255,255));
  m_con_treectrl.SetImageList(&m_imgState, TVSIL_STATE);

  TV_INSERTSTRUCT tvinsert;
  tvinsert.hParent=NULL;
  tvinsert.hInsertAfter=TVI_LAST;
  tvinsert.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_STATE;
  tvinsert.item.hItem=NULL;
  tvinsert.item.state=INDEXTOSTATEIMAGEMASK( 1 );
  tvinsert.item.stateMask=TVIS_STATEIMAGEMASK;
  tvinsert.item.cchTextMax=6;
  tvinsert.item.iSelectedImage=1;
  tvinsert.item.cChildren=0;
  tvinsert.item.lParam=0;

  tvinsert.item.pszText="行情数据产品";
  tvinsert.item.iImage=0;
  HTREEITEM hRoot=m_con_treectrl.InsertItem(&tvinsert);

  m_con_treectrl.SetItemState( hRoot, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK );

  for(int32_t i = 0; i < m_pProductManager->products_count(); ++i)
  {
    ProductDescripter* pPD = m_pProductManager->products(i);
    HTREEITEM hProductItem = m_con_treectrl.InsertItem(pPD->product_name().c_str(), hRoot, TVI_LAST);
    m_con_treectrl.SetItemData(hProductItem, pPD->product_id());

    vector<int16_t> arrMessageID;
    for(int32_t j = 0; j < pPD->support_versions_count(); ++j)
    {
      VersionDescripter* pVD = pPD->support_versions(j);
      for(int32_t k = 0; k < pVD->support_messages_count(); ++k)
      {
        MessageDescripter* pMD = pVD->support_message(k);

        // 防止加入重复的message id
        if( find(arrMessageID.begin(), arrMessageID.end(), pMD->message_id()) == arrMessageID.end() )
        {
          HTREEITEM hMessageItem = m_con_treectrl.InsertItem(pMD->message_description().c_str(), hProductItem, TVI_LAST);
          m_con_treectrl.SetItemData(hMessageItem, pMD->message_id());
          arrMessageID.push_back(pMD->message_id());
        }
      }
    }
  }

  // 展开至产品即可
  HTREEITEM hItem;
  hItem= m_con_treectrl.GetFirstVisibleItem();
  while (hItem != NULL)
  {
    m_con_treectrl.Expand(hItem,TVE_EXPAND);
    // hItem= m_con_treectrl.GetNextItem(hItem, TVGN_NEXTVISIBLE);
    hItem= m_con_treectrl.GetNextSiblingItem(hItem);
  }

  if(m_pUser)
  {
    // ****************************************
    // 利用user初始化界面
    if(!m_pUser->ip_address().empty())
    {
      ip_addr_ = UIUilties::StringToIp(m_pUser->ip_address());
    }

    str_user_name_ = m_pUser->user_name().c_str();
    str_password_ = m_pUser->passwd().c_str();
    str_company_ = m_pUser->company().c_str();
    str_contact_ = m_pUser->contacts().c_str();
    str_phone_number_ = m_pUser->phone_number().c_str();
    str_email_ = m_pUser->email_address().c_str();
    m_is_ip_limited = m_pUser->is_ip_limited()?1:0;

    switch(m_pUser->is_privileged())
    {
    case true:
      customer_type_int_ = 0;
      break;
    case false:
      customer_type_int_ = 1;
      break;
    }

    date_t date = DateTime::ymd2date(m_pUser->authorization_deadline());
    int year, month, day;
    DateTime::date2ymd(date, year, month, day);
    CTime tm(year, month, day, 0, 0, 0);
    m_conAuthDate.SetTime(&tm);
  }
  else
  {
    date_t today =  DateTime::today();
    int year,month,day;    
    DateTime::date2ymd(today,year,month,day);
    CTime tm(year, month, day, 0, 0, 0);
    CTimeSpan tsp(30,0,0,0);
    tm+=tsp;
    m_conAuthDate.SetTime(&tm);
  }

  UpdateData(FALSE);

  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserAddDlg::FillItem()
{
  if(!m_pUser)
    m_pUser = new User;

  UpdateData(TRUE);

  int32_t temp_user_id = m_pUser->user_id();

  m_pUser->Init();

  m_pUser->set_user_id(temp_user_id);

  m_pUser->set_user_name(string(str_user_name_.GetBuffer(0)));
  str_user_name_.ReleaseBuffer();

  m_pUser->set_passwd(string(str_password_.GetBuffer(0)));
  str_password_.ReleaseBuffer();

  BYTE nField[4];
  con_ip_addr_.GetAddress(nField[0], nField[1], nField[2], nField[3]);
  stringstream ss;
  ss << static_cast<int>(nField[0]) << "." << static_cast<int>(nField[1]) << "." << static_cast<int>(nField[2]) << "." << static_cast<int>(nField[3]);
  m_pUser->set_ip_address(ss.str());

  m_pUser->set_company(string(str_company_.GetBuffer(0)));
  str_company_.ReleaseBuffer();

  m_pUser->set_contacts(string(str_contact_.GetBuffer(0)));
  str_contact_.ReleaseBuffer();

  m_pUser->set_phone_number(string(str_phone_number_.GetBuffer(0)));
  str_phone_number_.ReleaseBuffer();

  m_pUser->set_email_address(string(str_email_.GetBuffer(0)));
  str_email_.ReleaseBuffer();

  m_pUser->set_is_ip_limited(m_is_ip_limited);

  switch(customer_type_int_)
  {
  case 0:
    m_pUser->set_is_privileged(true);
    break;
  case 1:
    m_pUser->set_is_privileged(false);
    break;
  default:
    break;
  }

  // 授权截止日期
  CTime time;
  m_conAuthDate.GetTime(time);
  m_pUser->set_authorization_deadline( time.GetYear()*10000+ time.GetMonth()*100+ time.GetDay());

  // ****** 添加授权产品 ******
  HTREEITEM root_item_handle = m_con_treectrl.GetNextItem(NULL, TVGN_ROOT);

  // 遍历整个TreeCtrl
  if(m_con_treectrl.ItemHasChildren(root_item_handle))
  {
    HTREEITEM product_item_handle;
    product_item_handle = m_con_treectrl.GetNextItem(root_item_handle, TVGN_CHILD);

    // 总共两层
    while(NULL != product_item_handle)
    {
      int32_t data = m_con_treectrl.GetItemData(product_item_handle);;
      // CString temp = m_con_treectrl.GetItemText(product_item_handle);

      AuthorizedProduct* ptr_authorized_product = NULL;

      if(m_con_treectrl.ItemHasChildren(product_item_handle))
      {
        HTREEITEM message_item_handle;
        message_item_handle = m_con_treectrl.GetNextItem(product_item_handle, TVGN_CHILD);
        while(NULL != message_item_handle)
        {
          if(m_con_treectrl.GetCheck(message_item_handle))
          {
            if(NULL == ptr_authorized_product)
            { // 如果product下有一个message被选中，则新建AuthorizedProduct，同时赋id
              ptr_authorized_product = new AuthorizedProduct;
              ptr_authorized_product->set_product_id(data);
            }

            data = m_con_treectrl.GetItemData(message_item_handle);
            // temp = m_con_treectrl.GetItemText(message_item_handle);
            ptr_authorized_product->add_message_id(data);
          }

          message_item_handle = m_con_treectrl.GetNextItem(message_item_handle, TVGN_NEXT);
        }
      }

      // 加入user_中
      if(NULL != ptr_authorized_product)
        m_pUser->add_authorized_products(ptr_authorized_product);
 
      product_item_handle = m_con_treectrl.GetNextItem(product_item_handle, TVGN_NEXT);
    }
  }
}

void CUserAddDlg::AddItem()
{
  FillItem();

  // 发送消息至框架，通知添加了一个新用户
  m_pParentWnd->SendMessage(WM_ADD_CUSTOMER, reinterpret_cast<WPARAM>(m_pUser), reinterpret_cast<LPARAM>(&m_bAddDialog));
}

void CUserAddDlg::TimerProcess()
{
  if(m_pUser)
  {
    // 填充授权产品
    for(int32_t i = 0; i < static_cast<int32_t>(m_pUser->authorized_products_count()); ++i)
    {
      int32_t product_id = m_pUser->authorized_products(i)->product_id();

      for(int32_t j = 0; j < static_cast<int32_t>(m_pUser->authorized_products(i)->message_count()); ++j)
      {
        int32_t message_id = m_pUser->authorized_products(i)->message_id(j);

        // 在m_con_treectrl中寻找
        HTREEITEM root_item_handle = m_con_treectrl.GetNextItem(NULL, TVGN_ROOT);

        // 遍历整个TreeCtrl
        if(m_con_treectrl.ItemHasChildren(root_item_handle))
        {
          HTREEITEM product_item_handle;
          product_item_handle = m_con_treectrl.GetNextItem(root_item_handle, TVGN_CHILD);

          // 总共两层
          while(NULL != product_item_handle)
          {
            int32_t temp_product_id = m_con_treectrl.GetItemData(product_item_handle);;
            // CString temp = m_con_treectrl.GetItemText(product_item_handle);

            if(m_con_treectrl.ItemHasChildren(product_item_handle))
            {
              HTREEITEM message_item_handle;
              message_item_handle = m_con_treectrl.GetNextItem(product_item_handle, TVGN_CHILD);
              while(NULL != message_item_handle)
              {
                int temp_message_id = m_con_treectrl.GetItemData(message_item_handle);
                // temp = m_con_treectrl.GetItemText(message_item_handle);

                if(product_id == temp_product_id && message_id == temp_message_id)
                {
                  CString strttt = m_con_treectrl.GetItemText(message_item_handle);
                  m_con_treectrl.SetCheck(message_item_handle);
                }

                message_item_handle = m_con_treectrl.GetNextItem(message_item_handle, TVGN_NEXT);
              }
            }

            product_item_handle = m_con_treectrl.GetNextItem(product_item_handle, TVGN_NEXT);
          }
        }
      }
    }
  }
}

void CUserAddDlg::SetProductManager(wmdf::ProductManager* pProductManager)
{
  m_pProductManager = pProductManager;
}
