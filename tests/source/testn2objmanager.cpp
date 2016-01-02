#include "stdio.h"
#include "CPPtester.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include <iostream>
#include "n2miniXml.h"
#include "n2view.h"
#include "n2connection.h"

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

///////////////////// HEADER //////////////////////

class test_n2ObjManager
    : public caTester
{
    CA_TEST_SUITE(test_n2ObjManager)
        CA_TEST(test_n2ObjManager::test1, "verifica set/get pos");
    CA_TEST(test_n2ObjManager::test2, "verifica tostring");
    CA_TEST(test_n2ObjManager::test4, "verifica tostring");
    CA_TEST(test_n2ObjManager::test5, "verifica get direction1");
    CA_TEST(test_n2ObjManager::test6, "verifica get direction2");
    CA_TEST(test_n2ObjManager::test7, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test8, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test9, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test10, "verifica connect/disconnect");
    CA_TEST(test_n2ObjManager::test11, "verifica move");
    CA_TEST(test_n2ObjManager::test12, "verifica save");
    CA_TEST(test_n2ObjManager::test13, "verifica save + load");
    CA_TEST(test_n2ObjManager::test14, "verifica undo");
    CA_TEST(test_n2ObjManager::test15, "verifica insert row");
    CA_TEST(test_n2ObjManager::test16, "verifica insert row");
    CA_TEST(test_n2ObjManager::test17, "verifica insert add Coil");
    CA_TEST(test_n2ObjManager::test18, "verifica insert add Coil");
    CA_TEST_SUITE_END()
        void setUp(void) {}
    void tearDown(void) {}
    void test1(void);
    void test2(void);
    void test4(void);
    void test5(void);
    void test6(void);
    void test7(void);
    void test8(void);
    void test9(void);
    void test10(void);
    void test11(void);
    void test12(void);
    void test13(void);
    void test14(void);
    void test15(void);
    void test16(void);
    void test17(void);
    void test18(void);
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


void test_n2ObjManager::test4(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent *c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    CA_ASSERT(mn.size() == 1);

}

void test_n2ObjManager::test5(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent *c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    InnObj * p = mn.getObj(5, 0);
    CA_ASSERT(p!=nullptr);
    STRING w = p->toString();
    //std::wcout << w.c_str()<<std::endl;
    w = c->toString();
    //std::wcout << w.c_str() << std::endl;
}


void test_n2ObjManager::test6(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent *c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    nnObjComponent *d = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, d);
    InnObj * p = mn.getObj(5, 0);
    STRING w = p->toString();
    std::wcout << w.c_str() << std::endl;
    w = c->toString();
    std::wcout << w.c_str() << std::endl;
    w = d->toString();
    std::wcout << w.c_str() << std::endl;
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(d->getConnections().back() == (int)0);
}

void test_n2ObjManager::test7(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo ");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent *c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    nnObjComponent *d = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, d);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(d->getConnections().back() == (int)0);
    mn.removeObj(5, 1);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
}


void test_n2ObjManager::test8(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo connect disconnect ");
    _INFO("inserimento 3 contatti serie e rimozione del contatto centrale");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent *c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    nnObjComponent *d = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 1, d);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(d->getConnections().back() == (int)0);
    nnObjComponent *e = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 2, e);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(d->getConnections().back() == (int)4);
    CA_ASSERT(e->getConnections().front() == (int)4);
    CA_ASSERT(e->getConnections().back() == (int)0);
    mn.removeObj(5, 1);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    CA_ASSERT(e->getConnections().front() == (int)0);
    CA_ASSERT(e->getConnections().back() == (int)0);
}


void test_n2ObjManager::test9(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo connect disconnect  ");
    _INFO("inserimento 2 contatti serie e rimozione del file centrale");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent* c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    nnObjWire *d = new nnObjWire(eWire::noWire);
    mn.addObj(5, 1, d);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);;
    nnObjComponent *e = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 2, e);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(e->getConnections().front() == (int)3);
    CA_ASSERT(e->getConnections().back() == (int)0);
    mn.removeObj(5,1);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    CA_ASSERT(e->getConnections().front() == (int)0);
    CA_ASSERT(e->getConnections().back() == (int)0);
}

void test_n2ObjManager::test10(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo connect disconnect  ");
    _INFO("inserimento 2 contatti serie e rimozione del file primo centrale");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjComponent* c = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 0, c);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    nnObjWire *d = new nnObjWire(eWire::noWire);
    mn.addObj(5, 1, d);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    nnObjWire *e = new nnObjWire(eWire::noWire);
    mn.addObj(5, 2, e);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(e->getConnections().front() == (int)3);
    nnObjComponent *f = new nnObjComponent(ObjContext::objContact);
    mn.addObj(5, 3, f);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)3);
    CA_ASSERT(d->getConnections().front() == (int)3);
    CA_ASSERT(e->getConnections().front() == (int)3);
    CA_ASSERT(f->getConnections().front() == (int)3);
    CA_ASSERT(f->getConnections().back() == (int)0);
    mn.removeObj(5, 1);
    CA_ASSERT(c->getConnections().front() == (int)1);
    CA_ASSERT(c->getConnections().back() == (int)0);
    CA_ASSERT(f->getConnections().front() == (int)0);
    CA_ASSERT(f->getConnections().back() == (int)0);
    CA_ASSERT(mn.size() == 2);
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
    res = mn.moveObj(nnPoint(10, 12), nnPoint(5, 3));
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    CA_ASSERT(mn.getObj(5, 3) != nullptr);
    CA_ASSERT(mn.getObj(5, 3)->isComponent());
    CA_ASSERT(mn.getObj(10, 12) == nullptr);
    res = mn.removeAll();
    CA_ASSERT(res == true);
}


void test_n2ObjManager::test12(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo save");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjCoil *c = new nnObjCoil();
    bool res = mn.addObj(10, 12, c);
    CA_ASSERT(res == true);
    c = new nnObjCoil();
    res = mn.addObj(11, 12, c);
    CA_ASSERT(res == true);
    c = new nnObjCoil();
    res = mn.addObj(12, 12, c);
    CA_ASSERT(res == true);
    nnObjContact *d = new nnObjContact();
    res = mn.addObj(10, 5, d);
    CA_ASSERT(res == true);
    d = new nnObjContact();
    res = mn.addObj(11, 5, d);
    CA_ASSERT(res == true);
    d = new nnObjContact();
    res = mn.addObj(12, 5, d);
    CA_ASSERT(res == true);
    remove(".\\testsave.xml");
    STRING name(X(".\\testsave.xml"));
    mn.save(name);
    miniXmlNode root(X(""));
    root.load(name.c_str(), &root);
    remove(".\\testsave.xml");
    root.print(stdout);
    res = mn.removeAll();
    CA_ASSERT(res == true);
}


void test_n2ObjManager::test13(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo save + load");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjCoil *c = new nnObjCoil();
    bool res = mn.addObj(10, 12, c);
    CA_ASSERT(res == true);
    c = new nnObjCoil();
    res = mn.addObj(11, 12, c);
    CA_ASSERT(res == true);
    c = new nnObjCoil();
    res = mn.addObj(12, 12, c);
    CA_ASSERT(res == true);
    nnObjContact *d = new nnObjContact();
    res = mn.addObj(10, 5, d);
    CA_ASSERT(res == true);
    d = new nnObjContact();
    res = mn.addObj(11, 5, d);
    CA_ASSERT(res == true);
    d = new nnObjContact();
    res = mn.addObj(12, 5, d);
    CA_ASSERT(res == true);
    remove(".\\testsaveload.xml");
    STRING name(X(".\\testsaveload.xml"));
    mn.save(name);
    nnObjManager mp(50, 20);
    mp.load(name);
    remove(".\\testsaveload.xml");
    CA_ASSERT(mn.size()==mp.size());
    // more test
}

void test_n2ObjManager::test14(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo undo");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjCoil *c = new nnObjCoil();
    bool res = mn.addObj(10, 12, c);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.size()==1);
    CA_ASSERT(mn.getUndoObjs().size() == 1);
    CA_ASSERT(mn.getUndoObjs().front().action == 2 );
    mn.undo();
    CA_ASSERT(mn.size() == 0);
    CA_ASSERT(mn.getUndoObjs().size() == 0);
    CA_ASSERT(mn.geRedoObjs().size() == 1);
    mn.redo();
    CA_ASSERT(mn.size() == 1);
    CA_ASSERT(mn.getUndoObjs().size() == 1);
    CA_ASSERT(mn.geRedoObjs().size() == 0);
}


void test_n2ObjManager::test15(void)
{
    struct tag_check
    {
        void revHashKey(hashkey & key, int & x, int &y)
        {
            y = key.v1;
            y = y / 2;
            x = key.v2;
            x = x / 2;
        }

    };
    _START();
    _INFO("verifica interrna alla classe: metodo insert row");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjCoil *c = new nnObjCoil();
    bool res = mn.addObj(10, 12, c);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.size() == 1);
    res=mn.insertRow(6);
    CA_ASSERT(res == true);
    res = mn.insertRow(6);
    CA_ASSERT(res == true);
    res = mn.insertRow(6);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.getHeight()==23);
    CA_ASSERT(mn.size()==1);
    CA_ASSERT(mn.getObj(10, 15) == c);	
    nnObjManager::iterator it = mn.begin();
    tag_check css;
    int x, y;
    hashkey k = it->first;
    css.revHashKey(k, x, y);
    CA_ASSERT(x == 10);
    CA_ASSERT(y == 15);
}



void test_n2ObjManager::test16(void)
{
    struct tag_check
    {
        void revHashKey(hashkey & key, int & x, int &y)
        {
            y = key.v1;
            y = y / 2;
            x = key.v2;
            x = x / 2;
        }

    };
    _START();
    _INFO("verifica interrna alla classe: metodo insert col");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjCoil *c = new nnObjCoil();
    bool res = mn.addObj(10, 12, c);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.size() == 1);
    res = mn.insertCol(8);
    CA_ASSERT(res == true);
    res = mn.insertCol(8);
    CA_ASSERT(res == true);
    res = mn.insertCol(8);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.getWidth() == 53);
    CA_ASSERT(mn.size() == 1);
    CA_ASSERT(mn.getObj(13, 12) == c);
    nnObjManager::iterator it = mn.begin();
    tag_check css;
    int x, y;
    hashkey k = it->first;
    css.revHashKey(k, x, y);
    CA_ASSERT(x == 13);
    CA_ASSERT(y == 12);
    res = mn.ResizeHeight(15);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.getHeight() == 15);
    res = mn.ResizeWidth(30);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.getWidth() == 30);
    res = mn.Resize(25,14);
    CA_ASSERT(res == true);
    c = new nnObjCoil();
    res = mn.addObj(24, 13, c);
    CA_ASSERT(res == true);
    res = mn.Resize(20, 14);
    CA_ASSERT(res == true);
    CA_ASSERT(mn.getObj(19, 13) == c);
}

void test_n2ObjManager::test17(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo add Coil");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjContact *v = new nnObjContact();
    bool res = mn.addContact(10, 0, v);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    nnObjCoil *u = new nnObjCoil();
    res = mn.addCoil(10,  u);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)20);
}


void test_n2ObjManager::test18(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo add Coil +connect");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjContact *v = new nnObjContact();
    bool res = mn.addContact(10, 0, v);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)1);
    nnObjCoil *u = new nnObjCoil();
    res = mn.addCoil(10, u);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn.size() == (int)20);
    v = new nnObjContact();
    res = mn.addContact(12, 0, v);
    CA_ASSERT(res == true);
    nnPoint p1(12, 0);
    nnPoint p2(10, 0);
    nnConnection::connectComponent(&mn, p1, p2);
}
