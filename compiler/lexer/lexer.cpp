#include "lexer.h"
#include "eof_token.h"

#include <cstdio>
#include <string>
#include <assert.h>

namespace chime
{
    lexer::lexer()
    {
        _current_line = 1;
        this->ignore_new_lines(true);
        token_buffer = new std::vector<token*>();
        
        _peeked_char         = 0;
        _inString            = false;
        _interpolatingString = false;
    }
    
    lexer::~lexer()
    {
        delete token_buffer;
    }
    
    token* lexer::next_token(void)
    {
        token* t;
        
        // get another token - makes sure there is always at least one
        token_buffer->push_back(this->extract_next_token());
        
        t = token_buffer->front();
        
        // remove the first token, which is now out of date
        token_buffer->erase(token_buffer->begin());
        
        if (t == NULL)
            t = new eof_token();
        
        if (t->equal_to("\n"))
            _current_line++;
        
        return t;
    }
    
    token* lexer::look_ahead(unsigned int distance)
    {
        token* t;
        int    i;
        
        // we only need to scan ahead if there isn't enough
        // in the buffer
        for (i = (distance - token_buffer->size()); i > 0; i--)
        {
            token_buffer->push_back(this->extract_next_token());
        }
        
        assert(distance - 1 < token_buffer->size());
        
        t = (*token_buffer)[distance-1];
        
        if (t == NULL)
            t = new eof_token();
        
        return t;
    }
    
    bool lexer::ignore_new_lines() const
    {
        return _ignore_new_lines;
    }
    void lexer::ignore_new_lines(bool i)
    {
        _ignore_new_lines = i;
    }
    
    int lexer::current_line(void) const
    {
        return _current_line;
    }
    
    token* lexer::extract_next_token(void)
    {
        token* t = new token();
        char c;
        
        while (!this->is_finished())
        {
			// handle a peeked character, which
			// will always be a dot
			if (_peeked_char != 0)
			{
				c = _peeked_char;
				_peeked_char = 0;
				
                t->append('.');
				
				return t;
			}
			else
				c = this->peek();
            
            // handle strings
            if (_inString && !_interpolatingString)
            {
                switch (c)
                {
                    case '"':
                        t->append(this->next_char());
                        
                        _inString            = false;
                        _interpolatingString = false;
                        return t; // closed string
                    case '}':
                        t->append(this->next_char());
                        t->append('"');
                        break;
                    case '{':
                        t->append('"');
                        t->append(this->next_char());
                        _interpolatingString = true;
                        return t;
                    case '\\':
                        this->next_char(); //  the '/'
                        t->append(this->escapeCodeToChar(this->next_char()));
                        break;
                    default:
                        t->append(this->next_char());
                        break;
                }
                
                continue;
            }
            
			// handle everything else
			switch (c)
			{
				case ' ':
				case '\t':
					if (!t->empty())
						return t;
					else
						this->next_char();
					break;
                case '\n':
                    if (_ignore_new_lines)
                    {
                        _current_line++;
                        
                        if (t->empty())
                            this->next_char();
                        else
                            return t;
                    }
					else
					{
						if (t->empty())
                            t->set_value(this->next_char());
                        
						return t;
					}
					break;
				case '.':
                    // if we have nothing so far, then we have a dot
                    if (t->empty())
                    {
                        t->set_value(this->next_char());
                        
                        return t;
                    }
                    
                    // we've got "7.8."
                    if (t->isFloatingPoint())
                        return t;
                    
                    // here's the tricky case.  if we have
                    // a number so far, then we might have
                    // a floating point definition,
                    // or we might have a method call on an
                    // integer
                    // "7." -> "7.8", "7..", "7.times"
                    if (!t->isInteger())
                        return t;
                    else
                    {
                        // crazy ugly hack
                        this->next_char(); // advance past the dot
                        _peeked_char = this->peek();
                        
                        // check for a number
                        if ((_peeked_char < '0') || (_peeked_char > '9'))
                        {
                            return t;
                        }
                        else
                        {
                            _peeked_char = 0;
                            t->append('.');
                            t->append(this->next_char());
                        }
                    }
                    
                    break;
                case '{':
                    assert(!_interpolatingString && "Open braces are not allowed within a string interpolation");
                    
                    if (t->empty())
                        t->set_value(this->next_char());
                    
                    return t;
                    break;
                case '}':
                    if (_interpolatingString)
                    {
                        assert(_inString);
                        _interpolatingString = false; // toggle state
                        if (t->empty())
                            continue;  // this is a weird case.  We do not want to swallow the }
                    }
                    
                    if (t->empty())
                        t->set_value(this->next_char());
                    
                    return t;
                    break;
                case '(':
                case ')':
                case ';':
                case ':':
                case ',':
                case '|':
                case '[':
                case ']':
                    if (t->empty())
                        t->set_value(this->next_char());
                    
                    return t;
                    break;
                case '+':
                case '*':
                case '<':
                case '>':
                case '=':
                    if (t->empty())
                    {
                        // handle '<=', '>=', '==', '+=', '*='
                        t->set_value(this->next_char());
                        if (!this->is_finished() && (this->peek() == '='))
                            t->append(this->next_char());
                        
                        // handle '<=>'
                        if (!this->is_finished() && (this->peek() == '>') && (t->equal_to("<=")))
                            t->append(this->next_char());
                        
                        // handle '==='
                        if (!this->is_finished() && (this->peek() == '=') && (t->equal_to("==")))
                            t->append(this->next_char());
                    }
                    
                    return t;
                    break;
                case '/':
                    this->next_char();
                    if (this->peek() == '/')
                    {
                        char next;
                        do
                        {
                            c = this->next_char();
                            next = this->peek();
                        }
                        while ((next != '\n') && (next != 0));
                    }
                    else if (this->peek() == '=')
                    {
                        t->set_value('/');
                        t->append(this->next_char());
                        return t;
                    }
                    else
                    {
                        t->set_value('/');
                        return t;
                    }
                    break;
                case '-':
                    _peeked_char = this->peek();
                    
                    if ((_peeked_char < '0') && (_peeked_char > '9')) // check for a number
                    {
                        return t;
                    }
                    
                    t->append(this->next_char());
                    if (!this->is_finished() && (this->peek() == '='))
                        t->append(this->next_char());
                    
                    _peeked_char = 0;
                    break;
                case '"':
                    if (_inString)
                        return t;
                    
                    t->set_value(this->next_char());
                    
                    _inString = true;
                    break;
                default:
                    t->append(this->next_char());
                    break;
            }
        }
        
        if (t->empty())
        {
            free(t);
            return NULL;
        }
        
        return t;
    }
    
    char lexer::escapeCodeToChar(char c)
    {
        switch (c)
        {
            case 'n':  return '\n'; break;
            case 'r':  return '\r'; break;
            case 't':  return '\t'; break;
            case '\\': return '\\'; break;
        }
        
        assert(0 && "Unhandled string escape code");
        
        return 0;
    }
}
