cmake_minimum_required(VERSION 3.17.5)
project(ex6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(ex6 ../ex6/MainTrain.cpp ../ex6/anomaly_detection_util.cpp ../ex6/anomaly_detection_util.h
        ../ex6/AnomalyDetector.h ../ex6/SimpleAnomalyDetector.h ../ex6/timeseries.cpp
        ../ex6/SimpleAnomalyDetector.cpp ../ex6/timeseries.h ../ex6/HybridAnomalyDetector.cpp
        ../ex6/HybridAnomalyDetector.h ../ex6/minCircle.h ../ex6/minCircle.cpp ../ex6/commands.h ../ex6/CLI.cpp
        ../ex6/CLI.h ../ex6/expected_output.txt ../ex6/input.txt ../ex6/Server.cpp ../ex6/Server.h
        ../ex6/expected_output_menu.txt)