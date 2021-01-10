#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    this->shareData = new ShareData;
    this->uploadCom = new UploadCommand(dio, shareData);
    this->settingCom = new AlgoSettingCommand(dio, shareData);
    this->detectionCom = new AnomalyDetectCommand(dio, shareData);
    this->resultsCom = new ShowResultsCommand(dio, shareData);
    this->analyzeCom = new AnalyzeCommand(dio, shareData);
    this->exitCommand = new ExitCommand(dio, shareData);
    this->allCommands.push_back(uploadCom);
    this->allCommands.push_back(settingCom);
    this->allCommands.push_back(detectionCom);
    this->allCommands.push_back(resultsCom);
    this->allCommands.push_back(analyzeCom);
    this->allCommands.push_back(exitCommand);
}

void CLI::start() {
    int choice = 0;
    while (choice != 6) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < allCommands.size(); ++i) {
            dio->write(allCommands[i]->getDescription());
        }
        choice = atoi(dio->read().c_str());
        allCommands.at(choice - 1)->execute();
    }
}

CLI::~CLI() {
    delete uploadCom;
    delete settingCom;
    delete detectionCom;
    delete resultsCom;
    delete analyzeCom;
    delete exitCommand;
    delete shareData;
}

