// NetworkMapEditorDoc.h : interface of the CNetworkMapEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef NETWORKMAPEDITORDOC_H
#define NETWORKMAPEDITORDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetworkEditor/NetworkEntityContainer.h"

class CNetworkMapEditorDoc : public CDocument
{
protected: // create from serialization only
	CNetworkMapEditorDoc();
	DECLARE_DYNCREATE(CNetworkMapEditorDoc)

// Attributes
public:

// Operations
public:

	CNetworkEntityContainer* GetData() { return &m_objs; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkMapEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetworkMapEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNetworkMapEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CNetworkEntityContainer	m_objs;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKMAPEDITORDEMODOC_H__B47B1EB9_777D_4082_96DB_AAC17F2F3E96__INCLUDED_)
