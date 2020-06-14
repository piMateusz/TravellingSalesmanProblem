//
// Created by pilecki on 13.11.2019.
//

#ifndef TSP_MAIN_H
#define TSP_MAIN_H

#include <iostream>
#include <vector>
#include <cfloat>
#include <algorithm>
#include <utility>
#include <map>

const double INF = DBL_MAX ;
using matrix = std::vector<std::vector<double>>;
using uint = unsigned;

class TSP_CostMatrix{
public:
    matrix CostMatrix;
    uint low_bound;
    uint num_of_summits;
    std::vector<double> forbidden_vector_indexes;
    std::vector<double> forbidden_column_indexes;
    std::vector<double> solution_vector;
    std::map<double, double> solution_map;
    TSP_CostMatrix(const matrix& CostMatrix_) : CostMatrix(CostMatrix_), low_bound(0),
                                                num_of_summits(uint(CostMatrix_.size())), solution_map({}), solution_vector({}),
                                                forbidden_column_indexes({}), forbidden_vector_indexes({}){}
    ~TSP_CostMatrix() {};
    const bool check_zero_in_row();
    const bool check_zero_in_col();
    const void reduce_all_rows();
    const void reduce_all_cols();
    const bool check_if_summit_visited(const uint &start);
    const void find_best_path();
    const void find_next_path();
    const std::vector<double> find_zero(const matrix &CostMatrix, const uint &start_summit);
    const double get_path_cost(const uint &row_num, const uint &col_num);
    const void eliminate_row_and_col(const uint &start_summit, const uint &stop_summit);
};

class TSP_solver{
public:

    TSP_CostMatrix TSPCostMatrix;
    uint algorithm_iteration;
    TSP_solver(const matrix& CostMatrix_): TSPCostMatrix(CostMatrix_), algorithm_iteration(0){}
    ~TSP_solver() {}
    const void print();
};

std::vector<int> tsp(matrix cost_matrix);
double get_forbidden_cost();
#endif //TSP_MAIN_H
