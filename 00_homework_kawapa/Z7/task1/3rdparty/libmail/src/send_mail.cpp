#include <chrono>
#include <thread>
#include <libmail/send_mail.h>

namespace libmail {

bool send_mail(const std::string&, const std::string&)
{
    using namespace std::chrono_literals;
	std::this_thread::sleep_for(1s);

    return std::rand() % 2;
}

}