#include <math.h>
#include <stdio.h>
#include <vector>

#include "barril.h"
#include "tiro.h"

using namespace std;


bool Barril::Valido(){
    return this->vida > 0;
}

void Barril::DesenhaBarril(GLfloat x, GLfloat y){
    vector<int> Cor =  {224, 104, 11};
    int h = 20;
    int w = 40;

    glRecti(0,0,10,50);

}



