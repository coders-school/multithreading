#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <libmail/send_mail.h>
#include <libmail/randomize_results.h>
#include <thread>
#include <vector>

std::string status(bool b);

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

	size_t howMany = responsible_devs.size();
	
	std::vector<std::thread> threads(howMany);
	std::vector<std::promise<bool>> promises(howMany);
	std::vector<std::future<bool>> futures(howMany);

	auto function = [](std::string message, std::string dev, std::promise<bool> promise)
	{
		try {
			libmail::send_mail(dev, message);
            promise.set_value(1);
        } catch(...) {
            promise.set_exception(std::current_exception());
        }
	};

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < howMany; i++)
	{
		futures[i] = promises[i].get_future();
        threads[i] = std::thread(function, message, responsible_devs[i], std::move(promises[i]));
	}
	
	for (size_t i = 0; i < howMany; i++)
	{
		std::cout << "Sending mail to: " << responsible_devs[i] << " - ";

		try {
			std::cout << status(futures[i].get()) << std::endl;
			threads[i].join();
        } catch(std::exception & e) {
			std::cout << e.what() << std::endl;
			threads[i].join();
        } 
	}

    auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> difference = end - start;
    std::cout << "Execution time: " << difference.count() << " ms" << std::endl;
	
	return 0;
}

std::string status(bool b)
{
	if (b)
		return "OK";
	else
		return "FAIL";
}