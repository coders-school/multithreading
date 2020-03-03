#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>

// version 1
// #include <libmail/include/send_mail.h>
// #include <libmail/include/randomize_results.h>

// version2
#include <libmail2.0/include/send_mail.h>
#include <libmail2.0/include/randomize_results.h>

decltype(std::chrono::steady_clock::now()) start;
decltype(start) stop;

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

	// ...

	std::vector<std::thread> threads;
	std::vector<std::future<bool>> futures;

	start = std::chrono::steady_clock::now();

	// start threads
	for ( std::string strDev : responsible_devs )
	{
		auto threadSendMail = []( std::string dev, std::string msg, std::promise<bool> promise )
		{
			try
			{
				// version 1.0
				// bool bResult = libmail::send_mail(dev, msg);
				// promise.set_value( bResult );

				// version 2.0
				libmail::send_mail(dev, msg);
				promise.set_value( true );
			}
			catch( ... )
			{
				promise.set_exception( std::current_exception() );
			}
		};

		std::promise<bool> p;
		std::future<bool> f = p.get_future();

		futures.push_back( std::move(f) );
		threads.push_back( std::thread( threadSendMail, strDev, "message", std::move(p) ) );
	}

	// join thread
	std::for_each( std::begin( threads ), std::end( threads ), std::mem_fn( &std::thread::join ) );

	// print results
	for ( unsigned int i = 0; i < responsible_devs.size(); ++i )
	{
		try
		{
			// version 1.0
			// bool bResult = futures[i].get();

			// version 2.0
			bool bResult = true;
			futures[i].get();

			std::cout << "> Sending mail to: " << responsible_devs[i] << " - ";
			std::cout << ((bResult) ? "OK" : "FAIL");
			std::cout << std::endl;
		}
		catch ( const std::exception& e )
		{
			std::cout << "> Sending mail to: " << responsible_devs[i] << " <exception> " << e.what();
			std::cout << std::endl;
		}
	}

	stop = std::chrono::steady_clock::now();

	std::cout << "\nSending mails time: " 
		<< std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() 
		<< "ms" << std::endl;
}
