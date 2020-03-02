#include <exception>
#include <functional>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>

#include <libmail/include/send_mail.h>
#include <libmail/include/randomize_results.h>

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

	start = std::chrono::steady_clock::now();

	// start threads
	for ( std::string str : responsible_devs )
	{
		threads.push_back( std::thread([]( std::string strDev )
			{
				try
				{
					bool bResult = libmail::send_mail(strDev, "message");
					std::stringstream ss;
					ss << "> Sending mail to: " << strDev << " - ";
					ss << ((bResult) ? "OK" : "FAIL");
					ss << std::endl;
					std::cout << ss.str();
				}
				catch( const std::exception& e )
				{
					std::stringstream ss;
					ss << "> Sending mail to: <exception> " << e.what();
					ss << std::endl;
					std::cout << ss.str();
				}	 				
			}, str));
	}

	// join thread
	std::for_each( std::begin( threads ), std::end( threads ), std::mem_fn( &std::thread::join ) );

	stop = std::chrono::steady_clock::now();

	std::cout << "\nSending mails time: " 
		<< std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() 
		<< "ms" << std::endl;
}
