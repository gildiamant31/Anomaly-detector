/*
 * timeseries.cpp
 *
 * Author: Gil Diamant, 314978412.
 */

#include "timeseries.h"

void TimeSeries::readCSVFile(const char *CSVfileName) {
    ifstream CSVfile(CSVfileName);
    if (!CSVfile.is_open()) {
        throw runtime_error("couldn't open this file.");
    }
    string line;
    //getting the first line from the file(the names of the features).
    getline(CSVfile, line);
    //creating features name vector.
    feature_name = dataSeparation(line, ',');
    //using this vector to save lines over and over again, till the 'while' loop wil be end.
    vector<string> newValues;
    //getting all the other lines from the text convert them to a float.
    while (getline(CSVfile, line)) {
        if (line == "done") {
            break;
        }
        newValues = dataSeparation(line, ',');
        addValuesToTHeMap(line, newValues);
    }
    CSVfile.close();
}

TimeSeries::TimeSeries(const char *CSVfileName) {
    readCSVFile(CSVfileName);
}

void TimeSeries::addValuesToTHeMap(string line, vector<string> vec) {
    for (int i = 0; i < feature_name.size(); ++i) {
        //pushing this value to its suitable vector according to the name of the feature.
        data_table[feature_name[i]].push_back(stof(vec[i]));
    }
}

vector<string> TimeSeries::dataSeparation(string line, char comma) {
    stringstream s(line);
    string str;
    vector<string> vec;
    //removing all the punctuation marks from this line, and pushing word after word to the vector.
    while (getline(s, str, comma)) {
        vec.push_back(str);
    }
    return vec;
}

map<string, vector<float>> TimeSeries::getDataTable() const {
    return data_table;
}

vector<float> TimeSeries::getValues(string str) const {
    return getDataTable()[str];
}



