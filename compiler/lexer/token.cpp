#include <string>

#include "token.h"

namespace chime
{
	token::token()
	{
		value = "";
	}
	
	token::~token()
	{
	}
	
    bool token::equal_to(const char* s)
    {
        if (s == NULL)
            return false;
            
        return value.compare(s) == 0;
    }
	
	bool token::empty(void)
	{
		return value.empty();
	}
	bool token::is_string(void)
	{
		return value[0] == '"';
	}
	bool token::is_number(void)
	{
		return this->is_floating_point() || this->is_integer();
	}
	bool token::is_integer(void)
	{
		char c;
		
		c = *value.begin();
		
		return (c >= '0' && c <= '9' );
	}
	bool token::is_floating_point(void)
	{
		char c;
		
		if (!this->is_integer())	return false;
		
		c = *value.end();
		if (c <= '0' || c >= '9' )	return false;

		return value.find(".",0) != std::string::npos;
	}
	bool token::is_boolean(void)
	{
		if (value == "true")	return true;
		if (value == "false")	return true;
		
		return false;
	}
	bool token::is_modifier(void)
	{
		if (value == "private")		return true;
		if (value == "protected")	return true;
		if (value == "abstract")	return true;
		if (value == "internal")	return true;
		
		return false;
	}
    bool token::is_structural(void)
    {
        if (value == "method")         return true;
        if (value == "class")          return true;
        if (value == "namespace")      return true;
        if (value == "import")         return true;
        if (value == "implementation") return true;
        if (value == "interface")      return true;
        
        return false;
    }
	bool token::is_conditional(void)
	{
		if (value == "if")			return true;
		if (value == "unless")		return true;
		
		return false;
	}
    bool token::is_loop(void)
    {
        if (value == "while") return true;
        if (value == "until") return true;
        if (value == "loop")  return true;
        
        return false;
    }
    bool token::is_control(void)
    {
        if (value == "for")         return true;
        if (value == "switch")      return true;
        if (value == "case")        return true;
        if (value == "break")       return true;
        if (value == "else")        return true;
        if (value == "return")      return true;
        if (value == "yield")       return true;
        if (value == "next")        return true;
        
        if (this->is_loop())        return true;
        if (this->is_conditional()) return true;
        
        return false;
    }
	bool token::is_literal(void)
	{
		return this->is_string() || this->is_number() || this->is_boolean();
	}
    bool token::is_identifier(void)
    {
        if (this->empty())          return false;
        if (this->is_literal())     return false;
        if (this->is_reserved())    return false;
        if (this->precedence() > 0) return false;
        if (this->is_punctuation()) return false;
        if (this->is_type())        return false;
        
        return true;
    }
    bool token::is_type(void)
    {
        char c;
        
        if (this->empty())          return false;
        if (this->is_literal())     return false;
        if (this->is_reserved())    return false;
        if (this->precedence() > 0) return false;
        if (this->is_punctuation()) return false;
        
        c = *value.begin();
        
        return (c >= 'A' && c <= 'Z' );
    }
    bool token::is_punctuation(void)
    {
        if (value == "[")   return true;
        if (value == "]")   return true;
        if (value == "{")   return true;
        if (value == "}")   return true;
        if (value == "(")   return true;
        if (value == ")")   return true;
        if (value == ";")   return true;
        if (value == ",")   return true;
        if (value == ":")   return true;
        if (value == "|")   return true;
        if (value == "\n")  return true;
        
        return false;
    }
	bool token::is_reserved(void)
	{
		if (this->is_control())		return true;
		if (this->is_modifier())	return true;
		if (this->is_structural())	return true;
		if (this->is_punctuation()) return true;
		if (this->is_boolean())		return true;
		
		return false;
	}
    bool token::is_block_start(void)
    {
        return value == "do";
    }
    bool token::is_ending(void) const
    {
        return (value == ";") || (value == "\n") || (value == "}");
    }
	int token::precedence(void)
	{
		if (value == ".")	return 60;
		if (value == "*")	return 50;
		if (value == "/")	return 50;
		if (value == "+")	return 40;
		if (value == "-")	return 40;
		if (value == "<")	return 30;
		if (value == ">")	return 30;
		if (value == "and")	return 20;
		if (value == "or")	return 20;
		if (value == "==")	return 15;
		if (value == ">=")	return 15;
		if (value == "<=")	return 15;
		if (value == "=")	return 10;
		
		return -1;
	}
	
	long token::integer_value(void)
	{
		return strtol(value.c_str(), NULL, 10);
	}
	std::string token::string_value(void)
	{
		// strips out the leading and trailing quotes
		return std::string(value.begin()+1, value.end()-1);
	}
}
