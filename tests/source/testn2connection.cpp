#include "stdio.h"
#include "CPPtester.h"
#include "windows.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2connection.h"
#include <iostream>


///////////////////// HEADER //////////////////////

class test_n2Connection
	: public caTester
{
	CA_TEST_SUITE(test_n2Connection)
		CA_TEST(test_n2Connection::test1, "verifica set/get pos");
	CA_TEST_SUITE_END()
		void setUp(void) {}
	void tearDown(void) {}
	void test1(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_n2Connection);

void test_n2Connection::test1(void)
{
	_START();
	_INFO("verifica interrna alla classe: metodo connectComponent ");
	_AUTHOR("Coppi Angelo n2draw library ");
	_STOP();
	nnObjManager mn(50, 20);
	nnObjCoil * coil = new nnObjCoil();
	nnObjContact * contact = new nnObjContact();
	mn.addObj(5, 2, contact);
	mn.addObj(5, 10, coil);
	bool res=n2Connection::connectComponent(&mn, n2Point(5, 2), n2Point(5, 10));
	InnObj *w = mn.getObj(5, 3);
	CA_ASSERT(w!=nullptr);
	CA_ASSERT(w->isComponent()==false);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}
