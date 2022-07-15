
#include <iostream>

#include "../task/task.cpp"


void test_list(){
    std::cout<< "test list \n";

    task test1 = task();
    printf("1: %i \n", test1.get_id());

    task test2 = task();
    printf("1: %i \n", test2.get_id());
}
