#ifndef Block_HPP
#define Block_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Block
{
private:
    int id;
    int width;
    int height;
    int x;
    int y;
    int weight;
    int nearSource;
    bool isNearSource; // changed from setNearSource
    bool isRotated;
    bool isPlaced;
    string name;

public:
    Block(int id, int width, int height) : id(id), width(width), height(height)
    {
        x = 0;
        y = 0;
        weight = 0;
        nearSource = 0;
        isNearSource = false;
        isRotated = false;
        isPlaced = false;
        name = "Block_" + to_string(id);
    }

    Block() {}

    int getId()
    {
        return id;
    }

    int getWidth()
    {
        return width;
    }

    int getHeight()
    {
        return height;
    }

    int getWeight()
    {
        return weight;
    }

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

    void setHeight(int width)
    {
        this->height = height;
    }

    void setWidth(int height)
    {
        this->width = width;
    }

    void setWeight(int weight)
    {
        this->weight = weight;
    }

    void setX(int x)
    {
        this->x = x;
    }

    void setY(int y)
    {
        this->y = y;
    }

    void setIsRotated(bool isRotated)
    {
        this->isRotated = isRotated;
    }

    void rotate()
    {
        swap(width, height);
        isRotated = !isRotated;
    }

    bool getIsRotated()
    {
        return isRotated;
    }

    void place(int x, int y)
    {
        this->x = x;
        this->y = y;
        isPlaced = true;
    }

    void unplace()
    {
        x = 0;
        y = 0;
        isPlaced = false;
    }

    bool getIsNearSource() // renamed from isNearSource to getIsNearSource
    {
        return isNearSource;
    }

    void setIsNearSource(bool test, int wh) // setIsNearSource
    {
        if (test)
        {
            if (wh == 0)
            {
                nearSource = width;
            }
            else if (wh == 1)
            {
                nearSource = height;
            }
            isNearSource = true;
        }
        else
        {
            nearSource = 0;
            isNearSource = false;
        }
    }

    int getNearSource()
    {
        return nearSource;
    }

    void print()
    {
        cout << "Block id: " << id << endl;
        cout << "Block width: " << width << endl;
        cout << "Block height: " << height << endl;
        cout << "Block weight: " << weight << endl;
        cout << "Block x: " << x << endl;
        cout << "Block y: " << y << endl;
        cout << "Block nearSource: " << nearSource << endl;
        cout << "Block isNearSource: " << isNearSource << endl;
        cout << "Block isRotated: " << isRotated << endl;
    }
};

#endif
