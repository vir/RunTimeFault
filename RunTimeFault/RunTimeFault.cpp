// RunTimeFault.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "cp1251.i"

void dump_stream_flags(std::istream& s)
{
	std::cout << "Pos: " << s.tellg() << " Good: " << s.good() << " Fail: " << s.fail() << " Eof: " << s.eof() << std::endl;
}

class RTFParser
{
public:
	struct ISink
	{
		virtual void GroupOpen() = 0;
		virtual void GroupClose() = 0;
		virtual void Char(int c) = 0;
		virtual void Control(const char * word, long arg) = 0;
	};
public:
	RTFParser(std::istream& stream)
		: m_stream(stream)
		, m_uc(1)
		, m_encoding_table(NULL)
	{
	}
	void set_enc(const long * enc)
	{
		m_encoding_table = enc;
	}
	void parse(ISink& sink)
	{
		while (m_stream.good())
		{
			int c = m_stream.get();
			switch (c)
			{
			case '{': sink.GroupOpen(); break;
			case '}': sink.GroupClose(); break;
			case '\\': parse_escape(sink); break;
			case -1:
				if (m_stream.eof())
					break;
				// else fall thrugh
			default: sink.Char(decode_char(c)); break;
			}
		}
	}
protected:
	void parse_escape(ISink& sink)
	{
		int c = m_stream.get();
		switch (c)
		{
		case '{':
		case '}':
		case '\\':
			sink.Char(c);
			break;
		case 'u':
			if (is_digit(m_stream.peek()))
			{
				m_stream >> c;
				sink.Char(c);
				for (unsigned int i = 0; i < m_uc; ++i)
					m_stream.get();
				break;
			}
			// else fallthrugh!!
		default:
			if (c >= 'a' && c <= 'z')
			{
				std::stringstream ss;
				ss << (char)c;
				while (m_stream.good())
				{
					c = m_stream.peek();
					if (c < 'a' || c > 'z')
						break;
					ss << (char)m_stream.get();
				}
				long arg = -1;
				if (is_digit(c))
					m_stream >> arg;
				if (m_stream.peek() == ' ')
					m_stream.get();
				sink.Control(ss.str().c_str(), arg);
			}
			else
			{
				error("Unsupported control char", c);
			}
		}
	}
	int decode_char(int c)
	{
		if (c & ~0x7F && m_encoding_table)
			return m_encoding_table[c & 0x7F];
		else
			return c;
	}
	void error(const char * msg, char c = 0)
	{
		std::cerr << msg;
		if (c)
			std::cerr << " " << c;
		std::cerr << " at offset " << m_stream.tellg() << std::endl;
	}
	inline bool is_digit(int c)
	{
		return (c >= '0' && c <= '9') || c == '-';
	}
private:
	std::istream& m_stream;
	unsigned int m_uc;
	const long * m_encoding_table;
};

class XmlDumperSink : public RTFParser::ISink
{
public:
	virtual void GroupOpen()
	{
		m_grp_pending = true;
	}
	virtual void GroupClose()
	{
		m_stream << "</" << *m_grpname.rbegin() << ">\n";
		m_grpname.pop_back();
	}
	virtual void Char(int c)
	{
		if ((c & 0xFF) == 0xFF)
		{
			std::cerr << "ops" << std::endl;
		}
		if (m_grp_pending)
			open_group();
		switch (c)
		{
		case '<': m_stream << "&lt;"; break;
		case '>': m_stream << "&gt;"; break;
		case '&': m_stream << "&amp;"; break;
		case '\"': m_stream << "&quot;"; break;
		default: write_wide_char(c); break;
		}
	}
	virtual void Control(const char * word, long arg)
	{
		if (m_grp_pending)
			open_group((word[0] >= 'a' && word[0] <= 'z') ? word : "group", arg);
		else
		{
			m_stream << "<" << word;
			if (arg != -1)
				m_stream << " arg=\"" << arg << "\"";
			m_stream << " /> \n";
		}
	}
public:
	XmlDumperSink(std::ostream& stream)
		: m_stream(stream)
	{
		m_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	}
protected:
	void open_group(const char * name = "group", long arg = -1)
	{
		m_stream << "<" << name;
		if (arg != -1)
			m_stream << " arg=\"" << arg << "\"";
		m_stream << ">";
		m_grpname.push_back(name);
		m_grp_pending = false;
	}
	void write_wide_char(int n)
	{
		if (n & ~0x7F)
		{
			int x = 0;
			unsigned char cbuf[6];
			unsigned char header = 0x80;

			while (n & ~0x3F)
			{
				cbuf[x++] = static_cast<unsigned char>(n & 0x3F);
				n >>= 6;
				header >>= 1;
				header |= 0x80;
			}

			m_stream << char(n | header);
			x--;
			while (x >= 0)
				m_stream << char(cbuf[x--] | 0x80);
		}
		else
			m_stream << char(n);
	}
private:
	std::ostream& m_stream;
	std::vector<std::string> m_grpname;
	bool m_grp_pending;
};

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " filename.rtf [filename.xml]" << std::endl;
		return 10;
	}
	std::ifstream f(argv[1]);
	RTFParser p(f);
	p.set_enc(encoding_1251);
	if (argc == 3)
	{
		std::ofstream o(argv[2]);
		XmlDumperSink w(o);
		p.parse(w);
		o.close();
	}
	else
	{
		XmlDumperSink w(std::cout);
		p.parse(w);
	}
	return 0;
}

