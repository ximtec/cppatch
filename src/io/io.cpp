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


#define GET_NAME(val) (#val)

const std::string WHITESPACE = " \n\r\t\f\v";
 
 bool is_whitespace(const char val){
    return WHITESPACE.find(val) != std::string::npos;
 }

bool check_string_to_bool(const std::string &name, const std::string &s){
    if (not( s=="T" || s=="F")){
        throw std::invalid_argument("Could not cast variable " + name + " with input '" + s + "' to bool \n \t\t --- Must be either 'T' or 'F' ");
    }else{
        return s=="T";
    }
}

bool check_string_to_int(const std::string &name, const std::string &s){
    std::stringstream ss(s);
    int i;
    if ((ss >> i).fail() || !(ss >> std::ws).eof())
    {
        throw std::invalid_argument("Could not cast variable " + name + " with input '" + s + "' to int");
        return false;
    }else {
        return true;
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

std::string prune(const std::string &s){
        std::string new_String = s;
        auto it = std::unique(new_String.begin(), new_String.end(),
                    [](char const &lhs, char const &rhs) {
                        return (lhs == rhs) && (is_whitespace(lhs)) ;
                    });
        new_String.erase(it, new_String.end());
        return new_String;
}



std::string remove_whitespaces(const std::string &s){
    return prune(trim(s));
}




class io_t{
    private:
        std::string input_file;

        std::string param_name = "io_params";

        std::map<std::string, std::map<std::string,std::string> > params;

        bool verbose;

    public:

    io_t(){};

    ~io_t(){};


    std::string read_file(){
        std::ifstream fin(this->input_file);
        std::string str;
        std::string line;
        //load each line, remove comments, and remove redundant whitespaces
        while (std::getline(fin,line)){
                //std::cout << line << std::endl;
                //remove comments
                size_t start = line.find_first_of('#'); 
                line =  (start == std::string::npos) ? line : line.substr(0,start);
                //remove redundant whitespace
                line = remove_whitespaces(line);
                str += line + "\n";
        }
        return str;
    }


    void find_params(std::string file, std::vector <std::string> &matches){
        std::regex regxp("&[a-z_A-Z0-9 =\n,.&]*?;");
        std::smatch res;
        std::string::const_iterator searchStart( file.cbegin() );
        while ( std::regex_search( searchStart, file.cend(), res, regxp ) )
        {
            std::string match =  res[0];
            //std::cout << match << std::endl;

            std::replace(match.begin(),match.end(),'\n',' '); //change newline to space
            std::replace(match.begin(),match.end(),';',' ');  //remove the ; and replace with space
            match = remove_whitespaces(match); //remove trailing whitespac
            //std::cout << match << std::endl;  
            //std::cout << ( searchStart == str.cbegin() ? "" : " " ) << res[0] << std::endl;  
            searchStart = res.suffix().first;
            matches.push_back(match);
        }
    }


    std::string get_param_class_name(std::string param_list){
        std::regex name_reg("&[a-zA-Z]+_params");
        std::smatch res;
        std::string::const_iterator searchStart( param_list.cbegin() );
        int i = 0;
        std::string name;

        while(std::regex_search( searchStart, param_list.cend(), res, name_reg )){
            name = res[0];
            name.erase(0,1);
            //std::cout << name << std::endl;
            searchStart = res.suffix().first;
            i++;
        }
        if (i != 1){
            throw std::invalid_argument("More than 1 name for parameter given: \n\t " + param_list);
        }
        return name;
    }

    void get_param_class_val(std::string param_list,std::map<std::string,std::string> &param_dict){
        std::regex param_reg("[a-zA-Z]+[ ]*=[ ]*[a-zA-Z0-9.]+(?:,[ ]*[a-zA-Z0-9.]+)*");
        std::string::const_iterator searchStart =  param_list.cbegin();
        std::smatch res;
        while(std::regex_search( searchStart, param_list.cend(), res, param_reg )){
            std::string param = res[0];
            //std::cout << param << std::endl;
            searchStart = res.suffix().first;

            size_t equal_pos = param.find_first_of('='); 

            std::string param_name = param.substr(0,equal_pos);
            std::string param_val  = param.substr(equal_pos+1);

            auto it1 = std::remove(param_name.begin(),param_name.end(),' ');
            param_name.erase(it1, param_name.end());

            auto it2 = std::remove(param_val.begin(),param_val.end(),' ');
            param_val.erase(it2, param_val.end());

            param_dict.insert(std::pair<std::string,std::string>(param_name,param_val));
        }
    }

    void print_all_params(){
        std::for_each(this->params.begin(),this->params.end(), [](std::pair<std::string,std::map<std::string,std::string>> pair){
            std::cout << pair.first << ":" << std::endl;
            std::for_each(pair.second.begin(),pair.second.end(), [](std::pair<std::string,std::string> val){
                std::cout<< "\t\t" << val.first << " = " << val.second << std::endl;

            });
        });
    }




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

        this->print_all_params();


    }

    void init(std::string  file_name){
        std::string params_name = "io_params";

        this->input_file = file_name;
        this->parse_input();

        this->verbose = this->check_value(params_name,"verbose",this->verbose) ? this->verbose : false;

    }

    bool check_value(std::string params_name, std::string val_name, int &val){
        if (this->params.find(params_name) == this->params.end()){
            return false;
        } else {
            std::map<std::string,std::string> params_tmp = params[params_name];
            //int tmp = val + 1;
            //std::cout << GET_NAME(&val) << "    " << tmp << std::endl;
            if (params_tmp.find(val_name) == params_tmp.end()){
                return false;
            }else{

                if (check_string_to_int(val_name,params_tmp[val_name])){
                    std::stringstream ss(params_tmp[val_name]);
                    ss >> val;
                    return true;
                }else{
                    return false;
                }
            
            }
            return true;
        }
    }

        bool check_value(std::string params_name, std::string val_name, bool &val){
        if (this->params.find(params_name) == this->params.end()){
            return false;
        } else {
            std::map<std::string,std::string> params_tmp = params[params_name];
            //int tmp = val + 1;
            //std::cout << GET_NAME(&val) << "    " << tmp << std::endl;
            if (params_tmp.find(val_name) == params_tmp.end()){
                return false;
            }else{

                if (check_string_to_bool(val_name,params_tmp[val_name])){
                    std::stringstream ss(params_tmp[val_name]);
                    ss >> val;
                    return true;
                }else{
                    return false;
                }
            
            }
            return true;
        }
    }

};



io_t io_glob = io_t();

#endif
