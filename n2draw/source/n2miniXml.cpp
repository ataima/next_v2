#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale>
#include <string>
#include <stdarg.h>
#include "n2draw.h"
#include "n2miniXml.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "n2utoatou.h"
#include "n2exception.h"
#include "n2resource.h"

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
#if defined(_MSC_VER ) && defined(_UNICODE)
std::locale::id std::codecvt<char16_t, char, struct _Mbstatet>::id;
#endif

miniXmlNode::miniXmlNode(const XCHAR *_name, XCHAR *_value,
        miniXmlNode *_parent, miniXmlNode *_child, miniXmlNode *_next)
: parent(_parent),
child(_child),
next(_next) {
    setName(_name);
    setValue(_value);
    if (parent != nullptr && parent->child == nullptr)
        parent->child = this;
    else if (parent != nullptr && parent->child != nullptr) {
        miniXmlNode *n = parent->child;
        miniXmlNode *t = parent->child;
        while (t != nullptr) {
            n = t;
            t = t->next;
        }
        n->next = this;
    }
}
/// default destructor

miniXmlNode::~miniXmlNode() {
    clearNode();
}

void miniXmlNode::clearNode(void) {
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

IXmlNode *miniXmlNode::add(const XCHAR *name, int idx, int value) {
    SSTREAM ss1;
    SSTREAM ss2;
    ss1 << name << idx;
    ss2 << value;
    return add(ss1.str().c_str(), const_cast<XCHAR *> (ss2.str().c_str()));
}

IXmlNode * miniXmlNode::add(const XCHAR *name, int value) {
    if (name != nullptr) {
        SSTREAM ss;
        ss << value;
        return add(name, const_cast<XCHAR *> (ss.str().c_str()));
    }
    return nullptr;
}

IXmlNode * miniXmlNode::add(const XCHAR *name, XCHAR *value) {
    // name as Xpath = A.B.C. es test.suite.doc
    const XCHAR *ptr = STRSTR(name, STRLEN(name), X('.'));
    if (ptr == nullptr) {
        if (child == nullptr)
            return new miniXmlNode(name, value, this, nullptr, lastchild());
        else {
            miniXmlNode *it = child->find(name);
            if (it != nullptr && value == nullptr) {

                return it;
            } else
                return new miniXmlNode(name, value, this, nullptr, nullptr);
        }
    } else {
        // has Xpath
        XCHAR buff[BUFFLENGTH];
        int offset = (int) (ptr - name);
        if (offset < BUFFLENGTH) {
            memcpy(buff, name, sizeof (XCHAR) * offset);
            buff[offset] = X('\0');
        }
        IXmlNode *it = add(const_cast<const XCHAR *> (buff), nullptr);
        return it->add(++ptr, value);
    }
}
/// this function link at this node a child miniXmlNode

miniXmlNode * miniXmlNode::link(XCHAR *name, miniXmlNode *_child) {
    miniXmlNode *res = find(name);
    if (res != nullptr) {
        if (res->child != nullptr)
            res->lastchild()->next = _child;
        else
            res->child = _child;
    }
    return res;
}
/// this function add a value at already existent value

miniXmlNode * miniXmlNode::cat(XCHAR * addValue) {
    STRING s = value;
    s += X('\r');
    s += X('\n');
    s += addValue;
    delete[]value;
    value = new XCHAR[s.size() + 2];
    memcpy(value, s.c_str(), sizeof (XCHAR) * s.size());
    value[s.size()] = X('\0');
    return this;
}
/// inbox for value, return the current value

const XCHAR * miniXmlNode::getValue(void) {
    return (const XCHAR *) value;
}

long miniXmlNode::getLong(void) {
    STRING v(value);
    UtoA toA(v);
    return ATOL(toA.utf8());
}
/// walking from child , return the child with has next field set a  nullptr pointer

miniXmlNode * miniXmlNode::lastchild(void) {
    miniXmlNode* first = child;
    if (first != nullptr) {
        while (1) {
            miniXmlNode *next = first->next;
            if (next == nullptr)return first;
            first = next;
        }
    }
    return nullptr;
}
/// this function return a node with name as requested

miniXmlNode * miniXmlNode::findNextChild(const XCHAR *_name) {
    miniXmlNode *res = nullptr;
    if (next != nullptr && next->child != nullptr)
        res = next->child->find(_name);
    return res;
}

miniXmlNode * miniXmlNode::findNext(const XCHAR *_name) {
    miniXmlNode *res = nullptr;
    if (name != nullptr && STRCMP(name, _name, STRLEN(_name)) == 0) {
        res = this;
    } else if (next != nullptr)
        res = next->findNext(_name);
    return res;
}

miniXmlNode *miniXmlNode::find(const XCHAR *name, int idx) {
    SSTREAM ss;
    ss << name << idx;
    return find(ss.str().c_str());
}

miniXmlNode * miniXmlNode::find(const XCHAR *_name) {
    const XCHAR *ptr = STRSTR(_name, STRLEN(_name), X('.'));
    if (ptr == nullptr) {
        miniXmlNode *res = nullptr;
        if (name != nullptr && STRCMP(name, _name, STRLEN(_name)) == 0) {
            res = this;
            return res;
        }
        if (next != nullptr) {
            res = next->findNext(_name);
            if (res != nullptr)
                return res;
        }
        if (child != nullptr) {
            res = child->find(_name);
            if (res != nullptr)
                return res;
        }
        if (next != nullptr) {
            res = next->findNextChild(_name);
            if (res != nullptr)
                return res;
        }
        return nullptr;
    } else {
        // has Xpath
        XCHAR buff[BUFFLENGTH];
        int offset = (int) (ptr - _name);
        if (offset < BUFFLENGTH) {

            memcpy(buff, _name, sizeof (XCHAR) * offset);
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

void miniXmlNode::print(FILE *out) {
    XCHAR buff[BUFFLENGTH];
    if (name != nullptr) {
#ifdef _UNICODE
        setlocale(LC_ALL, "");
#endif
        // name not set ->only container
        SSTREAM ss;
        ss << X("<") << name << X(">\n");
        fwrite(ss.str().c_str(), ss.str().size(), sizeof (XCHAR), out);
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
        if (value != nullptr && STRLEN(value) > 0) {
            SSTREAM ss;
            ss << value << X("\n");
            fwrite(ss.str().c_str(), ss.str().size(), sizeof (XCHAR), out);
        }
    }
    if (child != nullptr)
        child->print(out);
    if (name != nullptr) {
        SSTREAM ss;
        ss << X("</") << name << X(">\n");
        size_t t, len = BUFFLENGTH - 1;
        t = ss.str().size();
        if (t < len)
            len = t;
        memcpy(buff, ss.str().c_str(), len);
        for (size_t i = 0; i < len; i++) {
            if (buff[i] == '>')
                break;
            if (buff[i] == ' ') {
                buff[i++] = '>';
                buff[i++] = '\n';
                buff[i] = 0;
                break;
            }
        }
        fwrite(buff, len, sizeof (XCHAR), out);
    }
    if (next != nullptr)
        next->print(out);
}

bool miniXmlNode::load(const XCHAR *file_in, miniXmlNode *root) {
    bool res = false;
    if (root != nullptr) {
        miniXmlParse parser(file_in, root);
        res = parser.parse();
    }
    //root->print(stdout);
    return res;
}

bool miniXmlNode::save(const XCHAR *out, miniXmlNode * root) {
    bool res = false;
    if (out != nullptr && root != nullptr) {
        STRING fileout(out);
        UtoA toA(fileout);
        FILE *out = fopen(toA.utf8(), "wb");
        if (out != nullptr) {
            root->print(out);
            fclose(out);
        }
    }
    return res;
}


/// assign a requested name

void miniXmlNode::setName(const XCHAR *_name) {
    if (_name != nullptr) {
        size_t len = STRLEN(_name);
        name = new XCHAR[len + 1];
        memcpy(name, _name, sizeof (XCHAR) * len);
        name[len] = X('\0');
    } else
        name = nullptr;
}
/// assigne a requested value

void miniXmlNode::setValue(XCHAR *_value) {
    if (_value != nullptr) {
        size_t len = STRLEN(_value);
        value = new XCHAR[len + 1];
        memcpy(value, _value, sizeof (XCHAR) * len);
        value[len] = X('\0');
    } else
        value = nullptr;
}

bool swapNode(miniXmlNode *src, miniXmlNode* dst) {
    bool res = false;
    if (src == dst)
        return true;
    if (src != nullptr && dst != nullptr) {
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
        while (next_node != nullptr) {
            next_node->parent = dst;
            next_node = next_node->next;
        };
        res = true;
    }
    return res;
}




/////////////////////////file_out///////////////////////////////////////////////////////

miniXmlParse::miniXmlParse(const XCHAR *_in, miniXmlNode * _root) {
    if (_in != nullptr && _root != nullptr) {
        root = _root;
        STRING filename_in(_in);
        UtoA toA(filename_in);
        try {
            int res = nnResource::Get(filename_in.c_str(), (const unsigned char **) &filebuff, &max_size);
            if (res) {
                //try as  external file..
                FILE *in = fopen(toA.utf8(), "rb");
                if (in != nullptr) {
                    fseek(in, 0L, SEEK_END) + 1;
                    ssize_t lenght = ftell(in);
                    fseek(in, 0L, SEEK_SET);
                    if (lenght > 0) {
                        XCHAR* buff = new XCHAR[lenght];
                        if (buff != nullptr) {
                            max_size = fread(buff, sizeof (XCHAR), lenght, in);
                            if(max_size>0){
                                filebuff=const_cast<const XCHAR*>(buff);
                            }
                            tofree = true;
                        }
                    }
                    fclose(in);
                }
                root = nullptr;
                filebuff = nullptr;
                max_size = 0;
                tofree = false;
            } else {
                p_index = filebuff;
                p_end = &filebuff[max_size];
            }
        } catch (...) {
            appManagerConfigureFileUnknow *e = new appManagerConfigureFileUnknow(_in);
            throw (e);
        }
    } else {
        root = nullptr;
        filebuff = nullptr;
        max_size = 0;
        tofree = false;
    }

}

miniXmlParse::~miniXmlParse() {
    if (tofree == true) {
        delete [] filebuff;
    }
    filebuff = nullptr;
    max_size = 0;
}

bool miniXmlParse::parse(void) {
    bool res = false;
    if (filebuff != nullptr && max_size > 0 && root != nullptr) {
        miniXmlNode *current = root;
        if (p_index != nullptr) {
            if (*p_index != X('<')) {
#ifdef _UNICODE
                XCHAR p = *p_index;
                if (p == 0xfffe) {
                    //bad format of xml
                    xmlBadFormatException * e = new xmlBadFormatException();
                    throw (e);
                } else if (p == 0xfeff) {
                    p_index++;
                    p_index++;
                }
            }
#else
                p_index++;
                if (*p_index != X('<')) {
                    //bad format of xml
                    xmlBadFormatException * e = new xmlBadFormatException();
                    throw (e);
                }
#endif
        }
        bool first = false;
        do {
            res = getTokens(&current, &first);
        } while (res == true && p_index < p_end);
    }
}
return res;
}

bool miniXmlParse::getTokens(miniXmlNode **node, bool *firstNode) {
    if (*node != nullptr && firstNode != nullptr) {
        STRING token_name;
        STRING token_value;
        XCHAR *p_temp = (XCHAR *) p_index;
        skipSpaces();
        if (*p_index == X('<')) {
            p_index++;
            if (*p_index == X('/')) {
                //end previous token
                p_index = p_temp;
                return false;
            } else {
                p_index = p_temp;
            }
        }

        if (findNextChar(X('<'))) {
            if (captureToken(token_name)) {
                skipSpaces();
                if (captureValue(token_value)) {
                    //miniXmlNode *oldPos = *node;
                    if ((*firstNode) == false) {
                        (*firstNode) = true;
                        if (token_name.size())
                            (*node)->setName(token_name.c_str());
                        if (token_value.size())
                            (*node)->setValue((XCHAR*) token_value.c_str());
                    } else {
                        *node = dynamic_cast<miniXmlNode*> ((*node)->add(token_name.c_str(), (XCHAR*) token_value.c_str()));
                    }
                    while (getTokens(node, firstNode)) {
                        *node = dynamic_cast<miniXmlNode*> ((*node)->getParent());
                    }
                }

            }
        }
        p_temp = (XCHAR *) p_index;
        if (*p_index == X('<')) {
            p_index++;
            if (*p_index == X('/')) {
                //end previous token
                p_index++;
                if (skipSpaces()) {
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

bool miniXmlParse::findNextChar(XCHAR ch) {
    while (*p_index != ch && p_index < p_end)
        p_index++;
    p_index++;
    return isEnd();
}

bool miniXmlParse::captureToken(STRING & token) {
    // only token as <token>
    bool res = false;
    //XCHAR * pTemp = p_index;
    while (*p_index != X('>') && p_index < p_end) {
        token.push_back(*p_index);
        p_index++;
    }
    p_index++;
    res = isEnd();
    return res;
}

bool miniXmlParse::captureValue(STRING & token) {
    // only token as <token>
    bool res = false;
    //XCHAR * pTemp = p_index;
    while (*p_index != X('<') && p_index < p_end) {
        if (*p_index >= X(' '))
            token.push_back(*p_index);
        p_index++;
    }
    res = isEnd();
    return res;
}

bool miniXmlParse::skipSpaces(void) {

    struct test_space {

        bool check(XCHAR p) {
            int v = (int) p;
            if (v < (int) (' '))
                return true;
            if (v == 0xfffe)
                return true;
            return false;
        }
    } tp;
    while (tp.check(*p_index) && p_index < p_end)
        p_index++;
    return isEnd();
}

bool xmlConfig::readConfiguration(const XCHAR *name) {
    return miniXmlNode::load(name, &conf);
}

bool xmlConfig::writeConfiguration(const XCHAR *name) {
    return miniXmlNode::save(name, &conf);
}
