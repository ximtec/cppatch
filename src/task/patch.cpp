#ifndef __patch
#define __patch
#include "task.cpp"
#include <vector>


struct patch_dim{
    uint  lb[3];
    uint  lo[3];
    uint  li[3];

    uint  ui[3];
    uint  uo[3];
    uint  ub[3];
    uint n_g[3];
    uint   n[3];
};

class patch_t : public task_t{
    protected:
        patch_dim dims;



    public: 

    patch_t(){
        std::cout << "Calling constructor for patch " << _id <<  "\n";

    };
    ~patch_t(){
        std::cout << "Calling destructor for patch " << _id <<  "\n";
        //delete[] times;
        
    };

    void init(){
        task_t::init();
        std::cout << "initializing patch " << _id <<  "\n";


    }
    
    void print(){
        //task::print();
        std::cout<<"patch ID number " << this->get_id() << "\n";
    };

    void deallocate(){
        std::cout << "deleting patch " << _id <<  "\n";
       task_t::deallocate();
    }
   


};
#endif
