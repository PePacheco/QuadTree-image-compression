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
<<<<<<< HEAD
int c = 0;
=======
>>>>>>> 53611d6e247b6dbe8e9f944e080003b384db5e6c

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
<<<<<<< HEAD
    // for(i=0; i<10; i++)
        // printf("%02X %02X %02X\n",pixels[0][i].r,pixels[1][i].g,pixels[2][i].b);
=======
    for(i=0; i<10; i++)
        printf("%02X %02X %02X\n",pixels[0][i].r,pixels[1][i].g,pixels[2][i].b);
>>>>>>> 53611d6e247b6dbe8e9f944e080003b384db5e6c

    int width = pic->width;
    int height = pic->height;

    //////////////////////////////////////////////////////////////////////////
    // Implemente aqui o algoritmo que gera a quadtree, retornando o nodo raiz
    //////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
    QuadNode* raiz;
    raiz = drawRecursiveNode(0, 0, width, height);

// #define DEMO
// #ifdef DEMO

//     /************************************************************/
//     /* Teste: criando uma raiz e dois nodos a mais              */
//     /************************************************************/
//     int c = 1;
//     QuadNode* raiz = newNode(0,0,width,height);
//     raiz->status = PARCIAL;
//     raiz->color[0] = 0;
//     raiz->color[1] = 0;
//     raiz->color[2] = 255;

//     drawRecursiveNode(raiz, height, width, c);

// #endif
=======
// COMENTE a linha abaixo quando seu algoritmo ja estiver funcionando
// Caso contrario, ele ira gerar uma arvore de teste com 3 nodos

#define DEMO
#ifdef DEMO

    /************************************************************/
    /* Teste: criando uma raiz e dois nodos a mais              */
    /************************************************************/
    int c = 1;
    QuadNode* raiz = newNode(0,0,width,height);
    raiz->status = PARCIAL;
    raiz->color[0] = 0;
    raiz->color[1] = 0;
    raiz->color[2] = 255;

    drawRecursiveNode(raiz, height, width, c);

#endif
>>>>>>> 53611d6e247b6dbe8e9f944e080003b384db5e6c
    // Finalmente, retorna a raiz da árvore
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

<<<<<<< HEAD
QuadNode* drawRecursiveNode(int x, int y, int h, int w)
{
    c++;
    QuadNode* node = newNode(x, y, w, h);
    // 1 - calcular cor media do bloco
    // 2 - calcular nivel de detalhe
    if(c >= 100){ // 3 - se nivel de detalhe é menor do que o valor desejado
        node->status = PARCIAL; // 3.1 - se sim, cria e devolve o nodo total, com a cor media do bloco
        node->color[0] = 0;
        node->color[1] = 0;
        node->color[2] = 255;
        return node;
    }
    node->status = PARCIAL;// 3.2 - caso negativo, cria nodo parcial e preenche os 4 filhos dele com chamadas recursivas
    node->color[0] = 0;
    node->color[1] = 0;
    node->color[2] = 255;
    node->NE = drawRecursiveNode(x, y, h/2, w/2);
    node->NW = drawRecursiveNode(w/2, y, h/2, w/2);
    node->SE = drawRecursiveNode(x, h/2, h/2, w/2);
    node->SW = drawRecursiveNode(w/2, h/2, h/2, w/2);
    return node;
    
=======
void drawRecursiveNode(QuadNode* node,int height, int width, int c)
{
    drawRecursiveNodeTopLeft(node, height, width, c);
    drawRecursiveNodeTopRight(node, height, width, c);
    drawRecursiveNodeBottomRight(node, height, width, c);
    drawRecursiveNodeBottomLeft(node, height, width, c);
}

void drawRecursiveNodeTopLeft(QuadNode* node,int height, int width, int c)
{
    if(c > 31) {
        return;
    }
    QuadNode* nw;

    nw = newNode(0, 0,width/c,height/c);
    nw->status = PARCIAL;
    nw->color[0] = 0;
    nw->color[1] = 0;
    nw->color[2] = 255;

    node->NW = nw;

    c*=2;
    drawRecursiveNodeTopLeft(nw, height, width, c);
}

void drawRecursiveNodeTopRight(QuadNode* node,int height, int width, int c)
{
    if(c > 11) {
        return;
    }
    QuadNode* nw;

    nw = newNode(getXRight(width, c), 0,width/pow(2,c),height/pow(2,c));
    nw->status = PARCIAL;
    nw->color[0] = 0;
    nw->color[1] = 0;
    nw->color[2] = 255;

    node->NE = nw;

    c++;
    drawRecursiveNodeTopRight(nw, height, width, c);
}

void drawRecursiveNodeBottomRight(QuadNode* node,int height, int width, int c)
{
    if(c > 11) {
        return;
    }
    QuadNode* nw;

    nw = newNode(getXRight(width, c), getXRight(width, c),width/pow(2,c),height/pow(2,c));
    nw->status = PARCIAL;
    nw->color[0] = 0;
    nw->color[1] = 0;
    nw->color[2] = 255;

    node->SE = nw;

    c++;
    drawRecursiveNodeBottomRight(nw, height, width, c);
}

void drawRecursiveNodeBottomLeft(QuadNode* node,int height, int width, int c)
{
    if(c > 11) {
        return;
    }
    QuadNode* nw;

    nw = newNode(0, getXRight(width, c),width/pow(2,c),height/pow(2,c));
    nw->status = PARCIAL;
    nw->color[0] = 0;
    nw->color[1] = 0;
    nw->color[2] = 255;

    node->SW = nw;

    c++;
    drawRecursiveNodeBottomLeft(nw, height, width, c);
}

int getXRight(int begin, int depth)
{
    int total = 0;
    for(int i =1; i <= depth; i++) {
        total += begin/pow(2, i);
    }
    return total;
>>>>>>> 53611d6e247b6dbe8e9f944e080003b384db5e6c
}