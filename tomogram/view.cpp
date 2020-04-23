#pragma comment (lib, "opengl32.lib")
#include "view.h"





view::view(QWidget*parent):QOpenGLWidget(parent),QOpenGLFunctions()
{
	layer = 0;
	setFocus();
}



void view::initializeGL()
{
	max = data.getMax();
	min = data.getMin();
	
	initializeOpenGLFunctions();
	glClearColor(1,1,1,1);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);//режим матрицы
	glLoadIdentity();//загрузка единичной
	

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}



void view::paintGL()
{
	
	qDebug() << "repaint" << visualization_state;
	// очистка экрана
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (visualization_state)
	{
	case VISUALIZATION_QUADS:
		VisualizationQuads();
		break;
		
	case VISUALIZATION_QUADSTRIP:
		VisualizationQuadStrip();
		break;
	case VISUALIZATION_TEXTURE:
		VizualizationTexture();
		break;
	}
}



void view::resizeGL(int nWidth, int nHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getHeight() - 1, -1.0f, 1.0f);//ортогонально преоьразование
	glViewport(0, 0, nWidth, nHeight);// окно обзора
	//update();
}


int view:: clamp(int v, int max, int min) {
	if (v > max) { return max; }
	if (v < min) { return min; }
	else return v;
}

void view::loaddata(QString fileName) {
	data.readFile(fileName);
	resize(clamp(data.getWidth(), 1000, 250), clamp(data.getHeight(), 1000, 250));
	update();
}

void view::Down() {
	if ((layer - 1) >= 0) { layer--; }
	update();
}

void view::Down10() {
	if ((layer - 10) >= 0) { layer -= 10; }
	update();
}

void view::Up() {
	if ((layer + 1) < data.getDepth()) { layer++; }
	update();
}

void view::Up10() {
	if ((layer + 10) < data.getDepth()) { layer += 10; }
	update();
}

void view::functionminmax(int _min, int _max)
{
	if (_min == _max) { max++; }
	min = _min;
	max = _max;
}

float view::TransferFunction(short value) {
	float c = (value - min) * 1.f / (max - min);
	return c;
}

void view::VisualizationQuads() {
	float c;
	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();
	glBegin(GL_QUADS);
	for (int y = 0; y < (h - 1); y++) {
		for (int x = 0; x < (w - 1); x++) {
			c = TransferFunction(data[layer * w * h + y * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, y);

			c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, y + 1);

			c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
			glColor3f(c, c, c);
			glVertex2i(x + 1, y + 1);

			c = TransferFunction(data[layer * w * h + y * w + x + 1]);
			glColor3f(c, c, c);
			glVertex2i(x + 1, y);
		}
	}
	glEnd();
}

void view::VisualizationQuadStrip() {
	float c;
	int w = data.getWidth();
	int h = data.getHeight();
	for (int y = 0; y < h - 1; y++) {
		glBegin(GL_QUAD_STRIP);

		c = TransferFunction(data[layer * w * h + y * w]);
		glColor3f(c, c, c);
		glVertex2i(0, y);

		c = TransferFunction(data[layer * w * h + (y + 1) * w]);
		glColor3f(c, c, c);
		glVertex2i(0, y + 1);

		for (int x = 0; x < w - 1; x++) {
			c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
			glColor3f(c, c, c);
			glVertex2i(x + 1, y + 1);

			c = TransferFunction(data[layer * w * h + y * w + x + 1]);
			glColor3f(c, c, c);
			glVertex2i(x + 1, y);
		}
		glEnd();
	}
}


void view::GenTextureImage() {
	int w = data.getWidth();
	int h = data.getHeight();
	textureImage = QImage(w, h, QImage::Format_RGB32);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int c = clamp((int)(255 * TransferFunction(data[layer * w * h + w * y + x])), 255, 0);
			QColor qc(c, c, c);
			textureImage.setPixelColor(x, y, qc);
		}
	}
	texture = new QOpenGLTexture(textureImage);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
}

void view::VizualizationTexture() {
	glEnable(GL_TEXTURE_2D);
	texture->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(0, 1);
	glVertex2i(0, data.getHeight());
	glTexCoord2f(1, 1);
	glVertex2d(data.getWidth(), data.getHeight());
	glTexCoord2f(1, 0);
	glVertex2i(data.getWidth(), 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void view::changeLayer() {
	if (visualization_state == VISUALIZATION_TEXTURE) {

		GenTextureImage();
	}
}

void view::keyPressEvent(QKeyEvent* event) {

	if ((event->nativeVirtualKey() == Qt::Key_U) || (event->key() == Qt::Key_Up)) {
		Up();
		changeLayer();
	}
	else if ((event->nativeVirtualKey() == Qt::Key_D) || (event->key() == Qt::Key_Down)) {
		Down();
		changeLayer();
	}
	else if ((event->nativeVirtualKey() == Qt::Key_N) || (event->key() == Qt::Key_Right)) {
		visualization_state = (visualization_state + 1) % 3;
		if (visualization_state == VISUALIZATION_TEXTURE) {
			glEnable(GL_TEXTURE_2D);
			GenTextureImage();
		}
		else {
			glDisable(GL_TEXTURE_2D);
		}
	}
	else if ((event->nativeVirtualKey() == Qt::Key_B) || (event->key() == Qt::Key_Left)) {
		if (visualization_state == 0) {
			visualization_state = 2;
		}
		else { visualization_state = abs((visualization_state - 1) % 3); }
		if (visualization_state == VISUALIZATION_TEXTURE) {
			glEnable(GL_TEXTURE_2D);
			GenTextureImage();
		}
		else {
			glDisable(GL_TEXTURE_2D);
		}
	}
	update();
}
