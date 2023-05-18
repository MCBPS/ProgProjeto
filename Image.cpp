#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    this -> w = w;
    this -> h = h;
    pixels = new Color[w*h];

    for(int i=0; i<w*h; i++){
      pixels[i] = fill;
    }


  }

  Image::~Image()
  {
    delete[] pixels;
  }

  int Image::width() const
  {
    return w;
  }

  int Image::height() const
  {
    return h;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels.
  //Color DUMMY_color;

  Color& Image::at(int x, int y)
  {
    return pixels[x + (y * w)];
  }

  const Color& Image::at(int x, int y) const
  {
    return pixels[x + (y * w)];
  }
}
