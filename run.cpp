#include "http_server.cpp"
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

class COczko {  
public:   
  COczko() { kolor = "szary"; }   
  string dajKolor() { return kolor; }  
private:   
  string kolor;  
};    
class CMisio {  
public:   
  COczko lewe;   
  COczko prawe;  
};    

class Point {
private:
  float x, y;
public:
  Point(float x, float y) {
    if ((x*x) + (y*y) > 1) {
      throw std::invalid_argument("point out of the circle");
    }
    this->x = x;
    this->y = y;
  }
  float getX() {
    return this->x;
  }
  float getY() {
    return this->y;
  }
};

class CTowar {   
private:     
  int ilosc;     
  std::string nazwa;   
public:     
  CTowar() { nazwa = ""; ilosc = 0; }     
  void setNazwa(std::string _n) { nazwa = _n; }    
  void setIlosc(int _i) { ilosc = _i; }     
  void pisz() { cout << nazwa << ", ilosc: " << ilosc; }     
  CTowar operator+(int value) {
    CTowar result(*this);
    result.ilosc += value;
    return result;
  } 
}; 


int main(int, char** argv) {

std::map<std::string, int> dane;

dane.insert({"test", 1});


for(auto pair : dane) {
    std::cout << pair.first << " " << pair.second << std::endl;
};

 CTowar a; 
  a.setNazwa("jaja"); 
  a.setIlosc(5); 
  a = a+5; 
  a.pisz(); 

    return 0;
}