#include <fstream>
#include <string>

//#ifndef FILE_LEXER
//#define FILE_LEXER

#include "lexer.h"
#include "token.h"

namespace chime
{
	class filelexer : public lexer
	{
	public:
		filelexer(const char *path);
		~filelexer();
		
	protected:
		std::ifstream input_file;
		
		bool is_finished(void);
		void close(void);
		char next_char(void);
		char peek(void);
	};
}

//#endif