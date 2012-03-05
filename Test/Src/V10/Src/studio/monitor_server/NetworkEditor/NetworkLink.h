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

// Data
	CString	from;		// Name of the attached from-object.
	CString	to;			// Name of the attached to-object, if any.
};

#endif //_NETWORKLINK_H_