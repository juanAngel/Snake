#include "randontree.h"
#include <math.h>

#define DIS_MIN 15
#define PI 3.141613

int randonRange(int a, int b){
    return a+rand()%(b-a+1);
}



void MiArbol_PintarArbol(renderer::DrawContext context, SDL_Point p1, int distance, double angle, SDL_Color color){
    if (distance>=DIS_MIN) {
        int i,j;
        SDL_Point p2;
        p2.x = p1.x + distance*cos(angle);
        p2.y = p1.y - distance*sin(angle);

        for (i=0;i<distance/DIS_MIN;i++) {
            p1.x++;
            p2.x++;
            renderer::drawLine(context,&p1,&p2,&color);
        }
        p1.x -= distance/DIS_MIN;
        p2.x -= distance/DIS_MIN;

        SDL_Color colorAclarado = color;
        if (colorAclarado.r-1<235)
                colorAclarado.r += 20;
        if (colorAclarado.g-1<235)
                colorAclarado.g += 20;
        if (colorAclarado.b-1<235)
                colorAclarado.b += 20;


        int n = randonRange(1,7);


        for (j=1;j<=n;j++) {

            int hNueva = randonRange(distance,1.1*distance)*2.0/3.0;
            double anguloNuevo = angle-PI/2-PI/(2*n)+j*PI/n;
            MiArbol_PintarArbol(context,p2,hNueva,anguloNuevo,colorAclarado);
        }
    }


}

void drawTreeRandon(renderer::DrawContext context,SDL_Point p1,int distance,double angle, SDL_Color color){
    if (distance>=DIS_MIN) {
        int i,j;
        SDL_Point p2;
        p2.x = p1.x + distance*cos(angle);
        p2.y = p1.y - distance*sin(angle);

        for (i=0;i<distance/DIS_MIN;i++) {
            p1.x++;
            p2.x++;
            renderer::drawLine(context,&p1,&p2,&color);
        }
        p1.x -= distance/DIS_MIN;
        p2.x -= distance/DIS_MIN;

        SDL_Color colorAclarado = color;
        if (colorAclarado.r-1<235)
                colorAclarado.r += 20;
        if (colorAclarado.g-1<235)
                colorAclarado.g += 20;
        if (colorAclarado.b-1<235)
                colorAclarado.b += 20;


        int n = randonRange(1,7);

        int newDistance;
        double newAngle;
        for (j=1;j<=n;j++) {

            newDistance = randonRange(distance,1.1*distance)*2.0/3.0;
            newAngle = angle-PI/2-PI/(2*n)+j*PI/n;
            MiArbol_PintarArbol(context,p2,newDistance,newAngle,colorAclarado);
        }
    }
}

void drawForestRandon(renderer::DrawContext d, forestRandon* f){
    int treeWidth = f->Display->w/f->gridSize.w
            ,treeHeigth = f->Display->h/f->gridSize.h;
    SDL_Point p;
    int distance;
    double angle = PI/2;
    SDL_Color& colorArbol = f->treeColor;
    renderer::setClearColor(d,&f->backgrundColor);
    renderer::clear(d);

    int i,j;
    for (i=0;i<f->Display->w;i+=treeWidth){
        for(j=0;j<f->Display->h;j+=treeHeigth){
            distance=randonRange((treeWidth/2),treeWidth);
            p.x=randonRange(i,i+treeWidth-1);
            p.y=randonRange(j,j+treeHeigth-1);

            drawTreeRandon(d,p,distance,angle,colorArbol);
        }
    }

}
