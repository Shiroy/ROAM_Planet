#pragma once

class Triangle;

class Diamond
{
public:
	Diamond(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4);
	Diamond(Triangle *t);
	~Diamond(void);

	bool canBeMerged(/*A remplir*/);

	Triangle *pTriComposed[4];
};

