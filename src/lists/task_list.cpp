
#include <iostream>
#include <algorithm>
#include <list>

#include "link.cpp"

#include "../task/task.cpp"


bool compareId(const link& first, const link& second){
    return (first.task->get_id()  < second.task->get_id());
}

bool compareTime(const link& first, const link& second){
    return (first.task->get_time()  < second.task->get_time());
}


class task_list{
    private:
        std::list<link> ready_list;

    public:
        task_list(){
        };


        ~task_list(){};


        

        void print_list_id(){
            std::for_each(ready_list.begin(),ready_list.end(), [](link& link){
                link.task->print();
                //std::cout<<"ID =  "<< link.task->get_id() << " \n";
            });
        };

        void sort_id(){
            ready_list.sort(compareId);
        };

    void add_ready_task(task* t){
        link new_link;
        new_link.task = t;
        //TODO Add nbor stuff
        ready_list.push_back(new_link);
    };

    void add_ready_link(link link){
        ready_list.push_back(link);
    };

    task* pop_back(){
        link link = ready_list.back();
        task* task = link.task;
        ready_list.pop_back();
        return task;
    }

};
