// Region class definition
// class Region
#ifndef REGION_HPP
#define REGION_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Region
{
    private:
        string name;
        int id;
        int x0;
        int y0;
        int x1;
        int y1;
    public:
        Region(int id) : id(id){}

        int getId() const
        {
            return id;
        }

        void setX0(int x)
        {
            this->x0 = x;
        }

        void setY0(int y)
        {
            this->y0 = y;
        }

        void setX1(int x)
        {
            this->x1 = x;
        }

        void setY1(int y)
        {
            this->y1 = y;
        }

        void setName(string name)
        {
            this->name = name;
        }

        int getX0() const
        {
            return x0;
        }

        int getY0() const
        {
            return y0;
        }

        int getX1() const
        {
            return x1;
        }

        int getY1() const
        {
            return y1;
        }

        string getName() const
        {
            return name;
        }
};

#endif

        

