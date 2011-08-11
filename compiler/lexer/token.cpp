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
    
    bool token::empty(void) const
    {
        return _value.empty();
    }
    bool token::is_string(void)
    {
        return this->isString();
    }
    
    bool token::isString(void) const
    {
        return _value[0] == '"';
    }
    
    bool token::is_number(void)
    {
        return this->isNumber();
    }
    
    bool token::isNumber(void) const
    {
        return this->isFloatingPoint() || this->isInteger();
    }
    
    bool token::is_integer(void)
    {
        return this->isInteger();
    }
    
    bool token::isInteger(void) const
    {
        char c;
        
        c = *_value.begin();
        
        return (c >= '0' && c <= '9' );
    }
    
    bool token::is_floating_point(void)
    {
        return this->isFloatingPoint();
    }
    
    bool token::isFloatingPoint(void) const
    {
        char c;
        
        if (!this->isInteger())
            return false;
        
        c = *_value.end();
        if (c <= '0' || c >= '9' )
            return false;
        
        return _value.find(".",0) != std::string::npos;
    }
    
    bool token::is_boolean(void)
    {
        return this->isBoolean();
    }
    
    bool token::isBoolean(void) const
    {
        if (_value == "true")  return true;
        if (_value == "false") return true;
        
        return false;
    }
    
    bool token::isInheritanceRelated(void) const
    {
        if (_value == "self")  return true;
        if (_value == "super") return true;
        
        return false;
    }
    
    bool token::is_modifier(void)
    {
        return this->isModifier();
    }
    
    bool token::isModifier(void) const
    {
        if (_value == "private")    return true;
        if (_value == "protected")  return true;
        if (_value == "abstract")   return true;
        if (_value == "internal")   return true;
        
        return false;
    }
    
    bool token::isStructural(void) const
    {
        if (_value == "method")         return true;
        if (_value == "class")          return true;
        if (_value == "namespace")      return true;
        if (_value == "import")         return true;
        if (_value == "implementation") return true;
        if (_value == "interface")      return true;
        if (_value == "attribute")      return true;
        
        return false;
    }
    
    bool token::is_conditional(void)
    {
        return this->isConditional();
    }
    
    bool token::isConditional(void) const
    {
        if (_value == "if")     return true;
        if (_value == "unless") return true;
        
        return false;
    }
    
    bool token::is_loop(void)
    {
        return this->isLoop();
    }
    
    bool token::isLoop(void) const
    {
        if (_value == "while") return true;
        if (_value == "until") return true;
        if (_value == "loop")  return true;
        
        return false;
    }
    
    bool token::is_control(void)
    {
        return this->isControl();
    }
    
    bool token::isControl(void) const
    {
        if (_value == "for")            return true;
        if (_value == "switch")         return true;
        if (_value == "case")           return true;
        if (_value == "break")          return true;
        if (_value == "else")           return true;
        if (_value == "return")         return true;
        if (_value == "yield")          return true;
        if (_value == "next")           return true;
        
        if (this->isLoop())             return true;
        if (this->isConditional())      return true;
        if (this->isExceptionRelated()) return true;
        
        return false;
    }
    
    bool token::is_literal(void)
    {
        return this->isLiteral();
    }
    
    bool token::isLiteral(void) const
    {
        return this->isString() || this->isNumber() || this->isBoolean() || this->isInheritanceRelated();
    }
    
    bool token::is_identifier(void)
    {
        return this->isIdentifier();
    }
    
    bool token::isIdentifier(void) const
    {
        if (this->empty())          return false;
        if (this->isLiteral())      return false;
        if (this->isReserved())     return false;
        if (this->precedence() > 0) return false;
        if (this->isPunctuation())  return false;
        if (this->isType())         return false;
        
        return true;
    }
    
    bool token::is_type(void)
    {
        return this->isType();
    }
    
    bool token::isType(void) const
    {
        char c;
        
        if (this->empty())          return false;
        if (this->isLiteral())      return false;
        if (this->isReserved())     return false;
        if (this->precedence() > 0) return false;
        if (this->isPunctuation())  return false;
        
        c = *_value.begin();
        
        return (c >= 'A' && c <= 'Z' );
    }
    
    bool token::is_punctuation(void)
    {
        return this->isPunctuation();
    }
    
    bool token::isPunctuation(void) const
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
        return this->isReserved();
    }
    
    bool token::isReserved(void) const
    {
        if (this->isControl())            return true;
        if (this->isModifier())           return true;
        if (this->isStructural())         return true;
        if (this->isPunctuation())        return true;
        if (this->isBoolean())            return true;
        if (this->isInheritanceRelated()) return true;
        
        return false;
    }
    
    bool token::is_block_start(void)
    {
        return _value == "do";
    }
    bool token::is_ending(void) const
    {
        return this->isEnding();
    }
    bool token::isEnding() const
    {
        return (_value == ";") || (_value == "\n") || (_value == "}");
    }
    bool token::isExceptionRelated(void) const
    {
        if (_value == "try")     return true;
        if (_value == "catch")   return true;
        if (_value == "finally") return true;
        if (_value == "throw")   return true;
        
        return false;
    }
    int token::precedence(void) const
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
