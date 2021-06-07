#include <iostream>
#include <fstream>
#include <time.h>
#include <glut.h>

using namespace std;

//........yyy...........
//........yyy...........
//........yyy...........
//.....gggooobbbrrr.....
//.....gggooobbbrrr.....
//.....gggooobbbrrr.....
//........www...........
//........www...........
//........www...........

class Small_Cube {
private:
	unsigned char _color[3];

public:
	// шесть граней куба - шесть цветов
	unsigned int color[6];
	// размер ребра
	double size;

	void rotate_x() {
		swap(color[5], color[3]);
		swap(color[3], color[4]);
		swap(color[4], color[2]);
	}

	void rotate_y() {
		swap(color[2], color[1]);
		swap(color[1], color[3]);
		swap(color[3], color[0]);
	}

	void rotate_z() {
		swap(color[0], color[4]);
		swap(color[4], color[1]);
		swap(color[1], color[5]);
	}

	void setColor(int i, int color) {
		this->color[i] = color;
	}

	unsigned char* at(int i) {
		_color[0] = color[i] >> 16;
		_color[1] = color[i] >> 8;
		_color[2] = color[i];
		return _color;
	}

	// отрисовка куба:
	void draw() {
		glPushMatrix();
		glBegin(GL_QUADS);

		// верх
		glColor3ubv(at(0));
		glNormal3f(0, 0, 1);
		glVertex3f(size, size, size);
		glVertex3f(0, size, size);
		glVertex3f(0, 0, size);
		glVertex3f(size, 0, size);

		// низ
		glColor3ubv(at(1));
		glNormal3f(0, 0, -1);
		glVertex3f(size, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, size, 0);
		glVertex3f(size, size, 0);

		// спереди
		glColor3ubv(at(2));
		glNormal3f(0, -1, 0);
		glVertex3f(size, 0, size);
		glVertex3f(0, 0, size);
		glVertex3f(0, 0, 0);
		glVertex3f(size, 0, 0);

		// сзади
		glColor3ubv(at(3));
		glNormal3f(0, 1, 0);
		glVertex3f(size, size, 0);
		glVertex3f(0, size, 0);
		glVertex3f(0, size, size);
		glVertex3f(size, size, size);

		// слева
		glColor3ubv(at(4));
		glNormal3f(-1, 0, 0);
		glVertex3f(0, size, size);
		glVertex3f(0, size, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, size);

		// справа
		glColor3ubv(at(5));
		glNormal3f(1, 0, 0);
		glVertex3f(size, size, 0);
		glVertex3f(size, size, size);
		glVertex3f(size, 0, size);
		glVertex3f(size, 0, 0);

		glEnd();
		glPopMatrix();
	}

	// отрисовка куба со смещением (x, y, z)
	void draw(double x, double y, double z) {
		glPushMatrix();
		glTranslated(x, y, z);
		draw();
		glPopMatrix();
	}
};

class Cube {
private:
	char front[3][3], back[3][3], up[3][3], down[3][3], left[3][3], right[3][3];

	bool ok[3][3][3];
	Small_Cube tmp[3][3];
	// 27 частей
	Small_Cube a[3][3][3];
	//угол поворота каждой грани
	int rotate[6];
	// размер кубика
	double size;
	// цвета граней
	unsigned int color[6];
	// скорость поворота
	int speed_torn = 3;

public:
	Cube() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				front[i][j] = 'o';
				back[i][j] = 'r';
				up[i][j] = 'y';
				down[i][j] = 'w';
				left[i][j] = 'g';
				right[i][j] = 'b';
			}
	}

	bool color_ch = true;
	void show() {
		cout << endl;
		for (int i = 0; i < 3; i++) {
			cout << "     ";
			for (int j = 0; j < 3; j++)
				cout << up[i][j];
			cout << "        " << endl;
		}
		for (int i = 0; i < 3; i++) {
			cout << "  ";
			for (int j = 0; j < 3; j++)
				cout << left[i][j];
			for (int j = 0; j < 3; j++)
				cout << front[i][j];
			for (int j = 0; j < 3; j++)
				cout << right[i][j];
			for (int j = 0; j < 3; j++)
				cout << back[i][j];
			cout << "  " << endl;
		}
		for (int i = 0; i < 3; i++) {
			cout << "     ";
			for (int j = 0; j < 3; j++)
				cout << down[i][j];
			cout << "        " << endl;
		}
	}
	void read() {
		ifstream in("Cube.in");
		int o = 0, r = 0, g = 0, b = 0, y = 0, w = 0;
		for (int k = 0; k < 6; k++) {
			char t[3][3];
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++) {
					in >> t[i][j];
					if (t[i][j] == 'o') o++;
					if (t[i][j] == 'r') r++;
					if (t[i][j] == 'g') g++;
					if (t[i][j] == 'w') w++;
					if (t[i][j] == 'b') b++;
					if (t[i][j] == 'y') y++;
				}

			if (t[1][1] == 'o') {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						front[i][j] = t[i][j];
					}
				}
			}
			if (t[1][1] == 'w') {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						down[i][j] = t[i][j];
					}
				}
			}
			if (t[1][1] == 'y') {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						up[i][j] = t[i][j];
					}
				}
			}
			if (t[1][1] == 'g') {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						left[i][j] = t[i][j];
					}
				}
			}
			if (t[1][1] == 'b') {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						right[i][j] = t[i][j];
					}
				}
			}
			if (t[1][1] == 'r') {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						back[i][j] = t[i][j];
					}
				}
			}
		}
		if (o != 9 || r != 9 || y != 9 || g != 9 || b != 9 || w != 9) color_ch = false;
		show_read();
		show();
		in.close();
	}
	void show_read() {
		this->size = size;
		current = -1;

		int i, j, k;
		for (i = 0; i < 6; i++) {
			this->color[i] = color[i];
		}

		// спереди
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (front[2 - j][i] == 'y') a[i][j][2].setColor(0, color[3]);
				if (front[2 - j][i] == 'w') a[i][j][2].setColor(0, color[2]);
				if (front[2 - j][i] == 'r') a[i][j][2].setColor(0, color[1]);
				if (front[2 - j][i] == 'o') a[i][j][2].setColor(0, color[0]);
				if (front[2 - j][i] == 'g') a[i][j][2].setColor(0, color[4]);
				if (front[2 - j][i] == 'b') a[i][j][2].setColor(0, color[5]);
			}
		}
		// сзади
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (back[2 - j][2 - i] == 'y') a[i][j][0].setColor(1, color[3]);
				if (back[2 - j][2 - i] == 'w') a[i][j][0].setColor(1, color[2]);
				if (back[2 - j][2 - i] == 'o') a[i][j][0].setColor(1, color[0]);
				if (back[2 - j][2 - i] == 'r') a[i][j][0].setColor(1, color[1]);
				if (back[2 - j][2 - i] == 'g') a[i][j][0].setColor(1, color[4]);
				if (back[2 - j][2 - i] == 'b') a[i][j][0].setColor(1, color[5]);
			}
		}
		// снизу
		for (k = 0; k < 3; k++) {
			for (j = 0; j < 3; j++) {
				if (down[2 - k][j] == 'y') a[j][0][k].setColor(2, color[3]);
				if (down[2 - k][j] == 'w') a[j][0][k].setColor(2, color[2]);
				if (down[2 - k][j] == 'o') a[j][0][k].setColor(2, color[0]);
				if (down[2 - k][j] == 'r') a[j][0][k].setColor(2, color[1]);
				if (down[2 - k][j] == 'g') a[j][0][k].setColor(2, color[4]);
				if (down[2 - k][j] == 'b') a[j][0][k].setColor(2, color[5]);
			}
		}
		// сверху
		for (k = 0; k < 3; k++) {
			for (j = 0; j < 3; j++) {
				if (up[k][j] == 'y') a[j][2][k].setColor(3, color[3]);
				if (up[k][j] == 'w') a[j][2][k].setColor(3, color[2]);
				if (up[k][j] == 'o') a[j][2][k].setColor(3, color[0]);
				if (up[k][j] == 'r') a[j][2][k].setColor(3, color[1]);
				if (up[k][j] == 'g') a[j][2][k].setColor(3, color[4]);
				if (up[k][j] == 'b') a[j][2][k].setColor(3, color[5]);
			}
		}
		// слева
		for (i = 0; i < 3; i++) {
			for (k = 0; k < 3; k++) {
				if (left[2 - k][i] == 'y') a[0][k][i].setColor(4, color[3]);
				if (left[2 - k][i] == 'w') a[0][k][i].setColor(4, color[2]);
				if (left[2 - k][i] == 'o') a[0][k][i].setColor(4, color[0]);
				if (left[2 - k][i] == 'r') a[0][k][i].setColor(4, color[1]);
				if (left[2 - k][i] == 'g') a[0][k][i].setColor(4, color[4]);
				if (left[2 - k][i] == 'b') a[0][k][i].setColor(4, color[5]);
			}
		}
		// справа
		for (i = 0; i < 3; i++) {
			for (k = 0; k < 3; k++) {
				if (right[2 - k][2 - i] == 'y') a[2][k][i].setColor(5, color[3]);
				if (right[2 - k][2 - i] == 'w') a[2][k][i].setColor(5, color[2]);
				if (right[2 - k][2 - i] == 'o') a[2][k][i].setColor(5, color[0]);
				if (right[2 - k][2 - i] == 'r') a[2][k][i].setColor(5, color[1]);
				if (right[2 - k][2 - i] == 'g') a[2][k][i].setColor(5, color[4]);
				if (right[2 - k][2 - i] == 'b') a[2][k][i].setColor(5, color[5]);
			}
		}
		//размеры мелких деталей
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				for (k = 0; k < 3; k++) {
					a[i][j][k].size = (size / 3.0) * 0.95;
				}
			}
		}
	}
	void Random() {
		srand(time(0));
		int k = rand() % 50 + 15;
		int l;
		int rotation;

		for (int i = 0; i < k; i++) {
			l = rand() % 2;
			rotation = rand() % 5;

			if (rotation == 0) {
				for (int j = 0; j < l; j++) {
					L();
				}
			}
			else if (rotation == 1) {
				for (int j = 0; j < l; j++) {
					R();
				}
			}
			else if (rotation == 2) {
				for (int j = 0; j < l; j++) {
					F();
				}
			}
			else if (rotation == 3) {
				for (int j = 0; j < l; j++) {
					B();
				}
			}
			else if (rotation == 4) {
				for (int j = 0; j < l; j++) {
					U();
				}
			}
			else if (rotation == 5) {
				for (int j = 0; j < l; j++) {
					D();
				}
			}
		}

		cout << ' ' << k << " different turns for random cube" << endl;
		show();
	}

	bool check_cube() {
		bool t = true;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				if (front[i][j] != 'o') t = false;
				if (back[i][j] != 'r') t = false;
				if (up[i][j] != 'y') t = false;
				if (down[i][j] != 'w') t = false;
				if (left[i][j] != 'g') t = false;
				if (right[i][j] != 'b') t = false;
			}
		return t;
	}

	//R L U D B F - повороты по часовой стресле, с добавлением B - против часовой

	void R() {
		cout << "R";
		char t1 = front[0][2];
		char t2 = front[1][2];
		char t3 = front[2][2];
		front[0][2] = down[0][2];
		front[1][2] = down[1][2];
		front[2][2] = down[2][2];
		down[0][2] = back[2][0];
		down[1][2] = back[1][0];
		down[2][2] = back[0][0];
		back[2][0] = up[0][2];
		back[1][0] = up[1][2];
		back[0][0] = up[2][2];
		up[0][2] = t1;
		up[1][2] = t2;
		up[2][2] = t3;

		t1 = right[0][0];
		right[0][0] = right[2][0];
		right[2][0] = right[2][2];
		right[2][2] = right[0][2];
		right[0][2] = t1;

		t1 = right[0][1];
		right[0][1] = right[1][0];
		right[1][0] = right[2][1];
		right[2][1] = right[1][2];
		right[1][2] = t1;

		Rotate(5, speed_torn, -1);
	}
	void RB() {
		cout << "R'";
		char t1 = front[0][2];
		char t2 = front[1][2];
		char t3 = front[2][2];
		front[0][2] = up[0][2];
		front[1][2] = up[1][2];
		front[2][2] = up[2][2];
		up[0][2] = back[2][0];
		up[1][2] = back[1][0];
		up[2][2] = back[0][0];
		back[2][0] = down[0][2];
		back[1][0] = down[1][2];
		back[0][0] = down[2][2];
		down[0][2] = t1;
		down[1][2] = t2;
		down[2][2] = t3;

		t1 = right[0][0];
		right[0][0] = right[0][2];
		right[0][2] = right[2][2];
		right[2][2] = right[2][0];
		right[2][0] = t1;

		t1 = right[0][1];
		right[0][1] = right[1][2];
		right[1][2] = right[2][1];
		right[2][1] = right[1][0];
		right[1][0] = t1;

		Rotate(5, speed_torn, 1);
	}

	void L() {
		cout << "L";
		char t1 = front[0][0];
		char t2 = front[1][0];
		char t3 = front[2][0];
		front[0][0] = up[0][0];
		front[1][0] = up[1][0];
		front[2][0] = up[2][0];
		up[0][0] = back[2][2];
		up[1][0] = back[1][2];
		up[2][0] = back[0][2];
		back[2][2] = down[0][0];
		back[1][2] = down[1][0];
		back[0][2] = down[2][0];
		down[0][0] = t1;
		down[1][0] = t2;
		down[2][0] = t3;

		t1 = left[0][0];
		left[0][0] = left[2][0];
		left[2][0] = left[2][2];
		left[2][2] = left[0][2];
		left[0][2] = t1;

		t1 = left[0][1];
		left[0][1] = left[1][0];
		left[1][0] = left[2][1];
		left[2][1] = left[1][2];
		left[1][2] = t1;

		Rotate(4, speed_torn, 1);
	}
	void LB() {
		cout << "L'";
		char t1 = front[0][0];
		char t2 = front[1][0];
		char t3 = front[2][0];
		front[0][0] = down[0][0];
		front[1][0] = down[1][0];
		front[2][0] = down[2][0];
		down[0][0] = back[2][2];
		down[1][0] = back[1][2];
		down[2][0] = back[0][2];
		back[2][2] = up[0][0];
		back[1][2] = up[1][0];
		back[0][2] = up[2][0];
		up[0][0] = t1;
		up[1][0] = t2;
		up[2][0] = t3;

		t1 = left[0][0];
		left[0][0] = left[0][2];
		left[0][2] = left[2][2];
		left[2][2] = left[2][0];
		left[2][0] = t1;

		t1 = left[0][1];
		left[0][1] = left[1][2];
		left[1][2] = left[2][1];
		left[2][1] = left[1][0];
		left[1][0] = t1;

		Rotate(4, speed_torn, -1);
	}

	void U() {
		cout << "U";
		char t1 = front[0][0];
		char t2 = front[0][1];
		char t3 = front[0][2];
		front[0][0] = right[0][0];
		front[0][1] = right[0][1];
		front[0][2] = right[0][2];
		right[0][0] = back[0][0];
		right[0][1] = back[0][1];
		right[0][2] = back[0][2];
		back[0][0] = left[0][0];
		back[0][1] = left[0][1];
		back[0][2] = left[0][2];
		left[0][0] = t1;
		left[0][1] = t2;
		left[0][2] = t3;

		t1 = up[0][0];
		up[0][0] = up[2][0];
		up[2][0] = up[2][2];
		up[2][2] = up[0][2];
		up[0][2] = t1;

		t1 = up[0][1];
		up[0][1] = up[1][0];
		up[1][0] = up[2][1];
		up[2][1] = up[1][2];
		up[1][2] = t1;

		Rotate(3, speed_torn, -1);
	}
	void UB() {
		cout << "U'";
		char t1 = front[0][0];
		char t2 = front[0][1];
		char t3 = front[0][2];
		front[0][0] = left[0][0];
		front[0][1] = left[0][1];
		front[0][2] = left[0][2];
		left[0][0] = back[0][0];
		left[0][1] = back[0][1];
		left[0][2] = back[0][2];
		back[0][0] = right[0][0];
		back[0][1] = right[0][1];
		back[0][2] = right[0][2];
		right[0][0] = t1;
		right[0][1] = t2;
		right[0][2] = t3;

		t1 = up[0][0];
		up[0][0] = up[0][2];
		up[0][2] = up[2][2];
		up[2][2] = up[2][0];
		up[2][0] = t1;

		t1 = up[0][1];
		up[0][1] = up[1][2];
		up[1][2] = up[2][1];
		up[2][1] = up[1][0];
		up[1][0] = t1;

		Rotate(3, speed_torn, 1);
	}

	void D() {
		cout << "D";
		char t1 = front[2][0];
		char t2 = front[2][1];
		char t3 = front[2][2];
		front[2][0] = left[2][0];
		front[2][1] = left[2][1];
		front[2][2] = left[2][2];
		left[2][0] = back[2][0];
		left[2][1] = back[2][1];
		left[2][2] = back[2][2];
		back[2][0] = right[2][0];
		back[2][1] = right[2][1];
		back[2][2] = right[2][2];
		right[2][0] = t1;
		right[2][1] = t2;
		right[2][2] = t3;

		t1 = down[0][0];
		down[0][0] = down[2][0];
		down[2][0] = down[2][2];
		down[2][2] = down[0][2];
		down[0][2] = t1;

		t1 = down[0][1];
		down[0][1] = down[1][0];
		down[1][0] = down[2][1];
		down[2][1] = down[1][2];
		down[1][2] = t1;

		Rotate(2, speed_torn, 1);
	}
	void DB() {
		cout << "D'";
		char t1 = front[2][0];
		char t2 = front[2][1];
		char t3 = front[2][2];
		front[2][0] = right[2][0];
		front[2][1] = right[2][1];
		front[2][2] = right[2][2];
		right[2][0] = back[2][0];
		right[2][1] = back[2][1];
		right[2][2] = back[2][2];
		back[2][0] = left[2][0];
		back[2][1] = left[2][1];
		back[2][2] = left[2][2];
		left[2][0] = t1;
		left[2][1] = t2;
		left[2][2] = t3;

		t1 = down[0][0];
		down[0][0] = down[0][2];
		down[0][2] = down[2][2];
		down[2][2] = down[2][0];
		down[2][0] = t1;

		t1 = down[0][1];
		down[0][1] = down[1][2];
		down[1][2] = down[2][1];
		down[2][1] = down[1][0];
		down[1][0] = t1;

		Rotate(2, speed_torn, -1);
	}

	void F() {
		cout << "F";
		char t1 = up[2][0];
		char t2 = up[2][1];
		char t3 = up[2][2];
		up[2][0] = left[2][2];
		up[2][1] = left[1][2];
		up[2][2] = left[0][2];
		left[2][2] = down[0][2];
		left[1][2] = down[0][1];
		left[0][2] = down[0][0];
		down[0][2] = right[0][0];
		down[0][1] = right[1][0];
		down[0][0] = right[2][0];
		right[0][0] = t1;
		right[1][0] = t2;
		right[2][0] = t3;

		t1 = front[0][0];
		front[0][0] = front[2][0];
		front[2][0] = front[2][2];
		front[2][2] = front[0][2];
		front[0][2] = t1;

		t1 = front[0][1];
		front[0][1] = front[1][0];
		front[1][0] = front[2][1];
		front[2][1] = front[1][2];
		front[1][2] = t1;

		Rotate(1, speed_torn, -1);
	}
	void FB() {
		cout << "F'";
		char t1 = up[2][0];
		char t2 = up[2][1];
		char t3 = up[2][2];
		up[2][0] = right[0][0];
		up[2][1] = right[1][0];
		up[2][2] = right[2][0];
		right[0][0] = down[0][2];
		right[1][0] = down[0][1];
		right[2][0] = down[0][0];
		down[0][2] = left[2][2];
		down[0][1] = left[1][2];
		down[0][0] = left[0][2];
		left[2][2] = t1;
		left[1][2] = t2;
		left[0][2] = t3;

		t1 = front[0][0];
		front[0][0] = front[0][2];
		front[0][2] = front[2][2];
		front[2][2] = front[2][0];
		front[2][0] = t1;

		t1 = front[0][1];
		front[0][1] = front[1][2];
		front[1][2] = front[2][1];
		front[2][1] = front[1][0];
		front[1][0] = t1;

		Rotate(1, speed_torn, 1);
	}

	void B() {
		cout << "B";
		char t1 = up[0][0];
		char t2 = up[0][1];
		char t3 = up[0][2];
		up[0][0] = right[0][2];
		up[0][1] = right[1][2];
		up[0][2] = right[2][2];
		right[0][2] = down[2][2];
		right[1][2] = down[2][1];
		right[2][2] = down[2][0];
		down[2][2] = left[2][0];
		down[2][1] = left[1][0];
		down[2][0] = left[0][0];
		left[2][0] = t1;
		left[1][0] = t2;
		left[0][0] = t3;

		t1 = back[0][0];
		back[0][0] = back[2][0];
		back[2][0] = back[2][2];
		back[2][2] = back[0][2];
		back[0][2] = t1;

		t1 = back[0][1];
		back[0][1] = back[1][0];
		back[1][0] = back[2][1];
		back[2][1] = back[1][2];
		back[1][2] = t1;

		Rotate(0, speed_torn, 1);
	}
	void BB() {
		cout << "B'";
		char t1 = up[0][0];
		char t2 = up[0][1];
		char t3 = up[0][2];
		up[0][0] = left[2][0];
		up[0][1] = left[1][0];
		up[0][2] = left[0][0];
		left[2][0] = down[2][2];
		left[1][0] = down[2][1];
		left[0][0] = down[2][0];
		down[2][2] = right[0][2];
		down[2][1] = right[1][2];
		down[2][0] = right[2][2];
		right[0][2] = t1;
		right[1][2] = t2;
		right[2][2] = t3;

		t1 = back[0][0];
		back[0][0] = back[0][2];
		back[0][2] = back[2][2];
		back[2][2] = back[2][0];
		back[2][0] = t1;

		t1 = back[0][1];
		back[0][1] = back[1][2];
		back[1][2] = back[2][1];
		back[2][1] = back[1][0];
		back[1][0] = t1;

		Rotate(0, speed_torn, -1);
	}

	bool white_cross_check() {
		return (up[0][1] == 'w' && up[1][0] == 'w' && up[1][2] == 'w' && up[2][1] == 'w');
	}
	bool right_white_cross_check() {
		return down[0][1] == 'w' && down[1][0] == 'w' && down[1][2] == 'w' && down[2][1] == 'w'
			&& front[2][1] == 'o' && left[2][1] == 'g' && right[2][1] == 'b' && back[2][1] == 'r';
	}
	bool middle_check() {
		bool t = true;
		for (int i = 0; i < 3; i++)
			if (front[1][i] != 'o' || left[1][i] != 'g' || right[1][i] != 'b' || back[1][i] != 'r') t = false;
		return t;
	}
	bool o_check() {
		if (up[0][1] == 'o' && back[0][1] != 'y') { U(); U(); return true; }
		if (up[1][0] == 'o' && left[0][1] != 'y') { UB(); return true; }
		if (up[1][2] == 'o' && right[0][1] != 'y') { U(); return true; }
		if (up[2][1] == 'o' && front[0][1] != 'y') { return true; }
		return false;
	}
	bool r_check() {
		if (up[0][1] == 'r' && back[0][1] != 'y') { return true; }
		if (up[1][0] == 'r' && left[0][1] != 'y') { U(); return true; }
		if (up[1][2] == 'r' && right[0][1] != 'y') { UB(); return true; }
		if (up[2][1] == 'r' && front[0][1] != 'y') { U(); U(); return true; }
		return false;
	}
	bool b_check() {
		if (up[0][1] == 'b' && back[0][1] != 'y') { U(); return true; }
		if (up[1][0] == 'b' && left[0][1] != 'y') { U(); U(); return true; }
		if (up[1][2] == 'b' && right[0][1] != 'y') { return true; }
		if (up[2][1] == 'b' && front[0][1] != 'y') { UB(); return true; }
		return false;
	}
	bool g_check() {
		if (up[0][1] == 'g' && back[0][1] != 'y') { UB(); return true; }
		if (up[1][0] == 'g' && left[0][1] != 'y') { return true; }
		if (up[1][2] == 'g' && right[0][1] != 'y') { U(); U(); return true; }
		if (up[2][1] == 'g' && front[0][1] != 'y') { UB(); return true; }
		return false;
	}
	bool bottom_check() {
		bool t = true;
		for (int i = 0; i < 3; i++)
			if (front[2][i] != 'o' || left[2][i] != 'g' || right[2][i] != 'b' || back[2][i] != 'r') t = false;
		return t;
	}
	bool yellow_cross_check() {
		return up[0][1] == 'y' && up[1][0] == 'y' && up[1][2] == 'y' && up[2][1] == 'y';
	}
	bool stick_check() {
		if (up[0][1] == 'y' && up[2][1] == 'y') { U(); return true; }
		if (up[1][0] == 'y' && up[1][2] == 'y') return true;
		return false;
	}
	void stick() {
		R(); U(); RB(); UB(); RB(); F(); R(); FB();
	}
	bool angle_check() {
		if (up[1][0] == 'y' && up[0][1] == 'y') return true;
		if (up[0][1] == 'y' && up[1][2] == 'y') { UB(); return true; }
		if (up[1][2] == 'y' && up[2][1] == 'y') { U(); U(); return true; }
		if (up[2][1] == 'y' && up[1][0] == 'y') { U(); return true; }
		return false;
	}
	void angle() {
		F();  R(); UB(); RB(); UB(); R(); U(); RB(); FB();
	}
	bool colibri_check() {
		if (up[2][0] == 'y' && up[2][2] != 'y' && up[0][0] != 'y' && up[0][2] != 'y' && front[0][2] == 'y') return true;
		if (up[2][0] != 'y' && up[2][2] == 'y' && up[0][0] != 'y' && up[0][2] != 'y' && right[0][2] == 'y') { U(); return true; }
		if (up[2][0] != 'y' && up[2][2] != 'y' && up[0][0] == 'y' && up[0][2] != 'y' && left[0][2] == 'y') { UB(); return true; }
		if (up[2][0] != 'y' && up[2][2] != 'y' && up[0][0] != 'y' && up[0][2] == 'y' && back[0][2] == 'y') { U(); U(); return true; }

		return false;
	}
	void colibri() {
		R(); U(); RB(); U(); R(); U(); U(); RB();
	}
	bool cambria_check() {
		if (up[2][0] != 'y' && up[2][2] == 'y' && up[0][0] != 'y' && up[0][2] != 'y' && front[0][0] == 'y') { return true; }
		if (up[2][0] != 'y' && up[2][2] != 'y' && up[0][0] != 'y' && up[0][2] == 'y' && right[0][0] == 'y') { U(); return true; }
		if (up[2][0] == 'y' && up[2][2] != 'y' && up[0][0] != 'y' && up[0][2] != 'y' && left[0][0] == 'y') { UB(); return true; }
		if (up[2][0] != 'y' && up[2][2] != 'y' && up[0][0] == 'y' && up[0][2] != 'y' && back[0][0] == 'y') { U(); U(); return true; }

		return false;
	}
	void cambria() {
		LB(); UB(); L(); UB(); LB(); UB(); UB(); L();
	}
	bool yellow_side_check() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (up[i][j] != 'y') return false;
		return true;
	}
	bool top_check() {
		return front[0][0] == front[0][1] && front[0][1] == front[0][2] &&
			left[0][0] == left[0][1] && left[0][1] == left[0][2] &&
			right[0][0] == right[0][1] && right[0][1] == right[0][2] &&
			back[0][0] == back[0][1] && back[0][1] == back[0][2];
	}
	bool pair_check() {
		if (front[0][0] == front[0][2]) { U(); return true; }
		if (left[0][0] == left[0][2]) return true;
		if (right[0][0] == right[0][2]) { U(); U(); return true; }
		if (back[0][0] == back[0][2]) { UB(); return back; }
		return false;
	}
	bool right_top_check() {
		if (front[0][0] == front[0][2] && left[0][0] == left[0][2] &&
			right[0][0] == right[0][2] && back[0][0] == back[0][2])
		{
			if (front[0][1] == front[0][0]) { U(); U(); return true; }
			if (right[0][0] == right[0][1]) { UB(); return true; }
			if (back[0][0] == back[0][1]) { return true; }
			if (left[0][0] == left[0][1]) { U(); return true; }
			return true;
		}
		return false;
	}

	void first() {
		if (!check_cube())
			while (!white_cross_check()) {

				if (front[1][0] == 'w')
				{
					while (up[1][0] == 'w')
						U();
					LB();
				}
				if (front[1][2] == 'w') {
					while (up[1][2] == 'w')
						U();
					R();
				}
				if (right[1][0] == 'w') {
					while (up[2][1] == 'w')
						U();
					FB();
				}
				if (right[1][2] == 'w') {
					while (up[0][1] == 'w')
						U();
					B();
				}
				if (back[1][0] == 'w') {
					while (up[1][2] == 'w')
						U();
					RB();
				}
				if (back[1][2] == 'w') {
					while (up[1][0] == 'w')
						U();
					L();
				}
				if (left[1][0] == 'w') {
					while (up[0][1] == 'w')
						U();
					BB();
				}
				if (left[1][2] == 'w') {
					while (up[2][1] == 'w')
						U();
					F();
				}
				if (down[0][1] == 'w') {
					while (up[2][1] == 'w')
						U();
					F();
					F();
				}
				if (down[1][0] == 'w') {
					while (up[1][0] == 'w')
						U();
					L();
					L();
				}
				if (down[1][2] == 'w') {
					while (up[1][2] == 'w')
						U();
					L();
					L();
				}
				if (down[2][1] == 'w') {
					while (up[0][1] == 'w')
						U();
					B();
					B();
				}

				if (front[0][1] == 'w') {
					F();
					while (up[1][2] == 'w')
						UB();
					R();
				}
				else U();
				if (front[2][1] == 'w') {
					while (up[2][1] == 'w')
						U();
					F();
					while (up[1][0] == 'w')
						U();
					LB();
				}
				else D();
			}

		show();

		while (!right_white_cross_check()) {

			if (up[0][1] == 'w') {
				if (back[0][1] == 'r') { B(); B(); }
				else
					if (back[0][1] == 'g') { UB(); L(); L(); }
					else
						if (back[0][1] == 'b') { U(); R(); R(); }
						else
							if (back[0][1] == 'o') { U(); U(); F(); F(); }
			}
			if (up[1][0] == 'w') {
				if (left[0][1] == 'r') { U(); B(); B(); }
				else
					if (left[0][1] == 'g') { L(); L(); }
					else
						if (left[0][1] == 'b') { U(); U(); R(); R(); }
						else
							if (left[0][1] == 'o') { UB(); F(); F(); }
			}
			if (up[1][2] == 'w') {
				if (right[0][1] == 'r') { UB(); B(); B(); }
				else
					if (right[0][1] == 'g') { U(); U(); L(); L(); }
					else
						if (right[0][1] == 'b') { R(); R(); }
						else
							if (right[0][1] == 'o') { U(); F(); F(); }
			}
			if (up[2][1] == 'w') {
				if (front[0][1] == 'r') { U(); U(); B(); B(); }
				else
					if (front[0][1] == 'g') { U(); L(); L(); }
					else
						if (front[0][1] == 'b') { UB(); R(); R(); }
						else
							if (front[0][1] == 'o') { F(); F(); }
			}

		}

		show();

		while (!middle_check()) {
			int c = 0;
			if (o_check()) {
				if (front[0][1] == 'g')
				{
					LB(); U(); L(); c++;
				}
				else {
					R();
					UB();
					RB();
					c++;
				}
			}
			if (r_check()) {
				if (back[0][1] == 'b') {
					RB(); U(); R(); c++;
				}
				else {
					L(); UB(); LB(); c++;
				}
			}
			if (g_check()) {
				if (left[0][1] == 'o') {
					F(); UB(); FB(); c++;
				}
				else {
					BB(); U(); B(); c++;
				}
			}
			if (b_check()) {
				if (right[0][1] == 'o') {
					FB(); U(); F(); c++;
				}
				else {
					B(); UB(); BB(); c++;
				}
			}
			if (!middle_check()) {
				if (front[1][2] != 'y' && right[1][0] != 'y' && (front[1][2] != 'o' || right[1][0] != 'b'))
				{
					while (up[0][1] != 'y' && back[0][1] != 'y') U();
					R();
					U();
					RB();
					c++;
				}
				if (front[1][0] != 'y' && left[1][2] != 'y' && (front[1][0] != 'o' || left[1][2] != 'g')) {
					while (front[0][1] != 'y' && up[2][1] != 'y') U();
					LB(); U(); L(); c++;
				}
				if (back[1][2] != 'y' && left[1][0] != 'y' && (back[1][2] != 'r' || left[1][0] != 'g')) {
					while (up[0][1] != 'y' && back[0][1] != 'y') U();
					L(); UB(); LB(); c++;
				}
				if (back[1][0] != 'y' && right[1][2] != 'y' && (back[1][0] != 'r' || right[1][2] != 'b')) {
					while (up[0][1] != 'y' && back[0][1] != 'y') U();
					RB(); U(); R(); c++;
				}
			}
		}

		show();

		while (!bottom_check()) {
			if (front[2][2] == 'w' || right[2][0] == 'w' || (down[0][2] == 'w' && front[2][2] != 'o')) {
				while (front[0][2] == 'w' || up[2][2] == 'w' || right[0][0] == 'w') U();
				R(); U(); RB(); UB(); R(); U(); RB(); UB(); R(); U(); RB(); UB();
			}
			if (right[2][2] == 'w' || back[2][0] == 'w' || (down[2][2] == 'w' && right[2][2] != 'b')) {
				while (up[0][2] == 'w' || back[0][0] == 'w' || right[0][2] == 'w') U();
				B(); U(); BB(); UB(); B(); U(); BB(); UB(); B(); U(); BB(); UB();
			}
			if ((down[2][0] == 'w' && back[2][2] != 'r') || back[2][2] == 'w' || left[2][0] == 'w') {
				while (up[0][0] == 'w' || left[0][0] == 'w' || back[0][2] == 'w') U();
				L(); U(); LB(); UB(); L(); U(); LB(); UB(); L(); U(); LB(); UB();
			}
			if ((down[0][0] == 'w' && left[2][2] != 'g') || front[2][0] == 'w' || left[2][2] == 'w') {
				while (front[0][0] == 'w' || up[2][0] == 'w' || left[0][2] == 'w') U();
				F(); U(); FB(); UB(); F(); U(); FB(); UB(); F(); U(); FB(); UB();
			}
			show();
			while (up[2][2] == 'w' || up[0][0] == 'w' || up[2][0] == 'w' || up[0][2] == 'w') {
				if (up[2][2] == 'w') {

					if (front[0][2] == 'b') {
						R(); U(); RB(); UB(); R(); U(); RB(); UB(); R(); U(); RB(); UB();
					}
					else
						if (front[0][2] == 'r') {
							UB(); B(); UB(); BB(); U(); B(); UB(); BB(); U(); B(); UB(); BB(); U();
						}
						else
							if (front[0][2] == 'g') {
								U(); U(); L(); U(); LB(); UB(); L(); U(); LB(); UB(); L(); U(); LB(); UB();
							}
							else
								if (front[0][2] == 'o') {
									U(); F(); U(); FB(); UB(); F(); U(); FB(); UB(); F(); U(); FB(); UB();
								}
				}
				else U();
			}
			while (front[0][2] == 'w' || left[0][2] == 'w' || right[0][2] == 'w' || back[0][2] == 'w') {
				show();
				while (front[0][2] != 'w') U();
				if (up[2][2] == 'g') {
					F(); UB(); FB(); UB(); UB(); F(); UB(); FB();
				}
				else
					if (up[2][2] == 'o') {
						UB(); R(); UB(); RB(); UB(); UB(); R(); UB(); RB();
					}
					else
						if (up[2][2] == 'b') {
							UB(); UB(); B(); UB(); BB(); UB(); UB(); B(); UB(); BB();
						}
						else
							if (up[2][2] == 'r') {
								U(); L(); UB(); LB(); UB(); UB(); L(); UB();  LB();
							}
			}
			while (front[0][0] == 'w' || left[0][0] == 'w' || right[0][0] == 'w' || back[0][0] == 'w') {
				while (front[0][0] != 'w') U();
				if (up[2][0] == 'g') {
					U(); U(); L(); U(); LB(); U(); U(); L(); U(); LB();
				}
				else
					if (up[2][0] == 'o') {
						U();  F(); U(); FB(); U(); U(); F(); U(); FB();
					}
					else
						if (up[2][0] == 'b') {
							R(); U(); RB(); U(); U(); R(); U(); RB();
						}
						else
							if (up[2][0] == 'r') {
								UB(); B(); U(); BB(); U(); U(); B(); U(); BB();
							}
			}

		}
	}
	void second() {
		while (!yellow_cross_check()) {
			if (stick_check()) stick();
			else
				if (angle_check()) angle();
				else stick();
		}
		show();
		while (!yellow_side_check()) {
			if (colibri_check()) colibri();
			else if (cambria_check()) cambria();
			else {
				if (up[0][0] != 'y' && up[0][2] != 'y' && up[2][0] != 'y' && up[2][2] != 'y') {
					if (right[0][2] == 'y' && (left[0][0] == 'y' || front[0][2] == 'y')) colibri();
					else if (front[0][0] == 'y' && front[0][2] == 'y') { U(); colibri(); }
					else if (front[0][0] == 'y') { U(); U(); colibri(); }
					else { UB(); colibri(); }
				}
				else if (up[0][0] == 'y' && up[2][2] == 'y' && up[0][2] != 'y' && up[2][0] != 'y') {
					if (front[0][0] == 'y') colibri();
					else { U(); U(); colibri(); }
				}
				else if (up[0][0] != 'y' && up[2][2] != 'y' && up[0][2] == 'y' && up[2][0] == 'y') {
					if (right[0][0] == 'y') { U(); colibri(); }
					else { UB(); colibri(); }
				}
				else {
					if (up[0][0] != 'y' && up[2][0] != 'y')
						if (front[0][0] == 'y') colibri();
						else { UB(); colibri(); }
					else if (up[2][0] != 'y' && up[2][2] != 'y')
						if (front[0][0] == 'y') colibri();
						else { U(); colibri(); }
					else if (up[0][2] != 'y' && up[2][2] != 'y')
						if (front[0][2] == 'y') { U(); U(); colibri(); }
						else { U(); colibri(); }
					else if (up[0][0] != 'y' && up[0][2] != 'y')
						if (left[0][0] == 'y') { UB(); colibri(); }
						else { U(); U(); colibri(); }
				}
			}
		}
	}
	void third() {
		while (!top_check()) {
			while (!right_top_check()) {
				if (pair_check()) {
					stick();
					angle();
				}
				else {
					angle();
					stick();
				}
			}
			if (front[0][1] == left[0][2]) { colibri(); U(); cambria(); }
			else { cambria(); UB(); colibri(); }
		}
		while (!check_cube()) U();
	}

	void solve() {
		if (color_ch)
			if (!check_cube()) {
				first(); //собраны первые 2 ряда
				show();
				second(); //собран верхний слой
				show();
				third(); //кубик собран
			}
			else cout << "Your cube is ok" << endl;
		else cout << "Your cube is bad" << endl;

		show();
	}

	//визуализация

	int current;

	void clear(double size, unsigned int* color) {
		this->size = size;
		current = -1;

		int i, j, k;
		for (i = 0; i < 6; i++) {
			this->color[i] = color[i];
		}

		// верх
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				a[i][j][2].setColor(0, color[0]);
			}
		}
		// низ
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				a[i][j][0].setColor(1, color[1]);
			}
		}
		// спереди
		for (k = 0; k < 3; k++) {
			for (j = 0; j < 3; j++) {
				a[j][0][k].setColor(2, color[2]);
			}
		}
		// сзади
		for (k = 0; k < 3; k++) {
			for (j = 0; j < 3; j++) {
				a[j][2][k].setColor(3, color[3]);
			}
		}
		// слева
		for (i = 0; i < 3; i++) {
			for (k = 0; k < 3; k++) {
				a[0][k][i].setColor(4, color[4]);
			}
		}
		// справа
		for (i = 0; i < 3; i++) {
			for (k = 0; k < 3; k++) {
				a[2][k][i].setColor(5, color[5]);
			}
		}
		// размеры мелких деталей
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				for (k = 0; k < 3; k++) {
					a[i][j][k].size = (size / 3.0) * 0.95;
				}
			}
		}
		// возврат на начальное состояние
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				left[i][j] = 'g';
				front[i][j] = 'o';
				right[i][j] = 'b';
				back[i][j] = 'r';
				up[i][j] = 'y';
				down[i][j] = 'w';
			}
		}

	}

	void draw() {
		const double K = 0.65;
		// корпус - это просто куб черного цвета, размер которого равен K*size
		glPushMatrix();
		glColor3f(0, 0, 0); //устанавливаем черный (просто черный кубик)
		glTranslatef(((1.0 - K) / 2) * size + K * size / 2, ((1.0 - K) / 2) * size + K * size / 2, ((1.0 - K) / 2) * size + K * size / 2);
		glutSolidCube(size * K);
		glPopMatrix();

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					ok[i][j][k] = true;
				}
			}
		}

		if (current != -1) {
			glPushMatrix();
			int i, j, k;

			if (current == 0 || current == 1) {
				k = (current & 1) * 2;
				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						ok[i][j][k] = false;
					}
				}
				glTranslated(size / 2, size / 2, 0);   // сдвигаемся к центру
				glRotatef(rotate[current], 0, 0, 1);   // крутим
				glTranslated(-size / 2, -size / 2, 0); // сдвигаемся обратно
				// рисуем
				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}
			}
			else if (current == 2 || current == 3) {
				j = (current & 1) * 2;
				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						ok[i][j][k] = false;
					}
				}

				glTranslated(size / 2, 0, size / 2);
				glRotatef(rotate[current], 0, 1, 0);
				glTranslated(-size / 2, 0, -size / 2);
				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}
			}
			else if (current == 4 || current == 5) {
				i = (current & 1) * 2;
				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						ok[i][j][k] = false;
					}
				}

				glTranslated(0, size / 2, size / 2);
				glRotatef(rotate[current], 1, 0, 0);
				glTranslated(0, -size / 2, -size / 2);
				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}
			}
			glPopMatrix();
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (ok[i][j][k]) {
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
					}
				}
			}
		}
	}
	const int CUBE_SIZE = 13;
	int TIMER = 5;

	// проекции угла поворота на оси
	int xRot = 24;
	int yRot = 44;
	int zRot = 0;
	// отдаление
	double translateZ = -35.0;

	void display() {
		glPushMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(0, 0, 0);
		glTranslatef(0, 0, translateZ);
		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);
		glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
		draw();
		glPopMatrix();
		glutSwapBuffers();
	}

	void rotate90(int id, int sign) {
		int i, j, k;
		// если sign = -1, значит крутим 3 раза
		if (sign == -1)
			sign = 3;

		while (sign--) {
			switch (id) {
			case 0: //поворот левой стороны
				k = 0;
				// копируем повёрнутую на 90 градусов верхнюю/нижнюю грань
				// в массив tmp, затем грани присваиваем tmp
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						tmp[j][2 - i] = a[i][j][k];

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						tmp[i][j].rotate_x();
						a[i][j][k] = tmp[i][j];
					}
				}
				break;
			case 1: //правой
				k = 2;
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						tmp[j][2 - i] = a[i][j][k];

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						tmp[i][j].rotate_x();
						a[i][j][k] = tmp[i][j];
					}
				}
				break;
			case 2: //верхней
				j = 0;
				for (i = 0; i < 3; i++)
					for (k = 0; k < 3; k++)
						tmp[k][2 - i] = a[i][j][k];

				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						tmp[i][k].rotate_z();
						a[i][j][k] = tmp[i][k];
					}
				}
				break;
			case 3: //нижней
				j = 2;
				for (i = 0; i < 3; i++)
					for (k = 0; k < 3; k++)
						tmp[k][2 - i] = a[i][j][k];

				for (i = 0; i < 3; i++) {
					for (k = 0; k < 3; k++) {
						tmp[i][k].rotate_z();
						a[i][j][k] = tmp[i][k];
					}
				}
				break;
			case 4: //передней
				i = 0;
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++)
						tmp[k][2 - j] = a[i][j][k];

				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						tmp[j][k].rotate_y();
						a[i][j][k] = tmp[j][k];
					}
				}
				break;
			case 5: //задней
				i = 2;
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++)
						tmp[k][2 - j] = a[i][j][k];

				for (j = 0; j < 3; j++) {
					for (k = 0; k < 3; k++) {
						tmp[j][k].rotate_y();
						a[i][j][k] = tmp[j][k];
					}
				}
				break;
			}
		}
	}

	void timer(int RorL, int) {
		if (current != -1) {
			display();
			Rotate(current, speed_torn, RorL);
		}
	}

	void Rotate(int idx, int angle, int RorL) {
		if (current == -1 || current == idx) {
			if (RorL == -1) {
				rotate[idx] -= angle;
			}
			else if (RorL == 1) {
				rotate[idx] += angle;
			}

			if (rotate[idx] % 90 != 0) {
				current = idx;
				timer(RorL, 0);
			}
			else {
				// угол стал кратным 90, поварачиваем на массиве
				if ((rotate[idx] < 0) ^ (current == 2 || current == 3)) {
					rotate90(idx, 1);
				}
				else {
					rotate90(idx, -1);
				}
				rotate[idx] = 0;
				current = -1;
			}
		}
	}
};

const int CUBE_SIZE = 13;
const int TIMER = 5;
// обозначаем цвета
unsigned int color[6] = { 0xB45F06, 0xFF0000, 0xFFFFFF, 0xFFFF00, 0x00FF00, 0x0000FF };

// проекции угла поворота на оси
int xRot = 24;
int yRot = 44;
int zRot = 0;
// отдаление
double translateZ = -35.0;
// скорость поворота, должно быть 90 % speed_torn = 0 
int speed_torn = 3;

Cube cube = Cube();

void display() {
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //очистка буферов
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, translateZ); //умножает на матрицу перевода
	glRotatef(xRot, 1, 0, 0); // умнажает на матрицу поворота
	glRotatef(yRot, 0, 1, 0);
	glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
	cube.draw();
	glPopMatrix();
	glutSwapBuffers(); //сущ скрытый буфер, с которым постоянно меняется изображение, в общем тож нада
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(60, fAspect, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init() {
	// цвет фона
	glClearColor(0.98, 0.81, 0.69, 1.0);
	// освещение
	float ambientLight[] = { 0.9, 0.9, 0.9, 1.0 };
	glShadeModel(GL_SMOOTH);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	// инициализируем куб
	cube.clear(CUBE_SIZE, color);
}

void specialKeys(int key, int, int) {
	if (key == GLUT_KEY_DOWN) {
		xRot += 3;
		if (xRot >= 360)
			xRot -= 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_UP) {
		xRot -= 3;
		if (xRot < 0)
			xRot += 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_RIGHT) {
		yRot += 3;
		if (yRot >= 360)
			yRot -= 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_LEFT) {
		yRot -= 3;
		if (yRot < 0)
			yRot += 360;
		glutPostRedisplay();
	}
}

void keys(unsigned char key, int, int) {
	if (cube.current == -1 && key == '1') {
		cube.L();
	}
	else if (cube.current == -1 && key == '2') {
		cube.R();
	}
	else if (cube.current == -1 && key == '3') {
		cube.F();
	}
	else if (cube.current == -1 && key == '4') {
		cube.B();
	}
	else if (cube.current == -1 && key == '5') {
		cube.U();
	}
	else if (cube.current == -1 && key == '6') {
		cube.D();
	}
}

void timer(int) {
	glutTimerFunc(TIMER, timer, 0);

	if (cube.current != -1) {
		cube.Rotate(cube.current, speed_torn, -1);
	}

	display();
}

void processMenu(int option) {
	switch (option) {
	case 1:
		cube.read();
		break;
	case 2:
		cube.Random();
		break;
	case 3:
		cube.solve();
		break;
	}
}

void createGLUTMenus() {
	glutCreateMenu(processMenu);

	glutAddMenuEntry("Read", 1);
	glutAddMenuEntry("Random", 2);
	glutAddMenuEntry("Solve", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 1 чтоб не мерцало, 2 используем ржб 3 для 3д 
	glutInitWindowSize(800, 700); //размер окна
	glutInitWindowPosition(1, 1); //позиция окна на экране
	glutCreateWindow("Cube"); //у окна будет такое имя
	init(); //задает цвет фона и показывает сам кубик (изначально собранный)
	glutDisplayFunc(display); //функция рисовния 
	glutReshapeFunc(reshape); //меняет размеры окна
	glutKeyboardFunc(keys); // обработка нажатия клавиш
	glutTimerFunc(TIMER, timer, 0); //функция по таймеру
	glutSpecialFunc(specialKeys); //для действий с окном
	createGLUTMenus(); //меню команд
	glutMainLoop(); //запускаем


	return 0;
}
