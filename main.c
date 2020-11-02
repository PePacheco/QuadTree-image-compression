#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// para usar strings

#ifdef _WIN32
#include <windows.h>    /* includes only in MSWindows not in UNIX */
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>     /* OpenGL functions */
#include <GL/glu.h>    /* OpenGL utilitarian functions */
#include <GL/glut.h>
#endif

#include "include/SOIL.h"

#include "quadtree.h"

// Funções da interface gráfica e OpenGL

void init();
void draw();
int getDetailLevel(Img* pic, int width, int height);
void keyboard(unsigned char key, int x, int y);
int getMeanIntensity(Img* pic, int currentIndex, int step);
int resize(int width, int height);
int width,height;   // largura e altura da janela
GLuint tex; // mingw32-make -f Makefile.mk

QuadNode* raiz;
Img pic;

int detalhe = 20;     // nível de detalhe inicial

int main(int argc, char** argv)
{
    glutInit(&argc,argv);

    // Define do modo de operacao da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int chan;
    pic.img = (RGB *) SOIL_load_image(argv[1], &pic.width, &pic.height, &chan, SOIL_LOAD_AUTO); //, SOIL_FLAG_INVERT_Y); // | SOIL_FLAG_POWER_OF_TWO);
    if(!pic.img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }

    width = pic.width;
    height = pic.height;
    //getDetailLevel(&pic, width, height);

    // Especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(width, height);

    // Cria a janela passando como argumento o titulo da mesma
    glutCreateWindow("Quadtree");

    // Registra a funcao callback de redesenho da janela de visualizacao
    glutDisplayFunc(draw);

    // Registra a funcao callback para tratamento das teclas ASCII
    glutKeyboardFunc (keyboard);


    tex = SOIL_create_OGL_texture((unsigned char*)pic.img, pic.width, pic.height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    printf("Imagem: %d x %d (%d)\n", pic.width, pic.height, chan);

    raiz = NULL;

    // Define a janela de visualizacao 2D
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,width,height,0.0);
    glMatrixMode(GL_MODELVIEW);

    glutMainLoop();
    return 0;
}

void keyboard(unsigned char key, int x, int y)
{
    char changed=0;

    // ESC termina o programa
    if(key==27) {
        clearTree(raiz);
        free(pic.img);
        exit(1);
    }
    // 'w' escreve a raiz
    if(key == 'w') {
        writeTree(raiz);
    }
    // '=' aumenta o nível de detalhe
    if(key=='=') {
        detalhe++;
        changed=1;
    }
    // '-' reduz o nível de detalhe
    if(key=='-') {
        detalhe--;
        changed=1;
    }

    // 'b' liga/desliga o desenho das bordas de cada região
    if(key=='b') {
        toggleBorder();
    }

    // 'r' regenera e redesenha a árvore
    if(key == 'r' || changed) {
        if(raiz) // Se já tem uma árvore, limpa e refaz
            clearTree(raiz);

        printf("Detalhe: %d\n", detalhe);

        raiz = geraQuadtree(&pic, detalhe);
    }
    glutPostRedisplay();
}

/*
*
* Função chamada quando a janela precisa ser redesenhada
*
*/
void draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // fundo preto
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3ub(255, 255, 255);  // branco

    // Se a árvore ainda não foi criada, desenha apenas a imagem
    if(raiz == NULL) {
        glBindTexture(GL_TEXTURE_2D, tex);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        glTexCoord2f(0,0);
        glVertex2f(0,0);

        glTexCoord2f(1,0);
        glVertex2f(pic.width,0);

        glTexCoord2f(1,1);
        glVertex2f(pic.width, pic.height);

        glTexCoord2f(0,1);
        glVertex2f(0,pic.height);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    drawTree(raiz);

    glutSwapBuffers();
}

// int getDetailLevel(Img* pic, int height, int width)
// {
//     RGB* arrayRGB = pic->img;
//     int index = 0;
//     int c = 0;
//     for (int i = 0, j = 0; i <= pic->width && j < pic->height;)
//     {
//         if(c >= pic->width) {
//             c = 0;
//             continue;
//         }
//         if(i == 0){

//         }else if(j == 0){

//         }
//         if(i >= pic->width) {
//             i = 0;
//         }
//     }
// }

// int getMeanIntensity(Img* pic, int currentIndex, int step){
//     int total, n;
//     total = 0;
//     n = step * step;
//     for(int j = 0; j < step; j++){
//         for(int i = 0; i < step; i++)
//         {
//             total += getIntensity(pic->img[currentIndex + i]);
//         }
//         currentIndex += pic->width;
//     }
//     return total/n;
// }