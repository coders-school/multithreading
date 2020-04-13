#include <future>
#include <iostream>
#include <libmail2.0/include/send_mail.h>
#include <libmail2.0/include/randomize_results.h>
#include <thread>
#include <vector>

int main()
{
    std::vector<std::future<std::string>> futures;
    std::vector<std::thread> threads;
    libmail::randomize_results();
    std::vector<std::string> responsible_devs = {
        "dev1@company.com",
        "dev2@company.com",
        "dev3@company.com",
        "dev4@company.com",
        "dev5@company.com",
    };
    auto function = [] (std::promise<std::string> promise)
    {
        const std::string placeHolder = "";
        try{
            libmail::send_mail(placeHolder, placeHolder);
            promise.set_value("OK");
        } catch(const std::runtime_error& e){
            promise.set_value(e.what());
        }
    };
    for(int i=0; i < responsible_devs.size(); i++) {
        std::promise<std::string> promise;
        std::future<std::string> future = promise.get_future();
        futures.push_back(std::move(future));
        threads.emplace_back(std::thread(function, std::move(promise)));
    }
    for(int i=0; i < responsible_devs.size(); i++) {
        std::cout << "Sending mail to: " << responsible_devs.at(i) << " - " << futures.at(i).get()  << std::endl;
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
