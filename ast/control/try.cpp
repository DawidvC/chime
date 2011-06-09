// chime: try.cpp

#include "ast/control/try.h"
#include "parser/parser.h"

namespace ast
{
    Try::Try(chime::parser& parser)
    {
        _catchBlocks  = new std::vector<ast::Catch*>();
        _finallyBlock = NULL;
        
        parser.next_token("try");
        
        // parse the stuff inside
        this->parseBody(parser);
        
        // when we encounter a try, we must find
        // 1 or more catches and/or a finally
        
        this->parseCatchesAndFinally(parser);
    }
    
    Try::~Try()
    {
        delete _catchBlocks;
        
        if (_finallyBlock)
            delete _finallyBlock;
    }
    
    std::string Try::nodeName(void)
    {
        return std::string("try");
    }
    
    std::string Try::stringRepresentation(int depth)
    {
        std::vector<ast::node*>::iterator i;
        std::string                       str;
        
        str.append(depth*2, ' ');
        str.append(this->nodeName());
        
        for (i = this->children()->begin(); i < this->children()->end(); ++i)
        {
            str.append("\n");
            
            if ((*i) == NULL)
            {
                str.append(depth+1, ' ');
                str.append("(null)\n");
            }
            else
            {
                str.append((*i)->stringRepresentation(depth+1));
            }
        }
        
        std::vector<ast::Catch*>::iterator j;
        
        for (j = _catchBlocks->begin(); j < _catchBlocks->end(); ++j)
        {
            str.append("\n");
            str.append((*j)->stringRepresentation(depth));
        }
        
        if (_finallyBlock)
        {
            str.append("\n");
            str.append((*j)->stringRepresentation(depth));
        }
        
        return str;
    }
    
    std::vector<ast::Catch*>* Try::getCatchBlocks(void) const
    {
        return _catchBlocks;
    }
    
    void Try::addCatchBlock(ast::Catch* node)
    {
        _catchBlocks->push_back(node);
    }
    
    ast::Finally* Try::getFinallyBlock(void) const
    {
        return _finallyBlock;
    }
    
    void Try::setFinallyBlock(ast::Finally* node)
    {
        _finallyBlock = node;
    }
    
    void Try::parseCatchesAndFinally(chime::parser& parser)
    {
        while (true)
        {
            chime::token* t;
            
            parser.advance_past_ending_tokens();
            
            t = parser.look_ahead();
            if (t->is_ending())
                break;
            
            if (t->equal_to("catch"))
            {
                this->addCatchBlock(new ast::Catch(parser));
            }
            else if (t->equal_to("finally"))
            {
                this->setFinallyBlock(new ast::Finally(parser));
                break;
            }
            else
            {
                break;
            }
        }
        
        parser.advance_past_ending_tokens();
        
        if ((this->getCatchBlocks()->size() == 0) && (this->getFinallyBlock() == NULL))
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("a try statement also requires either a catch or finally");
            
            parser.add_error(e);
        }
    }
}