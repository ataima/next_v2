#include <stdio.h>
#include <string>
#include "CPPtester.h"
#include "miniXml.h"


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

class test_miniXml_class
    : public caTester
{
    CA_TEST_SUITE(test_miniXml_class)
    CA_TEST(test_miniXml_class::test1, "xlnode");
    CA_TEST(test_miniXml_class::test2, "xlnode");
    CA_TEST(test_miniXml_class::test3, "xlnode");
    CA_TEST(test_miniXml_class::test4, "xlnode");
    CA_TEST(test_miniXml_class::test5, "xlnode");
    CA_TEST(test_miniXml_class::test6, "xlnode");
    CA_TEST_SUITE_END()
    void setUp(void);
    void tearDown(void);
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_miniXml_class);

void test_miniXml_class::setUp(void)
{

}

void test_miniXml_class::tearDown(void)
{

}

void test_miniXml_class::test1(void)
{
    _START();
    _INFO("create XmlNode");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();

    miniXmlNode root(X("ROOT"), (XCHAR*)X(""));
    miniXmlNode* child1=root.add(X("child1"), (XCHAR*)X("1000"));
    miniXmlNode* child2=child1->add(X("child2"), (XCHAR*)X("2000"));
    root.print(stdout);
    CA_ASSERT(root.getParent() == nullptr);
    CA_ASSERT(root.getChild() == child1);
    CA_ASSERT(root.getNext() == nullptr);
    CA_ASSERT(child1->getChild() == child2);
}

void test_miniXml_class::test2(void)
{
    _START();
    _INFO("create XmlNode");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();

    miniXmlNode root(X("ROOT"), (XCHAR*)"");
    miniXmlNode* child1 = root.add(X("child1"),(XCHAR*) X("1000"));
    miniXmlNode* child2 = root.add(X("child2"), (XCHAR*)X("2000"));
    root.print(stdout);
    CA_ASSERT(root.getParent() == nullptr);
    CA_ASSERT(root.getChild() == child1);
    CA_ASSERT(root.getNext() == nullptr);
    CA_ASSERT(child1->getNext() == child2);
}



void test_miniXml_class::test3(void)
{
    _START();
    _INFO("parse XmlNode");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
        remove(".\\test3.xml");
        const XCHAR *xml=X("<ROOT>\r\n<child1>\r\n1000\r\n</child1>\r\n<child2>\r\n2000\r\n</child2>\r\n</ROOT>\r\n");
        FILE *f=fopen(".\\test3.xml","w+");
        fwrite(xml,sizeof(XCHAR),STRLEN(xml),f);
        fclose(f);
    miniXmlNode root(X(""),(XCHAR *)"");
    root.load(X(".\\test3.xml"),&root);
    root.print(stdout);
    remove(".\\test3.xml");
    CA_ASSERT(root.find(X("child1")) != NULL);
    CA_ASSERT(root.find(X("child2")) != NULL);
}

void test_miniXml_class::test4(void)
{
    _START();
    _INFO("create XmlNode and parse");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();

    miniXmlNode root(X("ROOT"), (XCHAR*)"");
    miniXmlNode* child1 = root.add(X("child1"), (XCHAR*)X("1000"));
    miniXmlNode* child2 = root.add(X("child2"), (XCHAR*)X("2000"));
    miniXmlNode* child3 = root.add(X("child3"), (XCHAR*)X("3000"));
    miniXmlNode* child4 = root.add(X("child4"), (XCHAR*)X("4000"));
    miniXmlNode* child5 = root.add(X("child5"), (XCHAR*)X("5000"));
    CA_ASSERT(root.getParent() == nullptr);
    CA_ASSERT(root.getChild() == child1);
    CA_ASSERT(root.getNext() == nullptr);
    CA_ASSERT(child1->getNext() == child2);
    root.print(stdout);
    remove(".\\test4.xml");
    FILE *f = fopen(".\\test4.xml", "w+");
    root.print(f);
    fclose(f);
    miniXmlNode root1;
    root1.load(X(".\\test4.xml"), &root1);
    root1.print(stdout);
    remove(".\\test4.xml");
    CA_ASSERT(root1.find(X("child1")) != NULL);
    CA_ASSERT(root1.find(X("child2")) != NULL);
    CA_ASSERT(root1.find(X("child3")) != NULL);
    CA_ASSERT(root1.find(X("child4")) != NULL);
    CA_ASSERT(root1.find(X("child5")) != NULL);
}

void test_miniXml_class::test5(void)
{
    _START();
    _INFO("create XmlNode and parse");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    miniXmlNode root(X("ROOT"), (XCHAR*)"");
    miniXmlNode* child1 = root.add(X("child1"), (XCHAR*)X("1000"));
    miniXmlNode* child2 = root.add(X("child1.child2"), (XCHAR*)X("2000"));
    miniXmlNode* child3 = root.add(X("child1.child2.child3"), (XCHAR*)X("3000"));
    miniXmlNode* child4 = root.add(X("child1.child2.child3.child4"), (XCHAR*)X("4000"));
    miniXmlNode* child5 = root.add(X("child1.child2.child3.child4.child5"), (XCHAR*)X("5000"));
    CA_ASSERT(root.getParent() == nullptr);
    CA_ASSERT(root.getChild() == child1);
    CA_ASSERT(root.getNext() == nullptr);
    CA_ASSERT(child1->getNext() == nullptr);
    CA_ASSERT(child1->getChild() == child2);
    root.print(stdout);
    remove(".\\test5.xml");
    FILE *f = fopen(".\\test5.xml", "w+");
    root.print(f);
    fclose(f);
    miniXmlNode root1(X(""), (XCHAR*)X(""));
    root1.load(X(".\\test5.xml"), &root1);
    root1.print(stdout);
    remove(".\\test5.xml");
    CA_ASSERT(root1.find(X("child1")) != NULL);
    CA_ASSERT(root1.find(X("child1.child2")) != NULL);
    CA_ASSERT(root1.find(X("child1.child2.child3")) != NULL);
    CA_ASSERT(root1.find(X("child1.child2.child3.child4")) != NULL);
    CA_ASSERT(root1.find(X("child1.child2.child3.child4.child5")) != NULL);
}


void test_miniXml_class::test6(void)
{
    _START();
    _INFO("create XmlNode and parse");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();

    miniXmlNode root(X("ROOT"), (XCHAR*)X("1000"));
    miniXmlNode* child1 = root.add(X("child1"), (XCHAR*)X("1000"));
    miniXmlNode* child2 = root.add(X("child2"), (XCHAR*)X("2000"));
    CA_ASSERT(root.getParent() == nullptr);
    CA_ASSERT(root.getChild() == child1);
    CA_ASSERT(root.getNext() == nullptr);
    CA_ASSERT(child1->getNext() == child2);
    root.print(stdout);
    remove(".\\test6.xml");
    FILE *f = fopen(".\\test6.xml", "w+");
    root.print(f);
    fclose(f);
    miniXmlNode root1;
    root1.load(X(".\\test6.xml"), &root1);
    root1.print(stdout);
    remove(".\\test6.xml");
    CA_ASSERT(STRCMP(root1.getValue(),X("1000")) ==0);
    CA_ASSERT(root1.find(X("child1")) != NULL);
	CA_ASSERT(root1.find(X("child1"))->getLong() ==1000);
	CA_ASSERT(root1.find(X("child2")) != NULL);
	CA_ASSERT(root1.find(X("child2"))->getLong() == 2000);
}
