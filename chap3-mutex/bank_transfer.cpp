// private int balance and a mutex
// void deposit(int amount)
// a friend void transfer(BankAccount& from, BankAccount& to, int amount)
//  that locks both accounts deadlock-free and moves the money
// int get_balance() const
#include <mutex>
#include <iostream>
#include <thread>
class BankAccount{


   int balance = 0; 
   mutable std::mutex m;

   public: 

   
   void deposit(int amount){
    std::lock_guard<std::mutex> lk(m);
    balance += amount;
   }

   int get_balance() const{
    std::lock_guard<std::mutex> lk(m);
    return balance;
   }

   friend void transfer(BankAccount& from, BankAccount& to, int amount){
    //  that locks both accounts deadlock-free and moves the money
    if(&from == &to) {return;}
    std::scoped_lock lockaccounts (from.m, to.m);
    from.balance -= amount;
    to.balance += amount;
    }
};

int main(){
    BankAccount acc1;
    BankAccount acc2;
    acc1.deposit(1000);
    acc2.deposit(2000);

    // transfer(acc1,acc2,200);
    // transfer(acc2,acc1,500);
    // int bal1 =  acc1.get_balance();
    // int bal2 = acc2.get_balance();
    // std::cout << bal1 << std::endl;
    // std::cout << bal2 <<std::endl ;
    
    std::thread t1{ [&] () {
        for(int i=0; i<1000; i++){
            transfer(acc1, acc2, 10);
        }
    }}; 
    std::thread t2{ [&] () {
        for(int i=0; i<1000; i++){
            transfer(acc2, acc1, 10);
        }
    }}; 

    if(t1.joinable()){
        t1.join();
    }
    if(t2.joinable()){
        t2.join();
    }

    if(acc1.get_balance() + acc2.get_balance() == 3000){
        std::cout<< "success";
    }
    else{
        std::cout<< acc1.get_balance()+acc2.get_balance();
    }

}