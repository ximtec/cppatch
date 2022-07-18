
#include <iostream>
#include <algorithm>
#include <list>

#include "link.cpp"

#include "../task/patch.cpp"


bool compareId(const link_t& first, const link_t& second){
    return (first.task->get_id()  < second.task->get_id());
}

bool compareTime(const link_t& first, const link_t& second){
    return (first.task->get_time()  < second.task->get_time());
}


class task_list_t{
    private:
        std::list<link_t> ready_list;

    public:
        task_list_t(){
        };


        ~task_list_t(){};


        

        void print_list_id(){
            std::for_each(ready_list.begin(),ready_list.end(), [](link_t& link){
                link.task->print();
                //std::cout<<"ID =  "<< link.task->get_id() << " \n";
            });
        };

        void sort_id(){
            ready_list.sort(compareId);
        };

    void add_ready_task(task_t* t){
        link_t new_link;
        new_link.task = t;
        //TODO Add nbor stuff
        ready_list.push_back(new_link);
    };

    void add_ready_link(link_t link){
        ready_list.push_back(link);
    };

    task_t* pop_back(){
        link_t link = ready_list.back();
        task_t* task = link.task;
        ready_list.pop_back();
        return task;
    }

    void dealloc_list(){
        std::for_each(ready_list.begin(),ready_list.end(), [](link_t& link){
                ((patch_t * )link.task)->deallocate();
            });
        ready_list.clear();
    }

};
