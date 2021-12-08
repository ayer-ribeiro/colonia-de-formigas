//#include "gLib.h"
#include "Ant.h"

#define MAP_SIZE 600
#define MAX_NODES 100
int windowsH;
int windowsW;

vector<Ant> ants;
graphNode g[MAX_NODES];
int objetivo;
int nest;
int quantNodes;

double disappearPheromone;
double pheromoneAntsFactor;
int quantAnts;

GLdouble lastIdleCycle;
GLdouble currentTime;
GLdouble difference;
int interval;
GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;

void print(string text, int x, int y){
    glRasterPos2f(x,y);
    for(int i = 0; i <text.size(); i++){
        glutBitmapCharacter(font_style, text[i]);
    }
    return;
}

void decPheromone(){
	for(int i = 1; i <= quantNodes; i++){
		for(int j = 0; j < g[i].v.size(); j++){
			g[i].v[j].pheromone -= disappearPheromone;
			if(g[i].v[j].pheromone < 0.0001) g[i].v[j].pheromone = 0.0001;
		}
	}
	
}

void drawCircle(float r, int div){
	float pass = (2.0*M_PI)/(float)div;
	glBegin( GL_TRIANGLE_FAN );
	for(float i = 0; i < 2*M_PI; i+=pass){
		float auxX,auxY;
		auxX = r*cos(i);
		auxY = r*sin(i);
		glVertex3f(auxX,auxY,0);
	}
	glEnd();
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT );

	//Draw Edges
	glLineWidth(0.04f*MAP_SIZE);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	
	for(int i = 1; i <= quantNodes; i++){
		for(int j = 0; j< g[i].v.size(); j++){
			int node = g[i].v[j].node;	
			glBegin(GL_LINES);
			glColor3f( 1, 1 - g[i].v[j].pheromone, 1);
			glVertex3f(g[i].posX, g[i].posY, 0.0f);
			glVertex3f(g[node].posX, g[node].posY, 0.0f);
			glEnd();

			stringstream sstm;
			sstm << (g[i].v[j].pheromone - 0.0001);
			glColor3f(0.3f,0.3f,0.3f);
			print(sstm.str(), (g[i].posX +  g[node].posX)/2.0f,  (g[i].posY + g[node].posY)/2.0f + 5 );
			stringstream sstm2;
			sstm2 << g[i].v[j].weight;
			glColor3f(0.0f,0.0f,0.0f);
			print(sstm2.str(), (g[i].posX +  g[node].posX)/2.0f,  (g[i].posY + g[node].posY)/2.0f - 10);
		}
	}
	
	//Draw nest
	glPushMatrix();
		glTranslatef( g[nest].posX,g[nest].posY, 0);
		glScalef(0.002*MAP_SIZE, 0.002*MAP_SIZE, 0.002*MAP_SIZE);    
		glColor3f( 0.6f, 0.3f, 0.1f );
		drawCircle( 20.0f ,20 );
	glPopMatrix();

	//Draw Nodes
	for(int i = 1; i <= quantNodes; i++){
		glPushMatrix();
			glTranslatef(g[i].posX, g[i].posY, 0.0f);
			glScalef(0.002*MAP_SIZE, 0.002*MAP_SIZE, 0.002*MAP_SIZE);
			glColor3f( 0.7f, 0.7f, 0.1f );
			if(i == objetivo ) glColor3f( 0.1f, 0.0f, 1.0f );
			if(i != nest && g[i].visibility)
				drawCircle( 20.0f ,20 );
		glPopMatrix();

		glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			stringstream sstm;
			sstm << i;
			print(sstm.str(), g[i].posX - 2.5f,  g[i].posY- 2.5f);
		glPopMatrix();
	}
	//Draw Ants
	for(int i = 0; i< ants.size(); i++){
		ants[i].setScale(0.004*MAP_SIZE, 0.004*MAP_SIZE, 0.004*MAP_SIZE);
		ants[i].draw();
	}
	glutSwapBuffers();
}

void init( void )
{
	srand(time(NULL));
	windowsW = 800;
	windowsH = 600;
	interval = 0;
	disappearPheromone = 0.000001;
	pheromoneAntsFactor = 0.01;
	quantAnts = 15;

	glClearColor( 0.9f, 0.9f, 0.9f, 0.0 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, MAP_SIZE, 0,  MAP_SIZE, -1.0, 1.0 );

	FILE * fp;
	int idNode,n,k;
	float positionx, positiony;
	unidirectionalEdge auxEdge;
	fp = fopen("graph.txt","r");
	fscanf(fp,"%d\n",&quantNodes);
	
	fscanf(fp,"%d %d",&nest,&objetivo);
	do{
		objetivo = rand()%10 + 1;
	}while(objetivo == 5);
	cout << objetivo << endl;
	
	n = quantNodes;
	while(n--){
		fscanf(fp,"%d %f %f %d",&idNode,&positionx,&positiony,&k);
		g[idNode].posX = positionx;
		g[idNode].posY = positiony;
		g[idNode].visibility = true;
		while(k--){
			int conectedNode;
			float conectionWeith;
			fscanf(fp,"%d %f",&conectedNode, &conectionWeith);
			auxEdge.node = conectedNode;
			auxEdge.weight = conectionWeith;
			auxEdge.pheromone = 0.0001;
			g[idNode].v.push_back(auxEdge);
		}
	}

	Ant ant;
	ant.setPosition(g[nest].posX,g[nest].posY,0);
	ant.wakeUp(g,nest);
	ant.setObjective(objetivo);
	ant.setPheromoneFactor(pheromoneAntsFactor);

	ants.push_back(ant);

}


void idle( void ){

	currentTime = glutGet(GLUT_ELAPSED_TIME);
	difference = currentTime - lastIdleCycle;

	if(ants.size() < quantAnts){
		Ant ant;
		ant.setPosition(g[nest].posX,g[nest].posY,0);
		ant.wakeUp(g,nest);
		ant.setObjective(objetivo);
		ant.setPheromoneFactor(pheromoneAntsFactor);
		ants.push_back(ant);

	}

	if (ants.size() > quantAnts){
		ants.erase(ants.begin() + ants.size() - 1);
	}
	for(int i = 0; i < ants.size(); i++){
		ants[i].setPheromoneFactor(pheromoneAntsFactor);
	}

	if(difference >= interval){
		decPheromone();
		for(int i = 0;i < ants.size();i++){
			ants[i].controller(g);
		}
		lastIdleCycle = currentTime;
		glutPostRedisplay();
	}
}
void keyboard(unsigned char tecla, int x, int y){
	switch(tecla){
		case 27:
			exit(0);
			break;
		case 's':
			if(interval == 0) interval = 100;
			else interval = 0;
			break;
	}
	glutPostRedisplay();
}
void SpecialKeys(int key, int x, int y){
	switch (key){
		case GLUT_KEY_PAGE_UP:
            quantAnts++;
            break;
        case GLUT_KEY_PAGE_DOWN:
            quantAnts--;
            if(quantAnts <= 0) quantAnts = 0;
            break;
		case GLUT_KEY_LEFT:
			disappearPheromone -= 0.00000001;
			if(disappearPheromone <= 0) disappearPheromone = 0;
			break;
		case GLUT_KEY_UP:
			disappearPheromone += 0.00000001;
			break;
		case GLUT_KEY_RIGHT:
			pheromoneAntsFactor += 0.001;
			break;
		case GLUT_KEY_DOWN:
			pheromoneAntsFactor -= 0.001;
			if (pheromoneAntsFactor <= 0) pheromoneAntsFactor = 0;
			break;
		default:
		break;
	}
}
void reshape( int w, int h ){ 
	windowsH = h;
	windowsW = w;
    //cout << "Largura: " << largura << " Altura: " << altura << endl;
}

int main(int argc, char *argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize( 600, 600 );

	//Atenção!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	glutInitWindowPosition( 1500, 30 );
	glutCreateWindow( "Hello" );

	init();


	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutSpecialFunc( SpecialKeys );

	glutMainLoop();
	return 0;
}

