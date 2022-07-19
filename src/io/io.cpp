#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex> 
#include <map>
#include <stdexcept>



const std::string WHITESPACE = " \n\r\t\f\v";
 
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

void prune(std::string &s){
        auto it = std::unique(s.begin(), s.end(),
                    [](char const &lhs, char const &rhs) {
                        return (lhs == rhs) && (lhs == ' ');
                    });
        s.erase(it, s.end());
}



class io_t{
    private:
        std::string input_file;

    public:

    io_t(){};

    ~io_t(){};


    void read_input(std::string  file_name){
        //std::ifstream in(file_name);
        //std::string contents((std::istreambuf_iterator<char>(in)), 
        //    std::istreambuf_iterator<char>());
        //std::fstream newfile;
//
        //newfile.open("input.nml",std::ios::in); //open a file to perform read operation using file object
        //if (newfile.is_open()){   //checking whether the file is open
        //    std::string tp;
        //    while(getline(newfile, tp)){ //read data from file object and put it into string.
        //        std::cout << tp << "\n"; //print the data of the string
        //    }
        //    newfile.close(); //close the file object.
        //}
        //std::cout << contents;

        this->input_file = file_name;
        //std::cout << this->input_file << std::endl;
        std::ifstream fin(this->input_file);
        if (!fin) {
            std::cout << "[Error][BifrostMesh] mesh: " << this->input_file << " read error!" << std::endl;
            return;
        }
        std::string line;
        std::string multi_line;

        std::stringstream line_stream;

        std::vector <std::string> lines;
        bool line_continue = false;

        
        while (std::getline(fin,line)){
            std::cout << "|" << line << "|" <<std::endl;
            //Handle comments
            if (line.find('#') != std::string::npos){
                int comment_loc = line.find('#');
                line = line.substr(0, comment_loc);
            }
            std::cout << "|" << line << "|" <<std::endl;

            if ( (line.find(';') != std::string::npos)) {
                if (line_continue) {
                    multi_line += " " + line;
                    multi_line = trim(multi_line);
                    prune(multi_line);
                    lines.push_back(multi_line);
                    multi_line = "";
                    line_continue = false;

                }else{
                    line = trim(line);
                    prune(line);
                    lines.push_back(line);
                }

            }else{
                line_continue = true;
                multi_line += line;
            }


            std::cout <<std::endl;

        }
        fin.close();

        std::for_each(lines.begin(),lines.end(), [] (std::string s){
            std::cout << s << std::endl;
        });




        //line = lines.back();
        //std::cout <<  line << std::endl;

    }

    void read_regex(std::string  file_name){
    this->input_file = file_name;

    std::ifstream fin(this->input_file);


    std::string str;

    std::string line;

    //load each line, remove comments, and remove redundant whitespaces
    while (std::getline(fin,line)){
            //std::cout << line << std::endl;
            //remove comments
            size_t start = line.find_first_of('#'); 
            line =  (start == std::string::npos) ? line : line.substr(0,start);
            //remove trailing whitespace
            line = trim(line);
            //remove multiple whitespace characters in a row
            prune(line);
            //std::cout << line << std::endl;

            str += line + "\n";
    }
    fin.close();
    //std::cout << str << std::endl;

    //find each
    std::regex regxp("&[a-z_A-Z0-9 =\n,.]*?;");
    std::smatch res;
    std::vector <std::string> matches;
    std::string::const_iterator searchStart( str.cbegin() );
    while ( std::regex_search( searchStart, str.cend(), res, regxp ) )
    {
        std::string match =  res[0];
        //std::cout << match << std::endl;

        std::replace(match.begin(),match.end(),'\n',' '); //change newline to space
        std::replace(match.begin(),match.end(),';',' ');  //remove the ; and replace with space
        match = trim(match); //remove trailing whitespac
        //std::cout << match << std::endl;  
        //std::cout << ( searchStart == str.cbegin() ? "" : " " ) << res[0] << std::endl;  
        searchStart = res.suffix().first;
        matches.push_back(match);
    }



    std::map<std::string, std::map<std::string,std::string> > params;

    std::regex name_reg("&[a-zA-Z]+_params");
    std::regex param_reg("[a-zA-Z]+[ ]*=[TF0-9., ]+");


     for (auto match : matches){

        //std::cout << match << std::endl;

        std::string::const_iterator searchStart( match.cbegin() );
        int i = 0;
        std::string name;

        while(std::regex_search( searchStart, match.cend(), res, name_reg )){
            name = res[0];
            name.erase(0,1);
            //std::cout << name << std::endl;
            searchStart = res.suffix().first;
            i++;
        }
        if (i != 1){
            throw std::invalid_argument("More than 1 name for parameter given: \n\t " + match);
        }

        std::map<std::string,std::string> params_tmp;
        searchStart =  match.cbegin();
        while(std::regex_search( searchStart, match.cend(), res, param_reg )){
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

            //std::cout << "param_name |" <<  param_name << "|" << std::endl;
            //std::cout << "param_val |" <<  param_val << "|" << std::endl;

            params_tmp.insert(std::pair<std::string,std::string>(param_name,param_val));
            //TODO split string and save everything in dictionary
        }

        //std::cout << "Loading parameters for: " << name << std::endl;
        //std::for_each(params_tmp.begin(),params_tmp.end(), [](std::pair<std::string,std::string> pair){
        //    std::cout << "Name is: " << pair.first << std::endl;
        //    std::cout << "val is: " << pair.second << std::endl;
//
        //});

        params.insert(std::pair<std::string, std::map<std::string,std::string>>(name, params_tmp));



    }

        std::for_each(params.begin(),params.end(), [](std::pair<std::string,std::map<std::string,std::string>> pair){
            std::cout << "Loading parameters for: " << pair.first << std::endl;

            std::for_each(pair.second.begin(),pair.second.end(), [](std::pair<std::string,std::string> val){
                std::cout << "Name is: " << val.first << " value is " << val.second << std::endl;

            });
        });

    }


};



io_t io_glob = io_t();
