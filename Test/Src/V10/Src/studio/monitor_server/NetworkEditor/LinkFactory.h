#if !defined(AFX_LINKFACTORY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_LINKFACTORY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

class CLinkFactory
{

public:
// Implementation
	static CString GetID();

private:
// Private data
	static int s_currentID;

};


#endif // !defined(AFX_LINKFACTORY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
