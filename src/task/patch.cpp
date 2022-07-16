#include "task.cpp"

class patch : public task{
    private:

    public: 

    void print(){
        //task::print();
        std::cout<<"patch ID number " << this->get_id() << "\n";
    }


};
