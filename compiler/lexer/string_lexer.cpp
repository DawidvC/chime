#include <cstdio>
#include <string>

#include "string_lexer.h"

namespace chime
{
	stringlexer::stringlexer(const char *input)
	{
		text = new std::string(input);
	}

	stringlexer::~stringlexer()
	{
		delete text;
	}
	
	void stringlexer::close()
	{
	}
	
	char stringlexer::next_char()
	{
		char c;
		
		c = this->peek();
		text->erase(0,1);
		
		return c;
	}
	
	char stringlexer::peek()
	{
		return text->at(0);
	}
	
	bool stringlexer::is_finished()
	{
		return text->empty();
	}
}