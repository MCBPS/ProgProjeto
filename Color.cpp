#include "Color.hpp"
#include<cassert>

namespace prog {
    Color::Color() {
        r = 0;
        g = 0;
        b = 0;
    }
    Color::Color(const Color& other) {
         this -> r = other.r;
         this -> g = other.g;
         this -> b = other.b;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        this -> r = red;
        this -> g = green;
        this -> b = blue;
    }
    rgb_value Color::red() const {
        return r;
    }
    rgb_value Color::green() const {
        return g;
    }
    rgb_value Color::blue() const {
        return b;
    }
    

    rgb_value& Color::red()  {  
        return r;
    }
    rgb_value& Color::green()  {
        return g;
    }
    rgb_value& Color::blue()  {
        return b; 
    }

}

