/*
    Tarea 3 - TAP

    Jose Hernandez
    Luis Caceres
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Pos2 {
    int posX;
    int posY;
    int direction; //0 -> North, 1 -> East, 2 -> South, 3 -> West
    int cost;
    bool justRotated;
};

int mapSize;
int **MAP_A, **MAP_B; //0 -> Can Walk, 1 -> Destination, -1 -> Wall

Pos2 robotA, robotB;
Pos2 destiA, destiB;

vector<Pos2> robotAMoves, robotBMoves;

int moveArrY[] = {0, 1, 0, -1};
int moveArrX[] = {-1, 0, 1, 0};

int CURRENT_COST = 0;

int findPath(){
    bool AMoved = false, BMoved = false;
    bool ACanMove = false, BCanMove = false;

    CURRENT_COST = 0;

    int help;

    while(!robotAMoves.empty() && !robotBMoves.empty()){ //Do movement while there's tiles to study

        //cout << "Current cost: " << CURRENT_COST << " list size: " << robotAMoves.size() << endl;
        //cin >> help;

        for(int cID = 0; cID < robotAMoves.size(); cID++){
            //cout << "Pos Cost: " << robotAMoves[cID].cost << "\nCurrent Cost: " << CURRENT_COST <<endl;

            if(robotAMoves[cID].cost != CURRENT_COST) continue; //Only check positions with the current cost

//            cout << "Robot A: " << robotAMoves[cID].posX << " " << robotAMoves[cID].posY << " Cost: " << robotAMoves[cID].cost << " Direction: " << robotAMoves[cID].direction <<endl;
//            cout << "Robot B: " << robotBMoves[cID].posX << " " << robotBMoves[cID].posY << " Cost: " << robotBMoves[cID].cost << " Direction: " << robotBMoves[cID].direction <<endl;

            //Check if end reached
            if((robotAMoves[cID].posX == destiA.posX && robotAMoves[cID].posY == destiA.posY &&
                robotBMoves[cID].posX == destiB.posX && robotBMoves[cID].posY == destiB.posY)||
               (robotAMoves[cID].posX == destiB.posX && robotAMoves[cID].posY == destiB.posY &&
                robotBMoves[cID].posX == destiA.posX && robotBMoves[cID].posY == destiA.posY)){ //End reached

                return robotAMoves[cID].cost;
            }

            ACanMove = BCanMove = false;
            //Move forward
            //cout << "Move A next: " << robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction] << " " << robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction] <<endl;
            if(robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction] != robotBMoves[cID].posX + moveArrX[robotBMoves[cID].direction] ||
               robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction] != robotBMoves[cID].posY + moveArrY[robotBMoves[cID].direction]){ //Check if robots collides with each other

                if(robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction] < mapSize && //Check bounds for robot A
                   robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction] > -1      &&
                   robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction] < mapSize &&
                   robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction] > -1      ) {

                   //Space is walkable
                   if(MAP_A[robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction]][robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction]] == 0){
                        ACanMove = true;
                   }
                }
                if(robotBMoves[cID].posX + moveArrX[robotBMoves[cID].direction] < mapSize && //Check bounds for robot B
                   robotBMoves[cID].posX + moveArrX[robotBMoves[cID].direction] > -1      &&
                   robotBMoves[cID].posY + moveArrY[robotBMoves[cID].direction] < mapSize &&
                   robotBMoves[cID].posY + moveArrY[robotBMoves[cID].direction] > -1      ) { //B robot can move

                   //Space is walkable
                   if(MAP_B[robotBMoves[cID].posX + moveArrX[robotBMoves[cID].direction]][robotBMoves[cID].posY + moveArrY[robotBMoves[cID].direction]] == 0){
                        BCanMove = true;
                   }
                }

//                cout << "Can move: A: " << ACanMove << " B: " << BCanMove <<endl;
//                cout << "robotApos + dir X: " << robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction] << " Y: " << robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction] <<endl;
//                cout << "robotBpos       X: " << robotBMoves[cID].posX << " Y: " << robotBMoves[cID].posX <<endl;

                AMoved = BMoved = false;
                if(ACanMove || BCanMove){ //At least one of them can move
                    if((ACanMove && !BCanMove && (robotBMoves[cID].posX != robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction]  || //A moves, B doesn't move nor collides with B
                                                  robotBMoves[cID].posY != robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction]))||
                       (ACanMove && BCanMove)){ //Both robots moves //Do robot A movement

                        AMoved = true; //Robot A moved flag
                    }

                    if((BCanMove && !ACanMove && (robotAMoves[cID].posX != robotBMoves[cID].posX + moveArrX[robotBMoves[cID].direction]  || //B moves, A doesn't move nor collides with A
                                                  robotAMoves[cID].posY != robotBMoves[cID].posY + moveArrY[robotBMoves[cID].direction]))||
                       (ACanMove && BCanMove)){ //Both robots moves //Do robot A movement

                        BMoved = true; //Robot B moved flag
                    }
                }

                //cout << "Moved Flags: A: " << AMoved << " B: " << BMoved <<endl;

                if(AMoved){ //Robot A moved correctly, apply movement
                    Pos2 _robotA;

                    //A ROBOT
                    _robotA.cost = robotAMoves[cID].cost + 1; //Set robot A values
                    _robotA.direction = robotAMoves[cID].direction;
                    _robotA.posX = robotAMoves[cID].posX + moveArrX[robotAMoves[cID].direction];
                    _robotA.posY = robotAMoves[cID].posY + moveArrY[robotAMoves[cID].direction];
                    _robotA.justRotated = false;

                    //MAP_A[_robotA.posX][_robotA.posY] = 0;

                    robotAMoves.push_back(_robotA); //Add to list
                }
                if(BMoved){ //Robot A moved correctly, apply movement
                    Pos2 _robotB;

                    //B ROBOT
                    _robotB.cost = robotBMoves[cID].cost + 1; //Set robot B values
                    _robotB.direction = robotBMoves[cID].direction;
                    _robotB.posX = robotBMoves[cID].posX + moveArrX[robotBMoves[cID].direction];
                    _robotB.posY = robotBMoves[cID].posY + moveArrY[robotBMoves[cID].direction];
                    _robotB.justRotated = false;

                    //MAP_B[_robotB.posX][_robotB.posY] = 0;

                    robotBMoves.push_back(_robotB); //Add to list
                }

                if(AMoved && !BMoved){ //Robot A moved to a walkable tile but Robot B is stuck in front of a wall / border
                    Pos2 _robotB;

                    //B ROBOT
                    _robotB.cost = robotBMoves[cID].cost + 1; //Set robot B values
                    _robotB.direction = robotBMoves[cID].direction;
                    _robotB.posX = robotBMoves[cID].posX;
                    _robotB.posY = robotBMoves[cID].posY;
                    _robotB.justRotated = false;

                    //MAP_B[_robotB.posX][_robotB.posY] = robotB.cost;

                    robotBMoves.push_back(_robotB); //Add to list
                }
                else if(!AMoved && BMoved){ //Robot B moved correctly but Robot A is tuck in front of a wall / border
                    Pos2 _robotA;

                    //A ROBOT
                    _robotA.cost = robotAMoves[cID].cost + 1; //Set robot A values
                    _robotA.direction = robotAMoves[cID].direction;
                    _robotA.posX = robotAMoves[cID].posX;
                    _robotA.posY = robotAMoves[cID].posY;
                    _robotA.justRotated = false;

                    //MAP_A[_robotA.posX][_robotA.posY] = robotA.cost; //Update Map

                    robotAMoves.push_back(_robotA); //Add to list
                }
            }

            //Set Rotations positions
            if(!robotAMoves[cID].justRotated && !robotBMoves[cID].justRotated){ //No rotations set
                Pos2 _robotAL; //Robot A -> Rotate to left

                _robotAL.cost = robotAMoves[cID].cost + 1;
                _robotAL.direction = (robotAMoves[cID].direction - 1 < 0) ? 3 : robotAMoves[cID].direction - 1; //Apply rotation
                _robotAL.posX = robotAMoves[cID].posX;
                _robotAL.posY = robotAMoves[cID].posY;
                _robotAL.justRotated = true;

                Pos2 _robotBL; //Robot B -> Rotate to left

                _robotBL.cost = robotBMoves[cID].cost + 1;
                _robotBL.direction = (robotBMoves[cID].direction - 1 < 0) ? 3 : robotBMoves[cID].direction - 1; //Apply rotation
                _robotBL.posX = robotBMoves[cID].posX;
                _robotBL.posY = robotBMoves[cID].posY;
                _robotBL.justRotated = true;

                Pos2 _robotAR; //Robot A -> Rotate to right

                _robotAR.cost = robotAMoves[cID].cost + 1;
                _robotAR.direction = (robotAMoves[cID].direction + 1 > 3) ? 0 : robotAMoves[cID].direction + 1; //Apply rotation
                _robotAR.posX = robotAMoves[cID].posX;
                _robotAR.posY = robotAMoves[cID].posY;
                _robotAR.justRotated = true;

                Pos2 _robotBR; //Robot B -> Rotate to right

                _robotBR.cost = robotBMoves[cID].cost + 1;
                _robotBR.direction = (robotBMoves[cID].direction + 1 > 3) ? 0 : robotBMoves[cID].direction + 1; //Apply rotation
                _robotBR.posX = robotBMoves[cID].posX;
                _robotBR.posY = robotBMoves[cID].posY;
                _robotBR.justRotated = true;

                Pos2 _robotAD; //Robot A -> Rotate down

                _robotAD.cost = robotAMoves[cID].cost + 2;
                _robotAD.direction = robotAMoves[cID].direction % 4; //Apply rotation
                _robotAD.posX = robotAMoves[cID].posX;
                _robotAD.posY = robotAMoves[cID].posY;
                _robotAD.justRotated = true;

                Pos2 _robotBD; //Robot B -> Rotate down

                _robotBD.cost = robotBMoves[cID].cost + 2;
                _robotBD.direction = robotBMoves[cID].direction % 4; //Apply rotation
                _robotBD.posX = robotBMoves[cID].posX;
                _robotBD.posY = robotBMoves[cID].posY;
                _robotBD.justRotated = true;

                //Add all rotations to list
                robotAMoves.push_back(_robotAL); //Robot A
                robotAMoves.push_back(_robotAR);
                robotAMoves.push_back(_robotAD);

                robotBMoves.push_back(_robotBL); //Robot B
                robotBMoves.push_back(_robotBR);
                robotBMoves.push_back(_robotBD);
            }

            //Delete current position
            robotAMoves.erase(robotAMoves.begin()+cID);
            robotBMoves.erase(robotBMoves.begin()+cID);
            cID -= 1;

            //cout << "Robot A moves size: " << robotAMoves.size() <<endl;
            //cout << "Robot B moves size: " << robotBMoves.size() <<endl;
        }

        CURRENT_COST += 1;
    }

    return -1;
}

int main(){
    string line, trash;
    bool robotFound = false;

    cin >> mapSize; //Tamaño del mapa
    while(mapSize != 0){
        MAP_A = new int*[mapSize]; //Reservar memoria
        MAP_B = new int*[mapSize];
        for(int i = 0; i < mapSize; i++) {
            MAP_A[i] = new int[mapSize];
            MAP_B[i] = new int[mapSize];
        }

        //Flag initiliaze
        destiA.justRotated = destiB.justRotated = false;
        robotFound = false;

        getline(cin, trash); //Obtiene '\n'
        for(int i = 0; i < mapSize; i++){//Obtener mapa linea por linea
            getline(cin, line);
            for(int j = 0; j < mapSize; j++) {
                if(line[j] == '.') { //Walkable space
                    MAP_A[i][j] = 0;
                    MAP_B[i][j] = 0;
                }
                else if(line[j] == '*') { //Wall
                    MAP_A[i][j] = -1;
                    MAP_B[i][j] = -1;
                }
                else if(line[j] == 'D') { //Destination
                    MAP_A[i][j] = 0;
                    MAP_B[i][j] = 0;

                    if(!destiA.justRotated){//Set first destination
                        destiA.posX = i;
                        destiA.posY = j;
                        destiA.justRotated = true;
                    }
                    else {//Set second destination
                        destiB.posX = i;
                        destiB.posY = j;
                        destiB.justRotated = true;
                    }
                }
                else {// N-S-E-O
                    if(!robotFound) {
                        robotFound = true;

                        robotA.posX = i;
                        robotA.posY = j;

                        robotA.direction = ((line[j] == 'N') ? 0 : ((line[j] == 'E') ? 1 : ((line[j] == 'S') ? 2 : 3)));
                        robotA.cost = 0;
                        robotA.justRotated = false;

                        robotAMoves.push_back(robotA); //Add to check list

                        MAP_A[i][j] = 0;
                        MAP_B[i][j] = 0;
                    }
                    else {
                        robotB.posX = i;
                        robotB.posY = j;

                        robotB.direction = ((line[j] == 'N') ? 0 : ((line[j] == 'E') ? 1 : ((line[j] == 'S') ? 2 : 3)));
                        robotB.cost = 0;
                        robotB.justRotated = false;

                        robotBMoves.push_back(robotB); //Add to check list

                        MAP_A[i][j] = 0;
                        MAP_B[i][j] = 0;
                    }
                }
            }

            line = ""; //Limpiar string
        }

//        for(int k = 0; k < mapSize; k++){
//            for(int p = 0; p < mapSize; p++){
//                cout << MAP_A[k][p];
//            }
//            cout <<endl;
//        }

        //DO STUFF
        cout << findPath() << endl;


        for(int i = 0; i < mapSize; i++){ //Limpiar memoria
            delete [] MAP_A[i];
            delete [] MAP_B[i];
        }
        delete [] MAP_A;
        delete [] MAP_B;

        robotAMoves.clear();
        robotBMoves.clear();

        cin >> mapSize; //Nuevo tamaño de mapa
    }

    return 0;
}
