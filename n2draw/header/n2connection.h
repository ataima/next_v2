#ifndef N2_CONNECTION_HEADER
#define N2_CONNECTION_HEADER

class IManager;

typedef struct tag_n2_point
{
	size_t x;
	size_t y;
	tag_n2_point(size_t _x, size_t _y) :x(_x), y(_y) {}
} n2Point;

class n2Connection
{
public:
	static bool connectComponent(IManager *manager,n2Point src, n2Point dst);
};


#endif