#ifndef Block_HPP
#define Block_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Block 
{
private:
    string name;
    int id;
    int width;
    int height;
    int x;
    int y;
    int weight;
    int nearSource;
    bool isNearSource; // changed from setNearSource
    float aspectRatio;
    bool isRotated;
    bool isPlaced;

public:
    Block(int id, int width, int height) : id(id), width(width), height(height), name("Block_" + to_string(id)), aspectRatio((float)width / height), isRotated(false), isPlaced(false), isNearSource(false), nearSource(0) {}
    Block(){}
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

    void rotate() 
    {
        int temp = width;
        width = height;
        height = temp;
        aspectRatio = (float)width / height;
        isRotated = !isRotated;
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
            if(wh == 0){
                nearSource = width;
            }else if(wh == 1){
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
};

#endif
