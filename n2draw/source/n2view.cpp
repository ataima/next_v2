#include "n2drawmanager.h"
#include "n2view.h"
#include <iostream>

void nn2TextView::draw(IManager * manager, void * context)
{
	if (manager != nullptr)
	{
		size_t h, w;
		n2Point start = manager->getStartPoint();
		n2Point stop = manager->getStopPoint();
		for (h = start.y; h <= stop.y; h++)
		{
			for (w = start.x; w <= stop.x; w++)
			{
				InnObj *obj = manager->getObj(w, h);
				if (obj != nullptr)
				{
					if (obj->isComponent())
					{
						if (obj->getContext() == ObjContext::objCoil)
						{
							std::cout << "O";
						}
						else
							if (obj->getContext() == ObjContext::objContact)
							{
								std::cout << "=";
							}
					}
					else
					{
						nnObjWire *wire = dynamic_cast<nnObjWire*>(obj);
						switch (wire->getWire())
						{
						case noWire:
							std::cout << ".";
							break;
						case wireHorizzontal:
							std::cout << "-";
							break;
						case wireVertical:
							std::cout << "|";
							break;
						case wireAngleUpRight:
							std::cout << "<";
							break;
						case wireAngleUpLeft:
							std::cout << ">";
							break;
						case wireAngleDownRight:
							std::cout << "[";
							break;
						case wireAngleDownLeft:
							std::cout << "]";
							break;
						case wireTHorizDown:
							std::cout << "T";
							break;
						case wireTHorizUp:
							std::cout << "L";
							break;
						case wireTVertRight:
							std::cout << "{";
							break;
						case wireTVertLeft:
							std::cout << "}";
							break;
						case wireCross:
							std::cout << "+";
							break;
						}
					}
				}
				else
				{
					std::cout << " ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}
