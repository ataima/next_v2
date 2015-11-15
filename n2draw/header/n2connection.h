#ifndef N2_CONNECTION_HEADER
#define N2_CONNECTION_HEADER

class IManager;

#include "n2draw.h"

class n2Connection
{
public:
	static bool connectComponent(IManager *manager, n2Point & src, n2Point & dst);
protected:
	static bool connectVertComponent(IManager *manager, n2Point &p_src, n2Point & p_dst);
	static bool connectHorzIncrUpSideComponet(IManager *manager, n2Point & src, n2Point & dst);
	static bool connectHorzIncrDownSideComponet(IManager *manager, n2Point & src, n2Point & dst);
	static bool connectHorzDecrUpSideComponet(IManager *manager, n2Point & src, n2Point & dst);
	static bool connectHorzDecrDownSideComponet(IManager *manager, n2Point & src, n2Point & dst);

	static bool connectVertWireComponent(IManager *manager, n2Point &p_src, n2Point & p_dst);
};


#endif