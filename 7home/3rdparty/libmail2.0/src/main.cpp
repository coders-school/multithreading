#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <exception>
#include <vector>
#include "../include/randomize_results.h"
#include "../include/send_mail.h"


int main()
{
    std::exception_ptr thread_exception = nullptr;
    std::vector<std::string> v_devs 
    {
        "dev1@company.com",
        "dev2@company.com",
        "dev3@company.com",
        "dev4@company.com",
        "dev5@company.com",
    };

    auto numberOfDevs = v_devs.size();
    
    std::vector<std::thread> v_thread (numberOfDevs);
    std::vector<std::promise<bool>> v_promise (numberOfDevs);
    std::vector<std::future<bool>> v_future (numberOfDevs);

    std::string message = "This is the message";

    auto sendMail = [](const std::string& address, const std::string& message, std::promise<bool> promise)
    {
        try
        {
            libmail::send_mail(address, message);
            promise.set_value(true);
        } 
        catch(...)
        {
            promise.set_exception(std::current_exception());
        }
    };

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < numberOfDevs; ++i)
    {
        v_future[i] = v_promise[i].get_future();
        v_thread[i] = std::thread(sendMail, std::cref(v_devs[i]), std::cref(message), std::move(v_promise[i]));
    }

    for (unsigned int i = 0; i < numberOfDevs; ++i)
    {
        std::cout << "Sending to: " << v_devs[i] << " -- ";
        try
        {
            v_future[i].get();
            v_thread[i].detach();
            std::cout << "Sent" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << "Failed \nReason: ";
            std::cerr << e.what() << '\n';
            v_thread[i].detach();
        }
        
        
    }

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> result = stop - start;
    std::cout << "Time duration: " << result.count() << " ms" << std::endl;

    return 0;

}