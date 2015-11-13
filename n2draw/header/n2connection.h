#ifndef N2_CONNECTION_HEADER
#define N2_CONNECTION_HEADER

class IManager;

#include "n2draw.h"

class n2Connection
{
public:
	static bool connectComponent(IManager *manager,n2Point & src, n2Point & dst);
};


#endif