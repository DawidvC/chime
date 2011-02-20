#include <string>
#include <vector>

#ifndef LEXER
#define LEXER

#include "token.h"

namespace chime
{
	class lexer
	{
	public:
		bool ignore_new_lines;
		
		lexer();
		virtual ~lexer();
		
		token* next_token(void);
		token* look_ahead(unsigned int);
		
		void raise_error(std::string);
		void invalid_token(void);
		
	protected:
		int current_line;
		std::vector<token*> *token_buffer;
		
		token* extract_next_token(void);
		
		virtual bool is_finished(void) = 0;
		virtual char next_char(void) = 0;
		virtual char peek(void) = 0;
		
	private:
		char _peeked_char;
	};
}

#endif
