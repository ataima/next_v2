#include <stdio.h>
#include <string>
#include "CPPtester.h"
#include "miniXml.h"


///////////////////// HEADER //////////////////////

class test_miniXml_class
	: public caTester
{
	CA_TEST_SUITE(test_miniXml_class)
	CA_TEST(test_miniXml_class::test1, "xlnode");
	CA_TEST(test_miniXml_class::test2, "xlnode");
	CA_TEST(test_miniXml_class::test3, "xlnode");
	CA_TEST_SUITE_END()
	void setUp(void);
	void tearDown(void);
	void test1(void);
	void test2(void);
	void test3(void);
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

	miniXmlNode root("ROOT", (char*)"");
	miniXmlNode* child1=root.add("child1", (char*)"1000");
	miniXmlNode* child2=child1->add("child2", (char*)"2000");
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

	miniXmlNode root("ROOT", (char*)"");
	miniXmlNode* child1 = root.add("child1",(char*) "1000");
	miniXmlNode* child2 = root.add("child2", (char*)"2000");
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
        const char *xml="<ROOT>\r\n<child1>\r\n1000\r\n</child1>\r\n<child2>\r\n2000\r\n</child2>\r\n</ROOT>\r\n";
        FILE *f=fopen(".\\test3.xml","w+");
        fwrite(xml,1,strlen(xml),f);
        fclose(f);
	miniXmlNode root("",(char *)"");
	root.load(".\\test3.xml",&root);
        root.print(stdout);
}

