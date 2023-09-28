#pragma once

#include<string>
#include<vector>
#include"Sound.hpp"
#include "data_path.hpp"
struct Choice
{
    Choice(std::string input);
    struct choiceData{
        std::string context;
        int choiceNext;
        int choiceEffect;
        choiceData(){
            context="";
            choiceNext=0;
            choiceEffect=0;
        }
        choiceData(std::string inputstring, int input1,int input2){
            for(int i=0;i<inputstring.size();++i){
                if(inputstring[i]=='#'){
                    inputstring[i]==',';
                }
            }
            context=inputstring;
            choiceNext=input1;
            choiceEffect=input2;
        }
    };

    int EventID;
    int type;//=1=2
    int choiceCount;
    std::string dataPath;
    Sound::Sample*  voice;
    choiceData baseChoice;
    std::vector<choiceData> potentialChoice;



    void TestChoice();
};
struct ChoiceTable{
    std::vector<Choice> choiceLibrary;
    Choice GetChoice(int id);
    void AddChoice(Choice input);
    void ReadCSVFile(std::string dataPath);
};
