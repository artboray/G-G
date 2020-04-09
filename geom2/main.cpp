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
uchar* mark;

int cnt1, cnt2;
int flasm = 0;

double rangle = 0;

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

void inlin(Line l, Point p) {
    double tmp  = l.a * p.x + l.b * p.y + l.c;
    if (tmp > 0.0) cnt1++;
    if (tmp < 0.0) cnt2++;
}

bool check(Point p) {
    cnt1 = 0, cnt2 = 0;
    for (int i = 0; i < 4; i++)
        inlin(ln[i], p);
    if (cnt1 == 4 || cnt2 == 4)
        return 1;
    return 0;
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

void draw(int x, int y, double intensity, double rng) {
    if (!steep) swap(x, y);

    if ((x >= height) || (y >= width) || (x < 0) || (y < 0))
        return;

    if (rng == 45.0 && mark[x * width + y] == 13)
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

    //cout << x << ' ' << y << endl;
    mark[x * width + y] = 13;
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

void ralgo(double x0, double y0, double x, double y) {
    double dx = x - x0;
    double dy = y - y0;

    double grad;
    if (dx == 0.0) grad = 1.0;
    else grad = dy / dx;

    double xx = round(x0);
    int xs = int(xx);
    double intery = y0;
    xx = round(x);
    int xf = (int)xx;

    for (int x = xs; x <= xf; x++) {
        draw(x, round(intery), 1 - fpart(intery), 0);
        intery += grad;
    }
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

    if (rangle == 45 && thickness > 1.0) {
        ralgo(x0, y0, x, y);
        return;
    }

    double dx = x - x0;
    double dy = y - y0;

    double grad;
    if (dx == 0.0) grad = 1.0;
    else grad = dy / dx;

    double xx = round(x0);
    int xs = int(xx);
    double intery = y0 + grad * (xx - x0);
    double xgap = 1;

    if (thickness > 1.0)
        xgap = rfpart(x0 + 0.5);

    xx = round(x);
    int xf = (int)xx;

    if (y0 == y) {
        intery = y0;
        if (flasm == 2)
            xs++, xf--;

        for (int x = xs; x <= xf; x++) {
            draw(x, ipart(intery), 1 - fpart(intery), 0);
            draw(x, ipart(intery) + 1, fpart(intery), 0);
        }

        flasm = min(flasm + 1, 2);
        return;
    }

    draw(xs, ipart(intery), rfpart(intery) * xgap, 0);
    draw(xs, ipart(intery) + 1, fpart(intery) * xgap, 0);

    intery += grad;

    double intr = y + grad * (xx - x);
    xgap = 1;

    if (thickness > 1.0)
        xgap = 1 - rfpart(x + 0.5);

    draw(xf, ipart(intr), rfpart(intr) * xgap, 0);
    draw(xf, ipart(intr) + 1, fpart(intr) * xgap, 0);

    for (int x = xs + 1; x <= xf - 1; x++) {

        draw(x, ipart(intery), 1 - fpart(intery), 0);
        draw(x, ipart(intery) + 1, fpart(intery), 0);

        intery += grad;
    }
}

bool valid(int x) {
    return (x >= 0) && (x < height * width);
}

double dist(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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

    bytes = new uchar[height * width + 100];
    mark = new uchar[height * width + 100];

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fread(&bytes[i * width + j], sizeof(uchar), 1, file);

    file = fopen(argv[2], "wb");
    if (file == nullptr) {
        std::cerr << "Output file can't be opened\n";
        delete[] bytes;
        delete[] mark;
        return 1;
    }

    for (int i = 0; i < height * width; i++) mark[i] = 12;

    if (thickness > 1) {

        Point p[4];
        double angle = atan2(y - y0, x - x0);

        rangle = angle * 180.0 / M_PI;
        rangle = fabs(rangle);

        p[0].x = x0 + (thickness / 2.0) * cos(angle + M_PI / 2.0);
        p[0].y = y0 + (thickness / 2.0) * sin(angle + M_PI / 2.0);
        p[1].x = x0 + (thickness / 2.0) * cos(angle - M_PI / 2.0);
        p[1].y = y0 + (thickness / 2.0) * sin(angle - M_PI / 2.0);
        p[2].x = x + (thickness / 2.0) * cos(angle - M_PI / 2.0);
        p[2].y = y + (thickness / 2.0) * sin(angle - M_PI / 2.0);
        p[3].x = x + (thickness / 2.0) * cos(angle + M_PI / 2.0);
        p[3].y = y + (thickness / 2.0) * sin(angle + M_PI / 2.0);

        sort(p, p + 4);

        ln[0] = Line(p[0].x, p[0].y, p[1].x, p[1].y);
        ln[1] = Line(p[3].x, p[3].y, p[2].x, p[2].y);
        ln[2] = Line(p[2].x, p[2].y, p[0].x, p[0].y);
        ln[3] = Line(p[1].x, p[1].y, p[3].x, p[3].y);

        bool skp = 0;
        if (dist(p[2], p[3]) < dist(p[1], p[3])) {
            algo(p[1].x, p[1].y, p[3].x, p[3].y);
            algo(p[2].x, p[2].y, p[0].x, p[0].y);
            skp = 1;
        }

        algo(p[2].x, p[2].y, p[3].x, p[3].y);
        algo(p[0].x, p[0].y, p[1].x, p[1].y);

        if (!skp) {
            algo(p[1].x, p[1].y, p[3].x, p[3].y);
            algo(p[2].x, p[2].y, p[0].x, p[0].y);
        }


        steep = 0;
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                if (check(Point(i, j)))
                    draw(i, j, 1, rangle);

        if (rangle == 45.0 && ((round(x0) == 0 && round(y0) == 0) || (round(x) == 0 && round(y) == 0)))
            if (thickness > 1)
                draw(0, 0, 1, 0);

        /*cout << "--------" << endl;
        if (rangle != 45.0) {
            while (1) {
                int q = 0;
                for (int i = 0; i < height; i++)
                    for (int j = 0; j < width; j++)
                        if (mark[i * width + j] != 13) {
                            int cnt = 0;
                            if (valid(i * width + j + 1) && mark[i * width + j + 1] == 13) cnt++;
                            if (valid(i * width + j - 1) && mark[i * width + j + 1] == 13) cnt++;
                            if (valid((i + 1) * width + j) && mark[(i + 1) * width + j] == 13) cnt++;
                            if (valid((i - 1) * width + j) && mark[(i - 1) * width + j] == 13) cnt++;

                            if (cnt >= 3) {
                                draw(j, i, 1);
                                q++;
                            }
                    }
                if (q == 0)
                    break;
            }*/
        //}

    } else algo(x0, y0, x, y);

    fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fwrite(&bytes[i * width + j], sizeof(uchar), 1, file);

    fclose(file);
    delete[] bytes;
    delete[] mark;
    return 0;
}
