#include "stdio.h"
#include "CPPtester.h"
#include <ctype.h>

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

// macro for create test suite in static mode an set any variable requested

     caTester* caContainerTest::allTest[MAXNUMCLASSTEST];
     int caContainerTest::numTest=0;
     int caContainerTest::maxTest=0;
     caContainerTest *caContainerTest::instance=NULL;
     int docFile::numfile=0;
     int caContainerTest::totalAssert=0;



// display usage mode when a error occurs on execute the program
void info(void)
{
    fprintf(stderr,"<your test program name> : \n");
    fprintf(stderr," - execute all tests included inside program  and display result on std out\n");
    fprintf(stderr,"<your test program name> : <fileout>\n [opz]");
    fprintf(stderr," - execute all tests included inside program  and save result on <fileout>.<ext>\n");
    fprintf(stderr,"Avaiable options :\n");
    fprintf(stderr,"  -f <ext> where avaiable <ext> are :\n");
    fprintf(stderr,"     - std or 1: as stdout\n");
    fprintf(stderr,"     - txt or 2: as text file \n");
    fprintf(stderr,"     - xml or 4: as xml file\n");
    fprintf(stderr,"     - htm or 8: as htm file\n");
    fprintf(stderr,"     - ORed nums are okey: ex. 9 = htm+stdout\n");
    fprintf(stderr,"  -t nn\n");
    fprintf(stderr,"     - execute only  test nn \n");
    fprintf(stderr,"     - you can insert more than one opt -t nn ex. -t 1 -t 4 -t 19 (max 255 tests)\n");
    fprintf(stderr,"     - you must insert in increasing order the number of test!!\n");
    fprintf(stderr,"  -c nn\n");
    fprintf(stderr,"     - execute only class test nn \n");
    fprintf(stderr,"  -n <name of test>\n");
    fprintf(stderr,"  -r \n");
    fprintf(stderr,"     - execute only register class  \n");
    fprintf(stderr,"  - options are'nt positional\n");
    fflush(stderr);
    exit(-1);
}

// decode the test request from user
// param 1 -t or -tnn (only in one string !)
// param 2 a numeric string if param 1 is -t only
int selectTest(const char* opt,const char* aux, int * n)
{
    int res=-1;
    *n=0;
    if (opt==NULL)
        return res;
    if (opt[2]==0 && aux==NULL)
        return res;
    else if (opt[2]==0 && aux!=NULL)
    {
        if (::isdigit(aux[0]))
            res=::atoi(aux);
        *n=1;
        return res;
    }
    else if (opt[2]!=0 )
    {
        const char *t=opt+(2*sizeof(char));
        if (::isdigit(t[0]))
            res=::atoi(t);
        return res;
    }
    return res;
}

// decode the format request
// param 1 -f or -fnn -fxxx(only in one string !)
// param 2 a string or numeric string if param 1 is -f only
int selectFormat( const char* opt)
{
    char _opt[255];
    int i;
    const char *frm[]= {"std","txt","xml","htm"};
    int res=0;
    if (opt==NULL)return 0;
    res=(int)strlen(opt);
    if (res==0)return 0;
    i=0;
    if (opt[i]=='-')i++;
    for (; i<res; i++)
        _opt[i]=(char )tolower(opt[i]);
    _opt[i]='\0';
    res=1;
    for ( i=0; i<4; i++)
    {
        if (strcmp(frm[i],_opt)==0)
            return res;
        res*=2;
    }
    return 0;
}

// opt -f & -t
int configure(int argc, const char* argv[],int *format,char *namefile,char *name,int *numTestReq,int *numFamily,int *tests,int sizeBuff)
{
    int res;
    int i=1;
    int n;
    const char *ptr;
    *format=0;
    *numTestReq=0;
    *numFamily=-1;
    bool reg=false;
    memset(tests,0,sizeBuff*sizeof(int));
    memset(name,0,sizeBuff*sizeof(char));
    memset(namefile,0,sizeBuff*sizeof(char));
    if (argc !=1)
    {
        while (i<argc)
        {
            ptr=argv[i];
            i++;
            if (ptr[0]=='-')
            {
                char t = ptr[1];
                if (t==0 && (i+1)<argc)
                {
                    //-  t  nn
                    i++;
                    ptr=argv[i];
                    t=ptr[0];
                }
                else if (t==0 && (i+1)==argc)
                    return 0;
                else
                    switch (t)
                    {
                    case 'c':
                        *numFamily=selectTest(ptr,i<argc?argv[i]:NULL,&n);
                        if (*numFamily==-1)return 0;
                        i+=n;
                        break;
                        //-  t  nn
                    case 't':
                        res=selectTest(ptr,i<argc?argv[i]:NULL,&n);
                        if (res==-1)return 0;
                        tests[*numTestReq]=res;
                        (*numTestReq)++;
                        i+=n;
                        break;
                    case 'f':
                        res = selectFormat((char *)(ptr));
                        if (res==0)
                            res =selectFormat((char *)(ptr+2*sizeof(char)));
                        if (res==0)
                            res =selectFormat(i<argc?argv[i]:NULL);
                        if (res!=0)
                        {
                            i++;
                            *format=res;
                        }
                        else
                        {
                            res=selectTest(ptr,i<argc?argv[i]:NULL,&n);
                            if (res==-1 || res>15)return 0;
                            *format=res;
                            i+=n;
                        }
                        break;
                    case 'n':
                        if (ptr[2]=='\0' && i<argc)
                        {
                            strcpy(name,argv[i]);
                            i++;
                        }
                        else if (ptr[2]=='\0' && i==argc)
                            return 0;
                        else
                            strcpy(name,&ptr[2]);
                        break;
                    case 'r':
                        if (ptr[2]=='\0' && i<argc)
                        {
                            reg=true;
                            i++;
                        }
                        else
                            reg=true;
                        break;
                    default:
                        return 0;
                        break;
                    }
            }
            else
                strncpy(namefile,ptr,255);
        }
    }
    if (*format==0)
        *format=1;
    if (reg==true)
        return -1;
    return 1;
}


int
#ifdef _MSC_VER
_cdecl
#endif
main(int argc, const char* argv[])
{
    int format;
    char buff[255];
    char nameS[255];
    int numTestReq;
    int numFamily;
    int tests[255];
    int res;
    int result;
    int i;
    res=configure(argc,argv,&format,buff,nameS,&numTestReq,&numFamily,tests,255);
    if (res==0)
        info();
    i=result=0;
    if (caContainerTest::Instance()->RegisterTest(nameS,buff,format,tests,&numTestReq,numFamily)!=0)
    {
        if (res>0)
        {
            if (caContainerTest::Instance()->RunTest(tests[i])!=0)result=-1;
            caContainerTest::Instance()->FinalizeTest();
        }
    }
#ifdef _MSC_VER
    system("pause");
#endif
    return result;
}

#define SELFTEST
#ifdef SELFTEST

///////////////////// HEADER //////////////////////
class tester_mainApp_class
    :public caTester
{
    CA_TEST_SUITE(tester_mainApp_class);
    CA_TEST(tester_mainApp_class::test1,"");
    CA_TEST(tester_mainApp_class::test2,"");
    CA_TEST(tester_mainApp_class::test3,"");
    CA_TEST(tester_mainApp_class::test4,"");
    CA_TEST(tester_mainApp_class::test5,"");
    CA_TEST(tester_mainApp_class::test6,"");
    CA_TEST(tester_mainApp_class::test7,"");
    CA_TEST(tester_mainApp_class::test8,"");
    CA_TEST(tester_mainApp_class::test9,"");
    CA_TEST(tester_mainApp_class::test10,"");
    CA_TEST(tester_mainApp_class::test11,"");
    CA_TEST(tester_mainApp_class::test12,"");
    CA_TEST(tester_mainApp_class::test13,"");
    CA_TEST(tester_mainApp_class::test14,"");
    CA_TEST(tester_mainApp_class::test15,"");
    CA_TEST(tester_mainApp_class::test16,"");
    CA_TEST(tester_mainApp_class::test17,"");
    CA_TEST(tester_mainApp_class::test18,"");
    CA_TEST(tester_mainApp_class::test19,"");
    CA_TEST(tester_mainApp_class::test20,"");
    CA_TEST(tester_mainApp_class::test21,"");
    CA_TEST(tester_mainApp_class::test22,"");
    CA_TEST(tester_mainApp_class::test23,"");
    CA_TEST(tester_mainApp_class::test24,"");
    CA_TEST(tester_mainApp_class::test25,"");
    CA_TEST(tester_mainApp_class::test26,"");
    CA_TEST(tester_mainApp_class::test27,"");
    CA_TEST(tester_mainApp_class::test28,"");
    CA_TEST(tester_mainApp_class::test29,"");
    CA_TEST(tester_mainApp_class::test30,"");
    CA_TEST(tester_mainApp_class::test31,"");
    CA_TEST(tester_mainApp_class::test32,"");
    CA_TEST(tester_mainApp_class::test33,"");
    CA_TEST(tester_mainApp_class::test34,"");
    CA_TEST(tester_mainApp_class::test35,"");
    CA_TEST(tester_mainApp_class::test36,"");
    CA_TEST(tester_mainApp_class::test37,"");
    CA_TEST(tester_mainApp_class::test38,"");
    CA_TEST(tester_mainApp_class::test39,"");
    CA_TEST(tester_mainApp_class::test40,"");
    CA_TEST(tester_mainApp_class::test41,"");
    CA_TEST(tester_mainApp_class::test42,"");
    CA_TEST(tester_mainApp_class::test43,"");
    CA_TEST(tester_mainApp_class::test44,"");
    CA_TEST(tester_mainApp_class::test45,"");
    CA_TEST(tester_mainApp_class::test46,"");
    CA_TEST(tester_mainApp_class::test47,"");
    CA_TEST(tester_mainApp_class::test48,"");
    CA_TEST(tester_mainApp_class::test49,"");
    CA_TEST(tester_mainApp_class::test50,"");
    CA_TEST(tester_mainApp_class::test51,"");
    CA_TEST(tester_mainApp_class::test52,"");
    CA_TEST(tester_mainApp_class::test53,"");
    CA_TEST(tester_mainApp_class::test54,"");
    CA_TEST(tester_mainApp_class::test55,"");
    CA_TEST(tester_mainApp_class::test56,"");
    CA_TEST(tester_mainApp_class::test57,"");
    CA_TEST(tester_mainApp_class::test58,"");
    CA_TEST(tester_mainApp_class::test59,"family param -c xx");
    CA_TEST(tester_mainApp_class::test60,"family param -cxx" );
    CA_TEST(tester_mainApp_class::test61,"register param -r" );
    CA_TEST(tester_mainApp_class::test62,"select test verifica se arg inizia con digit" );
    CA_TEST_SUITE_END();
    void setUp(void);
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
    void test12(void);
    void test13(void);
    void test14(void);
    void test15(void);
    void test16(void);
    void test17(void);
    void test18(void);
    void test19(void);
    void test20(void);
    void test21(void);
    void test22(void);
    void test23(void);
    void test24(void);
    void test25(void);
    void test26(void);
    void test27(void);
    void test28(void);
    void test29(void);
    void test30(void);
    void test31(void);
    void test32(void);
    void test33(void);
    void test34(void);
    void test35(void);
    void test36(void);
    void test37(void);
    void test38(void);
    void test39(void);
    void test40(void);
    void test41(void);
    void test42(void);
    void test43(void);
    void test44(void);
    void test45(void);
    void test46(void);
    void test47(void);
    void test48(void);
    void test49(void);
    void test50(void);
    void test51(void);
    void test52(void);
    void test53(void);
    void test54(void);
    void test55(void);
    void test56(void);
    void test57(void);
    void test58(void);
    void test59(void);
    void test60(void);
    void test61(void);
    void test62(void);
    void tearDown(void);
};

REGISTER_CLASS(tester_mainApp_class);

void tester_mainApp_class::setUp(void)
{
#ifdef _SAFE_MEMORY_LEAK
    START_RECORD_MEM(stderr);
#endif
}


void tester_mainApp_class::tearDown(void)
{
#ifdef _SAFE_MEMORY_LEAK
    int res=GET_LEAK_NUM();
    END_RECORD_MEM();
    CA_ASSERT_M("Warning memory leaks !",res==0);
#endif
}

void tester_mainApp_class::test1(void)
{
    int n;
    const char opt[]= {"-t12"};
    int res=selectTest(opt,NULL,&n);
    CA_ASSERT(res==12);
    CA_ASSERT(n==0);
}

void tester_mainApp_class::test2(void)
{
    const char opt[]= {"-t"};
    const char aux[]= {"23"};
    int n;
    int res=selectTest(opt,aux,&n);
    CA_ASSERT(res==23);
    CA_ASSERT(n==1);
}

void tester_mainApp_class::test3(void)
{
    int res=selectFormat(NULL);
    CA_ASSERT(res==0);
}

void tester_mainApp_class::test4(void)
{
    char buff[256];
    buff[0]=0;
    int res=selectFormat(buff);
    CA_ASSERT(res==0);
}

void tester_mainApp_class::test5(void)
{
    char buff[]="std";
    int res=selectFormat(buff);
    CA_ASSERT(res==1);
}

void tester_mainApp_class::test6(void)
{
    char buff[]="txt";
    int res=selectFormat(buff);
    CA_ASSERT(res==2);
}

void tester_mainApp_class::test7(void)
{
    char buff[]="tcs";
    int res=selectFormat(buff);
    CA_ASSERT(res==0);
}

void tester_mainApp_class::test8(void)
{
    char buff[]="xml";
    int res=selectFormat(buff);
    CA_ASSERT(res==4);
}

void tester_mainApp_class::test9(void)
{
    char buff[]="htm";
    int res=selectFormat(buff);
    CA_ASSERT(res==8);
}

void tester_mainApp_class::test10(void)
{
    char buff[255];
    char names[255];
    int t[255];
    int nt;
    int format;
    int nf;
    int res=configure(1,NULL,&format,names,buff,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(nf==-1);
}


void tester_mainApp_class::test11(void)
{
    const char *argv[]= {"pippo","out"};
    char buff[256];
    char buff1[256];
    int t[255];
    int nt;
    int format;
    int nf;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(nf==-1);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test12(void)
{
    const char *argv[]= {"pippo","out","-t14"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt;
    int format;
    int nf;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==1);
    CA_ASSERT(nf==-1);
    CA_ASSERT(t[0]==14);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test13(void)
{
    const char *argv[]= {"pippo","out","-t","14"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt;
    int format;
    int nf;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==1);
    CA_ASSERT(nf==-1);
    CA_ASSERT(t[0]==14);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test14(void)
{
    const char *argv[]= {"pippo","out","-t"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==0);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test15(void)
{
    const char *_argv[]= {"pippo","out","-fstd"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,_argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(_argv[1],buff)==0);
}

void tester_mainApp_class::test16(void)
{
    const char *argv[]= {"pippo","out","-f","std"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test17(void)
{
    const char *argv[]= {"pippo","out","-f1"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test18(void)
{
    const char *argv[]= {"pippo","out","-f","1"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test19(void)
{
    const char *argv[]= {"pippo","out","-ftxt"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test20(void)
{
    const char *argv[]= {"pippo","out","-f","txt"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test21(void)
{
    const char *argv[]= {"pippo","out","-f2"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test22(void)
{
    const char *argv[]= {"pippo","out","-f","2"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test23(void)
{
    const char *argv[]= {"pippo","out","-fxml"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test24(void)
{
    const char *argv[]= {"pippo","out","-f","xml"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test25(void)
{
    const char *argv[]= {"pippo","out","-f4"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test26(void)
{
    const char *argv[]= {"pippo","out","-f","4"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test27(void)
{
    const char *argv[]= {"pippo","out","-fhtm"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test28(void)
{
    const char *argv[]= {"pippo","out","-f","htm"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test29(void)
{
    const char *argv[]= {"pippo","out","-f8"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test30(void)
{
    const char *argv[]= {"pippo","out","-f","8"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test31(void)
{
    const char *argv[]= {"pippo","out","-f","25"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(4,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==0);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test32(void)
{
    const char *argv[]= {"pippo","out","-f16"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==0);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(argv[1],buff)==0);
}

void tester_mainApp_class::test33(void)
{
    const char *argv[]= {"pippo"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(1,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test34(void)
{
    const char *argv[]= {"pippo","-t14"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==1);
    CA_ASSERT(t[0]==14);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test35(void)
{
    const char *argv[]= {"pippo","-t","14"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==1);
    CA_ASSERT(t[0]==14);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test36(void)
{
    const char *argv[]= {"pippo","-t"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==0);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test37(void)
{
    const char *argv[]= {"pippo","-fstd"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test38(void)
{
    const char *argv[]= {"pippo","-f","std"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test39(void)
{
    const char *argv[]= {"pippo","-f1"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test40(void)
{
    const char *argv[]= {"pippo","-f","1"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==1);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test41(void)
{
    const char *argv[]= {"pippo","-ftxt"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test42(void)
{
    const char *argv[]= {"pippo","-f","txt"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test43(void)
{
    const char *argv[]= {"pippo","-f2"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test44(void)
{
    const char *argv[]= {"pippo","-f","2"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==2);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test45(void)
{
    const char *argv[]= {"pippo","-fxml"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test46(void)
{
    const char *argv[]= {"pippo","-f","xml"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test47(void)
{
    const char *argv[]= {"pippo","-f4"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test48(void)
{
    const char *argv[]= {"pippo","-f","4"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==4);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test49(void)
{
    const char *argv[]= {"pippo","-fhtm"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test50(void)
{
    const char *argv[]= {"pippo","-f","htm"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test51(void)
{
    const char *argv[]= {"pippo","-f8"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test52(void)
{
    const char *argv[]= {"pippo","-f","8"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==8);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test53(void)
{
    const char *argv[]= {"pippo","-f","25"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==0);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}

void tester_mainApp_class::test54(void)
{
    const char *argv[]= {"pippo","-f16"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(2,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==0);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(buff[0]==0);
}


void tester_mainApp_class::test55(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(3,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==0);
    CA_ASSERT(t[0]==0);
    CA_ASSERT(strcmp(buff,"out")==0);
}


void tester_mainApp_class::test56(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7","-nprova"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(6,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==1);
    CA_ASSERT(t[0]==7);
    CA_ASSERT(strcmp(buff,"out")==0);
    CA_ASSERT(strcmp(buff1,"prova")==0);
}

void tester_mainApp_class::test57(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7","-n","prova"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(7,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==1);
    CA_ASSERT(t[0]==7);
    CA_ASSERT(strcmp(buff,"out")==0);
    CA_ASSERT(strcmp(buff1,"prova")==0);
}

void tester_mainApp_class::test58(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7","-n"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(6,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==0);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==1);
    CA_ASSERT(t[0]==7);
    CA_ASSERT(strcmp(buff,"out")==0);
    CA_ASSERT(strcmp(buff1,"")==0);
}


void tester_mainApp_class::test59(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7","-n","prova","-c","20"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(9,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==1);
    CA_ASSERT(nf==20);
    CA_ASSERT(t[0]==7);
    CA_ASSERT(strcmp(buff,"out")==0);
    CA_ASSERT(strcmp(buff1,"prova")==0);
}

void tester_mainApp_class::test60(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7","-n","prova","-c20"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(8,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==1);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==1);
    CA_ASSERT(nf==20);
    CA_ASSERT(t[0]==7);
    CA_ASSERT(strcmp(buff,"out")==0);
    CA_ASSERT(strcmp(buff1,"prova")==0);
}

void tester_mainApp_class::test61(void)
{
    const char *argv[]= {"pippo","-f9","out","-t","7","-n","prova","-c20","-r"};
    char buff[255];
    char buff1[255];
    int t[255];
    int nt,nf;
    int format;
    int res=configure(9,argv,&format,buff,buff1,&nt,&nf,t,255);
    CA_ASSERT(res==-1);
    CA_ASSERT(format==9);
    CA_ASSERT(nt==1);
    CA_ASSERT(nf==20);
    CA_ASSERT(t[0]==7);
    CA_ASSERT(strcmp(buff,"out")==0);
    CA_ASSERT(strcmp(buff1,"prova")==0);
}



void tester_mainApp_class::test62(void)
{
    int n;
    const char opt[]= {"-ta12"};
    int res=selectTest(opt,NULL,&n);
    CA_ASSERT(res==-1);
    CA_ASSERT(n==0);
}

#endif

