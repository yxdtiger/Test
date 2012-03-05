#if !defined CLONABLE_H
#define CLONABLE_H

#include "persistable.h"

namespace wmdf 
{

#define CREATEOBJECT_CLONE(a) protected: virtual Clonable* CreateObject(){return new a;}

class Clonable: public Persistable
{
public:
	virtual ~Clonable(){}
protected:
	virtual Clonable* CreateObject() = 0;
public:
	void Assign(Clonable *source);
	void AssignTo(Clonable *dest);

	virtual Clonable *Clone();
};

}

#endif