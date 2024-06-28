//
// Created by gyz on 24.06.24.
//

#include "SuffixArray.h"


SuffixArray::SuffixArray(const std::string &text) {
    this->text = text;
    std::vector<int> data(text.size());
    for(int i=0; i<text.size(); i++) {
        data[i] = (uint8_t) text[i];
    }
    SA = build_suffix_array_nlog2n(data, 256);

}

std::vector<int> SuffixArray::build_suffix_array(std::vector<int> &data, int alphabet_size) {
    std::vector<bool> types = get_suffix_types(data);
    std::vector<int> bucket_sizes = get_bucket_sizes(data, alphabet_size);
    size_t size = data.size();
    std::vector<int> suffix_array(size, -1);
    lms(suffix_array, data, types, bucket_sizes);
    induce_l(suffix_array, data, bucket_sizes, types);
    induce_s(suffix_array, data, bucket_sizes, types);

    std::vector<int> offsets;
    std::vector<int> reduced_suffix_array = reduce_suffix_array(suffix_array, data, types, offsets);
    for(auto &i : suffix_array) {
        i = -1;
    }
    lms_offsets(suffix_array, data, types, bucket_sizes, offsets, reduced_suffix_array);
    induce_l(suffix_array, data, bucket_sizes, types);
    induce_s(suffix_array, data, bucket_sizes, types);
    return suffix_array;


}

std::vector<bool> SuffixArray::get_suffix_types(std::vector<int> &data) {

    int size = (int) data.size();
    std::vector<bool> types(size+1);
    types[size] = true;
    if(size == 0) return types;
    types[size-1] = false;
    for(int i = size-2; i >= 0; i--) {
        if(data[i] < data[i+1]) {
            types[i] = true;
        } else if(text[i] > text[i+1]) {
            types[i] = false;
        } else {
            types[i] = types[i+1];
        }
    }
    return types;
}

std::vector<int> SuffixArray::get_bucket_sizes(std::vector<int> &data, int alphabet_size) {

    std::vector<int> bucket_sizes(alphabet_size + 1, 0);
    for(int i : data) {
        bucket_sizes[i]++;
    }
    return bucket_sizes;
}

std::vector<int> SuffixArray::get_bucket_heads(std::vector<int> &bucket_sizes) {

        std::vector<int> bucket_heads(bucket_sizes.size());
        int offset=1;
        for(int i=1; i<bucket_sizes.size(); i++) {
            bucket_heads[i] = offset;
            offset += bucket_sizes[i];
        }
        return bucket_heads;
}

std::vector<int> SuffixArray::get_bucket_tails(std::vector<int> &bucket_sizes) {

        std::vector<int> bucket_tails(bucket_sizes.size());
        int offset=1;
        for(int i=0; i<bucket_sizes.size(); i++) {
            offset += bucket_sizes[i];
            bucket_tails[i] = offset - 1;
        }
        return bucket_tails;
}

void SuffixArray::lms(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<bool> &types,
                      std::vector<int> &bucket_sizes) {

    suffix_array[0] = data.size();
    std::vector<int> bucket_tails = get_bucket_tails(bucket_sizes);
    for(int i = data.size() - 1; i >= 0; i--) {
        if(!is_lms(types, i)) continue;
        suffix_array[bucket_tails[data[i]]] = i;
        bucket_tails[data[i]]--;
    }

}

bool SuffixArray::is_lms(std::vector<bool> &types, int i) {

    if(i == 0) return false;
    return types[i] && !types[i-1];
}

void SuffixArray::induce_l(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<int> &bucket_sizes,
                           std::vector<bool> &types) {

    std::vector<int> bucket_heads = get_bucket_heads(bucket_sizes);
    for(int i = 0; i < suffix_array.size(); i++) {
        int j = suffix_array[i] - 1;
        if(j<0 || types[j]) continue;
        suffix_array[bucket_heads[data[j]]] = j;
        bucket_heads[data[j]]++;
    }

}

void SuffixArray::induce_s(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<int> &bucket_sizes,
                           std::vector<bool> &types) {

    std::vector<int> bucket_tails = get_bucket_tails(bucket_sizes);
    for(int i = suffix_array.size() - 1; i >= 0; i--) {
        int j = suffix_array[i] - 1;
        if(j < 0 || !types[j]) continue;
        suffix_array[bucket_tails[data[j]]] = j;
        bucket_tails[data[j]]--;
    }

}

std::vector<int> SuffixArray::reduce_suffix_array(std::vector<int> &suffix_array, std::vector<int> &data,
                                 std::vector<bool> &types, std::vector<int> &offsets) {

    std::vector<int> lms_names(text.size() + 1, -1);
    int current_name = 0;
    int count = 1;
    lms_names[suffix_array[0]] = current_name;
    int current_offset, prev_offset = suffix_array[0];
    for(int i = 1; i < suffix_array.size(); i++) {
        if(!is_lms(types, suffix_array[i])) continue;
        current_offset = suffix_array[i];
        if(!is_lms_substring_equal(data, types, prev_offset, current_offset)) current_name++;
        prev_offset = current_offset;
        lms_names[current_offset] = current_name;
        count++;
    }

    std::vector<int> reduced_text(count);
    offsets.resize(count);

    for(int i = 0, j = 0; i < lms_names.size(); i++) {
        if(lms_names[i] == -1) continue;
        reduced_text[j] = lms_names[i];
        offsets[j] = i;
        j++;
    }

    if(current_name+1 == reduced_text.size()) {
        std::vector<int> reduced_suffix_array(reduced_text.size()+1);
        reduced_suffix_array[0] = reduced_text.size();
        for(int i = 1; i < reduced_text.size(); i++) {
            reduced_suffix_array[reduced_text[i] + 1] = i;
        }
        return reduced_suffix_array;
    }
    return build_suffix_array(reduced_text, current_name+1);

}

bool SuffixArray::is_lms_substring_equal(std::vector<int> &data, std::vector<bool> &types, int prev_offset,
                                         int current_offset) {

    if(prev_offset == data.size() || current_offset == data.size()) return false;
    if(data[prev_offset] != data[current_offset]) return false;
    int i = 1;
    while(i < data.size()) {
        bool is_prev_lms = is_lms(types, prev_offset + i);
        bool is_curr_lms = is_lms(types, current_offset + i);
        if(is_prev_lms && is_curr_lms) return true;
        if(is_prev_lms != is_curr_lms) return false;
        if(text[prev_offset + i] != text[current_offset + i]) return false;
        i++;
    }
    return false;
}

void SuffixArray::lms_offsets(std::vector<int> &suffix_array, std::vector<int> &data, std::vector<bool> &types,
                              std::vector<int> &bucket_sizes, std::vector<int> &offsets, std::vector<int> &reduced_suffix_array) {
    suffix_array[0] = data.size();
    std::vector<int> bucket_tails = get_bucket_tails(bucket_sizes);
    for(int i = reduced_suffix_array.size() - 1; i > 1; i--) {
        int char_index = offsets[reduced_suffix_array[i]];
        int bucket_index = data[char_index];
        suffix_array[bucket_tails[bucket_index]] = char_index;
        bucket_tails[bucket_index]--;
    }

}

std::vector<int> SuffixArray::build_suffix_array_nlog2n(std::vector<int> &data, int alphabet_size) {

        std::vector<int> suffix_array(data.size());
        for(int i = 0; i < data.size(); i++) {
            suffix_array[i] = i;
        }
        std::sort(suffix_array.begin(), suffix_array.end(), [&data](int i, int j) {
            while(i < data.size() && j < data.size()) {
                if(data[i] != data[j]) return data[i] < data[j];
                i++;
                j++;
            }
            return i == data.size();
        });
        return suffix_array;
}

size_t SuffixArray::find(const std::string &pattern) {
        int l = 0;
        int r = text.size() - 1;
        while(l <= r) {
            int m = (l + r) / 2;
            int res = text.compare(SA[m], pattern.size(), pattern);
            if(res == 0) return SA[m];
            if(res < 0) l = m + 1;
            else r = m - 1;
        }
        return std::string::npos;
}
