#pragma once

#include<string>
#include<vector>
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
            context=inputstring;
            choiceNext=input1;
            choiceEffect=input2;
        }
    };

    int EventID;
    int type;//=1=2
    int choiceCount;
    choiceData baseChoice;
    std::vector<choiceData> potentialChoice;



    void TestChoice();
};
struct ChoiceTable{
    std::vector<Choice*> choiceLibrary;
    Choice GetChoice(int id);
    Choice AddChoice(Choice* input);
    void ReadCSVFile(std::string dataPath);
};
