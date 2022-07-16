#ifndef __TASK
#define __TASK

class task {
    private:
        int _id;
        int curr;
        int next;

        double* times;
        double dt;
    public:
        task() {
            static int counter=0;
            _id=++counter;
            //time = 0.;
            dt = 0.1;
            times = new double[5];
        };

        ~task(){};

        int get_id () const {
            return _id;
        };

        int get_time () const {
            return times[curr];
        };
        
        void print(){
            std::cout<<"Task ID number " << _id << "\n";
        };

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

#endif
