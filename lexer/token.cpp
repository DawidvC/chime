#include <string>

#include "token.h"

namespace chime
{
    token::token()
    {
        this->clear_value();
    }
    
    token::~token()
    {
    }
    
    void token::set_value(const char c)
    {
        this->clear_value();
        this->append(c);
    }
    void token::set_value(const char* s)
    {
        _value = s;
    }
    void token::clear_value(void)
    {
        _value = "";
    }
    void token::append(const char c)
    {
        if (c > 0)
            _value += c;
    }
    std::string token::value(void) const
    {
        return _value;
    }
    const char* token::c_str(void) const
    {
        return _value.c_str();
    }
    
    bool token::equal_to(const char* s)
    {
        if (s == NULL)
            return false;
            
        return _value.compare(s) == 0;
    }
    
    bool token::empty(void)
    {
        return _value.empty();
    }
    bool token::is_string(void)
    {
        return _value[0] == '"';
    }
    bool token::is_number(void)
    {
        return this->is_floating_point() || this->is_integer();
    }
    bool token::is_integer(void)
    {
        char c;
        
        c = *_value.begin();
        
        return (c >= '0' && c <= '9' );
    }
    bool token::is_floating_point(void)
    {
        char c;
        
        if (!this->is_integer())    return false;
        
        c = *_value.end();
        if (c <= '0' || c >= '9' )  return false;

        return _value.find(".",0) != std::string::npos;
    }
    bool token::is_boolean(void)
    {
        if (_value == "true")   return true;
        if (_value == "false")  return true;
        
        return false;
    }
    bool token::is_modifier(void)
    {
        if (_value == "private")    return true;
        if (_value == "protected")  return true;
        if (_value == "abstract")   return true;
        if (_value == "internal")   return true;
        
        return false;
    }
    bool token::is_structural(void)
    {
        if (_value == "method")         return true;
        if (_value == "class")          return true;
        if (_value == "namespace")      return true;
        if (_value == "import")         return true;
        if (_value == "implementation") return true;
        if (_value == "interface")      return true;
        
        return false;
    }
    bool token::is_conditional(void)
    {
        if (_value == "if")     return true;
        if (_value == "unless") return true;
        
        return false;
    }
    bool token::is_loop(void)
    {
        if (_value == "while") return true;
        if (_value == "until") return true;
        if (_value == "loop")  return true;
        
        return false;
    }
    bool token::is_control(void)
    {
        if (_value == "for")        return true;
        if (_value == "switch")     return true;
        if (_value == "case")       return true;
        if (_value == "break")      return true;
        if (_value == "else")       return true;
        if (_value == "return")     return true;
        if (_value == "yield")      return true;
        if (_value == "next")       return true;
        
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
        
        c = *_value.begin();
        
        return (c >= 'A' && c <= 'Z' );
    }
    bool token::is_punctuation(void)
    {
        if (_value == "[")  return true;
        if (_value == "]")  return true;
        if (_value == "{")  return true;
        if (_value == "}")  return true;
        if (_value == "(")  return true;
        if (_value == ")")  return true;
        if (_value == ";")  return true;
        if (_value == ",")  return true;
        if (_value == ":")  return true;
        if (_value == "|")  return true;
        if (_value == "\n") return true;
        
        return false;
    }
    bool token::is_reserved(void)
    {
        if (this->is_control())     return true;
        if (this->is_modifier())    return true;
        if (this->is_structural())  return true;
        if (this->is_punctuation()) return true;
        if (this->is_boolean())     return true;
        
        return false;
    }
    bool token::is_block_start(void)
    {
        return _value == "do";
    }
    bool token::is_ending(void) const
    {
        return (_value == ";") || (_value == "\n") || (_value == "}");
    }
    int token::precedence(void)
    {
        if (_value == ".")   return 60;
        if (_value == "*")   return 50;
        if (_value == "/")   return 50;
        if (_value == "+")   return 40;
        if (_value == "-")   return 40;
        if (_value == "<")   return 30;
        if (_value == ">")   return 30;
        if (_value == "and") return 20;
        if (_value == "or")  return 20;
        if (_value == "<=>") return 15;
        if (_value == "==")  return 15;
        if (_value == ">=")  return 15;
        if (_value == "<=")  return 15;
        if (_value == "=")   return 10;
        
        return -1;
    }
    
    long token::integer_value(void)
    {
        return strtol(_value.c_str(), NULL, 10);
    }
    std::string token::string_value(void)
    {
        // strips out the leading and trailing quotes
        return std::string(_value.begin()+1, _value.end()-1);
    }
}
