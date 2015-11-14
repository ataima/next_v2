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
		CA_TEST(test_n2Connection::test1, "verify vertical connection from component");
		CA_TEST(test_n2Connection::test2, "verify vertical connection from component");
		CA_TEST(test_n2Connection::test3, "verify vertical connection from component");
		CA_TEST(test_n2Connection::test4, "verify vertical connection from component");
		CA_TEST(test_n2Connection::test5, "verify vertical connection from component");
		CA_TEST(test_n2Connection::test6, "verify vertical connection from component");
		CA_TEST(test_n2Connection::test7, "verify horiz connection from component");
		CA_TEST_SUITE_END()
		void setUp(void) {}
	void tearDown(void) {}
	void test1(void);
	void test2(void);
	void test3(void);
	void test4(void);
	void test5(void);
	void test6(void);
	void test7(void);
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
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}


void test_n2Connection::test2(void)
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
	bool res = n2Connection::connectComponent(&mn, n2Point(5,10), n2Point(5, 2));
	InnObj *w = mn.getObj(5, 3);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}

void test_n2Connection::test3(void)
{
	_START();
	_INFO("verifica interna alla classe: metodo connectComponent ");
	_INFO("verifica interna alla classe: componenti with connection ");
	_AUTHOR("Coppi Angelo n2draw library ");
	_STOP();
	nnObjManager mn(50, 20);
	nnObjCoil * coil = new nnObjCoil();
	nnObjContact * contact = new nnObjContact();
	mn.addObj(5, 2, contact);
	mn.addObj(5, 10, coil);
	nnObjWire *wire = new nnObjWire(eWire::wireAngleUpLeft);
	mn.addObj(5, 3, wire);
	bool res = n2Connection::connectComponent(&mn, n2Point(5, 2), n2Point(5, 10));
	InnObj *w = mn.getObj(5, 3);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireTVertLeft);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}


void test_n2Connection::test4(void)
{
	_START();
	_INFO("verifica interna alla classe: metodo connectComponent ");
	_INFO("verifica interna alla classe: componenti with connection ");
	_AUTHOR("Coppi Angelo n2draw library ");
	_STOP();
	nnObjManager mn(50, 20);
	nnObjCoil * coil = new nnObjCoil();
	nnObjContact * contact = new nnObjContact();
	mn.addObj(5, 2, contact);
	mn.addObj(5, 10, coil);
	nnObjWire *wire = new nnObjWire(eWire::wireAngleUpRight);
	mn.addObj(5, 3, wire);
	bool res = n2Connection::connectComponent(&mn, n2Point(5, 2), n2Point(5, 10));
	InnObj *w = mn.getObj(5, 3);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireTVertRight);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}


void test_n2Connection::test5(void)
{
	_START();
	_INFO("verifica interna alla classe: metodo connectComponent ");
	_INFO("verifica interna alla classe: componenti with connection ");
	_AUTHOR("Coppi Angelo n2draw library ");
	_STOP();
	nnObjManager mn(50, 20);
	nnObjCoil * coil = new nnObjCoil();
	nnObjContact * contact = new nnObjContact();
	mn.addObj(5, 2, contact);
	mn.addObj(5, 10, coil);
	nnObjWire *wire = new nnObjWire(eWire::wireTHorizUp);
	mn.addObj(5, 3, wire);
	bool res = n2Connection::connectComponent(&mn, n2Point(5, 2), n2Point(5, 10));
	InnObj *w = mn.getObj(5, 3);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireCross);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}

void test_n2Connection::test6(void)
{
	_START();
	_INFO("verifica interna alla classe: metodo connectComponent ");
	_INFO("verifica interna alla classe: componenti with connection ");
	_AUTHOR("Coppi Angelo n2draw library ");
	_STOP();
	nnObjManager mn(50, 20);
	nnObjCoil * coil = new nnObjCoil();
	nnObjContact * contact = new nnObjContact();
	mn.addObj(5, 2, contact);
	mn.addObj(5, 10, coil);
	nnObjWire *wire = new nnObjWire(eWire::wireAngleDownLeft);
	mn.addObj(5, 9, wire);
	bool res = n2Connection::connectComponent(&mn, n2Point(5, 2), n2Point(5, 10));
	InnObj *w = mn.getObj(5, 3);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 5);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 7);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 8);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireVertical);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(5, 9);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireTVertLeft);
	std::wcout << w->toString() << std::endl;
	CA_ASSERT(res == true);
	res = mn.removeAll();
	CA_ASSERT(res == true);
}



void test_n2Connection::test7(void)
{
	_START();
	_INFO("verifica interna alla classe: metodo connectComponent ");
	_INFO("verifica interna alla classe: component with connection ");
	_AUTHOR("Coppi Angelo n2draw library ");
	_STOP();
	nnObjManager mn(50, 20);
	nnObjCoil * coil = new nnObjCoil();
	nnObjContact * contact = new nnObjContact();
	mn.addObj(5, 5, contact);
	mn.addObj(11, 5, coil);
	n2Point p1(5, 5);
	n2Point p2(11, 5);
	bool res = n2Connection::connectComponent(&mn, p1, p2);
	InnObj *w = mn.getObj(5, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireAngleDownRight);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum()==3);
	std::wcout << w->toString() << std::endl;
	w = mn.getObj(6, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 3);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(7, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 3);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(8, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 3);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(9, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 3);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(10, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 3);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(11, 4);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireAngleDownLeft);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 3);

	std::wcout << w->toString() << std::endl;
	
	w = mn.getObj(5, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireAngleUpRight);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(6, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(7, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(8,6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(9, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(10, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireHorizzontal);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	w = mn.getObj(11, 6);
	CA_ASSERT(w != nullptr);
	CA_ASSERT(w->isComponent() == false);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getWire() == eWire::wireAngleUpLeft);
	CA_ASSERT(dynamic_cast<nnObjWire*>(w)->getNum() == 4);

	std::wcout << w->toString() << std::endl;
	res = mn.removeAll();
	CA_ASSERT(res == true);
}