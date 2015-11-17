
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

#define  BUFFLENGTH  256

#if (_MSC_VER < 1900)
#define snprintf  _snprintf
#endif		


class miniXmlNode
{
    miniXmlNode * parent;	/// the father of this node
    miniXmlNode * child;   /// the child of this node
    miniXmlNode * next;    /// the next node , child of this father
    char *name;     /// the node name
    char *value;    /// value of this node
    
public:
    /// defautl ctor
    miniXmlNode(const char *_name, char *_value, miniXmlNode *_parent = nullptr, miniXmlNode *_child = nullptr, miniXmlNode *_next = nullptr);
    /// default destructor
    ~miniXmlNode();
    /// add a child node a this node. _name as
    /// xPath for crossing from iinternal child nodes
    /// path separator is assumend the point '.'
    miniXmlNode * add(const char *name, char *value);
    miniXmlNode * add(const char *name, size_t value);
    miniXmlNode * add(const char *name, size_t idx,size_t value);
    /// this function link at this node a child xmlNode
    miniXmlNode * link(char *name, miniXmlNode  *_child);
    /// this function add a value at already existent value
    miniXmlNode * cat(char * addValue);
    /// inbox for value, return the current value
    const char * getValue(void);
    /// walking from child , return the child with has next field set a  nullptr pointer
    miniXmlNode * lastchild(void);
    /// this function return a node with name as requested
    miniXmlNode * findNextChild(const char *_name);
    /// this function return a next node 
    miniXmlNode * findNext(const char *_name);
    /// this function return a specified node 
    miniXmlNode * find(const char *name, size_t idx);
    miniXmlNode * find(const char *_name);
    /// generate a XML file
    void print(FILE *out);
        /// load a XML from file
    bool load(const char *file_in,miniXmlNode * root);
    /// assign a requested name
    void setName(const char *_name);
    /// assigne a requested value
    void setValue(char *_value);
    /// in line return parent
    inline miniXmlNode * getParent(void) { return parent; }
    inline miniXmlNode * getChild(void) { return child; }
    inline miniXmlNode * getNext(void) { return next; }
    inline const char  * getName(void) { return name; }
};
class miniXmlParse
{
    miniXmlNode * root; ///out
    char * buff;
    off_t max_size;
    char *p_index;
    char *p_end;
public:
    miniXmlParse(const char *filename, miniXmlNode * _root);
    ~miniXmlParse();
    bool parse(void);
protected:
    bool findNextChar(char ch);
    bool getTokens( miniXmlNode **currrent);
    bool captureToken(std::string & token);
    bool captureValue(std::string & token);
    bool skipSpaces(void);
    inline bool isEnd(void){ return ((size_t)(p_index) < (size_t)(p_end));}
};
#endif