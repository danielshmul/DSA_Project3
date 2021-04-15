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