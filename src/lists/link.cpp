#include "../task/task.cpp"
#include <list>

struct nbohr_t{
    task_t* me;
    task_t* nbor;


    bool needed;
    bool needs_me;
};


struct link_t{
    task_t* task;
    std::list<nbohr_t> nbohrs;
};

