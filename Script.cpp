#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <vector>
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

   /*done*/ Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    /*done*/Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert")
            {
                invert();
                continue;
            }
            if (command == "to_gray_scale")
            {
                to_gray_scale();
                continue;
            }
            
            if (command == "replace")
            {   
                replace();
                continue;
            }
            
            if (command == "fill")
            {
                fill();
                continue;
            }
            
            if (command == "h_mirror")
            {
                h_mirror();
                continue;
            }
            
            if (command == "v_mirror")
            {
                v_mirror();
                continue;
            }
            
            if (command == "add")
            {
                add();
                continue;
            }
            
            if (command == "crop")
            {
                crop();
                continue;
            }
            
            if (command == "rotate_left")
            {
                rotate_left();
                continue;
            }
            
            if (command == "rotate_right")
            {
                rotate_right();
                continue;
            }
            
            if (command == "median_filter")
            {   
                median_filter();
                continue;
            }
            
            if (command == "xpm2_open")
            {
                clear_image_if_any();
                string fname;
                input >> fname;
                image = loadFromXPM2(fname);
                continue;
            }
            
            if (command == "xpm2_save")
            {
                string fname;
                input >> fname;
                saveToXPM2(fname,image);
                continue;
            }
            

            // TODO ...

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert() {
        // Transforms each individual pixel (r, g, b) to (255-r,255-g,255-b).
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++)
        {
            for(int y =0; y < h; y++)
            {
                Color& pixels = (*image).at(x,y);
                pixels.red() = 255 - pixels.red();
                pixels.green() = 255 - pixels.green(); 
                pixels.blue() = 255 - pixels.blue();

            }
        }
    }
    void Script:: to_gray_scale()
    {
        //Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3. 
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++)
        {
            for(int y =0; y < h; y++)
            {
                Color& pixels = (*image).at(x,y);
                int grey;
                grey = (pixels.red() + pixels.green() + pixels.blue()) / 3;
                pixels.red() = grey;
                pixels.green() = grey; 
                pixels.blue() = grey;
            }
        }
    }
    void Script:: replace()
    {
        //Replaces all (r1,  g1, b1) pixels by (r2,  g2, b2).
        int r1,g1,b1,r2,g2,b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++)
        {
            for(int y =0; y < h; y++)
            {
                Color& pixels = (*image).at(x,y);
                if(pixels.red()==r1 && pixels.green()==g1 && pixels.blue()== b1)
                {
                    pixels.red() = r2;
                    pixels.green() = g2;
                    pixels.blue() = b2; 
                }
            }
        }
    }
    void Script::fill()
    {
        //Assign (r, g, b) to all pixels contained in rectangle defined by top-left corner (x, y), width w, and height h, i.e., all pixels (x', y') 
        //such that x <= x' < x + w and y <= y' < y + h. You may assume that the rectangle is always within the current image bounds.
        int x,y,w,h,r,g,b;
        input >> x >> y >> w >> h >> r >> g >> b;
        int moving_w;
        int moving_h;
        moving_w = image -> width();
        moving_h = image -> height();

        for(int moving_x = 0; moving_x < moving_w; moving_x++)
        {
            for(int moving_y =0; moving_y < moving_h; moving_y++)
            {
                Color& pixels = (*image).at(moving_x,moving_y);
                if(moving_x < x + w && x <= moving_x && moving_y < y + h && y <= moving_y)
                {
                    pixels.red() = r;
                    pixels.green() = g;
                    pixels.blue() = b; 
                }
            }
        }
    }

    void Script::h_mirror()
    {
        //Mirror image horizontally. Pixels (x, y) and (width() - 1 - x, y) for all 0 <= x < width() / 2 and 0 <= y < height().
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        for(int x = 0; x < w/2; x++)
        {
            for(int y =0; y < h; y++)
            {   
                swap((*image).at(w-1-x,y), (*image).at(x,y));
            }
        }
    }
    void Script::v_mirror()
    {
        //Mirror image vertically. Pixels (x, y) and (x, height() - 1 - y) for all 0 <= x < width() and 0 <= y < height() / 2.
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++)
        {
            for(int y =0; y < h/2; y++)
            {   
                swap((*image).at(x,h-1-y),(*image).at(x,y));
            }
        }

    }    

    void Script:: add()
    {
        string filename;
        int r,g,b,x,y;
        input >> filename >> r >> g >> b >> x >> y;
        Image *secondary = loadFromPNG(filename);
        int w2;
        int h2;
        w2 = secondary -> width();
        h2 = secondary -> height();

        for(int moving_x = 0; moving_x < w2; moving_x++)
        {
            for(int moving_y = 0; moving_y < h2; moving_y++)
            {   
                Color& pixels = (*image).at(x+moving_x,y+moving_y);
                Color& secondary_pixels = (*secondary).at(moving_x, moving_y);
                if(secondary_pixels.red()!=r || secondary_pixels.green()!=g || secondary_pixels.blue()!=b)
                {
                    pixels.red() = secondary_pixels.red();
                    pixels.green() = secondary_pixels.green();
                    pixels.blue() = secondary_pixels.blue();
                }

            }
        }
        delete secondary;
        
    }

    void Script:: crop()
    {
        int x,y,w,h;
        input >> x >> y >> w >> h;

        Image* temporary_image = new Image(w,h);
        for(int moving_x=0; moving_x<w; moving_x++)
        {
            for(int moving_y=0; moving_y<h;moving_y++)
            {
                (*temporary_image).at(moving_x,moving_y) = (*image).at(x+moving_x,y+moving_y);
            }
        }
        delete image;
        image = temporary_image;
    }

    void Script:: rotate_left()
    {
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        Image* temporary_image = new Image(h,w);
        for(int x = 0; x<h;x++)
        {
            for(int y = 0; y < w; y++)
            {
                (*temporary_image).at(x,y) = (*image).at(w-y-1,x);
            }
        }     
        delete image;
        image = temporary_image;   
    }

    void Script:: rotate_right()
    {
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        Image* temporary_image = new Image(h,w);
        for(int x = 0; x<h;x++)
        {
            for(int y = 0; y < w; y++)
            {
                (*temporary_image).at(x,y) = (*image).at(y,h-x-1);
            }
        }     
        delete image;
        image = temporary_image;           
    }

    void Script:: median_filter()
    {
        int ws;
        input >> ws;
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        Image* middle = new Image(w,h);
        vector<int> list_r, list_g, list_b;

        for(int x = 0; x<w;x++)
        {
            for(int y =0 ; y < h; y++)
            {
                for(int fx = max(0,x-ws/2); fx<=min(w-1,x+ws/2);fx++)
                { 
                    for(int fy = max(0,y-ws/2); fy<=min(h-1,y+ws/2);fy++)
                    {
                        Color& pixel = (*image).at(fx,fy);
                        list_r.push_back(pixel.red());
                        list_g.push_back(pixel.green());
                        list_b.push_back(pixel.blue());

                    }

                }
                    sort(list_r.begin(), list_r.end());
                    sort(list_g.begin(), list_g.end());
                    sort(list_b.begin(), list_b.end());

                    int size = list_r.size();

                    if(size%2 == 0)
                    {
                        (*middle).at(x,y).red() = (list_r.at(size/2) + list_r.at(size/2 -1 ))/2;
                        (*middle).at(x,y).green() = (list_g.at(size/2) + list_g.at(size/2 -1 ))/2;
                        (*middle).at(x,y).blue() = (list_b.at(size/2) + list_b.at(size/2 -1 ))/2; 
                    }
                    
                    else
                    {
                    (*middle).at(x,y).red() = list_r.at(size/2);
                    (*middle).at(x,y).green() = list_g.at(size/2);
                    (*middle).at(x,y).blue() = list_b.at(size/2);
                    }
     

                    list_r.clear();
                    list_g.clear();
                    list_b.clear();            

            }
        }     
        delete image;
        image = middle;
    }

}

