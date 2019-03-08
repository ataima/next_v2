#include "n2exthandler.h"
#include "n2exception.h"
#include <algorithm>
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





nnExtHandler::nnExtHandler(extHandler & _hook, void *unkObj, bool _pulse)
    :hook(_hook), unknow(unkObj),pulse(_pulse)
{}


void nnExtHandler::send(extHandler hook, void *unknow, size_t Tparam, IParam *in )
{
    //nnLOG1(size_t, Tparam);
    if(hook && unknow ) {
        try {
            hook(unknow,Tparam,in);
        } catch(...) {
            extHandlerException *e = new extHandlerException();
            throw(e);
        }
    }
}




void nnExtHandler::doHandler(size_t Tparam, IParam *in)
{
    if (hook && unknow) {
        if (pulse) {
            std::thread th(&nnExtHandler::send, hook, unknow, Tparam, in);
            th.detach();
        } else {
            try {
                hook(unknow, Tparam, in);
            } catch (...) {
                extHandlerException *e = new extHandlerException();
                throw(e);
            }
        }
    }
}