#include <csvpp/parser.h>

using namespace csv;

parser::parser(std::istream &istream,
               char separator,
               char quote,
               char escape) :
    impl::base(separator, quote, escape),
    _istream(istream),
    _linenumber(0),
    _colnumber(0),
    _list_separator(escape, separator, quote),
    _tokenizer(0)
{
    _getline();
}

void parser::_getline()
{
    std::getline(_istream, _line);
    delete _tokenizer;
    _tokenizer = new tokenizer(_line, _list_separator);
    _iterator = _tokenizer->begin();
    ++_linenumber;
    _colnumber = 1;
}

