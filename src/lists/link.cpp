#include "../task/task.cpp"
#include <list>

struct nbohr{
    task* me;
    task* nbor;


    bool needed;
    bool needs_me;
};


struct link{
    task* task;
    std::list<nbohr> nbohrs;
};

