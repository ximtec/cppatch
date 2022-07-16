#include <iostream>
#include "lists/task_list.cpp"
#include "task/patch.cpp"

int main(int argc, char const *argv[]) {
    for (int i = 1; i< argc; i++){
        std::cout<<argv[i]<< " ";
    }
    std::cout<<"\n";

    task_list t_l = task_list();

    task t1;; 
    patch p1;  

    t1.print();
    p1.print();

    //task *p1_ptr;
    //p1_ptr = &p1;


    t_l.add_ready_task(t1);

    t_l.add_ready_task(p1);


    //t_l.print_list_id();

    t_l.print_list_id();


    patch *p2_ptr;

    p2_ptr = (patch * ) t_l.pop_back();



    p2_ptr->print();
    //std::cout<< " " << p1.get_id() << "\n";

    //p1.print();
    //test_list();
    return 0 ;
}
