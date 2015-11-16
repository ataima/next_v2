#include <stdio.h>
#include <string.h>
#include <string>
#include "miniXml.h"
#ifndef _MSC_VER
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#endif

miniXmlNode::miniXmlNode(const char *_name, char *_value,
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

miniXmlNode * miniXmlNode::add(const char *name, char *value)
{
    // name as Xpath = A.B.C. es test.suite.doc
    const char *ptr = strstr(name, ".");
    if (ptr == nullptr)
    {
        if (child == nullptr)
            return new miniXmlNode(name, value, this, nullptr, lastchild());
        else
        {
            miniXmlNode *it = child->find(name);
            if (it != nullptr)
            {
                if (value != nullptr && strlen(value) != 0)
                    return it->cat(value);
                else
                    return it;
            }
            else
                return new miniXmlNode(name, value, this, nullptr, nullptr);
        }
    }
    else
    {
        // has Xpath
        char buff[BUFFLENGTH];
        size_t offset = (size_t) (ptr - name);
        if (offset < BUFFLENGTH)
        {
            memcpy(buff, name, offset);
            buff[offset] = '\0';
        }
        miniXmlNode *it = add(const_cast<const char *> (buff), const_cast<char *> (""));
        return it->add(++ptr, value);
    }
}
/// this function link at this node a child miniXmlNode

miniXmlNode * miniXmlNode::link(char *name, miniXmlNode *_child)
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

miniXmlNode * miniXmlNode::cat(char * addValue)
{
    std::string s = value;
    s += '\r';
    s += '\n';
    s += addValue;
    delete []value;
    value = new char[s.size() + 2];
    memcpy(value, s.c_str(), s.size());
    value[s.size()] = '\0';
    return this;
}
/// inbox for value, return the current value

const char * miniXmlNode::getValue(void)
{
    return (const char *) value;
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

miniXmlNode * miniXmlNode::findNextChild(const char *_name)
{
    miniXmlNode *res = nullptr;
    if (next != nullptr && next->child != nullptr)
        res = next->child->find(_name);
    return res;
}

miniXmlNode * miniXmlNode::findNext(const char *_name)
{
    miniXmlNode *res = nullptr;
    if (name != nullptr && strcmp(name, _name) == 0)
    {
        res = this;
    }
    else if (next != nullptr)
        res = next->findNext(_name);
    return res;
}

miniXmlNode * miniXmlNode::find(const char *_name)
{
    const char *ptr = strstr(_name, ".");
    if (ptr == nullptr)
    {
        miniXmlNode *res = nullptr;
        if (name != nullptr && strcmp(name, _name) == 0)
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
        char buff[BUFFLENGTH];
        size_t offset = (size_t) (ptr - _name);
        if (offset < BUFFLENGTH)
        {

            memcpy(buff, _name, offset);
            buff[offset] = '\0';
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
    char buff[BUFFLENGTH];
    if (name != nullptr)
    {
        // name not set ->only container
        snprintf(buff, BUFFLENGTH - 1, "<%s>\n", name);
        fwrite(buff, strlen(buff), sizeof (char), out);
        if (value != nullptr)
        {
            const char * as[] = {" &lt; ", " &gt; ", " &amp; ", " &apos; ", " &quot; ", " "};
            size_t len = strlen(value);
            size_t i;
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
                    fwrite(as[2], 7, sizeof (char), out);
                    break;
                case '\'':
                    fwrite(as[3], 8, sizeof (char), out);
                    break;
                case '"':
                    fwrite(as[4], 8, sizeof (char), out);
                    break;
                case '<':
                    fwrite(as[0], 6, sizeof (char), out);
                    break;
                case '>':
                    fwrite(as[1], 6, sizeof (char), out);
                    break;
                case '\r':
                case '\n':
                    fwrite(as[5], 1, sizeof (char), out);
                    break;
                default:
                    fwrite(&value[i], 1, sizeof (char), out);
                    break;
                }
            }
        }
    }
    if (child != nullptr)
        child->print(out);
    if (name != nullptr)
    {
        // name not set ->only container
        snprintf(buff, BUFFLENGTH - 1, "</%s>\n", name);
        for (int i = 0; i < 254; i++)
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
        fwrite(buff, strlen(buff), sizeof (char), out);
    }
    if (next != nullptr)
        next->print(out);
}

void miniXmlNode::load(const char *file_in, miniXmlNode *root)
{
    if (root != nullptr)
    {
        miniXmlParse parser(file_in, root);
        parser.parse();
    }
}

/// assign a requested name

void miniXmlNode::setName(const char *_name)
{
    if (_name != nullptr)
    {
        size_t len = strlen(_name);
        name = new char[len + 1];
        memcpy(name, _name, len);
        name[len] = '\0';
    }
    else
        name = nullptr;
}
/// assigne a requested value

void miniXmlNode::setValue(char *_value)
{
    if (_value != nullptr)
    {
        size_t len = strlen(_value);
        value = new char[len + 1];
        memcpy(value, _value, len);
        value[len] = '\0';

    }
    else
        value = nullptr;
}





////////////////////////////////////////////////////////////////////////////////

miniXmlParse::miniXmlParse(const char *file_in, miniXmlNode * _root)
{
    if (file_in != nullptr && _root != nullptr)
    {
        root = _root;
#ifdef _MSC_VER
		FILE *in = nullptr;
		fopen_s(&in,file_in, "r+");
#else
        FILE *in = fopen(file_in, "r+");
#endif
        if (in != nullptr)
        {
            p_end = p_index = nullptr;
            struct stat s;
            if (fstat(_fileno(in), &s) == 0)
            {
                buff = new char [s.st_size];
                if (buff != nullptr)
                {
                    max_size = fread(buff, 1, s.st_size, in);
                    if (max_size != 0)
                    {
                        max_size=s.st_size;
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
        delete []buff;
}

bool miniXmlParse::parse(void)
{

    bool res = false;
    if (buff != nullptr && max_size > 0 && root != nullptr)
    {
        do
        {
            res = getTokens();
        }
        while (res == true && p_index < p_end);
    }
    return res;
}

bool miniXmlParse::getTokens(void)
{
    bool res = false;
    if(findNextChar('<'))
    {
        std::string token;
        captureNextString(token);
    }
    return res;
}

bool miniXmlParse::findNextChar(char ch)
{
    while ( *p_index != ch && p_index < p_end )
        p_index++;
    p_index++;
    return (p_index<p_end);
}




bool miniXmlParse::captureNextString(std::string & token)
{ // only token as <token>
     bool res = false;
     char * pTemp=p_index;
     while ( *p_index != '>' && p_index < p_end )
     {
        token.push_back(*p_index);
        p_index++;
     }
     p_index++;
     res=(p_index<p_end);
     return res;
}