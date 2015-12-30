
#ifndef MINI_XML_HEADER
#define MINI_XML_HEADER

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

#define  BUFFLENGTH  512
#include <string>
#include <sstream>

typedef std::basic_stringstream<char16_t> 	u16stringstream;

#ifdef _UNICODE
//#include <tchar.h>
#define  XCHAR char16_t
#define  STRSTR   std::char_traits<char16_t>::find
#define  STRLEN   std::char_traits<char16_t>::length
#define  STRING   std::u16string
#define  SSTRING  u16stringstream
#define  STRCMP   std::char_traits<char16_t>::compare
#define  ATOL     atol
#ifdef _MSC_VER
#define  FOPEN    fopen_s
#else
#define  FOPEN    fopen
#endif
#define  X(msg)   u##msg
#else
#define  XCHAR    char
#define  STRSTR   strstr
#define  STRLEN   strlen
#define  STRING   std::string
#define  SSTRING   std::stringstream
#define  ATOL     atol
#ifdef _MSC_VER
#define  FOPEN    fopen_s
#else
#define  FOPEN    fopen
#endif
#define  STRCMP   strcmp
#define  X        
#endif


#if (_MSC_VER < 1900)
#define snprintf  _snprintf
#endif		
///simple xml for key<.key.....> - value pair 
/// warning : no replace symbols as standard xml ex < -> &lt
/// warning : unsupport attrib key value
class miniXmlNode
{
    miniXmlNode * parent;	/// the father of this node
    miniXmlNode * child;	/// the child of this node
    miniXmlNode * next;		/// the next node , child of this father
    XCHAR *name;				/// the node name
    XCHAR *value;			/// value of this node
public:
    /// defautl ctor
    miniXmlNode(const XCHAR *_name=nullptr, XCHAR *_value = nullptr, miniXmlNode *_parent = nullptr, miniXmlNode *_child = nullptr, miniXmlNode *_next = nullptr);
    /// default destructor
    ~miniXmlNode();
    /// clear the node ..
    void clearNode(void);
    /// add a child node a this node. _name as
    /// xPath for crossing from iinternal child nodes
    /// path separator is assumend the point '.'
    miniXmlNode * add(const XCHAR *name, XCHAR *value);
    miniXmlNode * add(const XCHAR *name, int value);
    miniXmlNode * add(const XCHAR *name, int idx, int value);
    /// this function link at this node a child xmlNode
    miniXmlNode * link(XCHAR *name, miniXmlNode  *_child);
    /// this function add a value at already existent value
    miniXmlNode * cat(XCHAR * addValue);
    /// inbox for value, return the current value
    const XCHAR * getValue(void);
    /// inbox for value, return the current value from atol(value)
    long  getLong(void);
    /// walking from child , return the child with has next field set a  nullptr pointer
    miniXmlNode * lastchild(void);
    /// this function return a node with name as requested
    miniXmlNode * findNextChild(const XCHAR *_name);
    /// this function return a next node 
    miniXmlNode * findNext(const XCHAR *_name);
    /// this function return a specified node 
    miniXmlNode * find(const XCHAR *name, int idx);
    miniXmlNode * find(const XCHAR *_name);
    /// generate a XML file
    void print(FILE *out);
    /// load a XML from file
    static bool load(const XCHAR *file_in, miniXmlNode * root);
    /// save a XML from file
    static bool save(const XCHAR *file_out, miniXmlNode * root);
    /// assign a requested name
    void setName(const XCHAR *_name);
    /// assigne a requested value
    void setValue(XCHAR *_value);
    /// in line return parent
    inline miniXmlNode * getParent(void) { return parent; }
    inline miniXmlNode * getChild(void) { return child; }
    inline miniXmlNode * getNext(void) { return next; }
    inline const XCHAR  * getName(void) { return name; }
    friend bool swapNode(miniXmlNode *src, miniXmlNode* dst);
private:
    //int xsprintf(XCHAR *buff,  const XCHAR* format, ...);
};


class miniXmlParse
{
    miniXmlNode * root; ///out
    XCHAR * buff;
    size_t max_size;
    XCHAR *p_index;
    XCHAR *p_end;
public:
    miniXmlParse(const XCHAR *filename, miniXmlNode * _root);
    ~miniXmlParse();
    bool parse(void);
protected:
    bool findNextChar(XCHAR ch);
    bool getTokens( miniXmlNode **currrent,bool *firstNode);
    bool captureToken(STRING & token);
    bool captureValue(STRING & token);
    bool skipSpaces(void);
    inline bool isEnd(void){ return ((size_t)(p_index) < (size_t)(p_end));}
};

class IConfig
{
public:
    virtual bool readConfiguration(const XCHAR *name) = 0;
    virtual bool writeConfiguration(const XCHAR *name) = 0;
    virtual miniXmlNode & getRoot(void) = 0;
};



class xmlConfig
    :public IConfig
{
    miniXmlNode conf;
public:
    bool readConfiguration(const XCHAR *name);
    bool writeConfiguration(const XCHAR *name);
    inline  miniXmlNode & getRoot(void) { return conf; }
};



#endif
