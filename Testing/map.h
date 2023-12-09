#ifndef MAP
#define MAP

#include "libs.h"

const int HEIGHT_MAP = 20;//размер карты высота
const int WIDTH_MAP = 25;//размер карты ширина

class Map{
public:
    string TileMap[HEIGHT_MAP]; // = массив с маской карты (тогда использовать конструктор без параметров)
    Map(string abs[HEIGHT_MAP]){
        for (int i=0; i<HEIGHT_MAP; i++){
            TileMap[i]=abs[i];
        }
    }
    Map() { }
    void randomMapGenerate(){//рандомно расставляем камни
        int randomElementX = 0;//переменная для хранения случайного элемента по горизонтали
        int randomElementY = 0;//переменная для хранения случайного элемента по вертикали
        int countStone = 5;//количество камней
        while (countStone>0){
            randomElementX = 1 + rand() % (WIDTH_MAP - 1);//псевдослучайное значение по “x” от 1 до
            //ширина карты-1. Ограничение введено чтобы не получать числа бордюра карты
            randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по “y”
            if (TileMap[randomElementY][randomElementX] == ' ') {//если встретили символ пробел,
                TileMap[randomElementY][randomElementX] = 's'; //то ставим туда камень.
                countStone--;
            }
        }
    }
    string* GetTileMap(){
        return TileMap;
    }
};

#endif // MAP

