/////////////////////////////////////////////////////////////////////////////////////////////////////
// This code exists to work around an MFC bug.
//
// see Microsoft Connect issue 404296 -- CFormView with Group Box display issues for both transparent and non-transparent Group Boxes
//                                     Microsoft claims that the problem will be fixed in VS 2010.
//
// The problem is that CMDIChildWndEx::AdjustClientArea sets the WS_CLIPCHILDREN and sometimes WS_CLIPSIBLINGS windows
// styles and leave them set. This causes group boxes to disappear (see "PRB: BS_GROUPBOX-Style Child Window Background Painting Wrong"
// at http://support.microsoft.com/kb/79982). Unfortunately AdjustClientArea is not a virtual function, so we can't easily overload it.
// To work around the problem code has been added to save the window style and then restore it after calls to AdjustDockingLayout
// and RecalcLayout both of which are virtual and which call AdjustClientArea.

#include <afxDockablePane.h>

// This helper class is used to save the window style and then restore the WS_CLIPCHILDREN and WS_CLIPSIBLINGS flags.
class ClipStyleReset
{
public:

    ClipStyleReset(CMDIChildWndEx * p_pMDIChildWndEx)
        : m_pChildWnd(NULL)
        , m_dwChildStyle(0)
    {
        // Get a pointer to the window that CMDIChildWndEx::AdjustClientArea will change the windows style on.
        // This logic is copied directly from CMDIChildWndEx::AdjustClientArea.
        CDockablePane * pTabbedControlBar = p_pMDIChildWndEx->GetTabbedPane();

        m_pChildWnd = (pTabbedControlBar != NULL && pTabbedControlBar->IsMDITabbed() &&
            pTabbedControlBar->GetParent() == p_pMDIChildWndEx) ? pTabbedControlBar : p_pMDIChildWndEx->GetDlgItem(AFX_IDW_PANE_FIRST);

        // If there is a child window, save its style.
        if (m_pChildWnd != NULL)
        {
            m_dwChildStyle = m_pChildWnd->GetStyle();
        }
    }

    ~ClipStyleReset()
    {
        if (m_pChildWnd != NULL)
        {
            // If there is a child window, restore the flags that may have changed.
            m_pChildWnd->ModifyStyle(
                WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                (WS_CLIPCHILDREN | WS_CLIPSIBLINGS) & m_dwChildStyle
                );
        }
    }

private:

    CWnd *    m_pChildWnd;
    DWORD    m_dwChildStyle;
};

// End MFC bug work around.
/////////////////////////////////////////////////////////////////////////////////////////////////////