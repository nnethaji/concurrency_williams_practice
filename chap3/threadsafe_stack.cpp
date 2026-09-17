#include <vector>
#include <mutex>
#include <memory>
#include <exception>
#include <iostream>
#include <thread>

struct empty_stack: std::exception {
   const char* what() const noexcept override { 
       return "empty stack";
   }
};

template <typename T>
class threadsafe_stack{
       std::vector<T> vec;
       mutable std::mutex m; 

public:
 
  void push(T value){
       std::lock_guard <std::mutex> lg(m);   
       vec.push_back(value);   
  } 

 void pop(T& value){  
         // top() and pop()
        std::lock_guard <std::mutex> lg(m);  
        if(vec.empty()) { throw empty_stack(); }
         value = vec.back();  
         vec.pop_back(); 
         
  };

std::shared_ptr<T> pop(){ 
        std::lock_guard <std::mutex> lg(m);
       
        if(vec.empty()) { throw empty_stack(); }
        std::shared_ptr p = std::make_shared<T>(vec.back());  
        vec.pop_back();  
        return p;   
        
 }    

bool empty() const{  
    std::lock_guard<std::mutex> lg(m); 
    return (vec.size() == 0);
}
};

int main() {
    threadsafe_stack<int> s;
    std::vector <std::thread> threads_vec;
    for(int i =0; i<4; i++){
        threads_vec.emplace_back([&]() {
            for(int j=0;j<1000;j++){
                s.push(j);
            }
        });
    } 
    for(auto& eachthread : threads_vec){
        if(eachthread.joinable()){
            eachthread.join();
        }
    }
    int count = 0; 
    while(!s.empty()){
        int x;
        s.pop(x);
        count++;
    }
    std::cout<< count <<std::endl;
    


}