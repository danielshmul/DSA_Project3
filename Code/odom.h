#pragma once

class Odom 
{
    double px, py, pz;
    double lvx, lvy, lvz; 
    double avx, avy, avz;

public:
    Odom(double px, double py, double pz, 
        double lvx, double lvy, double lvz, 
        double avx, double avy, double avz) :
    px(px), py(py), pz(pz), lvx(lvx), lvy(lvy), lvz(lvz), avx(avx), avy(avy), avz(avz) {}
    
    double px_() const {return px;}
    double py_() const {return py;}
    double pz_() const {return pz;}

    double lvx_() const {return lvx;}
    double lvy_() const {return lvy;}
    double lvz_() const {return lvz;}

    double avx_() const {return avx;}
    double avy_() const {return avy;}
    double avz_() const {return avz;}
};