#include "stdio.h"
#include "CPPtester.h"
#include "n2logger.h"



class test_n2logger_class
    : public caTester
{
    CA_TEST_SUITE(test_n2logger_class);
    CA_TEST(test_n2logger_class::test1, "create");
    CA_TEST(test_n2logger_class::test2, "printer");
    CA_TEST(test_n2logger_class::test3, "logs");
    CA_TEST(test_n2logger_class::test4, "macro")
    CA_TEST_SUITE_END();
    void setUp(void);
    void tearDown(void);
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
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
    _INFO("verifica interna alla classe: set get printer");
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
    logger->log(1,"%s:%d >prova errore livello 1",__func__, __LINE__);
    logger->log(2,"%s:%d >prova errore livello 2",__func__, __LINE__);
    logger->log(3,"%s:%d >prova errore livello 3",__func__, __LINE__);
    logger->log(4,"%s:%d >prova errore livello 4",__func__, __LINE__);
    logger->log(5,"%s:%d >prova errore livello 5",__func__, __LINE__);
    logger->log(6,"%s:%d >prova errore livello 6",__func__, __LINE__);
    logger->log(7,"%s:%d >prova errore livello 7",__func__, __LINE__);
    logger->log(8,"%s:%d >prova errore livello 8",__func__, __LINE__);
    logger->log(9,"%s:%d >prova errore livello 9",__func__, __LINE__);
    logger->log(10,"%s:%d >prova errore livello 10",__func__, __LINE__);
    sleep(1);
}


void test_n2logger_class::test4(void)
{
    _START();
    _INFO("verifica interna alla classe: macro");
    _AUTHOR("Coppi Angelo n2 logger");
    _STOP();
    ILogger *logger = new nnLogger();
    CA_ASSERT(logger!=nullptr);
    IPrinter *printer = new nnDefaultPrinter();
    logger->setOutput(printer);
    CA_ASSERT(printer==logger->output());
    Log0("%s:%d >prova errore livello 0",__func__, __LINE__);
    Log1("%s:%d >prova errore livello 1",__func__, __LINE__);
    Log2("%s:%d >prova errore livello 2",__func__, __LINE__);
    Log3("%s:%d >prova errore livello 3",__func__, __LINE__);
    Log4("%s:%d >prova errore livello 4",__func__, __LINE__);
    Log5("%s:%d >prova errore livello 5",__func__, __LINE__);
    Log6("%s:%d >prova errore livello 6",__func__, __LINE__);
    sleep(1);
}
