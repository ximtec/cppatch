#include <iostream>
#include "lists/task_list.cpp"
#include "task/patch.cpp"


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

    //int test = 5;

    patch *t;
    for (int i = 1; i< argc; i++){
        std::cout<<argv[i]<< " ";
    }
    std::cout<<"\n";

     task_list t_l = task_list();



    for (int i = 0; i < 10; i++){
        t = (patch *)malloc(sizeof(patch));
        t->init();
        t_l.add_ready_task(t);
    }

    return 0;
}
