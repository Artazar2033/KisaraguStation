#include "map.h"

Map::Map(string abs[HEIGHT_MAP]){
    for (int i=0; i<HEIGHT_MAP; i++){
        TileMap[i]=abs[i];
    }
}

Map::Map() { }

void Map::randomMapGenerate(){//рандомно расставляем камни
    int randomElementX = 0;//переменная для хранения случайного элемента по горизонтали
    int randomElementY = 0;//переменная для хранения случайного элемента по вертикали
    int countStone = 5;//количество камней
    while (countStone > 0){
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);//псевдослучайное значение по “x” от 1 до
        //ширина карты-1. Ограничение введено чтобы не получать числа бордюра карты
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по “y”
        if (TileMap[randomElementY][randomElementX] == ' ') {//если встретили символ пробел,
            TileMap[randomElementY][randomElementX] = 's'; //то ставим туда камень.
            countStone--;
        }
    }
}

string* Map::GetTileMap(){
    return TileMap;
}

void Map::draw(Sprite* s_map, RenderWindow* window){ //сюда можно подать указатель на другой спрайт,
                                                    //тогда карта будет выглядеть по-другому
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap[i][j] == ' ') s_map->setTextureRect(IntRect(0, 0, 32, 32));
            if (TileMap[i][j] == 's') s_map->setTextureRect(IntRect(32, 0, 32, 32));
            if (TileMap[i][j] == '0') s_map->setTextureRect(IntRect(64, 0, 32, 32));
            if (TileMap[i][j] == 'f') s_map->setTextureRect(IntRect(96, 0, 32, 32));//цветок
            if (TileMap[i][j] == 'h') s_map->setTextureRect(IntRect(128, 0, 32, 32));//сердце
            if (TileMap[i][j] == '?') s_map->setTextureRect(IntRect(162, 0, 32, 32));//вертикальная дверь, верх
            if (TileMap[i][j] == '!') s_map->setTextureRect(IntRect(193, 0, 32, 32));//вертикальная дверь, низ
            s_map->setPosition(j * 32, i * 32);
            (*window).draw(*s_map);
        }
}
