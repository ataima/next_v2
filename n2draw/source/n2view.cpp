#include "n2drawmanager.h"
#include "n2view.h"
#include <iostream>


/**************************************************************
Copyright(c) 2015 Angelo Coppi

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions :

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
********************************************************************/

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
