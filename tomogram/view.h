#pragma once
#pragma comment (lib, "opengl32.lib")
#include "Data.h"
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <qopengl.h>
#include <qopenglextrafunctions.h>
#include <qopenglwidget.h>
#include <qimage.h>
#include <qopengltexture.h>
#include <qevent.h>
#include <gl/GL.h>		 
#include <gl/GLU.h>

#define _QT_TEXTURE_

class view : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
private:
	short min;
	short max;
	Data data;
	int layer;
	int visualization_state = 0;
	static const int VISUALIZATION_QUADS = 0;
	static const int VISUALIZATION_QUADSTRIP = 1;
	static const int VISUALIZATION_TEXTURE = 2;
	void VisualizationQuads();
	void GenTextureImage();
	void VisualizationQuadStrip() ;
	void VizualizationTexture() ;
	void functionminmax(int _min, int _max);
	int clamp(int v, int max, int min);
	void Down();
	void Up();
	void Up10();
	void Down10();
	void changeLayer();
	float TransferFunction(short value);
	QImage textureImage;
	QOpenGLTexture* texture;

	//void Load2DTexture();
	GLuint VBOtexture;



protected:
	
	void initializeGL()override;
	void resizeGL(int nWidth, int nHeight)override;
	void paintGL()override;
public:
	void loaddata(QString Filename);
	view(QWidget*parent);
	~view() {};
public slots:

	void keyPressEvent(QKeyEvent* event)override;

};
