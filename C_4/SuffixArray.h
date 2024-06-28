//
// Created by gyz on 24.06.24.
//

#ifndef C_4_SUFFIXARRAY_H
#define C_4_SUFFIXARRAY_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class SuffixArray {
private:
    std::string text;
    std::vector<int> SA;
private:
    std::vector<int> build_suffix_array_nlog2n(std::vector<int> &data, int alphabet_size);

    // SA-IS Coś nie działa :(
    std::vector<int> build_suffix_array(std::vector<int> &data, int alphabet_size);
    std::vector<bool> get_suffix_types(std::vector<int> &data);
    std::vector<int> get_bucket_sizes(std::vector<int> &data, int alphabet_size);
    std::vector<int> get_bucket_heads(std::vector<int> &bucket_sizes);
    std::vector<int> get_bucket_tails(std::vector<int> &bucket_sizes);
    void lms(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<bool> &types, std::vector<int> &bucket_sizes);
    bool is_lms(std::vector<bool> &types, int i);
    void induce_l(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<int> &bucket_sizes, std::vector<bool> &types);
    void induce_s(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<int> &bucket_sizes, std::vector<bool> &types);
    std::vector<int> reduce_suffix_array(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<bool> &types, std::vector<int> &offsets);
    bool is_lms_substring_equal(std::vector<int> &data, std::vector<bool> &types, int prev_offset, int current_offset);
    void lms_offsets(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<bool> &types,
                     std::vector<int> &bucket_sizes, std::vector<int> &offsets, std::vector<int> &reduced_suffix_array);

public:
    explicit SuffixArray(const std::string &text);
    std::vector<int> get_suffix_array(){return SA;};
    size_t find(const std::string &pattern);


};


#endif //C_4_SUFFIXARRAY_H
