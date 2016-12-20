#include <csvpp/writer.h>

using namespace csv;




writer::writer(std::ostream &ostream,
               char separator,
               char quote,
               char escape) :
    impl::base(separator, quote, escape),
    _ostream(ostream),
    _first_in_line(true)
{}

