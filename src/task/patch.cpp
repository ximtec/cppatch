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

class patch : public task{
    protected:
        patch_dim dims;



    public: 

    patch(){
        std::cout << "Calling constructor for patch " << _id <<  "\n";

    };
    ~patch(){
        std::cout << "Calling destructor for patch " << _id <<  "\n";
        //delete[] times;
        
    };

    void init(){
        task::init();
        std::cout << "initializing patch " << _id <<  "\n";


    }
    
    void print(){
        //task::print();
        std::cout<<"patch ID number " << this->get_id() << "\n";
    };

    void deallocate(){
        std::cout << "deleting patch " << _id <<  "\n";
       task::deallocate();
    }
   


};
