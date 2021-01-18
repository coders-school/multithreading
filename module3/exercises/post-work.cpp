#include <stdexcept>
#include <future>
#include <complex>
#include <iostream>

int get_number() {
  return 10;
}

void throw_sth() {
    throw std::runtime_error{"Sorry"};
}

// std::future<int> schedule(std::function<int()> func) {
//   std::promise<int> p;
//   std::future<int> f = p.get_future();
//   auto wrapped_func = [func] (std::promise<int> p) {
//     try {
//       p.set_value(func());
//     } catch(...) {
//       p.set_exception(std::current_exception());
//     }
//   };
//   std::thread t(wrapped_func, std::move(p));
//   t.detach();
//   return f;
// }

template <typename Function, typename... Args>
auto schedule(Function&& func, Args&&... args) {
    using ReturnType = std::invoke_result_t<Function, Args&&...>;
    std::promise<ReturnType> p;
    std::future<ReturnType> f = p.get_future();
    auto wrapped_func = [] (std::promise<ReturnType> p, auto f, Args&&... a) {
        try {
            p.set_value(std::invoke(f, std::forward<Args>(a)...));
        } catch(...) {
            p.set_exception(std::current_exception());
        }
    };
    std::thread{wrapped_func, std::move(p), func, std::forward<Args>(args)...}.detach();
    return f;
}

std::complex<int> make_complex(int a, int b) {
  return {a, a - b};
}

int main() {
    auto future = schedule(throw_sth);
    try {
        auto result = future.get();
    } catch(...) {

    }
  //std::cout << result.real() << " " << result.imag();
  return 0;
}
