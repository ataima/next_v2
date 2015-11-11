#include "stdio.h"
#include "CPPtester.h"
#include "windows.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include <iostream>


///////////////////// HEADER //////////////////////

class test_n2ObjManager
    : public caTester
{
    CA_TEST_SUITE(test_n2ObjManager)
    CA_TEST(test_n2ObjManager::test1, "verifica set/get pos");
    CA_TEST(test_n2ObjManager::test2, "verifica tostring");
    CA_TEST(test_n2ObjManager::test3, "verifica et wire");
    CA_TEST(test_n2ObjManager::test4, "verifica tostring");
    CA_TEST(test_n2ObjManager::test5, "verifica get direction1");
    CA_TEST(test_n2ObjManager::test6, "verifica get direction2");
    CA_TEST(test_n2ObjManager::test7, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test8, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test9, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test10, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test11, "verifica move");
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
    void test8(void);
    void test9(void);
    void test10(void);
    void test11(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_n2ObjManager);


void test_n2ObjManager::test1(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjWire *v = new nnObjWire(eWire::wireAngleDownLeft);
    bool res = mn.addObj(10, 12, v);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    res = mn.removeAll();
    CA_ASSERT(res == true);
}



void test_n2ObjManager::test2(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjWire *v = new nnObjWire(eWire::wireAngleDownLeft);
    bool res = mn.addObj(10, 12, v);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    nnObjComponent *u = new nnObjCoil();
    res = mn.replaceObj(10, 12, u);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    res = mn.removeObj(10, 12);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == 0);
    res = mn.removeAll();
    CA_ASSERT(res == false);
}

void test_n2ObjManager::test3(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    CA_ASSERT((int)mn.size() == 100);
    bool res = mn.removeAll();
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == 0);
}

void test_n2ObjManager::test4(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    for (size_t i = 0; i < 50; i++)
    {
        InnWire * wire = dynamic_cast<InnWire*>(mn.getObj(i, 0));
                CA_ASSERT(wire!=NULL);
        CA_ASSERT(wire->getNum() == (size_t)1);
        wire = dynamic_cast<InnWire*>(mn.getObj(i, 19));
                CA_ASSERT(wire!=NULL);
        CA_ASSERT(wire->getNum() == (size_t)2);
    }
    nnObjComponent *c=new nnObjComponent(ObjContext::objContact);
    c->setXpos(5);
    c->setYpos(1);
    c->connect(mn.getObj(5, 0));
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);


}

void test_n2ObjManager::test5(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    nnObjComponent *c=new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, c);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    InnObj * p = mn.getObj(5, 0);
    std::wstring w = p->toString();
    std::wcout << w.c_str();
    w = c->toString();
    std::wcout << w.c_str();
}


void test_n2ObjManager::test6(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    nnObjComponent *c=new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, c);
    CA_ASSERT(c->getUpWire().getNum()== (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    nnObjComponent *d=new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 2, d);
    InnObj * p = mn.getObj(5, 0);
    std::wstring w = p->toString();
    std::wcout << w.c_str();
    w = c->toString();
    std::wcout << w.c_str();
    w = d->toString();
    std::wcout << w.c_str();
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getUpWire().getNum() == (size_t)3);
    CA_ASSERT(d->getDownWire().getNum() == (size_t)0);
}

void test_n2ObjManager::test7(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    nnObjComponent *c=new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, c);
    CA_ASSERT(c->getUpWire().getNum()== (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    nnObjComponent *d=new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 2, d);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getUpWire().getNum() == (size_t)3);
    CA_ASSERT(d->getDownWire().getNum() == (size_t)0);
    mn.removeObj(5, 2);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
}


void test_n2ObjManager::test8(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo connect disconnect ");
    _INFO("inserimento 3 contatti serie e rimozione del contatto centrale");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    nnObjComponent *c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, c);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    nnObjComponent *d = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 2, d);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getUpWire().getNum() == (size_t)3);
    CA_ASSERT(d->getDownWire().getNum() == (size_t)0);
    nnObjComponent *e = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 3, e);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getUpWire().getNum() == (size_t)3);
    CA_ASSERT(d->getDownWire().getNum() == (size_t)4);
    CA_ASSERT(e->getUpWire().getNum() == (size_t)4);
    CA_ASSERT(e->getDownWire().getNum() == (size_t)0);
    mn.removeObj(5, 2);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    CA_ASSERT(e->getUpWire().getNum() == (size_t)0);
    CA_ASSERT(e->getDownWire().getNum() == (size_t)0);
}


void test_n2ObjManager::test9(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo connect disconnect  ");
    _INFO("inserimento 2 contatti serie e rimozione del file centrale");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    nnObjComponent* c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, c);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    nnObjWire *d = new nnObjWire(eWire::noWire);
    mn.addObj(5, 2, d);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getNum() == (size_t)3);
    nnObjComponent *e = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 3, e);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getNum() == (size_t)3);
    CA_ASSERT(e->getUpWire().getNum() == (size_t)3);
    CA_ASSERT(e->getDownWire().getNum() == (size_t)0);
    mn.removeObj(5, 2);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    CA_ASSERT(e->getUpWire().getNum() == (size_t)0);
    CA_ASSERT(e->getDownWire().getNum() == (size_t)0);
}

void test_n2ObjManager::test10(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo connect disconnect  ");
    _INFO("inserimento 2 contatti serie e rimozione del file primo centrale");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    mn.setupPower();
    nnObjComponent* c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, c);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    nnObjWire *d = new nnObjWire(eWire::noWire);
    mn.addObj(5, 2, d);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getNum() == (size_t)3);
    nnObjWire *e = new nnObjWire(eWire::noWire);
    mn.addObj(5, 3, e);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getNum() == (size_t)3);
    CA_ASSERT(e->getNum() == (size_t)3);
    nnObjComponent *f = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 4, f);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)3);
    CA_ASSERT(d->getNum() == (size_t)3);
    CA_ASSERT(e->getNum() == (size_t)3);
    CA_ASSERT(f->getUpWire().getNum() == (size_t)3);
    CA_ASSERT(f->getDownWire().getNum() == (size_t)0);
    mn.removeObj(5, 2);
    CA_ASSERT(c->getUpWire().getNum() == (size_t)1);
    CA_ASSERT(c->getDownWire().getNum() == (size_t)0);
    CA_ASSERT(f->getUpWire().getNum() == (size_t)0);
    CA_ASSERT(f->getDownWire().getNum() == (size_t)0);
    CA_ASSERT(mn.size() == 102);
}


void test_n2ObjManager::test11(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo move");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjCoil *c = new nnObjCoil();
    bool res = mn.addObj(10, 12, c);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    CA_ASSERT(mn.getObj(10, 12) != nullptr);
    CA_ASSERT(mn.getObj(10, 12)->isComponent());
    res=mn.moveObj(n2Point(10, 12), n2Point(5, 3));
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    CA_ASSERT(mn.getObj(5, 3) != nullptr);
    CA_ASSERT(mn.getObj(5, 3)->isComponent());
    CA_ASSERT(mn.getObj(10, 12) == nullptr);
    res = mn.removeAll();
    CA_ASSERT(res == true);
}