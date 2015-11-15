#include "stdio.h"
#include "CPPtester.h"
#include "windows.h"
#include "miniXml.h"


///////////////////// HEADER //////////////////////

class test_miniXml_class
	: public caTester
{
	CA_TEST_SUITE(test_miniXml_class)
	CA_TEST(test_miniXml_class::test1, "xlnode");
	CA_TEST(test_miniXml_class::test2, "xlnode");
	CA_TEST_SUITE_END()
	void setUp(void);
	void tearDown(void);
	void test1(void);
	void test2(void);
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