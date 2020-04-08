#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

typedef unsigned char uchar;
typedef long long ll;

FILE *file;

int type, width, height, flag;
double brightness, thickness, gc;

uchar* bytes;
bool steep = 0;

struct Point {
    double x, y;
    Point(double a, double b) {
        x = a;
        y = b;
    }
    Point() {
        x = 0;
        y = 0;
    }
};

bool operator <(Point a, Point b) {
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

struct Line {
    double a, b, c;
    Line(double x0, double y0, double x, double y) {
        a = y - y0;
        b = x0 - x;
        c = -a * x - b * y;
    }
    Line() {
        a = 0;
        b = 0;
        c = 0;
    }
};

Line ln[4];

int inlin(Line l, Point p) {
    double tmp  = l.a * p.x + l.b * p.y + l.c;
    if (tmp > 0) return 1;
    else if (tmp < 0) return 0;
    else return 10;
}

bool check(Point p) {
    int ans = 0;
    for (int i = 0; i < 4; i++)
        ans += inlin(ln[i], p);
    if (ans == 0 || ans == 4)
        return 1;
}

double from_gc(double u) {
    return 255.0 * pow(u / 255.0, gc);
}

double to_gc(double u) {
    return 255.0 * pow(u / 255.0, 1.0 / gc);
}

double to_sRGB(double u) {
    if (u / 255.0 <= 0.0031308) return u * 12.92;
    else return 255.0 * (1.055 * pow((u / 255.0), 0.416) - 0.055);
}

double from_sRGB(double u) {
    if (u / 255.0 <= 0.04045) return u / 12.92;
    else return 255.0 * pow(((u / 255.0) + 0.055) / 1.055, 2.4);
}

void draw(int x, int y, double intensity) {
    if (!steep) swap(x, y);

    if ((x >= height) || (y >= width) || (x < 0) || (y < 0))
        return;

    if (intensity == 0.0) return;
    if (thickness < 1.0) intensity *= thickness;

    double tmp = 0.0;
    if (intensity != 1) {
        tmp = (int)bytes[x * width + y];

        if (gc != -1) tmp = from_gc(tmp);
        else tmp = from_sRGB(tmp);
    }
    if (gc != -1) bytes[x * width + y] = uchar(to_gc(tmp + ((brightness - tmp) * intensity)));
    else bytes[x * width + y] = uchar(to_sRGB(tmp + ((brightness - tmp) * intensity)));
}

int ipart(double x) {
    return int(x);
}

double fpart(double x) {
    return x - floor(x);
}

double rfpart(double x) {
    return 1.0 - fpart(x);
}

void algo(double x0, double y0, double x, double y) {
    steep = fabs(y - y0) > fabs(x - x0);

    if (steep) {
        swap(x, y);
        swap(y0, x0);
    }
    if (x0 > x) {
        swap(x, x0);
        swap(y, y0);
    }

    double dx = x - x0;
    double dy = y - y0;

    double grad;
    if (dx == 0.0) grad = 1.0;
    else grad = dy / dx;

    double xx = round(x0);
    double yy = y0 + grad * (xx - x0);
    double xgap = rfpart(x0);

    int xs = int(xx);
    int ys = ipart(yy);

    //draw(xs, ys, (1 - fpart(yy)) * xgap);
    //draw(xs, ys + 1, fpart(yy) * xgap);

    double intery = y0;

    xx = round(x);
    yy = y + grad * (xx - x);
    xgap = rfpart(x);

    int xf = (int)xx;
    int yf = ipart(yy);

    //draw(xf, yf, (1 - fpart(yy)) * xgap);
    //draw(xf, yf + 1, fpart(yy) * xgap);

    for (int x = xs; x <= xf; x++) {
        draw(x, ipart(intery), 1 - fpart(intery));
        draw(x, ipart(intery) + 1, fpart(intery));
        //draw(x, ipart(intery) - 1, fpart(intery));
        intery += grad;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 9 || argc > 10) {
        std::cerr << "Wrong number of arguments\n";
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == nullptr) {
        std::cerr << "Input file can't be opened\n";
        return 1;
    }

    brightness = atoi(argv[3]);
    thickness = atof(argv[4]);

    double x0 = atof(argv[5]);
    double y0 = atof(argv[6]);
    double x = atof(argv[7]);
    double y = atof(argv[8]);

    if (argc == 10) {
        gc = atof(argv[9]);
        brightness = from_gc(brightness);
    } else {
        gc = -1;
        brightness = from_sRGB(brightness);
    }

    fscanf(file, "P%d%d%d%d\n", &type, &width, &height, &flag);

    if (type != 5) {
        std::cerr << "Input file has a wrong format\n";
        fclose(file);
        return 1;
    }

    bytes = new uchar[height * width + 10];

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fread(&bytes[i * width + j], sizeof(uchar), 1, file);

    file = fopen(argv[2], "wb");
    if (file == nullptr) {
        std::cerr << "Output file can't be opened\n";
        delete[] bytes;
        return 1;
    }

    if (thickness > 1) {

        Point p[4];
        double angle = atan2(y - y0, x - x0);
        p[0].x = x0 + (thickness / 2.0) * cos(angle + M_PI / 2.0);
        p[0].y = y0 + (thickness / 2.0) * sin(angle + M_PI / 2.0);
        p[1].x = x0 + (thickness / 2.0) * cos(angle - M_PI / 2.0);
        p[1].y = y0 + (thickness / 2.0) * sin(angle - M_PI / 2.0);
        p[2].x = x + (thickness / 2.0) * cos(angle - M_PI / 2.0);
        p[2].y = y + (thickness / 2.0) * sin(angle - M_PI / 2.0);
        p[3].x = x + (thickness / 2.0) * cos(angle + M_PI / 2.0);
        p[3].y = y + (thickness / 2.0) * sin(angle + M_PI / 2.0);

        sort(p, p + 4);

        //cout << p[0].x << ' ' << p[0].y << endl;
        //cout << p[1].x << ' ' << p[1].y << endl;
        //cout << p[2].x << ' ' << p[2].y << endl;
        //cout << p[3].x << ' ' << p[3].y << endl;

        ln[0] = Line(p[0].x, p[0].y, p[1].x, p[1].y);
        ln[1] = Line(p[3].x, p[3].y, p[2].x, p[2].y);
        ln[2] = Line(p[2].x, p[2].y, p[0].x, p[0].y);
        ln[3] = Line(p[1].x, p[1].y, p[3].x, p[3].y);

        algo(p[2].x, p[2].y, p[0].x, p[0].y);
        algo(p[1].x, p[1].y, p[3].x, p[3].y);
        algo(p[0].x, p[0].y, p[1].x, p[1].y);
        algo(p[3].x, p[3].y, p[2].x, p[2].y);

        steep = 0;
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                if (check(Point(i, j)))
                    draw(i, j, 1);

    } else algo(x0, y0, x, y);

    fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fwrite(&bytes[i * width + j], sizeof(uchar), 1, file);

    fclose(file);
    delete[] bytes;
    return 0;
}
