#include <iostream>
#include <vector>

using namespace std;

pair<int, int> checkRatio(float minAspectRatio, float maxAspectRatio, int width, int height, int id);
int findDivider(int a, int b);
bool isInteger(double num);

int main()
{
    float minAspectRatio = 0.5;
    float maxAspectRatio = 2;
    int width = 400;
    int height = 1000;
    int id = 1;
    int x, y;
    x = checkRatio(minAspectRatio, maxAspectRatio, width, height, id).first;
    y = checkRatio(minAspectRatio, maxAspectRatio, width, height, id).second;

    cout << "x: " << x << " y: " << y << endl;
    
    return 0;
}


pair<int, int> checkRatio(float minAspectRatio, float maxAspectRatio, int width, int height, int id)
{
    float ratio = static_cast<float>(width) / static_cast<float>(height);
    int newHeight, newWidth;
    cout <<"Min Aspect Ratio: " << minAspectRatio << " Max Aspect Ratio: " << maxAspectRatio << endl;
    cout << "width: " << width << " height: " << height << endl;
    cout << "ratio: " << ratio << endl;
    if ((ratio < minAspectRatio) || (ratio > maxAspectRatio))
    {
        cout <<"test" << endl;
        if(ratio < minAspectRatio)
        {
            int d = findDivider(width, height);
            cout << "d: " << d << endl;
            newHeight = height / d;
            newWidth = width * d;
            return checkRatio(minAspectRatio, maxAspectRatio, newWidth, newHeight, id);
        }
        else if(ratio > maxAspectRatio)
        {
            int d = findDivider(width, height);
            cout << "d: " << d << endl;
            newWidth = width / d;
            newHeight = height * d;
            return checkRatio(minAspectRatio, maxAspectRatio, newWidth, newHeight, id);
        }
    }
    cout << "Block id: " << id << " width: " << width << " height: " << height << endl;
    return make_pair(width, height);
};

int findDivider(int a, int b)
{
    vector<int> divisors = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    for(int d : divisors)
    {
        if(a % d == 0 && b % d == 0)
        {
            return d;
        }
    }
    return 1;
};

bool isInteger(double num) {
    return num == static_cast<int>(num);
}
