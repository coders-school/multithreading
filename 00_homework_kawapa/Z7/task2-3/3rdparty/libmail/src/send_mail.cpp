#include <chrono>
#include <thread>

#include <libmail/send_mail.h>

namespace libmail {

void send_mail(const std::string&, const std::string&)
{
    using namespace std::chrono_literals;
	std::this_thread::sleep_for(1s);

    switch (std::rand() % 3)
    {
        case 0:
            throw std::runtime_error("server error");

        case 1:
            throw std::runtime_error("broken connection");

        case 2:
            return;
    }
}

}