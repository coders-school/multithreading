#include <vector>
#include <future>
#include <iostream>
#include <sstream>
#include <chrono>

#include <libmail/send_mail.h>
#include <libmail/randomize_results.h>

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
	
	std::vector<std::future<bool>> queue;

	auto start = std::chrono::high_resolution_clock::now();
	for (auto &&i : responsible_devs) {
		queue.push_back(
			std::async(std::launch::async, [&i]
				{
					std::stringstream ss;
					bool result  = libmail::send_mail(i, "message");
					std::string r = "FAIL";
					if(result) { r="OK"; } ;

					ss << "Sending to " << i << " " << r;
					ss << std::endl;

					std::cout << ss.str();
					return result;
				}
			)
		);
	}

	for (auto &&i : queue) {
		i.wait();
	};
	
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

	std:: cout << "Sending was done in: " << time.count() << std::endl;
}