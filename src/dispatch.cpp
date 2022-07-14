#include <iostream>
#include "lists/task_list.cpp"

int main(int argc, char const *argv[]) {
    for (int i = 0; i< argc; i++){
        std::cout<<argv[i]<< " ";
    }
    std::cout<<"\n";

    test_list();
    return 0 ;
}