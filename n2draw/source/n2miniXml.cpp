#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale>
#include <string>
#include <stdarg.h>
#include "n2miniXml.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <n2utoatou.h>

#ifndef _MSC_VER
#include <unistd.h> 
#endif


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


/**************************************************************
 *WARNING !!!!!!!!!!!!!!!!!
 * TO COMPILE THIS FILE AND BUILD THE LIBRARY YOU MUST EDIT
 * XLOCNUM INCLUDE FILE BECAUSE ARE MISSING SOME DEFINITIONS
 * AND CL.EXE  RUTURN ERROR  (CLANG AND GCC WORK FINE ....)
 * INSERT THE FOLLOWING TEMPLATE DEFINITION ON LINE 1574
 * OF FILE XLOCNUM (INSTALLDIR MSVC1400/VC/INCLUDE/XLOCNUM
 *
 A.Coppi FIX!!!! char16_t, char32_t for error
C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\xlocnum:259: error: C2491: 'std::numpunct<_Elem>::id': definizione di dllimport membro dati statici non consentita
with
[
    _Elem=char16_t
]

template __PURE_APPDOMAIN_GLOBAL locale::id numpunct<char16_t>::id;
template class _CRTIMP2_PURE num_get<char16_t,
    istreambuf_iterator<char16_t, char_traits<char16_t> > >;
template class _CRTIMP2_PURE num_put<char16_t,
    ostreambuf_iterator<char16_t, char_traits<char16_t> > >;


template __PURE_APPDOMAIN_GLOBAL locale::id numpunct<char32_t>::id;
template class _CRTIMP2_PURE num_get<char32_t,
    istreambuf_iterator<char32_t, char_traits<char32_t> > >;
template class _CRTIMP2_PURE num_put<char32_t,
    ostreambuf_iterator<char32_t, char_traits<char32_t> > >;

 END FIX A:COPPI
********************************************************************/




miniXmlNode::miniXmlNode(const XCHAR  *_name, XCHAR  *_value,
    miniXmlNode *_parent, miniXmlNode *_child, miniXmlNode *_next)
    : parent(_parent),
    child(_child),
    next(_next)
{
    setName(_name);
    setValue(_value);
    if (parent != nullptr && parent->child == nullptr)
        parent->child = this;
    else if (parent != nullptr && parent->child != nullptr)
    {
        miniXmlNode *n = parent->child;
        miniXmlNode *t = parent->child;
        while (t != nullptr)
        {
            n = t;
            t = t->next;
        }
        n->next = this;
    }
}
/// default destructor

miniXmlNode::~miniXmlNode()
{
    clearNode();
}

void miniXmlNode::clearNode(void)
{
    if (name != nullptr)
        delete[]name;
    if (value != nullptr)
        delete[]value;
    // cancella tutta la catena
    if (child != nullptr)
        delete child;
    child = nullptr;
    if (next != nullptr)
        delete next;
    next = nullptr;
}


/// add a child node a this node. _name as
/// xPath for crossing from iinternal child nodes
/// path separator is assumend the point '.'

miniXmlNode *miniXmlNode::add(const XCHAR  *name, int idx, int value)
{
    //XCHAR  buff1[BUFFLENGTH];
    //XCHAR buff[BUFFLENGTH];
    //xsprintf(buff1, X("%s%d"), name, idx);
    //xsprintf(buff, X("%d"), value);
    SSTRING ss1;
    SSTRING ss2;
    ss1<<name<<idx;
    ss2<<value;
    return add(ss1.str().c_str(), const_cast<XCHAR *>(ss2.str().c_str()));
}


miniXmlNode * miniXmlNode::add(const XCHAR *name, int value)
{
    //XCHAR buff[BUFFLENGTH];
    if (name != nullptr)
    {
        SSTRING ss;
        ss<<value;
        //xsprintf(buff, X("%d"), value);
        return add(name, const_cast <XCHAR *>(ss.str().c_str()));
    }
    return nullptr;
}

miniXmlNode * miniXmlNode::add(const XCHAR *name, XCHAR *value)
{
    // name as Xpath = A.B.C. es test.suite.doc
    const XCHAR *ptr = STRSTR(name,STRLEN(name),X('.'));
    if (ptr == nullptr)
    {
        if (child == nullptr)
            return new miniXmlNode(name, value, this, nullptr, lastchild());
        else
        {
            miniXmlNode *it = child->find(name);
            if(it != nullptr && value==nullptr)
            {
                
                    return it;
            }
            else
                return new miniXmlNode(name, value, this, nullptr, nullptr);
        }
    }
    else
    {
        // has Xpath
        XCHAR buff[BUFFLENGTH];
        int offset = (int)(ptr - name);
        if (offset < BUFFLENGTH)
        {
            memcpy(buff, name, sizeof(XCHAR)*offset);
            buff[offset] = X('\0');
        }
        miniXmlNode *it = add(const_cast<const XCHAR *> (buff),nullptr);
        return it->add(++ptr, value);
    }
}
/// this function link at this node a child miniXmlNode

miniXmlNode * miniXmlNode::link(XCHAR *name, miniXmlNode *_child)
{
    miniXmlNode *res = find(name);
    if (res != nullptr)
    {
        if (res->child != nullptr)
            res->lastchild()->next = _child;
        else
            res->child = _child;
    }
    return res;
}
/// this function add a value at already existent value

miniXmlNode * miniXmlNode::cat(XCHAR * addValue)
{
    STRING s = value;
    s += X('\r');
    s += X('\n');
    s += addValue;
    delete[]value;
    value = new XCHAR[s.size() + 2];
    memcpy(value, s.c_str(), sizeof(XCHAR)*s.size());
    value[s.size()] = X('\0');
    return this;
}
/// inbox for value, return the current value

const XCHAR * miniXmlNode::getValue(void)
{
    return (const XCHAR *)value;
}


long miniXmlNode::getLong(void)
{
    UtoA toA(value);
    return ATOL(toA.utf8());
}
/// walking from child , return the child with has next field set a  nullptr pointer

miniXmlNode * miniXmlNode::lastchild(void)
{
    miniXmlNode* first = child;
    if (first != nullptr)
    {
        while (1)
        {
            miniXmlNode *next = first->next;
            if (next == nullptr)return first;
            first = next;
        }
    }
    return nullptr;
}
/// this function return a node with name as requested

miniXmlNode * miniXmlNode::findNextChild(const XCHAR *_name)
{
    miniXmlNode *res = nullptr;
    if (next != nullptr && next->child != nullptr)
        res = next->child->find(_name);
    return res;
}

miniXmlNode * miniXmlNode::findNext(const XCHAR *_name)
{
    miniXmlNode *res = nullptr;
    if (name != nullptr && STRCMP(name, _name,STRLEN(_name)) == 0)
    {
        res = this;
    }
    else 
        if (next != nullptr)
        res = next->findNext(_name);
    return res;
}

miniXmlNode *miniXmlNode::find(const XCHAR *name, int idx)
{
    //XCHAR buff1[BUFFLENGTH];
    SSTRING ss;
    //xsprintf(buff1, X("%s%d"), name, idx);
    ss<<name<<idx;
    return find(ss.str().c_str());
}

miniXmlNode * miniXmlNode::find(const XCHAR *_name)
{
    const XCHAR *ptr = STRSTR(_name,STRLEN(_name), X('.'));
    if (ptr == nullptr)
    {
        miniXmlNode *res = nullptr;
        if (name != nullptr && STRCMP(name, _name,STRLEN(_name)) == 0)
        {
            res = this;
            return res;
        }
        if (next != nullptr)
        {
            res = next->findNext(_name);
            if (res != nullptr)
                return res;
        }
        if (child != nullptr)
        {
            res = child->find(_name);
            if (res != nullptr)
                return res;
        }
        if (next != nullptr)
        {
            res = next->findNextChild(_name);
            if (res != nullptr)
                return res;
        }
        return nullptr;
    }
    else
    {
        // has Xpath
        XCHAR buff[BUFFLENGTH];
        int offset = (int)(ptr - _name);
        if (offset < BUFFLENGTH)
        {

            memcpy(buff, _name, sizeof(XCHAR)*offset);
            buff[offset] = X('\0');
        }
        miniXmlNode *it = find(buff);
        if (it != nullptr)
            return it->find(++ptr);
        else
            return nullptr;
    }

}
/// generate a XML file

void miniXmlNode::print(FILE *out)
{
    XCHAR buff[BUFFLENGTH];
    if (name != nullptr)
    {
#ifdef _UNICODE
        setlocale(LC_ALL, "");
#endif
        // name not set ->only container
        //xsprintf(buff, X("<%ws>\n"), name);
        SSTRING ss;
        ss<<X("<")<<name<<X(">\n");
        fwrite(ss.str().c_str(), ss.str().size(), sizeof(XCHAR), out);
        /* No replace
        if (value != nullptr)
        {
            const XCHAR * as[] = { " &lt; ", " &gt; ", " &amp; ", " &apos; ", " &quot; ", " " };
            int len = STRLEN(value);
            int i;
            for (i = 0; i < len; i++)
            {
                //	&lt;	<	less than
                //	&gt;	>	greater than
                //	&amp;	&	ampersand
                //	&apos;	'	apostrophe
                //	&quot;	"	quotation mark
                switch (value[i])
                {
                case '&':
                    fwrite(as[2], 7, sizeof(char), out);
                    break;
                case '\'':
                    fwrite(as[3], 8, sizeof(char), out);
                    break;
                case '"':
                    fwrite(as[4], 8, sizeof(char), out);
                    break;
                case '<':
                    fwrite(as[0], 6, sizeof(char), out);
                    break;
                case '>':
                    fwrite(as[1], 6, sizeof(char), out);
                    break;
                case '\r':
                case '\n':
                    fwrite(as[5], 1, sizeof(char), out);
                    break;
                default:
                    fwrite(&value[i], 1, sizeof(char), out);
                    break;
                }
            }
            fwrite("\n", 1, sizeof(char), out);
        }
        */
        if (value != nullptr  && STRLEN(value) > 0)
        {
            SSTRING ss;
            ss<<value<<X("\n");
            //xsprintf(buff, X("%ws\n"), value);
            fwrite(ss.str().c_str(), ss.str().size(), sizeof(XCHAR), out);
        }
    }
    if (child != nullptr)
        child->print(out);
    if (name != nullptr)
    {
        //xsprintf(buff, X("</%ws>\n"), name);
        SSTRING ss;
        ss<<X("<")<<name<<X(">\n");
        size_t len=BUFFLENGTH-1;
        if(ss.str().size()<len)
            len=ss.str().size();
        memcpy(buff,ss.str().c_str(),len);
        for (size_t i = 0; i < len; i++)
        {
            if (buff[i] == '>')
                break;
            if (buff[i] == ' ')
            {
                buff[i++] = '>';
                buff[i++] = '\n';
                buff[i] = 0;
                break;
            }
        }
        fwrite(buff,len, sizeof(XCHAR), out);
    }
    if (next != nullptr)
        next->print(out);
}

bool miniXmlNode::load(const XCHAR *file_in, miniXmlNode *root)
{
    bool res = false;
    if (root != nullptr)
    {
        miniXmlParse parser(file_in, root);
        res = parser.parse();
    }
    return res;
}


bool miniXmlNode::save(const XCHAR *out, miniXmlNode * root)
{
    bool res = false;
    if (out != nullptr && root != nullptr)
    {
        UtoA toA(out);
#ifdef _MSC_VER
        FILE *out = FOPEN(toA.utf8(), "w+");
#else
        FILE *out = FOPEN(toA.utf8(), "w+");
#endif
        if (out != nullptr)
        {
            root->print(out);
            fclose(out);
        }
    }
    return res;
}


/// assign a requested name
void miniXmlNode::setName(const XCHAR *_name)
{
    if (_name != nullptr)
    {
        size_t len = STRLEN(_name);
        name = new XCHAR[len + 1];
        memcpy(name, _name, sizeof(XCHAR)*len);
        name[len] = X('\0');
    }
    else
        name = nullptr;
}
/// assigne a requested value

void miniXmlNode::setValue(XCHAR *_value)
{
    if (_value != nullptr)
    {
        size_t len = STRLEN(_value);
        value = new XCHAR[len + 1];
        memcpy(value, _value, sizeof(XCHAR)*len);
        value[len] = X('\0');
    }
    else
        value = nullptr;
}


/*
int  miniXmlNode::xsprintf(XCHAR *buff, const XCHAR* format, ...)
{254
    int Result;
    va_list ArgList;
#ifdef _UNICODE        
#ifdef _MSC_VER
    __crt_va_start(ArgList, format);
    Result = _vsnwprintf_s_l(buff, BUFFLENGTH, BUFFLENGTH - 1, format, NULL, ArgList);
    __crt_va_end(ArgList);
#else
    va_start(ArgList, format);
    Result = vswprintf(buff, BUFFLENGTH, format, ArgList);
    va_end(ArgList);
#endif
#else
#ifdef _MSC_VER
    __crt_va_start(ArgList, format);
    Result = _vsnprintf_s_l(buff, BUFFLENGTH, BUFFLENGTH - 1, format, NULL, ArgList);
    __crt_va_end(ArgList);
#else
    va_start(ArgList, format);
    Result = vsnprintf(buff, BUFFLENGTH, format, ArgList);
    va_end(ArgList);
#endif
#endif
        return Result;
}
*/
bool swapNode(miniXmlNode *src, miniXmlNode* dst)
{
    bool res = false;
    if (src == dst)
        return true;
    if (src != nullptr && dst != nullptr)
    {
        dst->name = src->name;
        dst->value = src->value;
        dst->child = src->child;
        dst->next = src->next;
        dst->parent = src->parent;
        src->name = nullptr;
        src->value = nullptr;
        src->child = src->next = src->parent = nullptr;
        dst->child->parent = dst;
        miniXmlNode *next_node = dst->next;
        while (next_node != nullptr)
        {
            next_node->parent = dst;
            next_node = next_node->next;
        };
        res = true;
    }
    return res;
}




/////////////////////////file_out///////////////////////////////////////////////////////

miniXmlParse::miniXmlParse(const XCHAR *_in, miniXmlNode * _root)
{
    if (_in != nullptr && _root != nullptr)
    {
        root = _root;
        UtoA toA(_in);
#ifdef _MSC_VER
        FILE *in =FOPEN(toA.utf8(), "r+");
#else
        FILE *in = FOPEN(toA.utf8(), "r+");
#endif
        if (in != nullptr)
        {
            p_end = p_index = nullptr;
            struct stat s;
#ifdef _MSC_VER             
            if (fstat(_fileno(in), &s) == 0)
#else
            if (fstat(fileno(in), &s) == 0)
#endif                
            {
                buff = new XCHAR[s.st_size];
                if (buff != nullptr)
                {
                    max_size = fread(buff, 1, s.st_size, in);
                    if (max_size != 0)
                    {
                        max_size = s.st_size;
                        p_index = buff;
                        p_end = &buff[max_size];
                    }
                }
            }
            else
            {
                root = nullptr;
                buff = nullptr;
            }
            fclose(in);
        }
        else
        {
            root = nullptr;
            buff = nullptr;
        }
    }
    else
    {
        root = nullptr;
        buff = nullptr;
    }

}

miniXmlParse::~miniXmlParse()
{
    if (buff != nullptr)
        delete[]buff;
}

bool miniXmlParse::parse(void)
{

    bool res = false;
    if (buff != nullptr && max_size > 0 && root != nullptr)
    {
        miniXmlNode *current = root;
        bool first = false;
        do
        {
            res = getTokens(&current, &first);
        } while (res == true && p_index < p_end);
    }
    return res;
}

bool miniXmlParse::getTokens(miniXmlNode **node, bool *firstNode)
{
    if (*node != nullptr && firstNode != nullptr)
    {
        STRING token_name;
        STRING token_value;
        XCHAR *p_temp = p_index;
        skipSpaces();
        if (*p_index == u'<')
        {
            p_index++;
            if (*p_index == u'/')
            { //end previous token
                p_index = p_temp;
                return false;
            }
            else
            {
                p_index = p_temp;
            }
        }

        if (findNextChar(u'<'))
        {
            if (captureToken(token_name))
            {
                skipSpaces();
                if (captureValue(token_value))
                {
                    //miniXmlNode *oldPos = *node;
                    if ((*firstNode) == false)
                    {
                        (*firstNode) = true;
                        if (token_name.size())
                            (*node)->setName(token_name.c_str());
                        if (token_value.size())
                            (*node)->setValue((XCHAR*)token_value.c_str());
                    }
                    else
                    {
                        *node = (*node)->add(token_name.c_str(), (XCHAR*)token_value.c_str());
                    }
                    while (getTokens(node, firstNode))
                    {
                        *node = (*node)->getParent();
                    }
                }

            }
        }
        p_temp = p_index;
        if (*p_index == u'<')
        {
            p_index++;
            if (*p_index == u'/')
            { //end previous token
                p_index++;
                if (skipSpaces())
                {
                    token_value.clear();
                    captureToken(token_value);
                    skipSpaces();
                    return token_value == (*node)->getName();
                }
            }
        }
    }
    return false;
}

bool miniXmlParse::findNextChar(XCHAR ch)
{
    while (*p_index != ch && p_index < p_end)
        p_index++;
    p_index++;
    return isEnd();
}

bool miniXmlParse::captureToken(STRING & token)
{ // only token as <token>
    bool res = false;
    //XCHAR * pTemp = p_index;
    while (*p_index != u'>' && p_index < p_end)
    {
        token.push_back(*p_index);
        p_index++;
    }
    p_index++;
    res = isEnd();
    return res;
}


bool miniXmlParse::captureValue(STRING & token)
{ // only token as <token>
    bool res = false;
    //XCHAR * pTemp = p_index;
    while (*p_index != u'<' && p_index < p_end)
    {
        if (*p_index >= u' ')
            token.push_back(*p_index);
        p_index++;
    }
    res = isEnd();
    return res;
}


bool miniXmlParse::skipSpaces(void)
{

    struct test_space
    {

        bool check(XCHAR p)
        {
            XCHAR space=u' ';
            if (p ==space )
                return true;
            return false;
        }
    } tp;
    while (tp.check(*p_index) && p_index < p_end)
        p_index++;
    return isEnd();
}



bool xmlConfig::readConfiguration(const XCHAR *name)
{
    return miniXmlNode::load(name,&conf);
}


bool xmlConfig::writeConfiguration(const XCHAR *name)
{
    return miniXmlNode::save(name,&conf);
}
