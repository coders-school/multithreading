#include <vector>
#include <iostream>
#include <future>

#include <libmail/send_mail.h>
#include <libmail/randomize_results.h>

std::string print_result(bool result) {
    return result ? " - TRUE\n" : " - FAIL\n";
}

int main()
{
	libmail::randomize_results();
	std::vector<std::string> responsible_devs = {
		"dev1@company.com",
		"dev2@company.com",
		"dev3@company.com",
		"dev4@company.com",
		"dev5@company.com",
	};
	std::string message = "build failed";

    auto size = responsible_devs.size();

    std::vector<std::promise<bool>> promises(size);
    std::vector<std::future<bool>> futures(size);
    std::vector<std::thread> threads(size);

    auto send_func = [](std::string address,
                        std::string message,
                        std::promise<bool> promise) {
        try {
            libmail::send_mail(address, message);
            promise.set_value(true);
        } catch(...) {
            promise.set_value(false);
        }
    };

    for(long unsigned int i = 0; i < size; i++) {
        futures[i] = promises[i].get_future();
        threads[i] = std::thread(send_func, responsible_devs[i], message, std::move(promises[i]));
    }

    for(long unsigned int i = 0; i < size; i++) {
        std::cout << "Sending mail to: " << responsible_devs[i] << print_result(futures[i].get());
    }

    for(auto& thread : threads) {
        thread.join();
    }
}
