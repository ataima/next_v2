#include "stdio.h"
#include "CPPtester.h"
#include "n2logger.h"



class test_n2logger_class
    : public caTester
{
    CA_TEST_SUITE(test_n2logger_class);
    CA_TEST(test_n2logger_class::test1, "create");
    CA_TEST(test_n2logger_class::test2, "printer");
    CA_TEST(test_n2logger_class::test3, "logE");
    CA_TEST_SUITE_END();
    void setUp(void);
    void tearDown(void);
    void test1(void);
    void test2(void);
    void test3(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_n2logger_class);



void test_n2logger_class::setUp(void)
{
}

void test_n2logger_class::tearDown(void)
{
}

void test_n2logger_class::test1(void)
{
    _START();
    _INFO("verifica interna alla classe: ctor");
    _AUTHOR("Coppi Angelo n2 logger");
    _STOP();
    ILogger *logger = new nnLogger();
    CA_ASSERT(logger!=nullptr);
    CA_ASSERT(logger=ILogger::getInstance());
}

void test_n2logger_class::test2(void)
{
    _START();
    _INFO("verifica interna alla classe: ste get printer");
    _AUTHOR("Coppi Angelo n2 logger");
    _STOP();
    ILogger *logger = new nnLogger();
    CA_ASSERT(logger!=nullptr);
    IPrinter *printer = new nnDefaultPrinter();
    logger->setOutput(printer);
    CA_ASSERT(printer==logger->output());

}

void test_n2logger_class::test3(void)
{
    _START();
    _INFO("verifica interna alla classe: logE");
    _AUTHOR("Coppi Angelo n2 logger");
    _STOP();
    ILogger *logger = new nnLogger();
    CA_ASSERT(logger!=nullptr);
    IPrinter *printer = new nnDefaultPrinter();
    logger->setOutput(printer);
    CA_ASSERT(printer==logger->output());
    logger->log(0,"%s:%d >prova errore livello 0",__func__, __LINE__);

}
