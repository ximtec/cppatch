#include <iostream>
#include "../io/io.cpp"


class data_1d_float{
    protected:
        int n;
        bool host_alloc;
        float* host_ptr;
    
    public:
        data_1d_float(){
            n = 0;
            host_alloc = false;
            host_ptr = NULL;
            std::cout << "Testing data_float constructor" << "\n";
        }

        data_1d_float(int xdim){
            n = xdim;
            host_alloc = false;
            host_ptr = NULL;
            
            if (io_verbose >= 2){
            std::cout << "Initializing data_1d_float with n=" << n  << "host alloc = " << host_alloc << "\n";
            }
            
        }

        float* host_data() {return host_ptr;}
        bool host_present() {return host_alloc;}
        int size() {return n;}

        void init(float val){
            free_arr();
            allocate();
            fill(val);
        }


        void free_arr(){
            if (host_alloc){
                delete[] host_ptr;
            }
            host_ptr = NULL;
            n = 0;
        }

        void allocate(){
            host_ptr = new float[n];

        }

        void fill(float val){
            for(int i=0; i<n; i++){
                host_ptr[i] = val;
            }
        }
};