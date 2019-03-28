 #include <thread>
 
 std::thread f() {
     return std::thread([]{});
 }
 
 int main() {
     auto t = f();   // copy ellision
     // auto t2 = t; // copying not allowed
     auto t3 = std::move(t);  // moving is ok
     t3.join();  // join on t3, not t
     return 0;
 }
