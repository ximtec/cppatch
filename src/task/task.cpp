#ifndef __TASK
#define __TASK

#include <iostream>
#include <atomic>
#include "../io/io.cpp"

class task_t {
    protected:
        int _id;
        int curr;
        int next;
        int nt;
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
            if (io_verbose){
                std::cout << "Calling constructor for task " << this->_id <<  "\n";
            }
            this->init();
    }

    ~task_t(){
            if (io_verbose){
                std::cout << "Calling destructor for task " << this->_id <<  "\n";
            }
           //delete[] times;
    }
    

    void init(){
        std::string params_name = "task_params";
        this->_id=nr_tasks++;
            if (io_verbose  >= 2 ){
        std::cout << "initializing task " << this->_id <<  "\n";
            }
        dt = 0.1;

        this->nt = io_glob.check_value(params_name,"nt",this->nt) ? this->nt : 5;

        this->times = new double[nt];
        this->curr = 0;
        this->next = 1;
        times[curr] = 0.0;

        
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
