//OpenGL Libs

#if defined __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

//General Libs
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <memory.h>
#include <string>
#include <stack>
#include <sstream>
#include <stdarg.h>

using namespace std;
typedef struct unidirectionalEdge_
{
	int node;
	double weight;
	double pheromone;
}unidirectionalEdge;

typedef struct graphNode_
{
	bool visibility;
	float posX,posY; //Coordenadas no plano cartesiiano
	//double pheromone;
	vector<unidirectionalEdge> v; //Adjscent List
}graphNode;

typedef struct mystack
{
	vector<int> myVectorStack;

	vector<int> getVector(){
		return myVectorStack;
	}
	int size()
	{
		return myVectorStack.size();
	}
	void push(int e){
		myVectorStack.push_back(e);
	}
	int top(){
		return myVectorStack[myVectorStack.size() - 1];
	}
	void pop(){
		if(myVectorStack.size() > 0){
			myVectorStack.erase(myVectorStack.begin() + myVectorStack.size() - 1);
		}else{
			cout << "Stack size is zero, cant remove" << endl;
			exit(1);
		}
	}
	bool exist(int e){
		for(int i = 0;i< myVectorStack.size();i++){
			if(myVectorStack[i] == e){
				return true;
			}
		}
		return false;
	}
	void cut(int pi, int pf){
		int quantToDelete = pf - pi;
		while(quantToDelete--){
			myVectorStack.erase(myVectorStack.begin() + pi + 1);
		}
	}
	void cutLoops(){
		bool flag = true;
		while(flag){
			for(int i = 0;i < myVectorStack.size() -1 ;i++){
				flag = false;
				for(int j = i+1; j< myVectorStack.size(); j++){
					if(myVectorStack[i] == myVectorStack[j]){
						flag = true;
						cut(i,j);
					}
				}
			}
		}
	}
}myStack;