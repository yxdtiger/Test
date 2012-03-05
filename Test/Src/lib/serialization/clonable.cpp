#include "clonable.h"
#include "memento.h"

using namespace wmdf;

void Clonable::Assign(Clonable *source)
{
	Memento *m = new Memento();
	source->SaveState(m);
	LoadState(m);
	delete m;
}
void Clonable::AssignTo(Clonable *dest)
{
	dest->Assign(this);
}

Clonable* Clonable::Clone()
{
	Clonable* Result = CreateObject();
	Result->Assign(this);
	return Result;
}
