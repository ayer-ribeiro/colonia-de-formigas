#ifndef ANT_H
#define ANT_H
#include "gLib.h"

using namespace std;


class Ant{
	//Atributos
private:
	float pheromoneToDrop;
	int quanCycleToFindFood;
	myStack way;
	bool searchingFood;
	int nest;
	int quantGraphNodes;
	int objective;
	double shortestPossibleWay;
	double wayWeight;
	double pheromoneFactor;

	int lastPosition;
	int positionN;
	int nextPosition;
	float positionX;
	float positionY;
	float positionZ;

	float rotateX;
	float rotateY;
	float rotateZ;

	float scaleX;
	float scaleY;
	float scaleZ;

	float red;
	float green;
	float blue;

	float walkTargetX;
	float walkTargetY;
	float antYAngle;

	float walkSpeed;
	bool walking;
	bool upCicle;

	string name;
	bool visible;
	bool AI;

public:
	/*Constructors and destructors*/
	Ant();
	~Ant();

	/*Getters*/
	float getColorR();
	float getColorG();
	float getColorB();
	float getXPosition();
	float getYPosition();
	float getZPosition();
	bool isVisible();
	bool isAI();
	string getName();

	/*Setters*/
	void setColor3f( float, float, float );
	void setPosition( float, float, float );
	void setVisibility( bool );
	void setAI( bool );
	void setName(string);

	void setRotate( float, float, float );
	void setScale( float, float, float );
	void setWalk( bool );
	void setWalkSpeed( float );
	void setNest( int );
	void setQuantGraphNodes( int );
	void setObjective( int );
	void setPheromoneFactor( double );


	/*Class methods*/
	void wakeUp(graphNode[], int);
	void walkInLineTo( float, float );
	void walkTo( float, float );
	float euclidianDistance( float, float, float, float );
	float euclidianDistanceFromTarget( float, float );
	bool isThereSomethingHere( float, float );
	void chooseBest( float*, float* );
	void smartWalkTo( float, float );
	void walkAnimation();
	void stop();
	bool undefineActions();

	bool foundFood();
	bool isOnNest();
	double countNearPheromone( int , graphNode[] );
	double sortRandomNumber();
	int calcNextPosition( int, graphNode[] );

	double countWayWeight(graphNode[]);
	void antAI(graphNode[]);
	void controller( graphNode[]);
	void drawCircle(float, int);
	void draw();
};
#endif