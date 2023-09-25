#pragma once

#include<string>
#include<vector>
struct Choice
{
    struct choiceData{
        std::string context;
        int choiceNext;
        int choiceEffect;
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
