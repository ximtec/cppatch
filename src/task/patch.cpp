#ifndef __patch
#define __patch
#include "task.cpp"
#include <vector>
#include <typeinfo>
#include <any>



struct patch_dim{
    int  lb[3];
    int  lo[3];
    int  li[3];
    int  ui[3];
    int  uo[3];
    int  ub[3];
    int  ng[3];
    int   n[3];
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
        std::map <std::string, input_token> input_maps{
            {"n", input_token{dims.n, "int[3]",3 }},
            {"ng",input_token{dims.ng, "int[3]",3 }}
        };
        std::string params_name = "patch_params";
        if (io_verbose >= 2){
            std::cout << "initializing patch " << _id <<  "\n";
        }
        //set default input parameter values
        std::fill_n(dims.n,3,16);
        std::fill_n(dims.ng,3,3);

        io_glob.parse_vars(params_name,input_maps,_id==0);

        //if (io_verbose >= 2){
        //    std::cout << " n = " << dims.n[0] << ", " << dims.n[1] << ", " << dims.n[2] << std::endl;
        //    std::cout << " ng = " << dims.ng[0] << ", " << dims.ng[1] << ", " << dims.ng[2] << std::endl;
        //}


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
