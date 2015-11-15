#ifndef N2VIEW_HEADER
#define N2VIEW_HEADER

// interface
class InnObjView
{
public:
	virtual void draw(IManager *manager, void * context) = 0;
};



class nn2TextView
	:public InnObjView
{
public:
	void draw(IManager * manager, void * context);
};
#endif
