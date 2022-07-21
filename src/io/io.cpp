#ifndef __IO
#define __IO

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex> 
#include <map>
#include <stdexcept>
#include <any>

#define GET_NAME(val) (#val)

int io_verbose;

const std::string WHITESPACE = " \n\r\t\f\v";
 
 //Check if a character is whitespace
 bool is_whitespace(const char val){
    return WHITESPACE.find(val) != std::string::npos;
 }


//Parse string with bool value to bool
void parse_value(const std::string &name, const std::string &s, bool &val){
    if (not( s=="T" || s=="F")){
        throw std::invalid_argument("Could not cast variable " + name + " with input '" + s + "' to bool \n \t\t --- Must be either 'T' or 'F' ");
    }else{
        val =  s=="T";
    }
}
//Parse string with integer value to integer
void parse_value(const std::string &name, const std::string &s, int &val){
    std::stringstream ss(s);
    if ((ss >> val).fail() || !(ss >> std::ws).eof())
    {
        if (s == "T") { val = 1; return ;}
        if (s == "F") { val = 0; return ;}
        throw std::invalid_argument("Could not cast variable " + name + " with input '" + s + "' to int");
    }

}
//Parse string with integer array to int[3] array
//If only 1 value is given, assumes same in each dimension
//if only 2 values are given, assumes last dimension is 1
void parse_value(const std::string &name, const std::string &s, int* val){
    std::string tmp_str = s;
    std::vector<std::string> dim_vals;    
    size_t pos = 0;
    std::string token;
    while ((pos = tmp_str.find(',')) != std::string::npos) {
        token = tmp_str.substr(0, pos);
        tmp_str.erase(0, pos + 1);
        dim_vals.push_back(token);
    }
    dim_vals.push_back(tmp_str);


    if (dim_vals.size() > 3 || dim_vals.size() < 1){
        throw std::invalid_argument("Could not cast variable " + name + " with input '" + s + "' to int[3] \n \t\t ---- Takes between 1-3 arguments: fx. 16,16,16 or 16,16 or 16");
    }

    int i = 0;
    int tmp_val;

    std::for_each(dim_vals.begin(), dim_vals.end(), [&name,&i,&val,&tmp_val] (const std::string &s_val){
        int tmp_val;
        parse_value(name, s_val, tmp_val);
        val[i] = tmp_val;
        i++;
    });

    if (i == 1){ //if only 1 value is given, assume 3D with equal size in each dimension
        val[1] = val[0]; 
        val[2] = val[0]; 
    }else if (i == 2) //if only 2 values are given, assume 2D with third dimension being 1
    {
        val[2] = 1;
    }
}


 //remove trailing whitespace from left
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
//remove trailing whitespace from right
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
//remove trailing whitespace in left and right
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}
//Remove multiple occurences 
std::string prune(const std::string &s){
        std::string new_String = s;
        auto it = std::unique(new_String.begin(), new_String.end(),
                    [](char const &lhs, char const &rhs) {
                        return (lhs == rhs) && (is_whitespace(lhs)) ;
                    });
        new_String.erase(it, new_String.end());
        return new_String;
}


//Remove trailing whitespaces and multiple occurences of whitespace in a row
//Does not remove ALL whitespaces from the string
std::string remove_whitespaces(const std::string &s){
    return prune(trim(s));
}




class io_t{
    private:
        std::string input_file;

        std::string param_name = "io_params";

        std::map<std::string, std::map<std::string,std::string> > params;

    public:

    io_t(){};

    ~io_t(){};

    //Read file input file, ignoring comments and removing redundant whitespaces
    std::string read_file(){
        std::ifstream fin(this->input_file);
        std::string str;
        std::string line;
        //load each line, remove comments, and remove redundant whitespaces
        while (std::getline(fin,line)){
                //remove comments
                size_t start = line.find_first_of('#'); 
                line =  (start == std::string::npos) ? line : line.substr(0,start);
                //remove redundant whitespace
                line = remove_whitespaces(line);
                str += line + "\n";
        }
        return str;
    }


    //Find parameter section in string, starting with '&' and ending with ';'
    void find_params(std::string file, std::vector <std::string> &matches){
        std::regex regxp("&[a-z_A-Z0-9 =\n,.&]*?;");
        std::smatch res;
        std::string::const_iterator searchStart( file.cbegin() );
        while ( std::regex_search( searchStart, file.cend(), res, regxp ) )
        {
            std::string match =  res[0];
            std::replace(match.begin(),match.end(),'\n',' '); //change newline to space
            std::replace(match.begin(),match.end(),';',' ');  //remove the ; and replace with space
            match = remove_whitespaces(match); //remove trailing whitespac
            searchStart = res.suffix().first;
            matches.push_back(match);
        }
    }

    //find the name for a set of parameters - fx. task_params
    std::string get_param_class_name(std::string param_list){
        std::regex name_reg("&[a-zA-Z]+_params");
        std::smatch res;
        std::string::const_iterator searchStart( param_list.cbegin() );
        int i = 0;
        std::string name;
        while(std::regex_search( searchStart, param_list.cend(), res, name_reg )){
            name = res[0];
            name.erase(0,1);
            searchStart = res.suffix().first;
            i++;
        }
        if (i != 1){
            throw std::invalid_argument("More than 1 name for parameter given: \n\t " + param_list);
        }
        return name;
    }

    //Find each variable name/value for a given parameter set
    void get_param_class_val(std::string param_list,std::map<std::string,std::string> &param_dict){
        //std::regex param_reg("[a-zA-Z]+[ ]*=[ ]*[a-zA-Z0-9.]+(?:,[ ]*[a-zA-Z0-9.]+)*");
        std::regex param_reg("[a-zA-Z]+ *= *[0-9TF.]*[ *, *[0-9]*]*");
        std::string::const_iterator searchStart =  param_list.cbegin();
        std::smatch res;
        //loop over each regex match
        while(std::regex_search( searchStart, param_list.cend(), res, param_reg )){
            std::string param = res[0];
            searchStart = res.suffix().first;
            //split string at '=' sign
            size_t equal_pos = param.find_first_of('='); 
            std::string param_name = param.substr(0,equal_pos);
            std::string param_val  = param.substr(equal_pos+1);
            //remove all whitespaces from variable name
            auto it1 = std::remove(param_name.begin(),param_name.end(),' ');
            param_name.erase(it1, param_name.end());
            //remove all whitespaces from variable value
            auto it2 = std::remove(param_val.begin(),param_val.end(),' ');
            param_val.erase(it2, param_val.end());
            //insert name/value pair into dictionary
            param_dict.insert(std::pair<std::string,std::string>(param_name,param_val));
        }
    }

    //Print all parameters parameters loaded by the io input parsing
    void print_all_params(){
        std::for_each(this->params.begin(),this->params.end(), [](std::pair<std::string,std::map<std::string,std::string>> pair){
            std::cout << pair.first << ":" << std::endl;
            std::for_each(pair.second.begin(),pair.second.end(), [](std::pair<std::string,std::string> val){
                std::cout<< "\t\t" << val.first << " = " << val.second << std::endl;

            });
        });
    }



    //Parse input and store in a dictionary containing each set of parameters, and each variable name/value in each set
    void parse_input(){
        std::string file_content = this->read_file();
        //find each param list beginning with &xxx_params and ending with a ';'
        std::vector <std::string> matches;
        this->find_params(file_content,matches);
        //Find the name for each match, and each value given for that class param
        //Then load everything into the global dictionary
        for (auto match : matches){
            std::string class_param_name = this->get_param_class_name(match);
            std::map<std::string,std::string> params_dict;
            this->get_param_class_val(match,params_dict);
            this->params.insert(std::pair<std::string, std::map<std::string,std::string>>(class_param_name, params_dict));
        }



    }

    //initialize io
    void init(std::string  file_name){
        std::string params_name = "io_params";

        this->input_file = file_name;
        this->parse_input();

        io_verbose = this->check_value(params_name,"verbose",io_verbose) ? io_verbose : 0;
        //TODO update input parsing

        if (io_verbose >= 1) {
            this->print_all_params();
        }

    }

    //Check if value has been loaded by the io parser
    //if value is loaded, then parse the value and but in it 'val' 
    template< class T>
    bool check_value(std::string params_name, std::string val_name, T &val){
        std::map<std::string,std::string> params_tmp;
        bool res; 

        if (this->params.find(params_name) == this->params.end()){ //no param given for class
            res = false;
        } else {
            params_tmp = params[params_name];
            if (params_tmp.find(val_name) == params_tmp.end()){ //no param given for varaible
                res = false;
            }else{
                res = true;
                parse_value(val_name,params_tmp[val_name],val);
            }
        }
        return res;
    }


    //Parse all variables in the dictionary
    void parse_vars(const std::string params_name,std::map <std::string, std::any> &maps, bool is_first=true){

        std::for_each(maps.begin(),maps.end(), [this,params_name,is_first] (std::pair<std::string,std::any> pair){
            
            if ( pair.second.type() == typeid(int*)){  //If the variable is an integer array
                int* val = std::any_cast<int*>(pair.second);
                bool read_succ = this->check_value(params_name,pair.first, val);
                this->print_pars_var(params_name,pair.first,pair.second,read_succ,is_first,1);
            } 
                
        });
    }


    //Function to print parsed variables
    void print_pars_var(std::string params_name, std::string var_name, std::any &val, bool succ, bool is_first, int type_nr){
        if (io_verbose >= 1 && is_first){
            switch (type_nr)
            {
            case 1: // int arr
                { //mu
                int* tmp_val = std::any_cast<int*>(val);
                if (succ){
                    std::cout << "Read input variable: " << params_name << ": " << var_name << " with values \n \t\t " << tmp_val[0] << " " << tmp_val[1] << " " << tmp_val[2] << std::endl;
                }else{
                    std::cout << "Could not find variable, " << params_name << ": " << var_name << " in input file" << std::endl;
                }
                }
                break;
            case 2: // int
                {
                int tmp_val = std::any_cast<int>(val);
                if (succ){
                    std::cout << "Read input variable: " << params_name << ": " << var_name << " with value \n \t\t " << tmp_val << std::endl;
                }else{
                    std::cout << "Could not find variable, " << params_name << ": " << var_name << " in input file" << std::endl;
                }
                }
                break;
            default:
                throw std::invalid_argument("Input type_nr did not match any statement");
                break;
            }
        }
    }

};



io_t io_glob = io_t();

#endif
