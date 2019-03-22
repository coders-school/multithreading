#include <deque>
#include <shared_mutex>

std::deque<int> ids;
std::shared_mutex mtxIds;

int getIdsIndex() { /* ... */ }
void process(int) { /* ... */ }
int newValue()    { /* ... */ }

void reader() {
    int index = getIdsIndex();
    std::shared_lock<std::shared_mutex> lock(mtxIds);
    int value = ids[index];
    lock.unlock();
    process(value);
}

void writer() {
    int index = getIdsIndex();
    std::lock_guard<std::shared_mutex> lock(mtxIds);
    ids[index] = newValue();
}

int main() {
    return 0;
}

