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
        //std::map <std::string,std::pair<const std::type_info &, void*>> maps = {
        //    {"n" ,  {typeid(dims.n), dims.n }},
        //    {"ng" , {typeid(int), dims.ng}}
        //    //{"ng" , {typeid(dims.ng), &dims.ng}}
        //};

        std::map <std::string, std::any> maps{
            {"n", dims.n},
            {"ng", dims.ng}
        };

        std::string params_name = "patch_params";

        if (io_verbose >= 2){
            std::cout << "initializing patch " << _id <<  "\n";
        }


        std::fill_n(dims.n,3,16);
        std::fill_n(dims.ng,3,3);

        std::for_each(maps.begin(),maps.end(), [params_name] (std::pair<std::string,std::any> pair){
                if ( pair.second.type() == typeid(int*)){
                    int* val = std::any_cast<int*>(pair.second);
                    bool read_succ = io_glob.check_value(params_name,pair.first, val);

                    if ( read_succ){
                        if (io_verbose >= 2){
                            std::cout << "Read input variables for value " << pair.first << " " << val[0] << " " << val[1] << " " << val[2] << std::endl;
                        }
                    }

                } 
                
        });

        if (io_verbose >= 2){
            std::cout << " n = " << dims.n[0] << ", " << dims.n[1] << ", " << dims.n[2] << std::endl;
            std::cout << " ng = " << dims.ng[0] << ", " << dims.ng[1] << ", " << dims.ng[2] << std::endl;
        }


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
