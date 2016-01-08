#include "stdio.h"
#include "CPPtester.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include <iostream>
#include "n2miniXml.h"
#include "n2view.h"
#include "n2connection.h"
#include "n2viewglue.h"

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

class test_viewglue
    : public caTester
{
    CA_TEST_SUITE(test_viewglue)
        CA_TEST(test_viewglue::test1, "verifica ctor");
    CA_TEST(test_viewglue::test2, "verifica read configuration");
    CA_TEST(test_viewglue::test3, "verifica select");
    CA_TEST(test_viewglue::test4, "verifica mouse event");
    CA_TEST(test_viewglue::test5, "verifica mouse event");
    CA_TEST_SUITE_END()
    void setUp(void) {}
    void tearDown(void) {}
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_viewglue);

void test_viewglue::test1(void)
{
    _START();
    _INFO("Verify of internal class nnViewGlue: method  ctor");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnObjWire *v = new nnObjWire(eWire::wireAngleDownLeft);
    bool res = mn.addObj(10, 12, v);
    CA_ASSERT(res == true);
    nnViewGlue gphy(&mn,nullptr,nullptr);
    int w, h;
    CA_ASSERT(gphy.getSelectAreaPhy(w, h) == true);
    CA_ASSERT(w == 0);
    CA_ASSERT(h == 0);
    CA_ASSERT(gphy.getManager() == &mn);
    gphy.setManager(nullptr);
    CA_ASSERT(gphy.getManager() == nullptr);

}

void test_viewglue::test2(void)
{
    _START();
    _INFO("Verify of internal class nnViewGlue: method  readconfiguration");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    miniXmlNode  node(X("PHY_MAP"));
    node.add(X("TYPE"), 1);
    node.add(X("X"), 100);
    node.add(X("Y"), 200);
    node.add(X("HEIGHT"), 800);
    node.add(X("WIDTH"), 1200);
    nnViewGlue gphy(nullptr, nullptr,nullptr);
    bool res = false; 
    try {
        res = gphy.readConfiguration(&node);
    }
    catch (...) {}
    CA_ASSERT(res == false);
    nnPoint input(1, 1);
    nnPoint p = gphy.getCoordPhy(input);
    CA_ASSERT(p.x == 100);
    CA_ASSERT(p.y == 200);
    input.set(10000, 20000);
    p = gphy.getCoordLog(input);
    CA_ASSERT(p.x == 100);
    CA_ASSERT(p.y == 100);
}


void test_viewglue::test3(void)
{
    _START();
    _INFO("Verify of internal class nnViewGlue: method  select");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnViewGlue gphy(&mn,nullptr,nullptr);
    miniXmlNode  node(X("PHY_MAP"));
    node.add(X("TYPE"), 1);
    node.add(X("X"), 24);
    node.add(X("Y"), 32);
    node.add(X("HEIGHT"), 600);
    node.add(X("WIDTH"), 1200);
    bool res = false;
    try {
        res = gphy.readConfiguration(&node);
    }
    catch (...) {}
    CA_ASSERT(res == false);
    CA_ASSERT(gphy.isStartValid() == false);
    CA_ASSERT(gphy.isStopValid() == false);
    gphy.selectStart(10, 10);
    gphy.selectStop(10, 10);
    CA_ASSERT(gphy.isStartValid() == true);
    CA_ASSERT(gphy.isStopValid() == true);
    int w, h;
    res = gphy.getSelectAreaPhy(w, h);
    CA_ASSERT(res == true);
    CA_ASSERT(w == 24);
    CA_ASSERT(h == 32);
    gphy.selectStop(25, 15);
    CA_ASSERT(gphy.isStopValid() == true);
    res = gphy.getSelectAreaPhy(w, h);
    CA_ASSERT(res == true);
    CA_ASSERT(w == 360);
    CA_ASSERT(h == 160);
    res = gphy.getSelectStartPhy(w, h);
    CA_ASSERT(res == true);
    CA_ASSERT(w == 240);
    CA_ASSERT(h == 320);
    gphy.unselect();
    CA_ASSERT(gphy.isStartValid() == false);
    CA_ASSERT(gphy.isStopValid() == false);
    res = gphy.getSelectAreaPhy(w, h);
    CA_ASSERT(res == true);
    CA_ASSERT(w == 0);
    CA_ASSERT(h == 0);
}


void test_viewglue::test4(void)
{
    _START();
    _INFO("Verify of internal class nnViewGlue: method  mouse event");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnViewGlue gphy(&mn,nullptr,nullptr);
    miniXmlNode  node(X("PHY_MAP"));
    node.add(X("TYPE"), 1);
    node.add(X("X"), 24);
    node.add(X("Y"), 32);
    node.add(X("HEIGHT"), 600);
    node.add(X("WIDTH"), 1200);
    bool res = false;
    try {
        res = gphy.readConfiguration(&node);
    }
    catch (...) {}
    CA_ASSERT(res == false);
    nn_mouse_buttons btn = nn_mouse_buttons::nn_m_button_left;
    nnPoint p(200, 140);
    nnPoint start,stop;
    res = gphy.handlerMouseButtonDown(btn, p);
    CA_ASSERT(res == true);
    p.set(250, 160);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    p.set(280, 170);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    p.set(210, 120);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    p.set(340, 400);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    p.set(330, 410);
    res = gphy.handlerMouseButtonUp(btn, p);
    CA_ASSERT(res == true);
    CA_ASSERT(gphy.isStartValid() == true);
    CA_ASSERT(gphy.isStopValid() == true);
    CA_ASSERT(gphy.getSelectArea(start, stop) == true);
    CA_ASSERT(start.isValid() == true);
    CA_ASSERT(stop.isValid() == true);
    CA_ASSERT(start < stop);
}


void test_viewglue::test5(void)
{
    _START();
    _INFO("Verify of internal class nnViewGlue: method  mouse event");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnObjManager mn(50, 20);
    nnViewGlue gphy(&mn,nullptr,nullptr);
    miniXmlNode  node(X("PHY_MAP"));
    node.add(X("TYPE"), 1);
    node.add(X("X"), 24);
    node.add(X("Y"), 32);
    node.add(X("HEIGHT"), 600);
    node.add(X("WIDTH"), 1200);
    bool res = false;
    try {
        res = gphy.readConfiguration(&node);
    }
    catch (...) {}
    CA_ASSERT(res == false);
    nn_mouse_buttons btn = nn_mouse_buttons::nn_m_button_left;
    nnPoint p(200, 140);
    nnPoint start,stop;
    res = gphy.handlerMouseButtonDown(btn, p);
    CA_ASSERT(res == true);
    p.set(250, 160);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    p.set(280, 170);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    p.set(210, 120);
    res = gphy.handlerMouseMove(btn, p);
    CA_ASSERT(res == true);
    res = gphy.handlerEscapeButton(false,false,false);
    CA_ASSERT(res == true);
    CA_ASSERT(gphy.isStartValid() == false);
    CA_ASSERT(gphy.isStopValid() == false);
}
