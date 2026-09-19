// void push(T value)
// void wait_and_pop(T& value) — blocks until an item is available
// bool try_pop(T& value) — returns false immediately if empty
// bool empty() const

// Members: a std::queue<T>, a mutable std::mutex, a std::condition_variable.

// Three things to get right: which lock type in which method, 
// where the notify goes relative to the unlock, and the predicate.
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class threadsafe_queue{
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable has_item_cv;

    public:
        void push(T value){
            std::unique_lock<std::mutex> lk(m);
            q.push(value);
            has_item_cv.notify_one();
        }

        void wait_and_pop(T& value){
            std::unique_lock<std::mutex> lk(m);
            has_item_cv.wait(lk, [&]{
                return !(q.empty());
            });
            value = q.front();
            q.pop();
        }

        bool try_pop(T& value){
            std::unique_lock<std::mutex> lk(m);
            if(q.empty()){
                return false;
            }
            else{
                value = q.front();
                q.pop();
                return true;
            }
        }

        bool empty() const{
            std::unique_lock<std::mutex> lk(m);
            return q.empty();
        }
};

int main(){
    threadsafe_queue<int> qu;

}