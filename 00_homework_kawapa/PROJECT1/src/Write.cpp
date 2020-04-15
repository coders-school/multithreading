#include "Write.hpp"
#include "Print.hpp"

void writeToBook( const std::string & name, 
                  const std::string & answer, 
                  const int &result, 
                  const int64_t &period, 
                  Book &book )
{
    std::lock_guard<std::shared_mutex> lock(book.mutexBook_);
    book.reflections_.emplace_back(name, answer, result, period);
}