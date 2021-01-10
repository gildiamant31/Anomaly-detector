

#ifndef CLI_H_
#define CLI_H_

#include "stdlib.h"
#include <vector>
#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO *dio;
    ShareData *shareData;
    //add all kinds of commands.
    AlgoSettingCommand *settingCom;
    AnomalyDetectCommand *detectionCom;
    AnalyzeCommand *analyzeCom;
    ExitCommand *exitCommand;
    ShowResultsCommand *resultsCom;
    UploadCommand *uploadCom;
    vector<Command *> allCommands;
    // you can add data members
public:
    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();
};

#endif /* CLI_H_ */
