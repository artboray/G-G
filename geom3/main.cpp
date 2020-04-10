#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

typedef unsigned char uchar;
typedef long long ll;

FILE *file;

int type, width, height, flag, diz, bit;
bool grad;
double gc;

uchar* bytes;
double* mark;

int palette[256];

double m8[8][8] = { {0, 48, 12, 60, 3, 51, 15, 63},
                    {32, 16, 44, 28, 35, 19, 47, 31},
                    {8, 56, 4, 52, 11, 59, 7, 55},
                    {40, 24, 36, 20, 43, 27, 39, 23},
                    {2, 50, 14, 62, 1, 49, 13, 61},
                    {34, 18, 46, 30, 33, 17, 45, 29},
                    {10, 58, 6, 54, 9, 57, 5, 53},
                    {42, 26, 38, 22, 41, 25, 37, 21} };

double m4[4][4] = { {7, 13, 11, 4},
                    {12, 16, 14, 8},
                    {10, 15, 6, 2},
                    {5, 9, 3, 1} };

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

int get_b(int n, int id) {
    return ((n >> id) & 1);
}

int convertion(int n) {
    int tmp[8];
    for (int i = 0; i < 8; i++)
        tmp[i] = get_b(n, i);

    int cnt = 0;
    for (int i = 7; i >= 0; i--) {
        tmp[i] = tmp[7 - cnt++];
        if (cnt == bit)
            cnt = 0;
    }
    int ans = 0;
    for (int i = 0; i < 7; i++)
        ans += (tmp[i] << i);

    return ans;
}

int findC(double n) {
    int ans = 0;
    for (int i = 1; i < 256; i++)
        if (fabs(n - palette[i]) < fabs(n - palette[ans])) ans = i;
    return palette[ans];
}

void noDiz() {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            mark[i * width + j] = convertion(round(mark[i * width + j]));
}

void ordered(int type) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            m8[i][j] /= 64.0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m4[i][j] /= 16.0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (type == 1) mark[i * width + j] = findC(mark[i * width + j] + 255.0 * m8[i % 8][j % 8]);
            else if (type == 2) mark[i * width + j] = findC(mark[i * width + j] + (rand() % 256));
            else if (type == 3) mark[i * width + j] = findC(mark[i * width + j] + 255.0 * m4[i % 4][j % 4]);
}

void plus1_check(int i, int j, double val) {
    if (j + 1 < width) mark[i * width + j + 1] += (val * 7.0 / 16.0);
    if (i + 1 < height) mark[(i + 1) * width + j] += (val * 5.0 / 16.0);
    if (i + 1 < height && j + 1 < width) mark[(i + 1) * width + j + 1] += (val * 1.0 / 16.0);
    if (i + 1 < height && j - 1 >= 0) mark[(i + 1) * width + j - 1] += (val * 3.0 / 16.0);
}

void plus2_check(int i, int j, double val) {
    if (i + 2 < height) {
        mark[(i + 2) * width + j] += (val * 5.0 / 48.0);
        if (j - 2 >= 0) mark[(i + 2) * width + j - 2] += (val * 1.0 / 48.0);
        if (j - 1 >= 0) mark[(i + 2) * width + j - 1] += (val * 3.0 / 48.0);
        if (j + 2 < width) mark[(i + 2) * width + j + 2] += (val * 1.0 / 48.0);
        if (j + 1 < width) mark[(i + 2) * width + j + 1] += (val * 3.0 / 48.0);
    }
    if (i + 1 < height) {
        mark[(i + 1) * width + j] += (val * 7.0 / 48.0);
        if (j - 2 >= 0) mark[(i + 1) * width + j - 2] += (val * 3.0 / 48.0);
        if (j - 1 >= 0) mark[(i + 1) * width + j - 1] += (val * 5.0 / 48.0);
        if (j + 2 < width) mark[(i + 1) * width + j + 2] += (val * 3.0 / 48.0);
        if (j + 1 < width) mark[(i + 1) * width + j + 1] += (val * 5.0 / 48.0);
    }
    if (j + 2 < width) mark[i * width + j + 2] += (val * 5.0 / 48.0);
    if (j + 1 < width) mark[i * width + j + 1] += (val * 7.0 / 48.0);
}

void plus3_check(int i, int j, double val) {
    if (i + 2 < height) {
        mark[(i + 2) * width + j] += (val * 3.0 / 32.0);
        if (j - 1 >= 0) mark[(i + 2) * width + j - 1] += (val * 2.0 / 32.0);
        if (j + 1 < width) mark[(i + 2) * width + j + 1] += (val * 2.0 / 32.0);
    }
    if (i + 1 < height) {
        mark[(i + 1) * width + j] += (val * 5.0 / 32.0);
        if (j - 2 >= 0) mark[(i + 1) * width + j - 2] += (val * 2.0 / 32.0);
        if (j - 1 >= 0) mark[(i + 1) * width + j - 1] += (val * 4.0 / 32.0);
        if (j + 2 < width) mark[(i + 1) * width + j + 2] += (val * 2.0 / 32.0);
        if (j + 1 < width) mark[(i + 1) * width + j + 1] += (val * 4.0 / 32.0);
    }
    if (j + 2 < width) mark[i * width + j + 2] += (val * 3.0 / 32.0);
    if (j + 1 < width) mark[i * width + j + 1] += (val * 5.0 / 32.0);
}

void plus4_check(int i, int j, double val) {
    if (i + 2 < height) mark[(i + 2) * width + j] += (val * 1.0 / 8.0);
    if (i + 1 < height) {
        mark[(i + 1) * width + j] += (val * 1.0 / 8.0);
        if (j - 1 >= 0) mark[(i + 1) * width + j - 1] += (val * 1.0 / 8.0);
        if (j + 1 < width) mark[(i + 1) * width + j + 1] += (val * 1.0 / 8.0);
    }
    if (j + 2 < width) mark[i * width + j + 2] += (val * 1.0 / 8.0);
    if (j + 1 < width) mark[i * width + j + 1] += (val * 1.0 / 8.0);
}

void algo(int type) {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            double tmp = mark[i * width + j];
            mark[i * width + j] = findC(tmp);
            double error = tmp - mark[i * width + j];
            if (type == 1) plus1_check(i, j, error);
            if (type == 2) plus2_check(i, j, error);
            if (type == 3) plus3_check(i, j, error);
            if (type == 4) plus4_check(i, j, error);
        }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 7) {
        std::cerr << "Wrong number of arguments\n";
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == nullptr) {
        std::cerr << "Input file can't be opened\n";
        return 1;
    }

    grad = atoi(argv[3]);
    diz = atoi(argv[4]);
    bit = atoi(argv[5]);
    gc = atof(argv[6]);

    if (gc == 0.0) gc = -1;

    fscanf(file, "P%d%d%d%d\n", &type, &width, &height, &flag);

    if (type != 5) {
        std::cerr << "Input file has a wrong format\n";
        fclose(file);
        return 1;
    }

    bytes = new uchar[height * width + 100];
    mark = new double[height * width + 100];

    for (int i = 0; i < 256; i++)
        palette[i] = convertion(i);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            fread(&bytes[i * width + j], sizeof(uchar), 1, file);
            mark[i * width + j] = bytes[i * width + j];
        }
    fclose(file);

    if (grad) {
        for (int i = 0; i < width; i++)
            mark[i] = round(255.0 * double(i) / (width - 1.0));
        for (int i = 1; i < height; i++)
            for (int j = 0; j < width; j++)
                mark[i * width + j] = mark[(i - 1) * width + j];
    } else {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                if (gc != -1) mark[i * width + j] = from_gc(mark[i * width + j]);
                else mark[i * width + j] = from_sRGB(mark[i * width + j]);
    }

    //------------------------------

    if (diz == 0) noDiz();
    else if (diz == 1) ordered(1);
    else if (diz == 2) ordered(2);
    else if (diz == 3) algo(1);
    else if (diz == 4) algo(2);
    else if (diz == 5) algo(3);
    else if (diz == 6) algo(4);
    else if (diz == 7) ordered(3);
    else {
        std::cerr << "Wrong dithering format\n";
        delete[] bytes;
        delete[] mark;
        return 1;
    }
    //-----------------------------

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            if (gc != -1) mark[i * width + j] = to_gc(mark[i * width + j]);
            else mark[i * width + j] = to_sRGB(mark[i * width + j]);

            bytes[i * width + j] = (uchar)mark[i * width + j];
        }

    file = fopen(argv[2], "wb");
    if (file == nullptr) {
        std::cerr << "Output file can't be opened\n";
        delete[] bytes;
        delete[] mark;
        return 1;
    }

    fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fwrite(&bytes[i * width + j], sizeof(uchar), 1, file);

    fclose(file);
    delete[] bytes;
    delete[] mark;
    return 0;
}