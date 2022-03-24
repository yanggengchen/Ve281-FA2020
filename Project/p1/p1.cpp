#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct coordinates{
    int x;
    int y;
};

coordinates lowest_point = {};

int ccw(const coordinates a, const coordinates b, const coordinates c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


bool compare(const coordinates &a, const coordinates &b){
    int angle = ccw(lowest_point, a, b);
    int distance_a = (a.x - lowest_point.x) * (a.x - lowest_point.x) + (a.y - lowest_point.y) * (a.y - lowest_point.y);
    int distance_b = (b.x - lowest_point.x) * (b.x - lowest_point.x) + (b.y - lowest_point.y) * (b.y - lowest_point.y);
    if (angle == 0) return distance_a < distance_b;
    else return angle > 0;
}

int main() {
    vector<coordinates> stack;
    vector<coordinates> result;
    int size;
    cin >> size;
    if (size == 0) return 0;
    for (int i = 0; i < size; i++){
        coordinates curr_coordinate = {};
        cin >> curr_coordinate.x >> curr_coordinate.y;
        stack.push_back(curr_coordinate);
    }
    coordinates p0 = stack[0];
    for (auto & i: stack){
        if (((i.y == p0.y) && (i.x < p0.x)) || (i.y < p0.y)) p0 = i;
    }
    lowest_point = p0;
    std::sort(stack.begin(), stack.end(), compare);
    for (auto & i: stack){
        while (result.size() > 1){
            if (ccw(result[result.size() - 2], result.back(), i) <= 0){
                result.pop_back();
            }
            else break;
        }
        result.push_back(i);
    }
    for (int i = 0; i < (int)result.size(); i++){
        if (i != 0){
            if ((result[i].x != result[i - 1].x) || (result[i].y != result[i - 1].y))
                cout << result[i].x << " " << result[i].y << endl;
        }
        else cout << result[i].x << " " << result[i].y << endl;
    }

}
