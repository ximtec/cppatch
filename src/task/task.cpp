#ifndef __TASK
#define __TASK

#include <iostream>
#include <atomic>

class task_t {
    protected:
        int _id;
        int curr;
        int next;

        double* times;
        double dt;

        static int nr_tasks;
    public:
        //task();
        //virtual ~task();
        //int get_id (void);
        //int get_time (void);
        //void print(void);


    task_t(){
            std::cout << "Calling constructor for task " << _id <<  "\n";
            this->init();
    }

    ~task_t(){
            std::cout << "Calling destructor for task " << _id <<  "\n";
           //delete[] times;
    }

    void init(){
        _id=nr_tasks++;
        std::cout << "initializing task " << _id <<  "\n";
        //time = 0.;
        dt = 0.1;
        times = new double[5];
    }

    void deallocate(){
        std::cout << "deleting task " << _id <<  "\n";
        delete [] times;
    }


    int get_id (void) {
        return _id;
    }
    int get_time (void) {
        //return times[curr];
        return 0.0;
    }
    
    void print(void){
        std::cout<<"Task ID number " << _id << "\n";
    }
       // bool operator <(const task& rhs)
       // {
       //     return this->_id < rhs._id;
       // };
//
       // bool operator +(const task& rhs)
       // {
       //     return this->_id + rhs._id;
       // };
//
       // bool operator -(const task& rhs)
       // {
       //     return this->_id - rhs._id;
       // };
};

int task_t::nr_tasks = 0;




#endif
