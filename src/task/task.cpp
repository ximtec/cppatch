class task {
    private:
        int _id;
    public:
        task() {
            static int counter=0;
            _id=++counter;
        }


    int get_id(){
        return _id;
    }
};
