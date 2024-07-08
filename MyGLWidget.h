#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:

    enum EMShape{
        eNone = 0,
        eRect,
        eTextureRect
    };

    enum EMPolygonMode
    {
        ePoint = 0,
        eLine,
        eFill
    };

    explicit MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget();

    void loadImage(const QImage &img);
    void loadImage(char const *filename);
    void saveImage(const QString &filename);
    void drawShape(const EMShape& shape);
    void setPolygonMode(const EMPolygonMode& shape);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    //着色器
    QOpenGLShaderProgram shaderProgram;

    GLuint vao;
    GLuint vertexBuffer_vbo1;
    GLuint vertexBuffer_vbo1_ebo;
    GLuint vertexBuffer_vbo2;
    GLuint vertexBuffer_vbo2_ebo;
    GLuint texture1;
    GLuint texture2;

    GLint posAttr;
    GLint texCoordAttr;
    GLint colorAttr;
    GLint textureSampler;
    GLint textureSamplerText;

    EMShape m_shape_;

    QSize old_image_wh1_;

    QString gfy = "go fuck yourself";
};



#endif // MYGLWIDGET_H
