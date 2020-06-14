#include "tsp.hpp"

const bool TSP_CostMatrix::check_zero_in_row() {
    bool flag = false;
    for (uint row = 0; row < num_of_summits; row++){
        if(std::find(forbidden_vector_indexes.begin(), forbidden_vector_indexes.end(), row) == forbidden_vector_indexes.end()){
            if(std::find(CostMatrix[row].begin(), CostMatrix[row].end(), 0) == CostMatrix[row].end()){
                flag = true;
                break;
            }
        }
    }
    return flag;
}

const bool TSP_CostMatrix::check_zero_in_col(){
    bool flag = false;
    for (uint j = 0; j < num_of_summits; j++) {
        std::vector<double> column = {};
        for (uint row = 0; row < num_of_summits; row++) {
            for (uint col = 0; col < num_of_summits; ++col) {
                if (col == j) {
                    column.push_back(CostMatrix[row][col]);
                }
            }
        }
        if(std::find(forbidden_column_indexes.begin(), forbidden_column_indexes.end(), j) == forbidden_column_indexes.end()) {
            if (std::find(column.begin(), column.end(), 0) == column.end()) {
                flag = true;
                break;
            }
        }
    }
    return flag;
}

const void TSP_CostMatrix::reduce_all_cols(){
    if(check_zero_in_row() || check_zero_in_col()){
        for (uint j = 0; j < num_of_summits; j++) {
            if(std::find(forbidden_column_indexes.begin(), forbidden_column_indexes.end(), j) == forbidden_column_indexes.end()){
                std::vector<double> column = {};
                for (uint row = 0; row < num_of_summits; row++) {
                    for (uint col = 0; col < num_of_summits; ++col) {
                        if (col == j) {
                            column.push_back(CostMatrix[row][col]);
                        }
                    }
                }
                auto min = *std::min_element(column.begin(), column.end());
                for (auto& x : column) {
                    x -= min;
                }
                low_bound += min;
                for (uint Row = 0; Row < num_of_summits; Row++) {
                    CostMatrix[Row][j] = column[Row];
                }
            }
        }
    }
}

const void TSP_CostMatrix::reduce_all_rows(){
    if(check_zero_in_row() || check_zero_in_col()){
        for(uint row = 0; row < num_of_summits; row++){
            if(std::find(forbidden_vector_indexes.begin(), forbidden_vector_indexes.end(), row) == forbidden_vector_indexes.end()){
                auto min = *std::min_element(CostMatrix[row].begin(), CostMatrix[row].end());
                for(auto &col : CostMatrix[row]){
                    col -= min;
                }
                low_bound+= min;
            }
        }
    }
}

const bool TSP_CostMatrix::check_if_summit_visited(const uint &start){
    return std::find(solution_vector.begin(), solution_vector.end(), start) == solution_vector.end();
}

const double TSP_CostMatrix::get_path_cost(const uint &row_num,const uint &col_num){
    matrix copy = CostMatrix;
    copy[row_num][col_num] = INF;
    auto min_row = *std::min_element(copy[row_num].begin(), copy[row_num].end());
    std::vector<double> column = {};
    for(uint summit = 0; summit < copy.size(); summit ++){
        column.push_back(copy[summit][col_num]);
    }
    auto min_col = *std::min_element(column.begin(),column.end());
    return min_col + min_row;
}

const void TSP_CostMatrix::eliminate_row_and_col(const uint &start_summit,const uint &stop_summit){
    for(uint summit = 0; summit < num_of_summits; summit++){
        CostMatrix[start_summit][summit] = INF;
        CostMatrix[summit][stop_summit] = INF;
    }
    forbidden_column_indexes.push_back(stop_summit);
    forbidden_vector_indexes.push_back(start_summit);
}

const std::vector<double> TSP_CostMatrix::find_zero(const matrix &CostMatrix,const uint &start_summit){
    std::vector<double> result = {};
    for(double end_summit = 0; end_summit < num_of_summits; end_summit++){
        if(CostMatrix[start_summit][end_summit] == 0) {
            result.push_back(end_summit);

        }
    }
    return result;
}

const void TSP_CostMatrix::find_next_path(){
    double temp_cost = -1;
    std::pair<double, double> next_summit = {};
    for(uint start_summit = 0; start_summit < num_of_summits; start_summit++){
        if(check_if_summit_visited(start_summit + 1)){
            auto stop_summit_vector = find_zero(CostMatrix, start_summit);
            for (auto &stop_summit : stop_summit_vector) {
                auto path_cost = get_path_cost(start_summit, stop_summit);
                if (path_cost > temp_cost) {
                    next_summit = std::make_pair(start_summit, stop_summit);
                    temp_cost = path_cost;
                }
            }
        }
    }
    solution_map[next_summit.first + 1] = next_summit.second + 1;
    solution_vector.push_back(next_summit.first + 1);
    CostMatrix[next_summit.second][next_summit.first] = INF;
    eliminate_row_and_col(next_summit.first, next_summit.second);
}

const void TSP_CostMatrix::find_best_path(){
    reduce_all_rows();
    reduce_all_cols();
    find_next_path();
}

const void TSP_solver::print() {
    for(auto &row : TSPCostMatrix.CostMatrix){
        for(auto &col : row){
            std::cout << col << "   ";
        }
        std::cout << std::endl;
    }
}

std::vector<int> tsp(matrix cost_matrix){
    TSP_solver A = TSP_solver(cost_matrix);
    while(A.algorithm_iteration < A.TSPCostMatrix.num_of_summits){
        A.TSPCostMatrix.find_best_path();
        A.algorithm_iteration ++;
    }
    std::vector<int> result;
    for (const auto &pair : A.TSPCostMatrix.solution_map) {
        if(!result.size()){
            result.push_back(pair.first);
            result.push_back(pair.second);
        }
        else{
            auto it = A.TSPCostMatrix.solution_map.find(result.back());
            result.push_back(it->second);
        }
    }
    return result;
}

double get_forbidden_cost(){
    return INF;
}



