#include "linear.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath> // for square root
#include<windows.h>

using namespace std;
linear::linear() {}

void linear::loadDataset(const char* filename, int choice) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }
    if (choice == 1) {
        X.resize(0, 7);
        Y.resize(0);

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream linestream(line);
            std::string value;
            Eigen::VectorXd row(7);
            for (int i = 0; i < 7; ++i) {
                if (std::getline(linestream, value, ',')) {
                    if (i == 0) {
                        row(i) = std::stoi(value.substr(1)); // season (int)
                    }
                    else if (i == 1) {
                        row(i) = std::stod(value); // farm size (double)
                    }
                    else if (i == 2) {
                        row(i) = std::stod(value); // crop yield (double)
                    }
                    else if (i == 3) {
                        row(i) = std::stod(value); // spoilage rate (double)
                    }
                    else if (i == 4) {
                        row(i) = std::stoi(value); // quality (int)
                    }
                    else if (i == 5) {
                        row(i) = std::stoi(value); // rainfall (int)
                    }
                    else if (i == 6) {
                        Y.conservativeResize(Y.size() + 1);
                        Y(Y.size() - 1) = std::stod(value.substr(0, -1)); // cost price (double)
                    }
                }
            }

            X.conservativeResize(X.rows() + 1, Eigen::NoChange);
            X.row(X.rows() - 1) = row;
        }
    }
    else if (choice == 2) {
        X.resize(0, 6);
        Y.resize(0);

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream linestream(line);
            std::string value;
            Eigen::VectorXd row(6);
            for (int i = 0; i < 6; ++i) {
                if (std::getline(linestream, value, ',')) {
                    if (i == 0) {
                        row(i) = std::stoi(value.substr(1)); // season (int)
                    }
                    else if (i == 1) {
                        row(i) = std::stod(value); // spoilage rate (double)
                    }
                    else if (i == 2) {
                        row(i) = std::stod(value); // supply amount (double)
                    }
                    else if (i == 3) {
                        row(i) = std::stod(value); // demand amount (double)
                    }
                    else if (i == 4) {
                        row(i) = std::stoi(value); // quality (int)
                    }
                    else if (i == 5) {
                        Y.conservativeResize(Y.size() + 1);
                        Y(Y.size() - 1) = std::stod(value.substr(0, -1)); // selling price (double)
                    }

                }
            }

            X.conservativeResize(X.rows() + 1, Eigen::NoChange);
            X.row(X.rows() - 1) = row;
        }
    }
    else if (choice == 3) {
        X.resize(0, 8);
        Y.resize(0);

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream linestream(line);
            std::string value;
            Eigen::VectorXd row(8);
            for (int i = 0; i < 8; ++i) {
                if (std::getline(linestream, value, ',')) {
                    if (i == 0) {
                        row(i) = std::stoi(value.substr(1)); // season (int)
                    }
                    else if (i == 1) {
                        row(i) = std::stoi(value); // rainfall (int)
                    }
                    else if (i == 2) {
                        row(i) = std::stod(value); // farm size (double)
                    }
                    else if (i == 3) {
                        row(i) = std::stod(value); // crop yield (double)
                    }
                    else if (i == 4) {
                        row(i) = std::stod(value); // spoilage rate (double)
                    }
                    else if (i == 5) {
                        row(i) = std::stoi(value); // quality (int)
                    }
                    else if (i == 6) {
                        row(i) = std::stod(value); // cost price (double)
                    }
                    else if (i == 7) {
                        Y.conservativeResize(Y.size() + 1);
                        Y(Y.size() - 1) = std::stod(value.substr(0, -1)); // supply amount (double)
                    }
                }
            }

            X.conservativeResize(X.rows() + 1, Eigen::NoChange);
            X.row(X.rows() - 1) = row;
        }
    }
    else if (choice == 4) {
        X.resize(0, 6);
        Y.resize(0);

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream linestream(line);
            std::string value;
            Eigen::VectorXd row(6);
            for (int i = 0; i < 6; ++i) {
                if (std::getline(linestream, value, ',')) {
                    if (i == 0) {
                        row(i) = std::stoi(value.substr(1)); // season (int)
                    }
                    else if (i == 1) {
                        row(i) = std::stod(value); // income (double)
                    }
                    else if (i == 2) {
                        row(i) = std::stoi(value); // shelf life (int)
                    }
                    else if (i == 3) {
                        row(i) = std::stoi(value); // quality (int)
                    }
                    else if (i == 4) {
                        row(i) = std::stod(value); // selling price (double)
                    }
                    else if (i == 5) {
                        Y.conservativeResize(Y.size() + 1);
                        Y(Y.size() - 1) = std::stod(value.substr(0, -1)); // demand amount (double)
                    }
                }
            }

            X.conservativeResize(X.rows() + 1, Eigen::NoChange);
            X.row(X.rows() - 1) = row;
        }

    }
    file.close();
}

void linear::trainModel() {
    Eigen::MatrixXd X_train = X.topRows(static_cast<int>(0.7 * X.rows()));
    Eigen::VectorXd Y_train = Y.head(static_cast<int>(0.7 * Y.size()));

    coefficients = (X_train.transpose() * X_train).ldlt().solve(X_train.transpose() * Y_train);
}

void linear::evaluateModel() {
    Eigen::MatrixXd X_test = X.bottomRows(static_cast<int>(0.3 * X.rows()));
    Eigen::VectorXd Y_test = Y.tail(static_cast<int>(0.3 * Y.size()));

    Eigen::VectorXd predictions = X_test * coefficients;
    Eigen::VectorXd residuals = predictions - Y_test;
    double mse = residuals.squaredNorm() / Y_test.size();
    double rmse = std::sqrt(mse);

    // Calculate mean actual Y
    double meanY = Y_test.mean();

    // Calculate total sum of squares (TSS) and regression sum of squares (RSS)
    double tss = (Y_test.array() - meanY).square().sum();
    double rss = residuals.squaredNorm();

    // Calculate R-squared (R^2)
    double rSquared = 1.0 - (rss / tss);

    // Calculate adjusted R-squared (adjusted R^2)
    int n = X_test.rows();
    int k = X_test.cols();
    double adjustedRSquared = 1.0 - (1.0 - rSquared) * (n - 1) / (n - k - 1);

    // Calculate accuracy and precision
    double accuracy = 1.0 - mse / tss;
    double precision = 1.0 - mse / rss;
    cout << " || Loading Data . . ." << endl;
    cout << " ||" << endl;
    Sleep(1000);
    cout << " || Calculating Metrics . . . " << endl;
    cout << " || " << endl;
    Sleep(1500);
    cout << " || Evaluating Model . . ." << endl;
    cout << " || " << endl;
    Sleep(2000);
    cout << " || Training and Testing complete! View the metrics below ->" << endl;
    cout << " || " << endl;
    cout << " || " << endl;
    Sleep(1500);
    std::cout << " || Mean Squared Error (MSE): " << mse << std::endl;
    Sleep(500);
    std::cout << " || Root Mean Squared Error (RMSE): " << rmse << std::endl;
    Sleep(500);
    std::cout << " || R-squared (R^2): " << rSquared << std::endl;
    Sleep(500);
    std::cout << " || Adjusted R-squared (Adjusted R^2): " << adjustedRSquared << std::endl;
    Sleep(500);
    std::cout << " || Accuracy: " << accuracy << std::endl;
    Sleep(500);
    std::cout << " || Precision: " << precision << std::endl;
    Sleep(500);
}


double linear::predictCostPrice(const Eigen::VectorXd& input) {
    // Add bias term (1) to the input
    int len;
    len = input.size();
    Eigen::VectorXd x(len);
    x << input, 1;
    return x.dot(coefficients);
}

double linear::predictCP(const Eigen::VectorXd& input) { // FOR THE FARMER
    // Add bias term (1) to the input
    int len;
    len = input.size();
    Eigen::VectorXd x(len);
    x << input, 1;
    return x.dot(coefficients);
}

double linear::predictSP(const Eigen::VectorXd& input) { // FOR THE MANAGER
    // Add bias term (1) to the input
    int len;
    len = input.size();
    Eigen::VectorXd x(len);
    x << input, 1;
    return x.dot(coefficients);
}

void linear::linearmainmenu() {
    try {
        linear lr;
        int choice;
        cout << " || Welcome to the Prediction Model!" << endl;
        cout << " ||" << endl;
        bool check = true;
        while (check) {
            cout << " || Here are your options!" << endl;
            cout << " || 1. Cost Price Prediction" << endl;
            cout << " || 2. Selling Price Prediction" << endl;
            cout << " || 3. Supply Amount Prediction" << endl;
            cout << " || 4. Demand Amount Prediction" << endl;
            cout << " || 5. Back" << endl;
            cout << " ||" << endl;
            cout << " || Enter your choice! : ";
            cin >> choice;
            cout << " ||" << endl;
            cout << " ||" << endl;
            if (choice == 1) {
                lr.loadDataset("C:/Users/Aarus/OneDrive/Documents/costprice.csv", choice);
                lr.trainModel();
                lr.evaluateModel();

                Eigen::VectorXd input(6);
                double predictedCost;
                cout << " || " << endl;
                std::cout << " || To predict the cost price -> " << endl;
                Sleep(500);
                cout << " ||" << endl;
                std::cout << " || Choose the season : " << endl;
                Sleep(250);
                cout << " || 1. Spring" << endl;
                Sleep(250);
                cout << " || 2. Summer" << endl;
                Sleep(250);
                cout << " || 3. Monsoon" << endl;
                Sleep(250);
                cout << " || 4. Winter" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(0);
                cout << " ||" << endl;
                std::cout << " || Input the farm size [ UNIT : HECTARES (HA), AVG RANGE : (0.8 to 2.5) ] " << endl;
                cout << " || Size : ";
                std::cin >> input(1);
                cout << " ||" << endl;
                std::cout << " || Input the crop yield [ UNIT : KG PER HECTARE, AVG RANGE : (9.00 to 20.00) ] " << endl;
                cout << " || Crop Yield : ";
                std::cin >> input(2);
                cout << " ||" << endl;
                std::cout << " || Enter spoilage rate [ UNIT : KG PER HOUR, AVG RANGE (0.01 to 0.08) ] " << endl;
                cout << " || Rate : ";
                std::cin >> input(3);
                cout << " ||" << endl;
                std::cout << " || Enter the quality of tomatoes : " << endl;
                Sleep(250);
                cout << " || 1. Semi-Ripe" << endl;
                Sleep(250);
                cout << " || 2. Ripe" << endl;
                Sleep(250);
                cout << " || 3. Overripe" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(4);
                cout << " ||" << endl;
                std::cout << " || Enter the amount of rainfall : " << endl;
                Sleep(250);
                cout << " || 1. Low" << endl;
                Sleep(250);
                cout << " || 2. Medium" << endl;
                Sleep(250);
                cout << " || 3. High" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(5);
                cout << " ||" << endl;

                predictedCost = lr.predictCostPrice(input);
                Sleep(500);
                if (predictedCost < 0) {
                    cout << " Metrics are dissaprovingly poor, the farmer will unfortunately face a loss" << endl;
                    cout << " ||" << endl;
                }
                else {
                    std::cout << " || Predicted Cost Price: " << predictedCost << std::endl;
                    cout << " ||" << endl;
                }
                Sleep(500);
            }
            else if (choice == 2) {
                lr.loadDataset("C:/Users/Aarus/OneDrive/Documents/sellingprice.csv", choice);
                lr.trainModel();
                lr.evaluateModel();

                Eigen::VectorXd input(5);
                double predictedCost;
                cout << " || " << endl;
                std::cout << " || To predict the selling price -> " << endl;
                Sleep(500);
                cout << " ||" << endl;
                std::cout << " || Choose the season : " << endl;
                Sleep(250);
                cout << " || 1. Spring" << endl;
                Sleep(250);
                cout << " || 2. Summer" << endl;
                Sleep(250);
                cout << " || 3. Monsoon" << endl;
                Sleep(250);
                cout << " || 4. Winter" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(0);
                cout << " ||" << endl;
                std::cout << " || Enter spoilage rate [ UNIT : KG PER HOUR, AVG RANGE (0.01 to 0.08) ] " << endl;
                cout << " || Rate : ";
                std::cin >> input(1);
                cout << " ||" << endl;
                std::cout << " || Enter supply amount [ UNITS : KG, AVG value is 105 KG ] : " << endl;
                cout << " || Amount : ";
                std::cin >> input(2);
                cout << " ||" << endl;
                std::cout << " || Enter demand amount [ UNITS : KG, AVG value is 1600 KG ] :" << endl;
                cout << " || Amount : ";
                std::cin >> input(3);
                cout << " ||" << endl;
                std::cout << " || Enter the quality of tomatoes : " << endl;
                Sleep(250);
                cout << " || 1. Semi-Ripe" << endl;
                Sleep(250);
                cout << " || 2. Ripe" << endl;
                Sleep(250);
                cout << " || 3. Overripe" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(4);
                cout << " ||" << endl;

                predictedCost = lr.predictCostPrice(input);
                Sleep(500);
                std::cout << " || Predicted Selling Price: " << predictedCost << std::endl;
                cout << " ||" << endl;
                Sleep(500);
            }
            else if (choice == 3) {
                lr.loadDataset("C:/Users/Aarus/OneDrive/Documents/supply.csv", choice);
                lr.trainModel();
                lr.evaluateModel();

                Eigen::VectorXd input(7);
                double predictedCost;
                cout << " || " << endl;
                std::cout << " || To predict the supply amount -> " << endl;
                Sleep(500);
                cout << " ||" << endl;
                std::cout << " || Choose the season : " << endl;
                Sleep(250);
                cout << " || 1. Spring" << endl;
                Sleep(250);
                cout << " || 2. Summer" << endl;
                Sleep(250);
                cout << " || 3. Monsoon" << endl;
                Sleep(250);
                cout << " || 4. Winter" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(0);
                cout << " ||" << endl;
                std::cout << " || Enter the amount of rainfall : " << endl;
                Sleep(250);
                cout << " || 1. Low" << endl;
                Sleep(250);
                cout << " || 2. Medium" << endl;
                Sleep(250);
                cout << " || 3. High" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(1);
                cout << " ||" << endl;
                std::cout << " || Input the farm size [ UNIT : HECTARES (HA), AVG RANGE : (0.8 to 2.5) ] " << endl;
                cout << " || Size : ";
                std::cin >> input(2);
                cout << " ||" << endl;
                std::cout << " || Input the crop yield [ UNIT : KG PER HECTARE, AVG RANGE : (9.00 to 20.00) ] " << endl;
                cout << " || Crop Yield : ";
                std::cin >> input(3);
                cout << " ||" << endl;
                std::cout << " || Enter spoilage rate [ UNIT : KG PER HOUR, AVG RANGE (0.01 to 0.08) ] " << endl;
                cout << " || Rate : ";
                std::cin >> input(4);
                cout << " ||" << endl;
                std::cout << " || Enter the quality of tomatoes : " << endl;
                Sleep(250);
                cout << " || 1. Semi-Ripe" << endl;
                Sleep(250);
                cout << " || 2. Ripe" << endl;
                Sleep(250);
                cout << " || 3. Overripe" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(5);
                std::cout << " || Enter the cost price of tomatoes [ UNITS : RS/KG, AVG IS RS.12/KG ] " << endl;
                std::cin >> input(6);
                cout << " ||" << endl;

                predictedCost = lr.predictCostPrice(input);
                Sleep(500);
                std::cout << " || Predicted Supply Amount: " << predictedCost << std::endl;
                Sleep(500);
            }
            else if (choice == 4) {
                lr.loadDataset("C:/Users/Aarus/OneDrive/Documents/demand.csv", choice);
                lr.trainModel();
                lr.evaluateModel();

                Eigen::VectorXd input(5);
                double predictedCost;
                cout << " || " << endl;
                std::cout << " || To predict the demand amount -> " << endl;
                Sleep(500);
                cout << " ||" << endl;
                std::cout << " || Choose the season : " << endl;
                Sleep(250);
                cout << " || 1. Spring" << endl;
                Sleep(250);
                cout << " || 2. Summer" << endl;
                Sleep(250);
                cout << " || 3. Monsoon" << endl;
                Sleep(250);
                cout << " || 4. Winter" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(0);
                cout << " ||" << endl;
                std::cout << " || Enter the income [ UNIT : LPA, AVG is 10 LPA ] ";
                cout << " || Income : ";
                std::cin >> input(1);
                cout << " ||" << endl;
                std::cout << " || Enter the Shelf Life of tomatoes [ UNITS : DAYS, AVG IS 11 DAYS ]  ";
                cout << " || Shelf Life : ";
                std::cin >> input(2);
                cout << " ||" << endl;
                std::cout << " || Enter the quality of tomatoes : " << endl;
                Sleep(250);
                cout << " || 1. Semi-Ripe" << endl;
                Sleep(250);
                cout << " || 2. Ripe" << endl;
                Sleep(250);
                cout << " || 3. Overripe" << endl;
                Sleep(250);
                cout << " || Choice : ";
                std::cin >> input(3);
                cout << " ||" << endl;
                std::cout << " || Enter the selling price of tomatoes [ UNIT : RS/KG, AVG IS RS.28/KG ] " << endl;
                cout << " || Selling Price : ";
                std::cin >> input(4);
                cout << " ||" << endl;

                predictedCost = lr.predictCostPrice(input);
                Sleep(500);
                std::cout << " || Predicted Demand Amount: " << predictedCost << std::endl;
                cout << " ||" << endl;
                Sleep(500);
            }
            else if (choice == 5) {
                check = false;
            }
            else {
                cout << "Invalid Choice!" << endl;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    linear lr;
    lr.linearmainmenu();
    return 0;
}