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
    void token::setValue(const char* s)
    {
        _value = s;
    }
    void token::set_value(const char* s)
    {
        this->setValue(s);
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
    void token::append(const std::string& str)
    {
        _value += str;
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
    
    bool token::isString() const
    {
        return this->isPlainString() || this->isInterpolatedStringStart() || this->isInterpolatedStringMiddle() || this->isInterpolatedStringEnd();
    }
    
    bool token::isPlainString(void) const
    {
        return *(_value.begin()) == '"' && *(_value.end()-1) == '"';
    }
    
    bool token::isInterpolatedStringStart() const
    {
        return *(_value.begin()) == '"' && *(_value.end()-2) == '"' && *(_value.end()-1) == '{';
    }
    
    bool token::isInterpolatedStringMiddle() const
    {
        return *(_value.begin()) == '}' && *(_value.begin()+1) == '"' && *(_value.end()-2) == '"' && *(_value.end()-1) == '{';
    }
    
    bool token::isInterpolatedStringEnd() const
    {
        return *(_value.begin()) == '}' && *(_value.begin()+1) == '"' && *(_value.end()-1) == '"';
    }
    
    bool token::isNumber(bool allowFloat) const
    {
        std::string::const_iterator it;
        
        it = _value.begin();
        
        // check for a possible leading '-'
        if ((*it) == '-')
        {
            if (_value.length() == 1)
                return false;
            else
                ++it; // advance past the '-'
        }
        
        // the next spot must be a number character
        if ((*it) < '0' || (*it) > '9')
            return false;
        
        ++it;
        
        for (; it < _value.end()-1; ++it)
        {
            if ((*it) == '.' && !allowFloat)
                return false;
            else
                continue;
            
            if ((*it) < '0' || (*it) > '9')
                return false;
        }
        
        it = _value.end() - 1;
        if ((*it) >= '0' && (*it) <= '9')
            return true;
        
        if ((*it) == 'i' || (*it) == 'j')
            return true;
        
        return false;
    }
    
    bool token::isInteger() const
    {
        std::string::const_iterator it;
        
        it = _value.begin();
        
        if (((*it) == '0') && (*(it+1) == 'x'))
        {
            it += 2;
            
            for (; it < _value.end()-1; ++it)
            {
                if ((*it) >= '0' || (*it) <= '9')
                    continue;
                
                if ((*it) >= 'A' || (*it) <= 'F')
                    continue;
                
                if ((*it) >= 'a' || (*it) <= 'f')
                    continue;
                
                return false;
            }
            
            return true;
        }
        
        // check for a possible leading '-'
        if ((*it) == '-')
        {
            if (_value.length() == 1)
                return false;
            else
                ++it; // advance past the '-'
        }
        
        for (; it < _value.end()-1; ++it)
        {
            if ((*it) < '0' || (*it) > '9')
                return false;
        }
        
        it = _value.end() - 1;
        if ((*it) >= '0' && (*it) <= '9')
            return true;
        
        // if the last character is an i or j, and the length is greater than 1
        if (((*it) == 'i' || (*it) == 'j') && (_value.length() > 1))
            return true;
        
        return false;
    }
    
    bool token::isImaginary() const
    {
        char c;
        
        if (!this->isNumber())
            return false;
        
        c = *(_value.end() - 1);
        
        return c == 'j' || c == 'i';
    }
    
    bool token::isFloatingPoint(void) const
    {
        if (_value.find(".", 0) == std::string::npos)
            return false;
        
        return this->isNumber(true);
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
    
    bool token::isConditional(void) const
    {
        if (_value == "if")     return true;
        if (_value == "unless") return true;
        
        return false;
    }
    
    bool token::isLoop(void) const
    {
        if (_value == "while") return true;
        if (_value == "until") return true;
        if (_value == "loop")  return true;
        
        return false;
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
    
    bool token::isAssignment() const
    {
        if (_value == "=")  return true;
        if (_value == "+=") return true;
        if (_value == "/=") return true;
        if (_value == "*=") return true;
        if (_value == "-=") return true;
        
        return false;
    }
    
    bool token::isLiteral() const
    {
        if (_value == "nil") return true;
        
        return this->isString() || this->isInteger() || this->isNumber() || this->isBoolean() || this->isInheritanceRelated();
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
    
    bool token::isReserved(void) const
    {
        if (this->isControl())            return true;
        if (this->isModifier())           return true;
        if (this->isStructural())         return true;
        if (this->isPunctuation())        return true;
        if (this->isBoolean())            return true;
        if (this->isInheritanceRelated()) return true;
        if (_value == "do")               return true;
        
        return false;
    }
    
    bool token::isEnding() const
    {
        return false;
    }
    bool token::isStatementEnding() const
    {
        return (_value == ";") || (_value == "\n") || this->isBlockEnding();
    }
    bool token::isBlockEnding() const
    {
        return (_value == "}") || (_value == "case") || (_value == "else");
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
        if (_value == ".")    return 60;
        if (_value == "*")    return 50;
        if (_value == "%")    return 50;
        if (_value == "/")    return 50;
        if (_value == "+")    return 40;
        if (_value == "-")    return 40;
        if (_value == "<")    return 30;
        if (_value == ">")    return 30;
        if (_value == "and")  return 20;
        if (_value == "or")   return 20;
        if (_value == "<=>")  return 15;
        if (_value == "==")   return 15;
        if (_value == "===")  return 15;
        if (_value == ">=")   return 15;
        if (_value == "<=")   return 15;
        if (_value == "=")    return 10;
        if (_value == "+=")   return 10;
        if (_value == "*=")   return 10;
        if (_value == "/=")   return 10;
        if (_value == "-=")   return 10;
        
        return -1;
    }
    
    long token::integerValue(void)
    {
        if ((*(_value.begin()) == '0') && (*(_value.begin()+1) == 'x'))
            return strtol(_value.c_str(), NULL, 16);
        
        return strtol(_value.c_str(), NULL, 10);
    }
    std::string token::stringValue(void)
    {
        // needs to be a little smarter
        if (this->isPlainString())
        {
            return std::string(_value.begin()+1, _value.end()-1);
        }
        else if (this->isInterpolatedStringStart())
        {
            return std::string(_value.begin()+1, _value.end()-2);
        }
        else if (this->isInterpolatedStringMiddle())
        {
            return std::string(_value.begin()+2, _value.end()-2);
        }
        else if (this->isInterpolatedStringEnd())
        {
            return std::string(_value.begin()+2, _value.end()-1);
        }
        
        assert(0 && "Trying to get the string_value of a non-string token");
        
        return std::string();
    }
    
    bool token::operator==(const char* cString)
    {
        if (cString == NULL)
            return false;
            
        return this->_value == cString;
    }
    
    bool token::operator!=(const char* cString)
    {
        return this->_value != cString;
    }
}
