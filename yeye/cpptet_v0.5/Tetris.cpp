#include <math.h>
#include "Tetris.h"

Matrix** Tetris::setOfBlockObjects = NULL;
int Tetris::iScreenDw = 0;
int Tetris::nBlockTypes = 0;
int Tetris::nBlockDegrees = 0;

void Tetris::init(int **setOfCBlockArrays, int nBlockTypes, int nBlockDegrees){
    Tetris::nBlockTypes = nBlockTypes;
    Tetris::nBlockDegrees = nBlockDegrees;
    int arrayBlk_maxSize = 0;
    for(int i = 0; i < nBlockTypes*nBlockDegrees; i+=nBlockDegrees){ // nBlockTypes
        int len;
        for(len = 0; setOfCBlockArrays[i][len] != -1; len++);
        if(arrayBlk_maxSize <= len){
            arrayBlk_maxSize = len;
        }
    }
    iScreenDw = sqrt(arrayBlk_maxSize);
    setOfBlockObjects = new Matrix*[nBlockTypes];
    for(int i = 0; i < nBlockTypes; i++){
        setOfBlockObjects[i] = new Matrix[nBlockDegrees];
        for(int j = 0; j < nBlockDegrees; j++){
            int len, n;
            for(len = 0; setOfCBlockArrays[i * nBlockDegrees + j][len] != -1; len++);
            n = sqrt(len);
            setOfBlockObjects[i][j] = Matrix(setOfCBlockArrays[i * nBlockDegrees + j], n, n);
            setOfBlockObjects[i][j].mulc(i + 102);
        }
    }
}

int* Tetris::createArrayScreen(){
    arrayScreenDy = iScreenDy + iScreenDw;
    arrayScreenDx = iScreenDw * 2 + iScreenDx;
    arrayScreen = new int[arrayScreenDy * arrayScreenDx];
    for(int i = 0; i < arrayScreenDy; i++){
        for(int j = 0; j < arrayScreenDx; j++){
            arrayScreen[i * arrayScreenDx + j] = 0;
        }
    }
    for(int y = 0; y < iScreenDy; y++){
        for(int x = 0; x < iScreenDw; x++){
            arrayScreen[y * arrayScreenDx + x] = 100;
        }
        for(int x = 0; x < iScreenDx; x++){
            arrayScreen[y * arrayScreenDx + iScreenDw + x] = 0;
        }
        for(int x = 0; x < iScreenDw; x++){
            arrayScreen[y * arrayScreenDx + iScreenDw + iScreenDx + x] = 100;
        }
    }

    for(int y = 0; y < iScreenDw; y++){
        for(int x = 0; x < arrayScreenDx; x++){
            arrayScreen[(iScreenDy + y) * arrayScreenDx + x] = 100;
        }
    }

    return arrayScreen;
}

Tetris::Tetris(int iScreenDy, int iScreenDx)
:iScreenDy(iScreenDy), iScreenDx(iScreenDx){
    currBlk = NULL;
    tempBlk = NULL;
    state = Running;
    top = 0;
    left = 0;
    idxBlockType = 0;
    idxBlockDegree = 0;
    arrayScreen = createArrayScreen();
    iScreen = new Matrix(arrayScreen, arrayScreenDy, arrayScreenDx);
    oScreen = new Matrix(iScreen);
    this->justStarted = true;
}

TetrisState Tetris::accept(char key){
    Matrix* tmp = NULL;
    state = Running;
    if(key >= '0' && key <= '6'){
        if(this->justStarted == false){
            deleteFullLines();
        }
        delete iScreen;
        iScreen = new Matrix(oScreen);
        idxBlockType = key - '0';
        idxBlockDegree = 0;
        if(currBlk != NULL)
            delete currBlk;
        currBlk = new Matrix(setOfBlockObjects[idxBlockType][idxBlockDegree]);

        top = 0;
        left = iScreenDw + iScreenDx/2;
        if(tempBlk != NULL){
            delete tempBlk;
            tempBlk = NULL;
        }
        tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
        tmp = tempBlk;
        tempBlk = tempBlk->add(currBlk);
        if(tmp != NULL) {
            delete tmp;
            tmp = NULL;
        }
        justStarted = false;
        cout <<endl;

        if(tempBlk->anyGreaterThan(200)){
            state = Finished;
        }
        delete oScreen;
        oScreen = new Matrix(iScreen);
        oScreen->paste(tempBlk, top, left);
        return state;
    }
    else if(key == 'q'){

    }
    else if(key == 'a'){ // move left
        left -= 1;
    }
    else if(key == 'd'){ // move right
        left += 1;
    }
    else if(key == 's'){ // move down
        top += 1;
    }
    else if(key == 'w'){ // rotate the block clockwise
        idxBlockDegree = (idxBlockDegree + 1) % nBlockDegrees;
        if(currBlk != NULL)
            delete currBlk;
        currBlk = new Matrix(setOfBlockObjects[idxBlockType][idxBlockDegree]);
    }
    else if(key == ' '){ // drop the block
        while(!(tempBlk->anyGreaterThan(200))) {
            top += 1;
            if(tempBlk != NULL){
                delete tempBlk;
                tempBlk = NULL;
            }
            tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
            tmp = tempBlk;
            tempBlk = tempBlk->add(currBlk);
            if(tmp != NULL) {
                delete tmp;
                tmp = NULL;
            }
        }
    }
    else{
        cout <<"Wrong key!!!"<<endl;
    }
    if(tempBlk != NULL){
        delete tempBlk;
        tempBlk = NULL;
    }
    tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    tmp = tempBlk;
    tempBlk = tempBlk->add(currBlk);
    if(tmp != NULL) {
        delete tmp;
        tmp = NULL;
    }
    if(tempBlk->anyGreaterThan(200)){
        if(key == 'a'){ // move left
            left += 1;
        }
        else if(key == 'd'){ // move right
            left -= 1;
        }
        else if(key == 's'){ // move down
            top -= 1;
            state = NewBlock;
        }
        else if(key == 'w'){ // rotate the block clockwise
            idxBlockDegree = (idxBlockDegree - 1) % nBlockDegrees;
            if(currBlk != NULL)
                delete currBlk;
            currBlk = new Matrix(setOfBlockObjects[idxBlockType][idxBlockDegree]);
        }
        else if(key == ' ') { // drop the block
            top -= 1;
            state = NewBlock;
        }
        if(tempBlk != NULL){
            delete tempBlk;
            tempBlk = NULL;
        }
        tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
        tmp = tempBlk;
        tempBlk = tempBlk->add(currBlk);
        if(tmp != NULL) {
            delete tmp;
            tmp = NULL;
        }
    }
    delete oScreen;
    oScreen = new Matrix(iScreen);
    oScreen->paste(tempBlk, top, left);
    return state;
}

void Tetris::deleteFullLines(){
    Matrix* tmpScreen = new Matrix(arrayScreen, arrayScreenDy, arrayScreenDx);
    int tmpScreenTop = iScreenDy-1;
    for(int y = iScreenDy-1; y >= 0; y--){
        bool need_delete = true;
        for(int x = 0; x < iScreenDx; x++) { // iScreenDw
            if(oScreen->get_array()[y][iScreenDw + x] == 0){
                need_delete = false;
            }
        }
        if(!need_delete){
            Matrix* tmp = oScreen->clip(y, iScreenDw, y + 1, iScreenDw + iScreenDx);
            tmpScreen->paste(tmp, tmpScreenTop, iScreenDw);
            tmpScreenTop--;
        }
    }
    delete oScreen;
    oScreen = tmpScreen;
    return;
}

Tetris::~Tetris(){
    for(int i = 0; i < nBlockTypes; i++){
        delete[] setOfBlockObjects[i];
    }
    delete[] setOfBlockObjects;
    delete[] arrayScreen;

    delete iScreen;
    delete oScreen;
}