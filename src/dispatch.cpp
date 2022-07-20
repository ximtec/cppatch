#include <iostream>
#include "lists/task_list.cpp"
#include "task/patch.cpp"
#include "io/io.cpp"

class test{
    private:
        int val;

    public:
    test() { std::cout << "In constructor\n"; val= 5; }
    ~test() { std::cout << "In destructor\n"; }
    void print(){std::cout << "test test test " << val << "\n" ;}
    void init(){val = 5;}

};

int main(int argc, char const *argv[]) {
;
    //int test = 5;

//    patch *t;
    for (int i = 1; i< argc; i++){
        std::cout<<argv[i]<< " ";
    }
    std::cout<<"\n";

    task_list_t t_l = task_list_t();

    //io_glob.read_input("input.nml");
    io_glob.read_regex("input.nml");


    int nt =  io_glob.check_value("task_params","nt",nt) ? nt : -1;
    
    std::cout << "nt = " << nt << std::endl;
//
//    for (int i = 0; i < 10; i++){
//        t = (patch *)malloc(sizeof(patch));
//        t->init();
//        t_l.add_ready_task(t);
//    }
//
//    t_l.print_list_id();
//
//
//    t_l.dealloc_list();

    return 0;
}
