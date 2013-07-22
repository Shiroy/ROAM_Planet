#include "Diamond.h"
#include "Triangle.h"
#define NULL 0

Diamond::Diamond(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4)
{
	pTriComposed[0] = t1;
	pTriComposed[1] = t2;
	pTriComposed[2] = t3;
	pTriComposed[3] = t4;

	pTriComposed[0]->diamond = this;
	pTriComposed[1]->diamond = this;
	pTriComposed[2]->diamond = this;
	pTriComposed[3]->diamond = this;

	callByMerge = false;
}


Diamond::~Diamond(void)
{
	if(!callByMerge)
	{
		pTriComposed[0]->diamond = NULL;
		pTriComposed[1]->diamond = NULL;
		pTriComposed[2]->diamond = NULL;
		pTriComposed[3]->diamond = NULL;
	}
}

void Diamond::merge()
{
	pTriComposed[0]->merge();
	callByMerge = true;
	delete this;
}
