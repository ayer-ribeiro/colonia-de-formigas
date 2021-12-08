#include "Ant.h"
#define FACTOR 1
/*====================Constructors and destructors====================*/
Ant::Ant( void ){
	//Ant constructor
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	red = 0.0;
	green = 0.0;
	blue = 0.0;
    positionX = 0;
    positionY = 0;
    positionZ = 0;
    visible = true;
    AI = true;
    searchingFood = true;
    walking = false;
}
Ant::~Ant( void ){
    //Ant destructor
}

/*====================Getters====================*/
float Ant::getColorR(){
    return red;
}
float Ant::getColorG(){
    return green;
}
float Ant::getColorB(){
    return blue;
}
float Ant::getXPosition(){
    return positionX;
}
float Ant::getYPosition(){
    return positionY;
}
float Ant::getZPosition(){
    return positionZ;
}
bool Ant::isVisible(){
    return visible;
}
bool Ant::isAI(){
    return AI;
}
string Ant::getName(){
    return name;
}

/*====================Setters====================*/
void Ant::setColor3f( float r, float g, float b){
    red = r;
    green = g;
    blue = b;
}
void Ant::setPosition( float x, float y, float z ){
    positionX = x;
    positionY = y;
    positionZ = z;
}
void Ant::setVisibility( bool v ){
    visible = v;
}
void Ant::setAI( bool ai ){
    AI = ai;
}
void Ant::setName( string n ){
    name = n;
}
void Ant::setRotate( float rx, float ry, float rz){
    rotateX = rx;
    rotateY = ry;
    rotateZ = rz;
}
void Ant::setScale( float sx, float sy, float sz){
    scaleX = sx;
    scaleY = sy;
    scaleZ = sz;
}
void Ant::setWalk(bool w){
    walking = w;
    if(!walking){
        //walking = true;
        //walkAnimation();
        walking = false;
    }
}
void Ant::setWalkSpeed( float speed ){
    walkSpeed = speed;
}

void Ant::setNest(int n){
	nest = n;
}
void Ant::setQuantGraphNodes(int n){
    quantGraphNodes = n;
}
void Ant::setObjective(int o){
	objective = o;
}
void Ant::setPheromoneFactor( double factor ){
	pheromoneFactor = factor;
}

/*====================Class methods====================*/
void Ant::wakeUp(graphNode g[], int n){
	nest = n;
	positionN = nest;
	int menor = 0;
	for(int i = 0 ;i < g[nest].v.size(); i++){
		if(g[nest].v[i].weight < g[nest].v[menor].weight){
			menor = i;
		}
	}
	shortestPossibleWay = g[nest].v[menor].weight;

}
void Ant::walkTo( float x, float y ){
    walkTargetX = x;
    walkTargetY = y;
}

float Ant::euclidianDistance( float  x1, float  z1, float x2, float z2 ){
    return sqrt( pow((x1 - x2 ),2.0f ) + (pow((z1 - z2 ),2.0f )));
}

float Ant::euclidianDistanceFromTarget( float x, float y ){
    return euclidianDistance( x,y, walkTargetX, walkTargetY );
}

bool Ant::isThereSomethingHere( float x, float z ){
	return true;
}
void Ant::chooseBest( float * x, float * z ){
	float actualX,actualZ;
	actualX = positionX;
	actualZ = positionY;
	float auxX,auxZ;
	float distBestWay = 9999999999;


	for(float  i = 0; i < 2*M_PI; i+=0.001 ){
		auxX = actualX + walkSpeed*cos(i);
		auxZ = actualZ + walkSpeed*sin(i);

		if( euclidianDistanceFromTarget(auxX,auxZ) < distBestWay ){
			(*x) = auxX;
			(*z) = auxZ;
			distBestWay = euclidianDistanceFromTarget(auxX,auxZ);
		}
	}
}

void Ant::smartWalkTo( float x, float z  ){
	float nx,nz;
	chooseBest( &nx, &nz );
	setPosition( nx, nz, positionZ );
	//walkInLineTo( nx, nz );

	//stop();
}

void Ant::walkInLineTo( float x, float y ){
  double oposto, adjascente;
    adjascente = -(x - positionX);
    oposto = (y - positionY);

    double aux =  atan(adjascente/oposto) * 180 / M_PI;
    if(oposto < 0) aux = aux + 180;
    antYAngle = aux + 90;
    //setRotate(0,0,antYAngle);

    float px,py;
    float passo = walkSpeed;
	setWalk(true);
	px = positionX;
	py = positionY;
	px = px + passo*cos(aux*M_PI/180);
	py = py + passo*sin(aux*M_PI/180);
	

}
void Ant::stop(){
    walkTo(positionX, positionY );
    setWalk(false);
    walkAnimation();
}
void Ant::walkAnimation( ){
    setRotate(0,antYAngle,0);
}
bool Ant::undefineActions(){
}
bool Ant::foundFood(){
	if(objective == positionN) return true;
	else return false;
}
bool Ant::isOnNest(){
	if(positionN == nest) return true;
    else return false;
}
double Ant::countNearPheromone( int n, graphNode g[]){
    double count = 0;
    vector<unidirectionalEdge> edges;
	edges = g[n].v;
    for(int i = 0; i < edges.size(); i++){
        count += edges[i].pheromone * (1.0/g[n].v[i].weight);
    }
    return count;
}
double Ant::sortRandomNumber(){
    double aux1 = rand()%100000001;
    return aux1/100000000.0;
}
int Ant::calcNextPosition(int n, graphNode g[]){
	double nearPheromone = countNearPheromone(positionN,g);
	if(nearPheromone == 0.0){
		int i = rand()%(g[n].v.size());
		return g[n].v[i].node;
	}
	double normalizatorFactor = 1.0/nearPheromone;
	double randomNumber = sortRandomNumber();
	double number = randomNumber/normalizatorFactor;
	vector<unidirectionalEdge> edges;
	edges = g[n].v;

	float count = 0;
	for(int i = 0; i < edges.size();i++){
		if(count > number){
			return edges[i-1].node;
		}
		count += edges[i].pheromone * (1.0/g[n].v[i].weight);
	}
	if(edges.size() != 0)
	return edges[edges.size()-1].node;

}
double Ant::countWayWeight(graphNode g[]){
	double count = 0;
	vector<int> vectorWay;
	vectorWay = way.getVector();
	for(int i = vectorWay.size() -1; i > 1; i--){
		int o = vectorWay[i];
		int d = vectorWay[i-1];
		int j = 0;
		while(g[o].v[j].node != d){
			j++;
		}
		count += g[o].v[j].weight;
		//cout << g[o].v[j].weight << endl;
	}
	return count;

}
void Ant::antAI(graphNode g[])
{
    if(searchingFood && !walking){
    	int ant = 0;
    	way.push(positionN);
        nextPosition = g[positionN].v[0].node;
        nextPosition = calcNextPosition(positionN, g);
		if(way.size() > 1){
			ant = way.getVector()[way.size() - 2];
			//do{
				//cout << nextPosition << endl << ant << endl << endl;;
				//nextPosition = calcNextPosition(positionN, g);
			//}while(nextPosition == ant);
		}
		setWalk(true);
		//setWalkSpeed(1.0f);
		int j = 0;
		while(g[positionN].v[j].node != nextPosition) j++;
		walkTo(g[nextPosition].posX,g[nextPosition].posY);
		setWalkSpeed(euclidianDistanceFromTarget(g[positionN].posX,g[positionN].posY)/g[positionN].v[j].weight);
		//cout << walkSpeed << endl;
		
    }
    
    if(searchingFood && walking){
    	
    	//if( abs(walkTargetX - positionX) <= 2*walkSpeed && abs(walkTargetY - positionY) <= 2*walkSpeed){	
    	if( euclidianDistanceFromTarget(positionX,positionY) < walkSpeed ){	

    		positionN = nextPosition;
			stop();
			if(foundFood()){
				searchingFood = false;
				way.push(nextPosition);
				way.cutLoops();
				quanCycleToFindFood = way.size();
				wayWeight = countWayWeight(g);
				pheromoneToDrop = 0.001*(wayWeight/shortestPossibleWay);
			}
		}else{
			smartWalkTo(walkTargetX,walkTargetY);
			walkAnimation();
		}
    }
    if(!searchingFood && !walking){
    	
    	if(isOnNest()){
    		searchingFood = true;
			way.pop();
    	}else{

			int actual = way.top();
			way.pop();
			int prox = way.top();
			nextPosition = prox;
			
			int j = 0;
			while(g[actual].v[j].node != prox) j++;
			g[actual].v[j].pheromone += pheromoneToDrop;
			//if(g[actual].v[j].pheromone > 1) g[actual].v[j].pheromone = 1;

			j = 0;
			while(g[prox].v[j].node != actual) j++;
			g[prox].v[j].pheromone += pheromoneToDrop;
			//if (g[prox].v[j].pheromone > 1) g[prox].v[j].pheromone = 1;
			setWalk(true);
			walkTo(g[nextPosition].posX,g[nextPosition].posY);
    	}
    }
    if(!searchingFood && walking){
    	if( abs(walkTargetX - positionX) <= 2*walkSpeed && abs(walkTargetY - positionY) <= 2*walkSpeed){
    		positionN = nextPosition;
    		stop();
    	}else{
    		smartWalkTo(walkTargetX,walkTargetY);
			walkAnimation();
    	}
    }
}

void Ant::controller(graphNode g[]){

    //if(proximity.size() == 0) return;
    if(isAI()){ 
        antAI(g);
    }else{

    }
}
void Ant::drawCircle(float r, int div){
	float pass = (2.0*M_PI)/(float)div;

	glBegin( GL_TRIANGLE_FAN );

	for(float  i = 0; i < 2*M_PI; i+=pass ){
		float auxX,auxY;
		auxX = r*cos(i);
        auxY = r*sin(i);
        glVertex3f(auxX,auxY,0);
	}
	glEnd();
}
void Ant::draw(){

    glPushMatrix();
        glTranslatef( positionX, positionY, positionZ );
        glScalef(scaleX,scaleY,scaleZ);
        glRotatef( antYAngle, 0,0,1 );
    	glPushMatrix();
    		glColor3f( red, green, blue );    
    		glPushMatrix();
    			glTranslatef(-2.3f, 0.0f, 0.0f);
    			drawCircle( 1.8f, 10);
    		glPopMatrix();

    		glPushMatrix();
    			drawCircle( 1.5f, 20 );
    		glPopMatrix();

    		glPushMatrix();
    			glTranslatef( 1.7f, 0.0f, 0.0f );
    			drawCircle( 1.0f, 10 );
    		glPopMatrix();
    	glPopMatrix();
    glPopMatrix();
}