#pragma once

class Triangle;

class Diamond
{
public:
	Diamond(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4);
	Diamond(Triangle *t);
	~Diamond(void);

	void merge();

private:

	Triangle *pTriComposed[4];
	bool callByMerge;
};

