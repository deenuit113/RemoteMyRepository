#ifndef TETRIS_H
#define TETRIS_H
#include "Matrix.h"

enum TetrisState { Running, NewBlock, Finished };

class Tetris {
public:
    static Matrix **setOfBlockObjects;
    static int iScreenDw;
    static int nBlockTypes;
    static int nBlockDegrees;
    static void init(int **setOfCBlockArrays, int nBlockTypes, int nBlockDegrees);
public:
    int iScreenDy;
    int iScreenDx;
    int arrayScreenDy;
    int arrayScreenDx;
    int *arrayScreen;
    bool justStarted;
    TetrisState state;
    Matrix* currBlk;
    int top;
    int left;
    Matrix* tempBlk;
    int idxBlockDegree;
    int idxBlockType;
    Matrix* iScreen;
    Matrix* oScreen;
public:
    int* createArrayScreen();
    Tetris(int iScreenDy, int iScreenDx);
    ~Tetris();
    TetrisState accept(char key);
    void deleteFullLines();
};
#endif