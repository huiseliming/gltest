#include <Windows.h>
#include <gl/glew.h>

#include <opencv2/opencv.hpp>
#include "common.h"
#include <memory>
#define width 1920
#define height 1080

// OpenGL Variables
GLuint textureId;              // ID of the texture to contain Kinect RGB Data
GLubyte data[width * height * 4];  // BGRA array containing the texture data

void drawKinectData() {
    std::memset(data,0xff, width * height * 4);
    glBindTexture(GL_TEXTURE_2D, textureId);
    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)data);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);  // 此处需要禁止深度测试
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(width, 0, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(width, height, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, height, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(width/2, 0, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(width/2, height/2, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, height/2, 0.0f);
    glEnd();
    glDisable(GL_BLEND);
}
cv::Mat readbuffer()
{
    int w, h;
    w = width;
    h = height;
    w = (w / 8) * 8;

    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned char* pixels = new unsigned char[3 * w * h];
    glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    return cv::Mat(h, w, CV_8UC3, pixels);

}

void draw() {

    drawKinectData();
    glutSwapBuffers();
    cv::Mat mat = readbuffer();
    cv::flip(mat, mat,0);
    cv::imwrite("C:\\Users\\DaiMingze\\Desktop\\xxcx.png",mat);
}

void execute() {
    glutMainLoop();
}

bool init(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("Kinect SDK Tutorial");
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    return true;
}

void BindTexture(cv::Mat mat, GLuint& texId)
{
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mat.cols, mat.rows,
            0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)mat.data);
    glBindTexture(GL_TEXTURE_2D, 0);

}

int main(int argc, char* argv[]) {
    cv::Mat icon = cv::imread("C:\\Users\\DaiMingze\\Desktop\\Icon128.png",  -1);
    if (!init(argc, argv)) return 1;






    /* ...OpenGL texture and camera initialization... */
        // Initialize textures
    BindTexture(icon, textureId);
    //glGenTextures(1, &textureId);
    //glBindTexture(GL_TEXTURE_2D, textureId);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
    //    0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)data);
    //glBindTexture(GL_TEXTURE_2D, 0);

    // OpenGL setup
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);

    // Camera setup
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Main loop
    execute();
    return 0;
}






