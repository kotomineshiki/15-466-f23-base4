#include "Choice.hpp"
#include<fstream>
#include<iostream>
Choice::Choice(std::string input){
    //int currentIndex=0;
    int offset=0;
 //   std::string temp;
    input=input+',';
    std::vector<std::string> splitted;
    offset=(int)input.find(',');

    while(offset!=input.npos){
        std::string temp=input.substr(0,offset);
        splitted.push_back(temp);
    //    std::cout<<temp<<std::endl;
        input=input.substr(offset+1,input.size());
        offset=(int)input.find(',');
    }
    //std::cout<<splitted.size()<<std::endl;
    EventID=atoi(splitted[0].c_str());
    type=atoi(splitted[1].c_str());
    choiceCount=atoi(splitted[2].c_str());
    dataPath=splitted[3];
    choiceData base(splitted[4],atoi(splitted[5].c_str()),atoi(splitted[6].c_str()));
    baseChoice=base;
    if(dataPath!=""){
         voice=new Sound::Sample(data_path(dataPath));
        std::cout<<dataPath<<std::endl;
    	//Sound::play(*voice, 1.0f, 1.0f);
    }

    if(type==2){
        choiceData potential1(splitted[7],atoi(splitted[8].c_str()),atoi(splitted[9].c_str()));
        choiceData potential2(splitted[10],atoi(splitted[11].c_str()),atoi(splitted[12].c_str()));
        potentialChoice.push_back(potential1);
        potentialChoice.push_back(potential2);
    }
    if(type==3){
        choiceData potential1(splitted[7],atoi(splitted[8].c_str()),atoi(splitted[9].c_str()));
        choiceData potential2(splitted[10],atoi(splitted[11].c_str()),atoi(splitted[12].c_str()));
        choiceData potential3(splitted[13],atoi(splitted[14].c_str()),atoi(splitted[15].c_str()));
        potentialChoice.push_back(potential1);
        potentialChoice.push_back(potential2);
        potentialChoice.push_back(potential3);
    }
 //   std::cout<<EventID<<type<<choiceCount<<baseChoice.context<<std::endl;
}

void Choice::TestChoice(){

    std::cout<<"EEEEEEE"<<std::endl;
}

void ChoiceTable::AddChoice(Choice input){
    choiceLibrary.push_back(input);
}
Choice ChoiceTable::GetChoice(int id){
    for (auto i:choiceLibrary)
    {
        /* code */
        if(i.EventID==id){
            return i;
        }
    }

        std::cout<<"Warning!"<<std::endl;
        return nullptr;

}

void ChoiceTable::ReadCSVFile(std::string dataPath){
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
        Choice choice(choiceString);
        AddChoice(choice);

    }
    input.close();
    std::cout<<"Done"<<std::endl;
    return;
}
