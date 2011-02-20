#include <fstream>
#include <string>

//#ifndef _STRING_LEXER_
//#define _STRING_LEXER_

#include "lexer.h"
#include "token.h"

namespace chime
{
	class stringlexer : public lexer
	{
	public:
		stringlexer(const char *input);
		~stringlexer();
		
	protected:
		std::string *text;
		
		bool is_finished(void);
		void close(void);
		char next_char(void);
		char peek(void);
	};
}

//#endif