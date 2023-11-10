#ifndef PREDICTION_H
#define PREDICTION_H
#include <Eigen/Dense>
class linear {
public:
    linear();

    void loadDataset(const char* filename, int choice);
    void trainModel();
    void evaluateModel();
    double predictCostPrice(const Eigen::VectorXd& input);
    double predictCP(const Eigen::VectorXd& input);
    double predictSP(const Eigen::VectorXd& input);

    void linearmainmenu();

private:
    Eigen::MatrixXd X;
    Eigen::VectorXd Y;
    Eigen::VectorXd coefficients;
};


#endif