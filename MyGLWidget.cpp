#include "MyGLWidget.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MY_INDICES_SIZE 4

//!GLfloat vertices[] = {
//!    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
//!    1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
//!    -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//!    1.0f, 1.0f, 0.0f, 1.0f, 0.0f
//!};
//!//

//共八个点都是由：x、y、z、u、v
GLfloat vertices[] = {
    -0.9f, -0.6f, -1.0f, 0.0f, 0.0f,
    0.4f, -0.6f, -1.0f, 1.0f, 0.0f,
    -0.9f, 0.9f, -1.0f, 0.0f, 1.0f,
    0.4f, 0.9f, -1.0f, 1.0f, 1.0f,

    -0.4f, 0.6f, 0.5f, -2.0f, 3.0f,
    0.9f, 0.6f, 0.5f, 3.0f, 3.0f,
    -0.4f, -0.9f, 0.5f, -2.0f, -2.0f,
    0.9f, -0.9f, 0.5f, 3.0f, -2.0f,

//    -0.4f, 0.6f, 0.5f, 2.2f, 2.5f,
//    0.9f, 0.6f, 0.5f, 2.6f, 2.5f,
//    -0.4f, -0.9f, 0.5f, 2.2f, 2.0f,
//    0.9f, -0.9f, 0.5f, 2.6f, 2.0f,
};

//vertices索引
GLint indices[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 6, 2, 4,
};

//共四个点都是由：x、y、z、u、v
GLfloat vertices2[] = {
    -0.8f, -0.7f, -1.0, 0.0f, 1.0f, 0.0f,
    0.8f, -0.7f, -1.0f, 1.0f, 1.0f, 0.0f,
    -0.8f, 0.7f, -0.0f, 0.0f, 0.0f, 0.1f,
    0.8f, 0.7f, -0.0f, 1.0f, 0.0f, 0.0f,
};

//vertices2索引
GLint indices2[] = {
    0, 1, 2,
    2, 3, 1,
};

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_shape_(MyGLWidget::eTextureRect)
{

}

MyGLWidget::~MyGLWidget()
{
    makeCurrent();
    glDeleteBuffers(1, &vertexBuffer_vbo1);
    doneCurrent();
}

void MyGLWidget::loadImage(const QImage &img)
{
    makeCurrent();
    shaderProgram.bind();

    glBindTexture(GL_TEXTURE_2D, texture1);

    int width1 = img.width(), height1 = img.height(), nrChannels1;
//    unsigned char *data1 = stbi_load(filename, &width1, &height1, &nrChannels1, 0);

    if(!img.isNull())
    {
        if(old_image_wh1_ != QSize(width1, height1))
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width1, height1, GL_RGB, GL_UNSIGNED_BYTE, img.bits());
    }
    else
    {
        qDebug() << "Failed to load image";
    }
    shaderProgram.release();

    //记录此刻纹理分辨率
    old_image_wh1_ = {width1, height1};

    update();
    doneCurrent();
}

void MyGLWidget::loadImage(char const *filename)
{
    makeCurrent();
    shaderProgram.bind();

    glBindTexture(GL_TEXTURE_2D, texture1);

    int width1, height1, nrChannels1;
    unsigned char *data1 = stbi_load(filename, &width1, &height1, &nrChannels1, 0);
    if(data1)
    {
        if(old_image_wh1_ != QSize(width1, height1))
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width1, height1, GL_RGB, GL_UNSIGNED_BYTE, data1);
        stbi_image_free(data1);
    }
    else
    {
        qDebug() << "Failed to load image";
    }
    shaderProgram.release();

    //记录此刻纹理分辨率
    old_image_wh1_ = {width1, height1};

    update();
    doneCurrent();
}

void MyGLWidget::saveImage(const QString &filename)
{
    makeCurrent();
    shaderProgram.bind();

    // 绑定你要保存的纹理
    glBindTexture(GL_TEXTURE_2D, texture1);

    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    qDebug() << width << height;

    unsigned char* data = new unsigned char[width * height * 3]; // 3 channels for RGB
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    QImage(data, width, height, 0, QImage::Format_RGB888).mirrored().save(filename);

    // 保存为PNG文件
//    stbi_flip_vertically_on_write(1); // 如果需要翻转图像
//    stbi_write_jpg(filename, width, height, 3, data, 0);

    delete[] data; // 释放内存

    shaderProgram.release();
    doneCurrent();
}

void MyGLWidget::drawShape(const EMShape &shape)
{
    m_shape_ = shape;
}

void MyGLWidget::setPolygonMode(const EMPolygonMode &shape)
{
    makeCurrent();
    switch (shape) {
    case EMPolygonMode::eFill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case EMPolygonMode::eLine:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case EMPolygonMode::ePoint:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    default:
        break;
    }
    update();
    doneCurrent();
}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    GLfloat red = 0.0f;
    GLfloat green = 0.5f;
    GLfloat blue = 0.5f;
    GLfloat alpha = 1.0f;
    glClearColor(red, green, blue, alpha);

    //    glEnable(GL_DEPTH_TEST);

    QFile vertexShaderFile(":/Resource/vertexShader.vert");
    QFile fragmentShaderFile(":/Resource/fragmentShader.frag");

    if (vertexShaderFile.open(QFile::ReadOnly | QFile::Text) &&
        fragmentShaderFile.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream vertexShaderStream(&vertexShaderFile);
        QTextStream fragmentShaderStream(&fragmentShaderFile);

        //        const GLchar *string = vertexShaderStream.readAll().toLatin1().constData();
        QString vertexShaderSource = vertexShaderStream.readAll();
        QString fragmentShaderSource = fragmentShaderStream.readAll();

        // 添加并链接顶点着色器和片段着色器
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        qDebug() << "Shader ink:" << shaderProgram.link();
    }
    //顶点坐标(layout(location = 1) in vec3 inPosition;)
    posAttr = shaderProgram.attributeLocation("inPosition");
    //纹理坐标(layout(location = 2) in vec2 inTexCoord;)
    texCoordAttr = shaderProgram.attributeLocation("inTexCoord");
    //颜色值(layout(location = 3) in vec2 inColor;)
    colorAttr = shaderProgram.attributeLocation("inColor");

    qDebug() << posAttr << texCoordAttr << colorAttr;

    //创建一个vao并激活
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 创建两个VBO用于存储不同的坐标数据
    glGenBuffers(1, &vertexBuffer_vbo1);
    glGenBuffers(1, &vertexBuffer_vbo2);

    //创建两个EBO
    glGenBuffers(1, &vertexBuffer_vbo1_ebo);
    glGenBuffers(1, &vertexBuffer_vbo2_ebo);

    //绑定第一个VBO并将数据复制到缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //绑定第二个VBO并将数据复制到缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    //绑定EBO并将所引数据赋值到缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_vbo1_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //绑定EBO并将所引数据赋值到缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_vbo2_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    // 解绑VBO和VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //创建纹理
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   //缩小使用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //放大使用线性过滤
    const GLfloat color_params[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color_params);//图片边缘颜色

    char const *filename1 = "C:\\Users\\SONG\\Desktop\\素材\\张天爱.jpg";
    int width1, height1, nrChannels1;
    //OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data1 = stbi_load(filename1, &width1, &height1, &nrChannels1, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);

    //记录此刻纹理分辨率
    old_image_wh1_ = {width1, height1};

    //创建纹理
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char const *filename2 = "C:\\Users\\SONG\\Desktop\\素材\\Swamp Insect.png";
    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load(filename2, &width2, &height2, &nrChannels2, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    textureSampler = glGetUniformLocation(shaderProgram.programId(), "textureSampler");
    textureSamplerText = glGetUniformLocation(shaderProgram.programId(), "textureSamplerText");
    qDebug() << textureSampler << textureSamplerText;

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data1);
    stbi_image_free(data2);
}

void MyGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用着色器程序
    shaderProgram.bind();

    // 激活并绑定纹理单元
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 将纹理单元索引传递给着色器
    glUniform1i(textureSampler, 0);  // 0 对应于 GL_TEXTURE0
    glUniform1i(textureSamplerText, 1);  // 1 对应于 GL_TEXTURE1

    //绑定VAO数据
    glBindVertexArray(vao);

    //画
    switch (m_shape_) {
    case EMShape::eTextureRect:
    {
        // 绘制第一个VBO的数据
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_vbo1);
        shaderProgram.setUniformValue("isTexture", true);

        // 配置第一个VBO的顶点属性指针
        glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(posAttr);

        // 配置第一个VBO的纹理属性指针
        glVertexAttribPointer(texCoordAttr, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoordAttr);

        //        glDrawArrays(GL_QUAD_STRIP, 0, 8);

        //利用ebo保存的索引画
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_vbo1_ebo);
        glDrawElements(GL_QUAD_STRIP, MY_INDICES_SIZE, GL_UNSIGNED_INT, nullptr);

        // 解绑VBO和VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    break;
    case EMShape::eRect:
    {
        shaderProgram.setUniformValue("isTexture", false);

        //绑定到第二个VBO数据
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_vbo2);

        // 配置第二个VBO的顶点属性指针
        glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(posAttr);

        // 配置第二个VBO的颜色属性指针
        glVertexAttribPointer(colorAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(colorAttr);

        //直接使用vbo2的数据画
        //        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //利用ebo保存的索引画
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_vbo2_ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // 解绑VBO和VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    break;
    default:
        break;
    }

    shaderProgram.release();
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

