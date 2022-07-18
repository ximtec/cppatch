#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex> 

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

    //if(fin){
    //  std::ostringstream ss;
    //  ss << fin.rdbuf(); // reading data
    //  str = ss.str();
    //}
    //std::cout << str << std::endl;

    fin.close();


    std::regex regxp("&[a-z_A-Z0-9 =\n]*?;");

    std::smatch res;

    std::vector <std::string> matches;

    std::string::const_iterator searchStart( str.cbegin() );
    while ( std::regex_search( searchStart, str.cend(), res, regxp ) )
    {
        std::string match =  res[0];


        std::replace(match.begin(),match.end(),'\n',' '); //change newline to space
        std::replace(match.begin(),match.end(),';',' ');  //remove the ; and replace with space
        match = trim(match); //remove trailing whitespace


        //std::cout << match << std::endl;  
        //std::cout << ( searchStart == str.cbegin() ? "" : " " ) << res[0] << std::endl;  

        searchStart = res.suffix().first;

        matches.push_back(match);
    }


    for (auto res : matches){
        std::cout << res << std::endl;
    }
    }

};



io io_glob = io();
