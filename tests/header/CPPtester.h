#ifndef _CPP_TESTER_INCLUDED_
#define _CPP_TESTER_INCLUDED_


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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <stdarg.h>
using namespace std;

#ifdef _WIN32
//#include <windows.h>
//#include <lmcons.h>
// multi platform  glue
#define snprintf _snprintf
#elif defined _WIN64
#define snprintf _snprintf
#elif defined BORLAND
#include <tchar.h>
#else
#include <unistd.h>
#endif

// max (1) test class with (2) tests
// calc. from app.
#define MAXNUMCLASSTEST 128
#define MAXNUMTEST 256
#define BUFFLENGTH	256
//////////////////////////////
// throw object
typedef struct exc_fail
{
	const char *file;
	int line;
	const char *msg;
	exc_fail(const char *_file = NULL, int _line = 0, const char *_msg = NULL)
		: file(_file), line(_line), msg(_msg)
	{}
} EXC_FAIL;
// define more macros for debugging
#define CA_ASSERT(b) \
{\
	caContainerTest::totalAssert++;\
	if(b==0)\
{\
	EXC_FAIL fail(__FILE__,__LINE__);\
	throw(fail);\
}\
}

#define CA_ASSERT_M(msg,b) \
{\
	caContainerTest::totalAssert++;\
	if(b==0)\
{\
	EXC_FAIL fail(__FILE__,__LINE__,msg);\
	throw(fail);\
}\
}

#define CA_NASSERT(b) \
{\
	caContainerTest::totalAssert++;\
	if(b!=0)\
{\
	EXC_FAIL fail(__FILE__,__LINE__);\
	throw(fail);\
}\
}

#define CA_RECOVERY(b) \
{\
	caContainerTest::totalAssert++;\
	try{\
	b;\
{\
	EXC_FAIL fail(__FILE__,__LINE__);\
	throw(fail);\
}\
}catch(...){/* OK!*/}\
}

/// simple class for retrieve the environment info.
/// todo : complete infoSystem ...
class infoSystem
{
public:
	static void getDate(char *buff, int len)
	{

		time_t t;
		struct tm *_tm;
		if (len < 16)return;
		time(&t);
		_tm = localtime(&t);
		sprintf(buff, "%02d/%02d/%04d%c", _tm->tm_mday, _tm->tm_mon + 1, 1900 + _tm->tm_year, '\0');
	}
	static void getTime(char *buff, int len)
	{
		time_t t;
		struct tm *_tm;
		if (len < 16)return;
		time(&t);
		_tm = localtime(&t);
		sprintf(buff, "%02d:%02d:%2d", _tm->tm_hour, _tm->tm_min, _tm->tm_sec);
	}
	static void getUser(char *buff, int len)
	{
#if defined (_WIN32) || defined (_WIN64)|| defined(BORLAND)
		unsigned long w = len;
		//		GetUserName(buff, &w);
		strcpy(buff, "Windows user");
#else
		sprintf(buff, "%s", getlogin());
#endif
	}
	static void getOsName(char *buff, int len)
	{
#if defined (_WIN32) || defined (_WIN64)|| defined(BORLAND)
		strncpy(buff, getenv("OS"), len);
#else
		// DA FARE
		strncpy(buff, "Unix System", len);
#endif
	}
	static void getDomain(char *buff, int len)
	{
#if defined (_WIN32) || defined (_WIN64)|| defined(BORLAND)
		strncpy(buff, getenv("USERDOMAIN"), len);
#else
		// DA FARE
		strncpy(buff, getenv("PWD"), len);
#endif
	}
};

class htmlWorker
{
	FILE *out;
public:
	htmlWorker(FILE *_out = NULL)
	{
		out = _out;
	}
	void addStartPage(void)
	{
		const char *MSG = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n";
		fputs(MSG, out);
		fflush(out);
	}
	void addEndPage(void)
	{
		const char *MSG = "</HTML>\n";
		fputs(MSG, out);
		fflush(out);
	}
	void addHEAD(void)
	{
		const char *MSG = "<HEAD>\n<META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=windows-1252\">\n"
			"<TITLE>C.A. TEST REPORT</TITLE>\n"
			"<META NAME=\"AUTHOR\" CONTENT=\"Coppi Angelo\">\n"
			"<STYLE>\n"
			"<!--\n"
			"@page { size: 21cm 29.7cm; margin: 2cm }\n"
			"P { margin-bottom: 0.21cm }\n"
			"TD P { margin-bottom: 0cm }\n"
			"-->\n"
			"</STYLE>\n"
			"</HEAD>\n";
		fputs(MSG, out);
		fflush(out);
	}
	void addPageFormat(void)
	{
		const char *MSG = "<BODY LANG=\"it-IT\" BGCOLOR=\"#ffcc99\" DIR=\"LTR\">\n";
		fputs(MSG, out);
		fflush(out);
	}
	void addTittle(const char *title)
	{
		const char *MSG = "<P ALIGN=CENTER STYLE=\"margin-bottom: 0cm\">\n"
			"<FONT FACE=\"Bitstream Vera Sans Mono, monospace\">"
			"<FONT SIZE=5>%s</FONT></FONT></P>\n";
		fprintf(out, MSG, title);
		fflush(out);
	}
	void addSummaryTable(const char *success, const char * fail)
	{
		const char *MSG = "<TABLE ALIGN=CENTER WIDTH=768 BORDER=1 BORDERCOLOR=\"#000000\" CELLPADDING=5 CELLSPACING=0>\n"
			"<COL WIDTH=374>\n<COL WIDTH=372>\n"
			"<TR><TD COLSPAN=2 WIDTH=756 VALIGN=TOP BGCOLOR=\"#00b8ff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\"><FONT SIZE=2>Summary of report</FONT></FONT></P>\n"
			"</TD></TR><TR VALIGN=TOP>\n"
			"<TD WIDTH=374 BGCOLOR=\"#ccffff\">\n"
			"<P><FONT FACE=\"Bitstream Vera Sans Mono, monospace\">Test <FONT COLOR=\"#00ae00\">OK</FONT></FONT></P>\n"
			"</TD><TD WIDTH=372 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=RIGHT STYLE=\"margin-left: -0.08cm; margin-right: 0.01cm\"><FONT FACE=\"Bitstream Vera Sans Mono, monospace\">%s</FONT></P>\n"
			"</TD>\n"
			"</TR>\n"
			"<TR VALIGN=TOP><TD WIDTH=374 BGCOLOR=\"#ccffff\">\n"
			"<P><FONT FACE=\"Bitstream Vera Sans Mono, monospace\">Test <FONT COLOR=\"#dc2300\">FAIL</FONT></FONT></P>\n"
			"</TD><TD WIDTH=372 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=RIGHT STYLE=\"margin-left: -0.08cm; margin-right: 0.01cm\"><FONT COLOR=\"#dc2300\"><FONT FACE=\"Bitstream Vera Sans Mono, monospace\">%s</FONT></FONT></P>\n"
			"</TD></TR></TABLE>";
		fprintf(out, MSG, success, fail);
		fflush(out);
	}
	void addSpace(void)
	{
		const char *MSG = "<P STYLE=\"margin-bottom: 1cm\"><BR></P>";
		fputs(MSG, out);
		fflush(out);
	}
	void addStartBriefTable(const char *title, int color = 0)
	{
		const char *MSG = "<TABLE ALIGN=CENTER WIDTH=768 BORDER=1 BORDERCOLOR=\"#000000\" CELLPADDING=5 CELLSPACING=0>\n"
			"<COL WIDTH=30><COL WIDTH=300><COL WIDTH=438>\n"
			"<TR><TD COLSPAN=4 WIDTH=756 VALIGN=TOP BGCOLOR=\"#00b8ff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\">\n"
			"<FONT SIZE=2 COLOR=\"%d\">%s</FONT></FONT></P></TD></TR>\n"
			"<TR VALIGN=TOP><TD WIDTH=43 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER> <FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Id</FONT></P></TD>\n"
			"<TD WIDTH=82 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Name</FONT></P></TD>\n"
			"<TD WIDTH=601 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Brief</FONT></P></TD></TR>\n";
		fprintf(out, MSG, color, title);
		fflush(out);
	}
	void addStartBriefTable1(const char *title, int color = 0)
	{
		const char *MSG = "<TABLE ALIGN=CENTER WIDTH=768 BORDER=1 BORDERCOLOR=\"#000000\" CELLPADDING=5 CELLSPACING=0>\n"
			"<COL WIDTH=30><COL WIDTH=300><COL WIDTH=400><COL WIDTH=38>\n"
			"<TR><TD COLSPAN=4 WIDTH=756 VALIGN=TOP BGCOLOR=\"#00b8ff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\">\n"
			"<FONT SIZE=2 COLOR=\"%d\">%s</FONT></FONT></P></TD></TR>\n"
			"<TR VALIGN=TOP><TD WIDTH=43 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER> <FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Id</FONT></P></TD>\n"
			"<TD WIDTH=82 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Name</FONT></P></TD>\n"
			"<TD WIDTH=551 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Brief</FONT></P></TD>\n"
			"<TD WIDTH=50 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" COLOR=\"#aa0000\">Time</FONT></P></TD></TR>\n";
		fprintf(out, MSG, color, title);
		fflush(out);
	}
	void addRowBriefTable(const char *id, const char *name, const char *brief)
	{
		const char *MSG = "<TR VALIGN=TOP><TD WIDTH=30 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER> <FONT SIZE=2  FACE=\"Bitstream Vera Sans Mono, monospace\"><A HREF=\"#Tabella_%s\">%s</A></FONT></P></TD>\n"
			"<TD WIDTH=300 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=JUSTIFY><FONT SIZE=1 FACE=\"Bitstream Vera Sans Mono, monospace\">%s</FONT></P></TD>\n"
			"<TD WIDTH=438 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=JUSTIFY><FONT SIZE=1 FACE=\"Bitstream Vera Sans Mono, monospace\">%s</FONT></P></TD></TR>\n";
		fprintf(out, MSG, id, id, name, brief);
		fflush(out);
	}
	void addRowBriefTable(const char *id, const char *name, const char *brief, const char * time)
	{
		const char *MSG = "<TR VALIGN=TOP><TD WIDTH=30 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER> <FONT SIZE=2  FACE=\"Bitstream Vera Sans Mono, monospace\"><A HREF=\"#Tabella_%s\">%s</A><A NAME=\"brief_%s\"></A></FONT></P></TD>\n"
			"<TD WIDTH=300 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=JUSTIFY><FONT SIZE=1 FACE=\"Bitstream Vera Sans Mono, monospace\">%s</FONT></P></TD>\n"
			"<TD WIDTH=400 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=JUSTIFY><FONT SIZE=1 FACE=\"Bitstream Vera Sans Mono, monospace\">%s</FONT></P></TD>\n"
			"<TD WIDTH=38 BGCOLOR=\"#ccffff\">\n"
			"<P ALIGN=CENTER><FONT SIZE=1 FACE=\"Bitstream Vera Sans Mono, monospace\">%s ms</FONT></P></TD></TR>\n";
		fprintf(out, MSG, id, id, id, name, brief, time);
		fflush(out);
	}
	inline void addEndTable(void)
	{
		fputs("</TABLE>", out);
		fflush(out);
	}
	void addStartSingleTestTable(const char *title, const char *Id = NULL, const char *color = "000000")
	{
		char buffer[BUFFLENGTH];
		if (Id != NULL)
			snprintf(buffer, BUFFLENGTH - 1, "%s Id(%s)", title, Id);
		else
			snprintf(buffer, BUFFLENGTH - 1, "%s", title);
		const char *MSG = "<P ALIGN=RIGHT><A HREF=\"#brief_%s\">Return</A></P>"
			"<A NAME=\"Tabella_%s\"></A>"
			"<TABLE ALIGN=CENTER WIDTH=768 BORDER=1 BORDERCOLOR=\"#000000\" CELLPADDING=5 CELLSPACING=0>"
			"<COL WIDTH=250><COL WIDTH=512><TR>"
			"<TD COLSPAN=2 WIDTH=756 VALIGN=TOP BGCOLOR=\"#00b8ff\">"
			"<P ALIGN=CENTER><FONT COLOR=\"#%s\" FACE=\"Bitstream Vera Sans Mono, monospace\" SIZE=1>%s</FONT></P>"
			"</TD></TR>";
		if (Id != NULL)
			fprintf(out, MSG, Id, Id, color, buffer);
		else
			fprintf(out, MSG, Id, Id, color, buffer);
		fflush(out);
	}
	void addRowSingleTestTable(const char *title, const char *data)
	{
		const char *MSG = "<TR VALIGN=TOP><TD WIDTH=250 BGCOLOR=\"#ccffff\">"
			"<P ALIGN=LEFT><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" SIZE=1>%s</FONT></P>"
			"</TD><TD WIDTH=512 BGCOLOR=\"#ffffcc\">"
			"<P ALIGN=LEFT><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" SIZE=1>%s</FONT></P></TD></TR>";
		fprintf(out, MSG, title, data);
		fflush(out);
	}

	void addRowSingleTestFailTable(const char *title, const char *data, const char *color)
	{
		const char *MSG = "<TR VALIGN=TOP><TD WIDTH=250 BGCOLOR=\"#ccffff\">"
			"<P ALIGN=LEFT><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" SIZE=1>%s</FONT></P>"
			"</TD><TD WIDTH=512 BGCOLOR=\"#ffffcc\">"
			"<P ALIGN=LEFT><FONT COLOR=\"#%s\" FACE=\"Bitstream Vera Sans Mono, monospace\" SIZE=1 STYLE=\"font-size: 9pt\"><B>%s</B></FONT></P></TD></TR>";
		fprintf(out, MSG, title, color, data);
		fflush(out);
	}

	void addInfoSingleTestTable(const char *title, const char *data)
	{
		const char *ptrStart = NULL;
		const char *ptrStop = NULL;
		const char *MSG1 = "<TR><TD COLSPAN=2 WIDTH=756 VALIGN=TOP BGCOLOR=\"#00b8ff\">"
			"<P ALIGN=CENTER><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" SIZE=1 >%s</FONT></P></TD></TR>"
			"<TR><TD COLSPAN=2 WIDTH=756 VALIGN=TOP BGCOLOR=\"#ccffff\">"
			"<P ALIGN=JUSTIFY><FONT FACE=\"Bitstream Vera Sans Mono, monospace\" FONT SIZE=1 ><UL><LI>";
		const char *MSG2 = "</UL></FONT></P></TD></TR>";
		const char *MSG3 = "<LI>";
		fprintf(out, MSG1, title);
		ptrStart = data;
		do
		{
			ptrStop = strstr(ptrStart, "\n\r");
			if (ptrStop == NULL)
			{
				fputs(ptrStart, out);
				break;
			}
			else
			{
				fwrite(ptrStart, (size_t)(ptrStop - ptrStart) + 2, sizeof(char), out);
				ptrStop += 2;
				ptrStart = ptrStop;
				fwrite(MSG3, 4, sizeof(char), out);
			}
		} while (1);
		fputs(MSG2, out);
		fflush(out);
	}
	void addImageSingleTestTable(const char *file)
	{
		const char *MSG = "<TR><TD COLSPAN=2 WIDTH=756 VALIGN=TOP >"
			"<P ALIGN=CENTER><IMG SRC=\"%s\"  ALIGN=CENTER BORDER=0></P></TD></TR>";
		fprintf(out, MSG, file);
		fflush(out);
	}
};


/// spready list like XML recursive a cross-sectional
/// has next and parent as double linked list but any node has childs
/// for XML childs tag
class xmlNode
{
	xmlNode * parent;	/// the father of this node
	xmlNode * child;   /// the child of this node
	xmlNode * next;    /// the next node , child of this father
	char *name;     /// the node name
	char *value;    /// value of this node
public:
	/// defautl ctor
	xmlNode(const char *_name, char *_value, xmlNode *_parent = NULL, xmlNode *_child = NULL, xmlNode *_next = NULL)
		:parent(_parent),
		child(_child),
		next(_next)
	{
		setName(_name);
		setValue(_value);
		if (parent != NULL && parent->child == NULL)
			parent->child = this;
		else if (parent != NULL && parent->child != NULL)
		{
			xmlNode *n = parent->child;
			xmlNode *t = parent->child;
			while (t != NULL)
			{
				n = t;
				t = t->next;
			}
			n->next = this;
		}
	}
	/// default destructor
	~xmlNode()
	{
		if (name != NULL)
			delete[]name;
		if (value != NULL)
			delete[]value;
		// cancella tutta la catena
		if (child != NULL)
			delete child;
		child = NULL;
		if (next != NULL)
			delete next;
		next = NULL;
	}
	/// add a child node a this node. _name as
	/// xPath for crossing from iinternal child nodes
	/// path separator is assumend the point '.'
	xmlNode * add(const char *name, char *value)
	{
		// name as Xpath = A.B.C. es test.suite.doc
		const char *ptr = strstr(name, ".");
		if (ptr == NULL)
		{
			if (child == NULL)
				return  new xmlNode(name, value, this, NULL, lastchild());
			else
			{
				xmlNode *it = child->find(name);
				if (it != NULL)
				{
					if (value != NULL && strlen(value) != 0)
						return it->cat(value);
					else
						return it;
				}
				else
					return  new xmlNode(name, value, this, NULL, NULL);
			}
		}
		else
		{
			// has Xpath
			char buff[BUFFLENGTH];
			size_t offset = (size_t)(ptr - name);
			if (offset < BUFFLENGTH)
			{
				memcpy(buff, name, offset);
				buff[offset] = '\0';
			}
			xmlNode *it = add(const_cast<const char *>(buff), const_cast<char *>(""));
			return it->add(++ptr, value);
		}
	}
	/// this function link at this node a child xmlNode
	xmlNode * link(char *name, xmlNode  *_child)
	{
		xmlNode *res = find(name);
		if (res != NULL)
		{
			if (res->child != NULL)
				res->lastchild()->next = _child;
			else
				res->child = _child;
		}
		return res;
	}
	/// this function add a value at already existent value
	xmlNode * cat(char * addValue)
	{
		size_t lenold = strlen(value);
		size_t newlen = strlen(addValue) + 1;
		lenold += newlen + 6;
		char *t = new char[lenold];
		strcpy(t, value);
		char *tmp = t + strlen(t)*sizeof(char);
		*(tmp) = '\n';
		*(++tmp) = '\r';
		//       *(++tmp)='<';
		//       *(++tmp)='L';
		//       *(++tmp)='I';
		//       *(++tmp)='>';
		*(tmp + 1) = '\0';
		strcat(tmp, addValue);
		delete[]value;
		value = t;
		return this;
	}
	/// inbox for value, return the current value
	inline const char * getValue(void)
	{
		return (const char *)value;
	}
	/// walking from child , return the child with has next field set a  NULL pointer
	xmlNode * lastchild(void)
	{
		xmlNode* first = child;
		if (first != NULL)
		{
			while (1)
			{
				xmlNode *next = first->next;
				if (next == NULL)return first;
				first = next;
			}
		}
		return NULL;
	}
	/// this function return a node with name as requested
	xmlNode * findNextChild(const char *_name)
	{
		xmlNode *res = NULL;
		if (next != NULL && next->child != NULL)
			res = next->child->find(_name);
		return res;
	}
	xmlNode * findNext(const char *_name)
	{
		xmlNode *res = NULL;
		if (name != NULL && strcmp(name, _name) == 0)
		{
			res = this;
		}
		else if (next != NULL)
			res = next->findNext(_name);
		return res;
	}
	xmlNode * find(const char *_name)
	{
		const char *ptr = strstr(_name, ".");
		if (ptr == NULL)
		{
			xmlNode *res = NULL;
			if (name != NULL && strcmp(name, _name) == 0)
			{
				res = this;
				return res;
			}
			if (next != NULL)
			{
				res = next->findNext(_name);
				if (res != NULL)
					return res;
			}
			if (child != NULL)
			{
				res = child->find(_name);
				if (res != NULL)
					return res;
			}
			if (next != NULL)
			{
				res = next->findNextChild(_name);
				if (res != NULL)
					return res;
			}
			return NULL;
		}
		else
		{
			// has Xpath
			char buff[BUFFLENGTH];
			size_t offset = (size_t)(ptr - _name);
			if (offset < BUFFLENGTH)
			{

				memcpy(buff, _name, offset);
				buff[offset] = '\0';
			}
			xmlNode *it = find(buff);
			if (it != NULL)
				return it->find(++ptr);
			else
				return NULL;
		}

	}
	/// generate a XML file
	void print(FILE *out)
	{
		char buff[BUFFLENGTH];
		if (name != NULL)
		{
			// name not set ->only container
			snprintf(buff, BUFFLENGTH - 1, "<%s>\n", name);
			fwrite(buff, strlen(buff), sizeof(char), out);
			if (value != NULL)
			{
				const char * as[] = { " &lt; "," &gt; "," &amp; "," &apos; "," &quot; "," " };
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
			}
		}
		if (child != NULL)
			child->print(out);
		if (name != NULL)
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
			fwrite(buff, strlen(buff), sizeof(char), out);
		}
		if (next != NULL)
			next->print(out);
	}
	/// generate a htML file
	void createHTML(FILE *out)
	{
		htmlWorker html(out);
		html.addStartPage();
		html.addHEAD();
		html.addPageFormat();
		html.addTittle("CA Test Suite Report");
		html.addSpace();
		xmlNode *success = this->find("FailuresTotal");
		xmlNode *fail = this->find("SuccessTotal");
		if (success != NULL && fail != NULL)
			html.addSummaryTable(fail->value, success->value);
		fail = this->find("FailedTests");
		if (fail != NULL)
		{
			bool res = false;
			xmlNode *child = fail->child;
			if (child != NULL)
				res = true;
			if (res == true)
			{
				html.addSpace();
				html.addStartBriefTable("Bad Tests");
			}
			while (child != NULL)
			{
				char *id, *name, *brief;
				id = name = brief = NULL;
				xmlNode *temp = child->find("Id");
				if (temp != NULL)
					id = temp->value;
				temp = child->find("Name");
				if (temp != NULL)
					name = temp->value;
				temp = child->find("Doc.Brief");
				if (temp != NULL)
					brief = temp->value;
				html.addRowBriefTable(id, name, brief);
				child = child->next;
			}
			if (res == true)
				html.addEndTable();
		}
		html.addSpace();
		success = this->find("SuccessTests");
		if (success != NULL)
		{
			html.addStartBriefTable1("Good Tests");
			xmlNode *child = success->child;
			while (child != NULL && child->child != NULL)
			{
				char *id, *name, *brief, *time;
				id = name = brief = time = NULL;
				xmlNode *temp = child->child->find("Id");
				if (temp != NULL)
					id = temp->value;
				temp = child->child->find("Name");
				if (temp != NULL)
					name = temp->value;
				temp = child->child->find("Doc.Brief");
				if (temp != NULL)
					brief = temp->value;
				temp = child->child->find("Doc.Time");
				if (temp != NULL)
					time = temp->value;
				html.addRowBriefTable(id, name, brief, time);
				child = child->next;
			}
			html.addEndTable();
		}
		html.addSpace();
		if (fail != NULL)
		{
			xmlNode *child = fail->child;
			while (child != NULL && child->child != NULL)
			{
				char *data;
				data = NULL;
				xmlNode *temp = child->child->find("Id");
				if (temp != NULL)
					data = temp->value;
				html.addStartSingleTestTable("Single Test Report", data, "ff3333");
				temp = child->child->find("Name");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Test Name", data);
				}
				temp = child->child->find("Doc.Brief");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Brief Description", data);
				}
				temp = child->child->find("Doc.Author");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Author", data);
				}
				html.addRowSingleTestFailTable("Result", "Fail", "ff3333");

				temp = child->child->find("Doc.FailureType");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestFailTable("Failure Type", data, "ff3333");
				}
				temp = child->child->find("Doc.Location.File");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestFailTable("On File", data, "ff3333");
				}
				temp = child->child->find("Doc.Location.Line");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestFailTable("At Line", data, "ff3333");
				}
				temp = child->child->find("Doc.Message");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestFailTable("Fail Message", data, "ff3333");
				}

				temp = child->child->find("Doc.Objective");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("More Info", data);
				}
				temp = child->child->find("Doc.Conf_override");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("System Configuration", data);
				}
				temp = child->child->find("Doc.Input_data");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Inputs", data);
				}
				temp = child->child->find("Doc.Output_req");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Outputs", data);
				}
				temp = child->child->find("Doc.Code_req");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Code", data);
				}
				temp = child->child->find("Doc.Encloses");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Encloses", data);
				}
				temp = child->child->find("Doc.Image");
				if (temp != NULL)
				{
					data = temp->value;
					html.addImageSingleTestTable(data);
				}
				html.addEndTable();
				html.addSpace();
				child = child->next;
			}
		}
		html.addSpace();
		if (success != NULL)
		{
			xmlNode *child = success->child;
			while (child != NULL && child->child != NULL)
			{
				char *data;
				data = NULL;
				xmlNode *temp = child->child->find("Id");
				if (temp != NULL)
					data = temp->value;
				html.addStartSingleTestTable("Single Test Report", data, "33ff33");
				temp = child->child->find("Name");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Test Name", data);
				}
				temp = child->child->find("Doc.Brief");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Brief Description", data);
				}
				temp = child->child->find("Doc.Author");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Author", data);
				}
				html.addRowSingleTestFailTable("Result", "Okey", "33ff33");
				temp = child->child->find("Doc.Time");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Exec Time", data);
				}

				temp = child->child->find("Doc.Objective");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("More Info", data);
				}

				temp = child->child->find("Doc.Conf_override");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("System Configuration", data);
				}

				temp = child->child->find("Doc.Input_data");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Inputs", data);
				}

				temp = child->child->find("Doc.Output_req");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Outputs", data);
				}

				temp = child->child->find("Doc.Code_req");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Code", data);
				}

				temp = child->child->find("Doc.Encloses");
				if (temp != NULL)
				{
					data = temp->value;
					html.addInfoSingleTestTable("Encloses", data);
				}

				temp = child->child->find("Doc.Image");
				if (temp != NULL)
				{
					data = temp->value;
					html.addImageSingleTestTable(data);
				}
				html.addEndTable();
				html.addSpace();
				child = child->next;
			}
		}
		success = this->find("SuiteInfo");
		if (success != NULL)
		{
			html.addSpace();
			html.addStartSingleTestTable("System");
			xmlNode *child = success->child;
			while (child != NULL && child->child != NULL)
			{
				char *data;
				data = NULL;
				xmlNode *temp = child->child->find("Date");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Date", data);
				}
				temp = child->child->find("Time");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Time", data);
				}
				temp = child->child->find("User");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("User", data);
				}
				temp = child->child->find("OS");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("O.S.", data);
				}
				temp = child->child->find("Domain");
				if (temp != NULL)
				{
					data = temp->value;
					html.addRowSingleTestTable("Domain", data);
				}
				child = child->next;
			}
			html.addEndTable();
			html.addSpace();
		}
		html.addEndPage();
	}


	/// assign a requested name
	void setName(const char *_name)
	{
		if (_name != NULL)
		{
			name = new char[strlen(_name) + 1];
			strcpy(name, _name);
		}
		else
			name = NULL;
	}
	/// assigne a requested value
	void setValue(char *_value)
	{
		if (_value != NULL)
		{
			value = new char[strlen(_value) + 1];
			strcpy(value, _value);

		}
		else
			value = NULL;
	}


};

/// simple class for generate XML output
/// For any test class a array of this class in created
/// where memorized the info for documentation
class infodoc
{
public:
	int result;			/// field for result test 0 failed (default), 1 successful.
	xmlNode *root;		/// root xmlNode for this infodoc ( generally have  childs)
	long exec_t;		/// executing time , calculated 	in ms
	const char *file;	/// associate file for source code (only if test fail)
	int line;			/// associate line for source code (only if test fail)
	const char *msg;    /// associate mesage for assertion on source code (only if test fail and have message)
	int id;				/// sequential identifier of test as on registration
public:
	/// inbox for root xmlNode
	inline xmlNode *getRoot(void)
	{
		return root;
	}
	/// default ctor
	infodoc(const char *_name, const char *_brief, int num = 0)
	{
		char buff[BUFFLENGTH];
		result = 0; // fail
		exec_t = 0;
		line = -1;
		file = NULL;
		msg = NULL;
		id = num;
		root = new xmlNode(NULL, NULL, NULL, NULL);
		snprintf(buff, BUFFLENGTH - 1, "%d", id);
		root->add("Id", (char *)buff);
		if (_name != NULL)
			root->add("Name", (char *)_name);
		if (_brief != NULL)
			root->add("Doc.Brief", (char *)_brief);
	}
	/// default destructor
	~infodoc()
	{
		if (root != NULL)
			delete root;
		root = NULL;
	}
	/// set var if test is terminated with no failure
	void Ok(clock_t stop, clock_t start)
	{
		result = 1;
		exec_t = (long)(stop - start);
	}
	/// set var if test is terminated with  failure
	inline void Fail(const char *_msg = NULL, const char *_file = NULL, int _line = -1)
	{
		result = 0;
		exec_t = 0;
		file = _file;
		line = _line;
		msg = _msg;
	}
	/// get execution time in ms
	inline long getTime(void)
	{
		return exec_t;
	}
	/// add a xmlNode to parent xmlNode
	xmlNode * add(const char * parent, char *name)
	{
		xmlNode *it = root->find(parent);
		if (it != NULL)
			return it->add(name, NULL);
		else
		{
			it = root->add(parent, NULL);
			it->add(name, NULL);
		}
		return NULL;
	}
	/// add a xmlNode to parent xmlNode with value
	xmlNode * add(char * parent, char *name, char *value)
	{
		xmlNode *it = root->find(parent);
		if (it != NULL)
			return it->add(name, value);
		else
		{
			it = root->add(parent, NULL);
			it->add(name, value);
		}
		return NULL;
	}
	/// add a xmlNode to parent xmlNode with value as request from info
	xmlNode * add(char* parent, char *name, const char *info, ...)
	{
		char buff[BUFFLENGTH];
		va_list va;
		va_start(va, info);
		vsprintf(buff, info, va);
		va_end(va);
		xmlNode *it = root->find(parent);
		if (it != NULL)
			return it->add(name, buff);
		else
		{
			it = root->add(parent, NULL);
			it->add(name, buff);
		}
		return NULL;
	}
	/// search a specified xmlNode
	inline xmlNode * find(const char *name)
	{
		return root->find(name);
	}
	/// set execution time
	inline  void setExec(long v)
	{
		exec_t = v;
	}
	/// set link a specified parent
	void link(xmlNode* parent)
	{
		char buff[BUFFLENGTH];
		if (root != NULL)
		{
			if (result == 0)
			{
				snprintf(buff, BUFFLENGTH - 1, "Test id=\"%d\"", id);
				root->setName(buff);
				root->add("FailureType", const_cast<char*>("Assertion"));
				root->add("Location.File", const_cast<char*>(file));
				snprintf(buff, BUFFLENGTH - 1, "%d", line);
				root->add("Location.Line", buff);
				root->add("Message", const_cast<char*>(msg));
				parent->link(const_cast<char*>("FailedTests"), root);
			}
			else if (result == 1)
			{
				snprintf(buff, BUFFLENGTH - 1, "Test id=\"%d\"", id);
				root->setName(buff);
				snprintf(buff, BUFFLENGTH - 1, "%ld", exec_t);
				root->add("Doc.Time", buff);
				parent->link(const_cast<char*>("SuccessTests"), root);
			}
		}
	}
	/// print to file XML output
	inline void print(FILE *out)
	{
		root->print(out);
	}
	/// print to file HTML output
	inline void createHTML(FILE *out)
	{
		root->createHTML(out);
	}
};
/////////////////////////////
/// enums for file type
typedef enum docTypes
{
	f_std = 1,  /// std out
	f_txt = 2,  /// file txt type
	f_xml = 4,  /// file xml type
	f_html = 8  /// file html type
} DOCTYPE;


/// public inteface for file output (any type)
class small_doc
{
public:
	/// interface member for open generic file
	virtual void OpenFile(void) = 0;
	/// interface member for closing genereic file
	virtual void CloseFile(void) = 0;
	/// interface member for add information at run time
	virtual void addInfo(const char *info) = 0;
	/// interface merber for formatting a start of document
	virtual void startDocument(const char *info) = 0;
	/// interface merber for formatting the end of document
	virtual void endDocument(const char *info) = 0;
	/// interface merber for add result of test at runtime
	virtual void addResult(const char *info, int f = 0, int timeU = 0) = 0;
	/// interface merber for retrieve the type of document in use
	virtual DOCTYPE getType(void) = 0;
	/// interface merber for retrieve the FILE linked to obj
	virtual FILE  *getFile(void) = 0;
};


/// simple implementation for any type of test file (default extension .txt) its only at intermediate interface
class docFile
	:public small_doc
{
protected:
	DOCTYPE type;   	/// why type is it?
	FILE *file;     	/// file associated
	char nameFile[64]; 	/// name of file
	static int numfile; 	/// static member for crete univoke name of file
public:
	/// default ctor
	docFile()
	{
		file = NULL;
	}
	/// implementation of ...
	virtual void OpenFile(void)
	{
		if (file == NULL)
		{
			file = fopen(nameFile, "w+");
		}
	}
	/// implementation of ...
	virtual void CloseFile(void)
	{
		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}
	}
	/// default destructor
	~docFile()
	{
		CloseFile();
	}
	/// implementation of ...
	inline DOCTYPE getType(void)
	{
		return type;
	}
	/// implementation of ...
	inline FILE *getFile(void)
	{
		return file;
	}
	/// implementation of ...
	virtual void startDocument(const char *info) {}
	/// implementation of ...
	virtual void endDocument(const char *info) {}
};


/// simple specialization for generic text files
class docText
	:public docFile
{
public:
	/// default ctor
	docText(const char *name)
	{
		numfile++;
		if (name == NULL || strlen(name) == 0)
			sprintf(nameFile, "noname_%d", numfile);
		else
			strncpy(nameFile, name, 59);
		strcat(nameFile, ".txt");
		type = f_txt;
	}
	/// implementation of ...
	virtual  void addInfo(const char *info)
	{
		fprintf(file, "%s\n", info);
		fflush(file);
	}
	/// implementation of ...
	virtual void startDocument(const char *info)
	{
		docFile::startDocument(info);
		fprintf(file, "%s:\n", info);
		fflush(file);
	}
	/// implementation of ...
	virtual void endDocument(const char *info)
	{
		docFile::endDocument(info);
		fprintf(file, "%s:\n", info);
		fflush(file);
	}
	/// implementation of ...
	virtual void addResult(const char *info, int f = 0, int timeU = 0)
	{
		if (timeU != -1)
			fprintf(file, "%s:%d ms\n", info, timeU);
		else
			fprintf(file, "%s\n", info);
		fflush(file);
	}
};


/// simple specialization for standard output on stderr
class docStdout
	:public docText
{
public:
	/// default ctor
	docStdout(const char *name) :docText(name)
	{
		nameFile[0] = 0;
		file = stdout;
		type = f_std;
	}
	/// implementation of ...
	virtual void CloseFile(void) {}
};

/// simple specialization for create XML file
class docXML
	:public docFile
{
public:
	/// default ctor
	docXML(const char *name)
	{
		if (name == NULL || strlen(name) == 0)
			sprintf(nameFile, "noname_%d", numfile);
		else
			strncpy(nameFile, name, 59);
		strcat(nameFile, ".xml");
		docFile::OpenFile();
		type = f_xml;
	}
	/// implementation of ...
	void startDocument(const char *info)
	{
		const char msg[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		fprintf(file, "%s", msg);
	}
	/// implementation of ...
	void addInfo(const char *info)
	{
		/*fprintf(file,"<!--%s-->",info);*/
	}
	/// implementation of ...
	void addResult(const char *info, int f = 0, int timeU = 0)
	{
		/*fprintf(file,"<!--%s %d,%d-->",info,f,timeU);*/
	}
};


/// simple specialization for create HTML file
class docHTML
	:public docFile
{
public:
	/// default ctor
	docHTML(const char *name)
	{
		if (name == NULL || strlen(name) == 0)
			sprintf(nameFile, "noname_%d", numfile);
		else
			strncpy(nameFile, name, 59);
		strcat(nameFile, ".html");
		docFile::OpenFile();
		type = f_html;
	}
	/// implementation of ...
	void startDocument(const char *info) {}
	/// implementation of ...
	void addInfo(const char *info)
	{
		/*fprintf(file,"<!--%s-->",info);*/
	}
	/// implementation of ...
	void addResult(const char *info, int f = 0, int timeU = 0)
	{
		/*fprintf(file,"<!--%s %d,%d-->",info,f,timeU);*/
	}
};


/// manager of documents . simple container for reply a message to all documents
class docManager
{
	small_doc ** files;		/// array of small_doc* objects
	int numFile;			/// dimension of array
	char buff[1024];		/// use in formatting input by va_list
public:
	/// default ctor
	docManager(const char *name, int opt)
	{
		numFile = 0;
		files = new small_doc*[sizeof(DOCTYPE)];
		memset(files, 0, sizeof(small_doc)*sizeof(DOCTYPE));
		if (opt == 0 || (opt&f_std) != 0)
			files[numFile++] = new docStdout(name);
		if ((opt&f_txt) != 0)
			files[numFile++] = new docText(name);
		if ((opt&f_xml) != 0)
			files[numFile++] = new docXML(name);
		if ((opt&f_html) != 0)
			files[numFile++] = new docHTML(name);
		for (int i = 0; i < numFile; i++)
		{
			files[i]->OpenFile();
		}
	}
	/// default desctructor
	~docManager()
	{
		for (int i = 0; i < numFile; i++)
		{
			files[i]->CloseFile();
			delete files[i];
		}
		delete[]files;
		numFile = 0;
	}
	/// close all documents
	void close(void)
	{
		for (int i = 0; i < numFile; i++)
		{
			files[i]->CloseFile();
		}
	}
	/// add a info to all documents
	void addInfo(const char *info, ...)
	{
		va_list va;
		va_start(va, info);
		vsprintf(buff, info, va);
		for (int i = 0; i < numFile; i++)
		{
			files[i]->addInfo(buff);
		}
	}
	/// add a result info to all documents
	void addResult(int f, int timeUsed, const char *info, ...)
	{
		va_list va;
		va_start(va, info);
		vsprintf(buff, info, va);
		va_end(va);
		for (int i = 0; i < numFile; i++)
		{
			files[i]->addResult(buff, f, timeUsed);
		}
	}
	/// start  all documents
	void startDocument(const char *info)
	{
		for (int i = 0; i < numFile; i++)
		{
			files[i]->startDocument(info);
		}
	}
	/// end  all documents
	void endDocument(const char *info)
	{
		for (int i = 0; i < numFile; i++)
		{
			files[i]->endDocument(info);
		}
	}
	/// retrieve XML file
	FILE * getXML(void)
	{
		for (int i = 0; i < numFile; i++)
		{
			if (files[i]->getType() == f_xml)
				return files[i]->getFile();
		}
		return NULL;
	}
	/// retrieve HTML file
	FILE * getHTML(void)
	{
		for (int i = 0; i < numFile; i++)
		{
			if (files[i]->getType() == f_html)
				return files[i]->getFile();
		}
		return NULL;
	}

};
//
// interface for test class
class caTester
{
public:
	///  to print class name
	virtual const char * toString(void) = 0;
	///  Run all test into a single test class
	virtual int RunTest(docManager *doc, int * skip) = 0;
	///  register  the test into class
	virtual int RegisterTest(docManager *doc, int *req, int *numreq, int total, int family) = 0;
	///  only for XML output link xmlNode to root node
	virtual void link(xmlNode *parent) = 0;
};

/// test suite , manager of all test class
class caContainerTest
{
protected:
	/// static array of pointer of test class
	static caTester* allTest[MAXNUMCLASSTEST];
	/// number of test class effectively registered
	static int numTest;
	/// numberof total test in the suit , is the summ of all test in the test class
	static int maxTest;
	/// pointer to this class for quick access any where in the program
	static caContainerTest* instance;
	/// the doc manager for that suite
	docManager *docs;
	/// xml manager , this is the main root node for XML output
	infodoc *result;
	/// number of total test for statistics
	int totalTest;
	/// number of failed test for statistics
	int failed;
	/// number of skipped test for statistics
	int skipped;
public:
	/// total number of test assert
	static int totalAssert;
	/// method for get a link to the doc manager for this duite
	inline docManager * getDocManager(void)
	{
		return docs;
	}
	/// method for quick retrieve the suite any where il the program
	static caContainerTest* Instance(void)
	{
		return instance;
	}
	/// stati method for create the suite before the run time
	static caContainerTest* createApp(void)
	{
		instance = new caContainerTest();
		maxTest = MAXNUMCLASSTEST;
		numTest = 0;
		int i;
		for (i = 0; i < maxTest; i++)
			allTest[i] = NULL;
		return instance;
	}
	/// static method for added a single test class to the vector of test class
	static caTester * Addtest(caTester * test)
	{
		if (instance == NULL)createApp();
		if (test != NULL && numTest < maxTest)
		{
			allTest[numTest++] = test;
		}
		else
			printf("ERRORE caTESTER == NULL\n");
		return test;
	}
	/// method for register the class in the suite and prepare the output
	int RegisterTest(const char *name, const char *out, int option, int *testReq, int *numTestReq, int familyReq)
	{
		int res = 0;
		int i;
		totalTest = 0;
		failed = 0;
		skipped = 0;
		char buff[BUFFLENGTH];
		// modificato
		quickSort(allTest, 0, numTest - 1);
		// if(familyReq!=-1)testReq=0;
		try
		{
			docs = new docManager(out, option);
			if ((option&f_xml) != 0)
			{
				result = new infodoc("TestRun", NULL);
				result->getRoot()->setName("TestRun");
				result->add("TestRun", const_cast<char*>("SuiteInfo"));
				result->add("TestRun", const_cast<char*>("FailedTests"));
				result->add("TestRun", const_cast<char*>("SuccessTests"));
				result->add("TestRun", const_cast<char*>("Statistics"));
				result->add("SuiteInfo", const_cast<char*>("Doc"));
				infoSystem::getDate(buff, BUFFLENGTH - 1);
				result->add(const_cast<char*>("Doc"), const_cast<char*>("Date"), buff);
				infoSystem::getTime(buff, BUFFLENGTH - 1);
				result->add(const_cast<char*>("Doc"), const_cast<char*>("Time"), buff);
				infoSystem::getUser(buff, BUFFLENGTH - 1);
				result->add(const_cast<char*>("Doc"), const_cast<char*>("User"), buff);
				infoSystem::getOsName(buff, BUFFLENGTH - 1);
				result->add(const_cast<char*>("Doc"), const_cast<char*>("OS"), buff);
				infoSystem::getDomain(buff, BUFFLENGTH - 1);
				result->add(const_cast<char*>("Doc"), const_cast<char*>("Domain"), buff);
			}
		}
		catch (...)
		{
			printf("No docs avaiable\n");
			exit(-1);
		}
		docs->startDocument("RUNNING");
		try
		{
			docs->addInfo("REGISTER TEST");
			for (i = 0; i < numTest; i++)
			{
				if (allTest[i] == NULL)continue;
				if ((familyReq == -1) || (familyReq == i))
				{
					//docs->addInfo("---------- FAMILY TEST ID = %d ----------",i);
					totalTest += allTest[i]->RegisterTest(docs, testReq, numTestReq, totalTest, i);
				}
				else
					allTest[i] = NULL;
			}
			docs->addInfo("REGISTERED %d TEST", totalTest);
			res = 1;
		}
		catch (...)
		{
			docs->addInfo("TEST SUITE INTERNAL ERROR ON REGISTER TEST");
		}
		return res;
	}
	/// method for sort on test family name
	/// need to univoke family ID on multiplatform
	void quickSort(caTester * list[], int beg, int end)
	{
		caTester* piv;
		caTester* tmp;
		int  l, r, p;
		while (beg < end)    // This while loop will substitude the second recursive call
		{
			l = beg;
			p = (beg + end) / 2;
			r = end;
			piv = list[p];
			while (1)
			{
				while ((l <= r) && (strcmp(list[l]->toString(), piv->toString()) <= 0)) l++;
				while ((l <= r) && (strcmp(list[r]->toString(), piv->toString())  > 0)) r--;
				if (l > r) break;
				tmp = list[l];
				list[l] = list[r];
				list[r] = tmp;
				if (p == r) p = l;
				l++;
				r--;
			}
			list[p] = list[r];
			list[r] = piv;
			r--;
			if ((r - beg) < (end - l))
			{
				quickSort(list, beg, r);
				beg = l;
			}
			else
			{
				quickSort(list, l, end);
				end = r;
			}
		}
	}


	/// method for execute any single test
	int RunTest(int testReq)
	{
		int i, res;
		char bDate[64];
		char bTime[64];
		try
		{
			if (testReq >= 0)
			{
				docs->addInfo("ALL TEST START");
				for (i = 0; i < numTest; i++)
				{
					if (allTest[i] == NULL)continue;
					//failed+=allTest[i]->RunTest(docs,&skipped);
					res = allTest[i]->RunTest(docs, &skipped);
					if (res != 0)
						failed++;
				}
				docs->addInfo("ALL TEST STOP");
				infoSystem::getDate(bDate, 64);
				infoSystem::getTime(bTime, 64);
				docs->addInfo("[%s,%s] SUCCESS %d  -  FAIL %d  -  SKIPPED %d\n", bDate, bTime, totalTest - failed - skipped, failed, skipped);
				docs->addInfo("REAL NUMBER OF TEST %d\n", totalAssert);
			}
		}
		catch (...)
		{
			docs->addInfo("TEST SUITE INTERNAL ERROR ON RUNNING TEST");
		}
		return failed;
	}
	/// method for to collect the final result ( XML mode only, other modes are at run time )
	void FinalizeTest(void)
	{
		int i;
		try
		{
			FILE * out = docs->getXML();
			if (out != NULL)
			{
				for (i = 0; i < numTest; i++)
				{
					if (allTest[i] == NULL)continue;
					allTest[i]->link(result->getRoot());
				}
				result->add(const_cast<char*>("Statistics"), const_cast<char*>("Tests"), "%d", totalTest);
				result->add(const_cast<char*>("Statistics"), const_cast<char*>("FailuresTotal"), "%d", failed);
				if (skipped != 0)
					result->add(const_cast<char*>("Statistics"), const_cast<char*>("SuccessTotal"), "%d", totalTest - failed - skipped);
				else
					result->add(const_cast<char*>("Statistics"), const_cast<char*>("SuccessTotal"), "%d", totalTest - failed);
				result->add(const_cast<char*>("Statistics"), const_cast<char*>("Errors"), const_cast<char*>("0"));
				result->add(const_cast<char*>("Statistics"), const_cast<char*>("Failures"), "%d", failed);
				FILE * out1 = docs->getHTML();
				if (out1 != NULL)
					result->createHTML(out1);
				result->print(out);

			}
		}
		catch (const char *s)
		{
			docs->addInfo("On create XML doc : %s", s);
		}
		catch (...)
		{
			docs->addInfo("Unespected Exception on create XML doc");
		}
		docs->endDocument("END.");
		docs->close();
	}
};


/// any test class  was implemented through MACROS
/// prototype are :
/// class  A
///	:public caTester
///  {
///   CA_TEST_SUITE(A)
///      CA_TEST(p)
///   CA_TEST_SUITE_END()
///   void setUp(void){}
///   void tearDown(void){}
///   void P(void){}
///  };
/// ca test suite is the  body of test class
#define CA_TEST_SUITE(className)\
	protected:\
	static caTester * instance;\
public :\
	const char * toString(void) {return #className;}\
	typedef  void (className::*functor )(void) ;\
	className()\
{\
	numTest=currentTest=0;\
}\
	~className()\
{\
	int i;\
	for(i=0;i<MAXNUMTEST;i++)\
{\
	if(result[i]!=NULL)\
	delete result[i];\
	result[i]=NULL;\
}\
}\
private :\
	functor allTest[MAXNUMTEST];\
	infodoc *result[MAXNUMTEST];\
	int numTest;\
	int currentTest;\
	int printFlg;\
	int offsetSingleTest;\
	void AddTest(functor test,const char *name,const char *brief,docManager *docs,int *req,int *numTestReq,int *Total)\
{\
	if(test!=NULL && numTest<MAXNUMTEST)\
{\
	if(req[offsetSingleTest]==0 || ((*Total+numTest+1)==(req[offsetSingleTest]/*-1*/)))\
{\
	allTest[numTest]=test;\
	result[numTest]=new infodoc(name,brief,(*Total+numTest+1));\
	printFlg=1;\
	docs->addInfo("   %d) - insert test %s",(*Total+numTest+1),/*toString(),*/name);\
	if((*numTestReq)>1)\
{\
	offsetSingleTest++;\
	(*numTestReq)--;\
}\
}\
}\
	else\
{\
	printf("%s Erron on Addtest \n",toString());\
	exit(-1);\
}\
	numTest++;\
}\
	int RunTest(docManager *docs,int * skip)\
{\
	int failed =0;\
	int skipped=0;\
	clock_t start,stop;\
	if(printFlg!=0)docs->addInfo("%s TEST START ",toString());\
	for(currentTest=0;currentTest<numTest;currentTest++)\
{\
	if(allTest[currentTest]==NULL)\
{\
	skipped++;\
	continue;\
}\
	const char * testName = result[currentTest]->find("Name")->getValue();\
	try\
{\
	setUp();\
}\
	catch(EXC_FAIL f)\
{\
	docs->addResult(0,-1,"    - %s - setUp FAIL :%s",testName,f.msg);\
	result[currentTest]->Fail(f.msg,f.file,f.line);\
	failed++;\
	continue;\
}\
	catch(...)\
{\
	docs->addResult(0,-1,"    - %s - ERROR :setUp() Unespected Exception!",testName);\
	result[currentTest]->Fail("setup :Unespected Exception",NULL,-1);\
	failed++;\
	continue;\
}\
	try\
{\
	functor test = allTest[currentTest];\
	start=clock();\
	(this->*test)();\
	stop=clock();\
	docs->addResult(1,(int)(stop-start),"    - %d:%s (%d)- OK :",currentTest,testName,result[currentTest]->id);\
	result[currentTest]->Ok(stop,start);\
}\
	catch(EXC_FAIL f)\
{\
	result[currentTest]->Fail(f.msg,f.file,f.line);\
	docs->addResult(0,-1,"    - %s (%d)- FAILED : %d-:%s",testName,result[currentTest]->id,f.line,f.file);\
	failed++;\
}\
	catch(...)\
{\
	docs->addResult(0,-1,"    - %s (%d)- ERROR :Unespected Exception!",testName,result[currentTest]->id);\
	result[currentTest]->Fail("Unespected Exception",NULL,-1);\
	failed++;\
}\
	try\
{\
	tearDown();\
}\
	catch(EXC_FAIL f)\
{\
	result[currentTest]->Fail(f.msg,f.file,f.line);\
	docs->addResult(0,-1,"    - %s - tearDown FAIL :%s",testName,f.msg);\
	failed++;\
}\
	catch(...)\
{\
	docs->addResult(0,-1,"%s - ERROR :tearDown() Unespected Exception!",testName);\
	result[currentTest]->Fail("Unespected Exception",NULL,-1);\
	failed++;\
}\
}\
	if(printFlg!=0){\
	docs->addInfo("%s TEST END ",toString());\
	docs->addInfo("SUCCESS %d  -  FAIL %d  -  SKIPPED %d",numTest-failed-skipped,failed,skipped);\
	}\
	*skip+=skipped;\
	return failed;\
}\
	void link(xmlNode *parent)\
{\
	for(currentTest=0;currentTest<numTest;currentTest++)\
	if(result[currentTest]!=NULL)\
	result[currentTest]->link(parent);\
}\
	int RegisterTest(docManager *docs,int *testReq,int *numTestReq,int totalTest,int family)\
{\
	for(numTest=0;numTest<MAXNUMTEST;numTest++)\
{\
	allTest[numTest]=(functor)0;\
	result[numTest]=NULL;\
}\
	numTest=0;printFlg=0;offsetSingleTest=0;\
	docs->addInfo("Register tests for class %s: ID = %d",toString(),family);


// any test class was register in the suite, necessary on source file
#define REGISTER_CLASS(className)\
	caTester *className::instance=caContainerTest::Addtest(new className())
// any test method was register into test class
/*
#ifndef _WIN32
#define CA_TEST(memberTest,brief)\
AddTest((functor)(&this->memberTest),#memberTest,#brief,docs,testReq,numTestReq,&totalTest)
#else
#define CA_TEST(memberTest,brief)\
AddTest((functor)(this->memberTest),#memberTest,#brief,docs,testReq,numTestReq,&totalTest)
#endif
*/


#define CA_TEST(memberTest,brief)\
	AddTest((functor)(&memberTest),#memberTest,#brief,docs,testReq,numTestReq,&totalTest)

// macro for close regitration
#define CA_TEST_SUITE_END();		return numTest; }


/// on doc macro.h the implementation of macro for documentation only
#include "docMacro.h"

#endif


