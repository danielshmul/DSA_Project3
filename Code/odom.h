#pragma once

class Odom 
{
    double px, py, pz;
    double lvx, lvy, lvz; 
    double avx, avy, avz;

public:
    Odom(double px_, double py_, double pz_, 
        double lvx_, double lvy_, double lvz_, 
        double avx_, double avy_, double avz_) :
    px(px_), py(py_), pz(pz_), lvx(lvx_), lvy(lvy_), lvz(lvz_), avx(avx_), avy(avy_), avz(avz_) {}
    
    double px() const {return px;}
    double py() const {return py;}
    double pz() const {return pz;}

    double lvx() const {return lvx;}
    double lvy() const {return lvy;}
    double lvz() const {return lvz;}

    double avx() const {return avx;}
    double avy() const {return avy;}
    double avz() const {return avz;}
};