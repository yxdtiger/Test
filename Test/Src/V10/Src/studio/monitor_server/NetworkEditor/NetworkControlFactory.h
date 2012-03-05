#ifndef _NETWORKCONTROLFACTORY_H_
#define _NETWORKCONTROLFACTORY_H_

#include "DiagramEditor/DiagramEntity.h"

class CNetworkControlFactory 
{

public:
// Implementation
	static CDiagramEntity* CreateFromString( const CString& str );

};


#endif //_NETWORKCONTROLFACTORY_H_
