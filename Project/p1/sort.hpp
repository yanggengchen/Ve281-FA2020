#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>

template<typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    int length = (int)vector.size();
    for (int i  = length - 1; i > 0; i--){ //sort from small to large
        for (int j  = length - 1; j > length - 1 - i; j--){
            if (comp(vector[j], vector[j - 1])){ // if vector[j] < vector[j - 1], swap
                T temp = vector[j];
                vector[j] = vector[j - 1];
                vector[j - 1] = temp;
            }
        }
    }
}

template<typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    int length = (int)vector.size();
    for(int i  = 1; i < length; i++){
        T temp = vector[i];
        for (int j = i - 1; j >= 0; j--){
            if (comp(temp, vector[j])){
                vector[j + 1] = vector[j];
                if (j == 0) vector[0] = temp;
            }
            else{
                vector[j + 1] = temp;
                break;
            }
        }
    }
}

template<typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    int length = (int)vector.size();
    for(int i = 0; i < length - 1; i++){
        T smallest = vector[i];
        int pos = i;
        for (int j = i; j < length; j++){
            if (comp(vector[j], smallest)){
                smallest = vector[j];
                pos = j;
            }
        }
        vector[pos] = vector[i];
        vector[i] = smallest;
    }
}

template<typename T, typename Compare>
void merge_helper(std::vector<T> &vector,int left, int mid, int right, Compare comp = std::less<T>()){
    int i = left;
    int j = mid + 1;
    std::vector<T> temp;
    while(i < mid + 1 && j <= right){
        if (!comp(vector[j],vector[i])){
            temp.push_back(vector[i]);
            i++;
        }
        else {
            temp.push_back(vector[j]);
            j++;
        }
    }
    if (i == mid + 1){
        while(j < right){
            temp.push_back(vector[j]);
            j++;
        }
    }
    if (j == right + 1){
        while(i < mid + 1){
            temp.push_back(vector[i]);
            i++;
        }
    }
    j--;
    int pos = (int)temp.size()-  1;
    bool is_empty = temp.empty();
    while(!is_empty){
        temp.pop_back();
        vector[j] = temp[pos];
        j--;
        pos--;
        is_empty = temp.empty();
    }
}

template<typename T, typename Compare>
void merge_sort_helper(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()){
    if (left >= right) return;
    int mid = (left + right) / 2;
    merge_sort_helper(vector, left, mid, comp);
    merge_sort_helper(vector, mid + 1, right, comp);
    merge_helper(vector, left, mid, right, comp);
}

template<typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    int length = (int)vector.size();
    merge_sort_helper(vector, 0, length - 1, comp);
}

template<typename T, typename Compare>
int partition_extra(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()){
    int pos = left;
    std::vector<T> front;
    std::vector<T> back;
    std::vector<T> temp;
    for(int i = left + 1; i <= right; i++){
        if (comp(vector[i], vector[left])){
            front.push_back(vector[i]);
        }
        else
            back.push_back(vector[i]);
    }
    bool is_empty = front.empty();
    while(!is_empty){
        temp.push_back(front.back());
        front.pop_back();
        pos++;
        is_empty = front.empty();
    }
    temp.push_back(vector[left]);
    is_empty = back.empty();
    while (!is_empty){
        temp.push_back(back.back());
        back.pop_back();
        is_empty = back.empty();
    }
    for (int i = left; i <= right; i++){
        vector[i] = temp[i - left];
    }
    return pos;
}

template<typename T, typename Compare>
void quick_sort_extra_helper(std::vector<T> &vector,int left, int right, Compare comp = std::less<T>()){
    int pivot;
    if (left >= right) return;
    pivot = partition_extra(vector, left, right, comp);
    quick_sort_extra_helper(vector, left, pivot - 1, comp);
    quick_sort_extra_helper(vector, pivot + 1, right, comp);
}

template<typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    int length = (int)vector.size();
    quick_sort_extra_helper(vector, 0, length - 1, comp);
}


template<typename T, typename Compare>
int partition_inplace(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()) {
    int i = left + 1;
    int j = right;
    while(i <= j){
        while(comp(vector[i], vector[left]) && (i < j)){
            i++;
        }
        while(!comp(vector[j], vector[left]) && (i < j)){
            j--;
        }
        if (i < j){
            T temp = vector[i];
            vector[i] = vector[j];
            vector[j] = temp;

        }
        i++;
        j--;
    }
    j++;
    if(!comp(vector[j], vector[left])) j--;
    T temp = vector[left];
    vector[left] = vector[j];
    vector[j] = temp;
    return j;
}

template<typename T, typename Compare>
void quick_sort_inplace_helper(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()){
    int pivot;
    if (left >= right) return;
    pivot = partition_inplace(vector, left, right, comp);
    quick_sort_inplace_helper(vector, left, pivot - 1, comp);
    quick_sort_inplace_helper(vector, pivot + 1, right, comp);
}

template<typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    int length = (int)vector.size();
    quick_sort_inplace_helper(vector, 0, length - 1, comp);
}

#endif //VE281P1_SORT_HPP
