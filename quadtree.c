#include "quadtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>     /* OpenGL functions */
#endif

unsigned int first = 1;
char desenhaBorda = 1;

QuadNode* newNode(int x, int y, int width, int height)
{
    QuadNode* n = malloc(sizeof(QuadNode));
    n->x = x;
    n->y = y;
    n->width = width;
    n->height = height;
    n->NW = n->NE = n->SW = n->SE = NULL;
    n->color[0] = n->color[1] = n->color[2] = 0;
    n->id = first++;
    return n;
}

QuadNode* geraQuadtree(Img* pic, float minDetail)
{
    // Converte o vetor RGB para uma MATRIZ que pode acessada por pixels[linha][coluna]
    RGB (*pixels)[pic->width] = (RGB(*)[pic->width]) pic->img;

    // Veja como acessar os primeiros 10 pixels da imagem, por exemplo:
    int i;

    int width = pic->width;
    int height = pic->height;

    //////////////////////////////////////////////////////////////////////////
    // Implemente aqui o algoritmo que gera a quadtree, retornando o nodo raiz
    //////////////////////////////////////////////////////////////////////////

    QuadNode* raiz;
    raiz = drawRecursiveNode(0, 0, width, height, pic, minDetail);
    return raiz;
}

// Limpa a memória ocupada pela árvore
void clearTree(QuadNode* n)
{
    if(n == NULL) return;
    if(n->status == PARCIAL)
    {
        clearTree(n->NE);
        clearTree(n->NW);
        clearTree(n->SE);
        clearTree(n->SW);
    }
    //printf("Liberando... %d - %.2f %.2f %.2f %.2f\n", n->status, n->x, n->y, n->width, n->height);
    free(n);
}

// Ativa/desativa o desenho das bordas de cada região
void toggleBorder() {
    desenhaBorda = !desenhaBorda;
    printf("Desenhando borda: %s\n", desenhaBorda ? "SIM" : "NÃO");
}

// Desenha toda a quadtree
void drawTree(QuadNode* raiz) {
    if(raiz != NULL)
        drawNode(raiz);
}

// Grava a árvore no formato do Graphviz
void writeTree(QuadNode* raiz) {
    FILE* fp = fopen("quad.dot", "w");
    fprintf(fp, "digraph quadtree {\n");
    if (raiz != NULL)
        writeNode(fp, raiz);
    fprintf(fp, "}\n");
    fclose(fp);
    printf("\nFim!\n");
}

void writeNode(FILE* fp, QuadNode* n)
{
    if(n == NULL) return;

    if(n->NE != NULL) fprintf(fp, "%d -> %d;\n", n->id, n->NE->id);
    if(n->NW != NULL) fprintf(fp, "%d -> %d;\n", n->id, n->NW->id);
    if(n->SE != NULL) fprintf(fp, "%d -> %d;\n", n->id, n->SE->id);
    if(n->SW != NULL) fprintf(fp, "%d -> %d;\n", n->id, n->SW->id);
    writeNode(fp, n->NE);
    writeNode(fp, n->NW);
    writeNode(fp, n->SE);
    writeNode(fp, n->SW);
}

// Desenha todos os nodos da quadtree, recursivamente
void drawNode(QuadNode* n)
{
    if(n == NULL) return;

    glLineWidth(0.1);

    if(n->status == CHEIO) {
        glBegin(GL_QUADS);
        glColor3ubv(n->color);
        glVertex2f(n->x, n->y);
        glVertex2f(n->x+n->width-1, n->y);
        glVertex2f(n->x+n->width-1, n->y+n->height-1);
        glVertex2f(n->x, n->y+n->height-1);
        glEnd();
    }

    else if(n->status == PARCIAL)
    {
        if(desenhaBorda) {
            glBegin(GL_LINE_LOOP);
            glColor3ubv(n->color);
            glVertex2f(n->x, n->y);
            glVertex2f(n->x+n->width-1, n->y);
            glVertex2f(n->x+n->width-1, n->y+n->height-1);
            glVertex2f(n->x, n->y+n->height-1);
            glEnd();
        }
        drawNode(n->NE);
        drawNode(n->NW);
        drawNode(n->SE);
        drawNode(n->SW);
    }
    // Nodos vazios não precisam ser desenhados... nem armazenados!
}

QuadNode* drawRecursiveNode(int x, int y, int h, int w, Img* pic, float minDetail)
{
    QuadNode* node = newNode(x, y, w, h);
    unsigned char difIntensity[4];
    getDifMeanIntensity(x, y, h, w, pic, difIntensity);
    if(difIntensity[3] < minDetail){
        node->status = CHEIO;
        node->color[0] = difIntensity[0];
        node->color[1] = difIntensity[1];
        node->color[2] = difIntensity[2];
        return node;
    }
    node->status = PARCIAL;
    node->color[0] = difIntensity[0];
    node->color[1] = difIntensity[1];
    node->color[2] = difIntensity[2];
    node->NE = drawRecursiveNode(x + w/2, y, h/2, w/2, pic, minDetail);
    node->NW = drawRecursiveNode(x, y, h/2, w/2, pic, minDetail);
    node->SE = drawRecursiveNode(x + w/2, y + h/2, h/2, w/2, pic, minDetail);
    node->SW = drawRecursiveNode(x, y + h/2, h/2, w/2, pic, minDetail);
    return node;
    
}

void getDifMeanIntensity(int x, int y, int height, int width, Img* pic, unsigned char* a) 
{
    int total = height*width;
    int totalR = 0, totalG = 0, totalB = 0;
    RGB (*pixels)[pic->width] = (RGB(*)[pic->width]) pic->img;
    for(int h = y; h < y+height; h++){
        for(int w = x; w < x+width; w++) {
            totalR += pixels[h][w].r;
            totalG += pixels[h][w].g;
            totalB += pixels[h][w].b;
        }
    }
    int mediaR = totalR/total;
    int mediaG = totalG/total;
    int mediaB = totalB/total;
    int dif = getDif(x, y, height, width, pic, mediaR, mediaG, mediaB);
    a[0] = mediaR;
    a[1] = mediaG;
    a[2] = mediaB;
    a[3] = dif;
}

int getDif(int x, int y, int height, int width, Img* pic, int mediaR, int mediaG, int mediaB)
{
    int totalDif = 0;
    int total = height*width;
    RGB (*pixels)[pic->width] = (RGB(*)[pic->width]) pic->img;
    for(int h = y; h < y+height; h++){
        for(int w = x; w < x+width; w++) {
            totalDif += sqrt(pow(pixels[h][w].r - mediaR, 2) + pow(pixels[h][w].g - mediaG, 2) + pow(pixels[h][w].b - mediaB, 2));
        }
    }
    return totalDif/total;
}