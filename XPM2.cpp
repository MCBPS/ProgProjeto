#include "XPM2.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

namespace prog {

    int hex_to_dec (string hex) {

        int result = 0;

        if (hex[1] >= '0' && hex[1] <= '9') {
            result += (int(hex[1]) - 48) * 1;
        }
        else if (hex[1] >= 'A' && hex[1] <= 'F') {
            result += (int(hex[1]) - 55) * 1;
        }
        else if (hex[0] >= 'a' && hex[0] <= 'f') {
            result += (int(hex[0]) - 'a' + 10) * 1;
        }

        if (hex[0] >= '0' && hex[0] <= '9') {
            result += (int(hex[0]) - 48) * 16;
        }
        else if (hex[0] >= 'A' && hex[0] <= 'F') {
            result += (int(hex[0]) - 55) * 16;
        }
        else if (hex[0] >= 'a' && hex[0] <= 'f') {
            result += (int(hex[0]) - 'a' + 10) * 16;
        }

        return result;
    }

    string dec_to_hex(Color in){
        string out = "#000000";

        char chr[] = "0123456789ABCDEF";

        out[1] = chr[in.red()/16]; // mais significativo
        out[2] = chr[in.red()%16]; // menos significativo

        out[3] = chr[in.green()/16];
        out[4] = chr[in.green()%16];

        out[5] = chr[in.blue()/16];
        out[6] = chr[in.blue()%16];

        return out;
    }


    Image* loadFromXPM2(const std::string& file) {
        ifstream f(file); // ler ficheiro
        string line;
        getline(f,line); //lixo
        getline(f,line); //ler informacao
        istringstream info(line); //lê um num de cada vez
        int width, height, n;
        info >> width >> height >> n;
        Image* img = new Image(width,height);
        map<char,Color> dic; //dict 
        for(int i = 0; i < n; i++){
            getline(f,line);
            istringstream iss(line);
            string symbol, blarg, colour;
            iss >> symbol >> blarg >> colour;
            int red, green, blue;
            red = hex_to_dec(colour.substr(1,2)); // substr prinmeiro o int onde começa e depois o size
            green = hex_to_dec(colour.substr(3,2));
            blue = hex_to_dec(colour.substr(5,2));
            Color c(red,green,blue);
            dic[symbol[0]] = c;
        }

        for(int y = 0; y < height; y++){
            getline(f,line);
            for(int x = 0; x < width; x++){
                img->at(x,y) = dic[ line[x] ]; // altera o pxl da imagem com a cor daquele pixel (line[x])

            }
        }


        return img;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        ofstream f(file);
        f << "! XPM2\n";
        map<string,char> dic; // leio uma string e associo a um char
        char cod[] = "*!#$&/()=-.,ZXCVBNMLKJHGFDSAQWERTYUIOPplokmijnuhbygvtfcrdxeszwaq2134567890"; // cada cor vai estar associada a um destes simbolos
        f << image->width() << ' ' << image->height() << ' ';
        
        for(int y = 0; y < image->height(); y++){ //ciclo para ver o num de cores dif
            for(int x = 0; x < image->width(); x++){
                string curColor = dec_to_hex(image->at(x,y));
                if(dic.find(curColor) == dic.end()) dic[curColor] = cod[dic.size()]; // estou à procura da cor no dicionario, caso nao exista, coloco-a lá

            }

        }

        f << dic.size() << " 1";
        for(auto c : dic){ // ciclo para escrever as cores dif
            f << '\n' << c.second << " c " << c.first;
        }


        for(int y = 0; y < image->height(); y++){ // ciclo para escrever a imagem com os chars
            f << '\n';
            for(int x = 0; x < image->width(); x++){
                string curColor = dec_to_hex(image->at(x,y));
                f << dic[curColor];
            }
        }
    }

    

    

    
}
