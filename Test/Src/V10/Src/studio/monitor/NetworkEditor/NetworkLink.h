#ifndef _NETWORKLINK_H_
#define _NETWORKLINK_H_

class CNetworkLink : public CObject
{
public:
	CNetworkLink();
	virtual ~CNetworkLink();

// Implementation
	virtual CString GetString() const;
	virtual CNetworkLink* Clone() const;
	virtual BOOL FromString( const CString& str );

  int network_stat;//0=>连接断开 1=>数据阻塞 2=>正常

// Data
	CString	from;		// Name of the attached from-object.
	CString	to;			// Name of the attached to-object, if any.
  double flow;       //该link对应的流量
};

#endif //_NETWORKLINK_H_