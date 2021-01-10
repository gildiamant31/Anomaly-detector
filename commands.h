

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#include <thread>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
protected:
    ofstream out;
    ifstream in;
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}


};

class StandardIO : public DefaultIO {

public:
    StandardIO(string inputFile, string outputFile) {
        in.open(inputFile);
        out.open(outputFile);
    }

    string read() {
        string str;
        in >> str;
        return str;
    }

    void write(string text) {
        out << text;
    }

    void write(float f) {
        out << f;
    }

    void read(float *f) {
        in >> *f;
    }
};

class SocketIO : public DefaultIO {
    int userID;
public:
    SocketIO(int userID) {
        this->userID = userID;
    }

    virtual string read() {
        string serverInput = "";
        char c = 0;
        int r1 = ::read(userID, &c, sizeof(char));
        while (c != '\n') {
            serverInput += c;
            int r2 = ::read(userID, &c, sizeof(char));
        }
        return serverInput;
    }

    virtual void write(string text) {
        ::write(userID, text.c_str(), text.length());
    }

    virtual void write(float f) {
        string strF = to_string(f);
        ::write(userID, strF.c_str(), to_string(f).length());
    }

    virtual void read(float *f) {
        in >> *f;
    }
};


class ShareData {
    HybridAnomalyDetector hybridDetector;
    vector<AnomalyReport> ar;

public:
    string trainFile, testFile;

    ShareData() {
        this->hybridDetector = HybridAnomalyDetector();
    }

    HybridAnomalyDetector getDetector() {
        return hybridDetector;
    }

    vector<AnomalyReport> getAnomaly() {
        return this->ar;
    }

    void setAnomalyReport(const TimeSeries &tsTrain, const TimeSeries &tsTest) {
        this->hybridDetector.learnNormal(tsTrain);
        this->ar = this->hybridDetector.detect(tsTest);
    }

    float getThreshold() {
        return this->hybridDetector.getThreshold();
    }

    void setNewThreshold(float f) {
        this->hybridDetector.setNewThreshold(f);
    }

    string deleteAllZero(string x) {
        std::size_t endOfNum = x.find_last_not_of('0') + 1;
        string y = x.erase(endOfNum);
        std::size_t point = y.find_first_of('.');
        if (y[point] == y[y.length() - 1]) {
            return y.erase(point);
        }
        return y;
    }

};

// you may edit this class
class Command {
protected:
    DefaultIO *dio;
    ShareData *data;
    string commandDescrip = "empty";  //description of the command in the menu.
    string down = "\n";
public:
    Command(DefaultIO *dio, ShareData *data) : dio(dio), data(data) {}

    virtual void execute() = 0;

    string getDescription() {
        return this->commandDescrip;
    }

    void upload(string str, string str2) {
        ofstream newFile(str);
        dio->write(str2);
        string line1 = "0";
        while (line1 != "done") {
            line1 = dio->read();
            newFile << line1 << endl;
        }
        dio->write("Upload complete." + down);
        newFile.close();

    }

    virtual ~Command() {}
};

class UploadCommand : public Command {
public:
    UploadCommand(DefaultIO *dio, ShareData *data) : Command(dio, data) {
        this->commandDescrip = "1.upload a time series csv file" + down;
    }

    void execute() {
        string trainFile = "trainFile.csv";
        string testFile = "testFile.csv";
        string trainCmd = "Please upload your local train CSV file." + down;
        string testCmd = "Please upload your local test CSV file." + down;
        upload(trainFile, trainCmd);
        data->trainFile = trainFile;
        //upload test file.
        upload(testFile, testCmd);
        data->testFile = testFile;
    }
};

class AlgoSettingCommand : public Command {
public:
    AlgoSettingCommand(DefaultIO *dio, ShareData *data) : Command(dio, data) {
        this->commandDescrip = "2.algorithm settings" + down;
    }

    void execute() {
        float threshold = data->getThreshold();
        string corr = data->deleteAllZero(to_string(threshold));
        dio->write("The current correlation threshold is " + corr + down + "Type a new threshold" + down);
        string corStr = dio->read();
        float newThreshold = stof(corStr);
        while (newThreshold <= 0 || newThreshold >= 1) {
            dio->write("Please choose a value between 0 to 1." + down);
            dio->write("The current correlation threshold is " + corr + down
                       + "Type a new threshold" + down);
            corStr = dio->read();
            newThreshold = stof(corStr);
        }
        data->setNewThreshold(newThreshold);
    }
};

class AnomalyDetectCommand : public Command {
public:
    AnomalyDetectCommand(DefaultIO *dio, ShareData *data) : Command(dio, data) {
        this->commandDescrip = "3.detect anomalies" + down;
    }

    void execute() {
        string trainFile = "trainFile.csv";
        string testFile = "testFile.csv";
        TimeSeries trainTS(trainFile.c_str());
        TimeSeries testTS(testFile.c_str());
        data->setAnomalyReport(trainTS, testTS);
        dio->write("anomaly detection complete." + down);
    }
};

class ShowResultsCommand : public Command {
public:
    ShowResultsCommand(DefaultIO *dio, ShareData *data) : Command(dio, data) {
        this->commandDescrip = "4.display results" + down;
    }

    void execute() {
        for (AnomalyReport report : data->getAnomaly()) {
            dio->write(to_string(report.timeStep) +
                       '\t' + " " + report.description + down);
        }
        dio->write("Done." + down);
    }
};

class AnalyzeCommand : public Command {
public:
    AnalyzeCommand(DefaultIO *dio, ShareData *data) : Command(dio, data) {
        this->commandDescrip = "5.upload anomalies and analyze results" + down;
    }

    bool isIntersect(long startU, long endU, long startM, long endM) {
        if (startM > endU || startU > endM) {
            return false;
        }
        return true;
    }

    map<string, vector<long>> readUserReports(string txt) {
        map<string, vector<long>> userReport;
        string line;
        ifstream userFile(txt);
        if (!userFile.is_open()) {
            throw runtime_error("Could not open file");
        }

        while (getline(userFile, line)) {
            if (line == "done") {
                break;
            }
            stringstream ss(line);
            vector<string> times;
            string str;
            while (getline(ss, str, ',')) {
                times.push_back(str);
            }
            userReport["S"].push_back(stol(times.at(0)));
            userReport["E"].push_back(stol(times.at(1)));
            times.clear();
        }
        return userReport;
    }

    map<string, vector<long>> unionAnomaly(vector<AnomalyReport> ar) {
        long S, E;
        map<string, vector<long>> newAr;
        int i = 0;
        newAr["S"].push_back(data->getAnomaly().at(i).timeStep);
        newAr["E"].push_back(data->getAnomaly().at(i).timeStep);
        string currentDescrip = data->getAnomaly().at(i).description;
        for (i = 1; i < ar.size(); ++i) {
            if (currentDescrip != (data->getAnomaly().at(i).description)) {
                currentDescrip = data->getAnomaly().at(i).description;
                newAr["S"].push_back(data->getAnomaly().at(i).timeStep);
                newAr["E"].push_back(data->getAnomaly().at(i).timeStep);

            } else {
                newAr["E"].pop_back();
                newAr["E"].push_back(data->getAnomaly().at(i).timeStep);
            }
        }
        return newAr;

    }


    void execute() {
        float P, N, U;
        float TP = 0, FP = 0;
        TimeSeries ts(data->testFile.c_str());
        float n = ts.getDataTable().begin()->second.size();
        map<string, vector<long>> unitedAnomalyR = unionAnomaly(data->getAnomaly());
        upload("Reports.csv", "Please upload your local anomalies file." + down);
        map<string, vector<long>> userReports = readUserReports("Reports.csv");
        P = userReports["S"].size();
        U = unitedAnomalyR["S"].size();
        float x = 0;
        for (int i = 0; i < P; ++i) {
            x = (userReports["E"][i] - userReports["S"][i]);
        }
        N = n - x;
        bool flag1 = false;
        for (int i = 0; i < U; ++i) {
            for (int j = 0; j < P; ++j) {
                // checks if there is an intersection between the report to the anomaly.
                bool flag2 = isIntersect(userReports["S"][j], userReports["E"][j],
                                         unitedAnomalyR["S"][i], unitedAnomalyR["E"][i]);
                if (flag2) {
                    TP++;
                    flag1 = true;
                }
            }
            if (!flag1)
                FP++;
            flag1 = false;
        }
        TP = TP / P;
        FP = FP / N;
        string sTP = to_string(TP);
        string sFP = to_string(FP);
        if (sTP.length() > 5) {
            sTP.erase(5);
        }
        if (sFP.length() > 5) {
            sFP.erase(5);
        }
        dio->write("True Positive Rate: " + data->deleteAllZero(sTP) + down);
        dio->write("False Positive Rate: " + data->deleteAllZero(sFP) + down);
    }
};

class ExitCommand : public Command {
public:
    ExitCommand(DefaultIO *dio, ShareData *data) : Command(dio, data) {
        this->commandDescrip = "6.exit" + down;
    }

    void execute() {
    }
};

// implement here your command classes



#endif /* COMMANDS_H_ */
