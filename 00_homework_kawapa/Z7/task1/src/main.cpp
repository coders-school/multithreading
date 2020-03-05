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
		promise.set_value(libmail::send_mail(dev, message));
	};

	for (size_t i = 0; i < howMany; i++)
	{
		futures[i] = promises[i].get_future();
        threads[i] = std::thread(function, message, responsible_devs[i], std::move(promises[i]));
	}
	
	for (size_t i = 0; i < howMany; i++)
	{
		std::cout << "Sending mail to: " << responsible_devs[i] << " - "
				  << status(futures[i].get()) << std::endl;
		threads[i].join();
	}
	
	return 0;
}

std::string status(bool b)
{
	if (b)
		return "OK";
	else
		return "FAIL";
}