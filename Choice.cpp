#include "Choice.hpp"
#include<iostream>


void Choice::TestChoice(){

    std::cout<<"EEEEEEE"<<std::endl;
}

void ChoiceTable::ReadCSVFile(std::string dataPath){
<<<<<<< HEAD
 //   std::ifstream input;
 //   input.open(dataPath,)
=======
    std::ifstream input;
    input.open(dataPath,std::ios::in);
    std::cout<<"StartReading"+dataPath<<std::endl;
    if(!input.is_open()){
        std::cout<<"Read Error"<<std::endl;
        return;
    }
    std::string choiceString;
    std::getline(input,choiceString);//abandan first string because they are Header
    while(std::getline(input,choiceString)){
        std::cout<<choiceString<<std::endl;
        Choice Choice(choiceString);

    }
    input.close();
    std::cout<<"Done"<<std::endl;
    return;
>>>>>>> parent of a577a20 (Upload Choice Fix)
}
