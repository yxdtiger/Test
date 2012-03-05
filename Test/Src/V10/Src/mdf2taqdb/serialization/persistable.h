#if !defined PERSISTABLE_H
#define PERSISTABLE_H

namespace wmdf{

class Memento;
 class  Persistable
{
public:
	Persistable(void);
	virtual ~Persistable(void);
public:
	virtual bool LoadState(Memento* m) = 0;
	virtual bool SaveState(Memento* m) = 0;
};

}

#endif
