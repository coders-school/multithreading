#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

class PingPongLockFree
{

    atomic<int> _count = 0;
    int _pings = 0;
    int _pongs = 0;
    atomic<bool> _canPong = false;

public:
    PingPongLockFree(int count) : _count(count)
    {}

    void writePing()
    {
        while(_pings < _count)
        {
            if (!_canPong)
            {
                if (_pings == _pongs)
                {
                    ++_pings;
                    stringstream ss;
                    ss << _pings << ". Ping ->" << endl;
                    cout << ss.rdbuf();
                }
                _canPong = true;
            }
        }
    }

    void writePong(int timeout)
    {
        auto start = std::chrono::steady_clock::now();
        while (_pongs < _count)
        {
            if (_canPong)
            {
                if (_pongs < _pings)
                {
                    ++_pongs;
                    stringstream ss;
                    ss << _pongs << ". Pong <-" << endl;
                    cout << ss.rdbuf();
                }
                if (timeout > 0 && std::chrono::steady_clock::now() - start > std::chrono::seconds(timeout))
                {
                    _count = _pings;
                    cout << "Time Out" << endl;
                    break;
                }

                _canPong = false;
            }
        }
    }

};

int main()
{
    int n, t;
    cout << "Enter number of ping pong bounces: ";
    cin >> n;
    cout << "Enter timeout value in seconds (zero for none): ";
    cin >> t;
    cout << "Ready ... Steady ... Go!" << endl;

    PingPongLockFree pingPong(n);

    thread pongThread(&PingPongLockFree::writePong, &pingPong, t);
    thread pingThread(&PingPongLockFree::writePing, &pingPong);

    pingThread.join();
    pongThread.join();

    cout << "Game Over" << endl;

    return 0;
}