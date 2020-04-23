#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

typedef unsigned char uchar;
typedef long long ll;

FILE *file;

int type, width, height, flag, cnt, data_size = 0;
bool data_flag = 0;

struct TRP {
    uchar mas[3];
};

TRP *data;

string fcs, tcs;
vector <string> file_names, out_names;

void reader(int ind, int type, int cicle) {
    if (type == 5)
        fread(&data[ind].mas[cicle], sizeof(uchar), 1, file);
    else
        fread(&data[ind], sizeof(TRP), 1, file);
}

void writer(int ind, int type, int cicle) {
    if (type == 5)
        fwrite(&data[ind].mas[cicle], sizeof(uchar), 1, file);
    else
        fwrite(&data[ind], sizeof(TRP), 1, file);
}

int get_info() {
    for (int ct = 0; ct < file_names.size(); ct++) {
        file = fopen(file_names[ct].c_str(), "rb");

        if (file == nullptr) {
            std::cerr << "Input file can't be opened\n";
            return 1;
        }

        fscanf(file, "P%d%d%d%d\n", &type, &width, &height, &flag);

        if (data_flag == 0) {
            data_size = width * height;
            data = new TRP[width * height];
            data_flag = 1;
        } else if (width * height != data_size) {
                std::cerr << "Input files have different size formats\n";
                fclose(file);
                delete[] data;
                return 1;
            }

        if (type != 5 && type != 6) {
            std::cerr << "Input file has a wrong format\n";
            fclose(file);
            delete[] data;
            return 1;
        }
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                reader(i * width + j, type, ct);
        fclose(file);
    }
    return 0;
}

int return_info() {
    if (out_names.size() == 3) type = 5;
    else type = 6;

    for (int ct = 0; ct < out_names.size(); ct++) {
        file = fopen(out_names[ct].c_str(), "wb");

        if (file == nullptr) {
            std::cerr << "Output file can't be opened\n";
            return 1;
        }

        fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                writer(i * width + j, type, ct);
        fclose(file);
    }
    return 0;
}

double max3(double a, double b, double c) {
    return max(a, max(b, c));
}

double min3(double a, double b, double c) {
    return min(a, min(b, c));
}

void RGB_HSL() {
    for (int i = 0; i < width * height; i++) {
        double r = data[i].mas[0];
        double g = data[i].mas[1];
        double b = data[i].mas[2];
        r /= 255.0, g /= 255.0, b /= 255.0;
        double mx = max3(r, g, b);
        double mn = min3(r, g, b);
        double h, s, l;
        l = (mx + mn) / 2.0;
        if (mx == mn) h = 0, s = 0;
        else {
            double delta = mx - mn;
            if (l > 0.5) s = delta / (2.0 - mx - mn);
            else s = delta / (mx + mn);

            if (r == mx) h = (g - b) / delta + (g < b ? 6 : 0);
            else if (g == mx) h = (b - r) / delta + 2.0;
            else h = (r - g) / delta + 4.0;

            h /= 6.0;
        }
        data[i].mas[0] = round(h * 255.0);
        data[i].mas[1] = round(s * 255.0);
        data[i].mas[2] = round(l * 255.0);
    }
}

double addict(double p, double q, double t) {
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;
    if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
    if (t < 0.5) return q;
    if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    return p;
}

void HSL_RGB() {
    for (int i = 0; i < width * height; i++) {
        double h = data[i].mas[0];
        double s = data[i].mas[1];
        double l = data[i].mas[2];
        h /= 255.0, s /= 255.0, l /= 255.0;
        double r, g, b;

        if (s == 0) r = g = b = l;
        else {
            double q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
            double p = 2.0 * l - q;
            r = addict(p, q, h + 1.0 / 3.0);
            g = addict(p, q, h);
            b = addict(p, q, h - 1.0 / 3.0);
        }

        data[i].mas[0] = round(r * 255.0);
        data[i].mas[1] = round(g * 255.0);
        data[i].mas[2] = round(b * 255.0);
    }
}

void RGB_HSV() {
    for (int i = 0; i < width * height; i++) {
        double r = data[i].mas[0];
        double g = data[i].mas[1];
        double b = data[i].mas[2];
        r /= 255.0, g /= 255.0, b /= 255.0;
        double mx = max3(r, g, b);
        double mn = min3(r, g, b);
        double delta = mx - mn;

        double h, s, v;
        if (delta > 0) {
            if (mx == r) h = 60.0 * (fmod(((g - b) / delta), 6));
            else if (mx == g) h = 60.0 * (((b - r) / delta) + 2);
            else if (mx == b) h = 60.0 * (((r - g) / delta) + 4);

            if (mx > 0) s = delta / mx;
            else s = 0;

            v = mx;
        } else {
            h = 0;
            s = 0;
            v = mx;
        }

        if (h < 0) h = 360 + h;
        data[i].mas[0] = round((h / 360.0) * 255.0);
        data[i].mas[1] = round(s * 255.0);
        data[i].mas[2] = round(v * 255.0);
    }
}

void HSV_RGB() {
    for (int i = 0; i < width * height; i++) {
        double h = data[i].mas[0];
        double s = data[i].mas[1];
        double v = data[i].mas[2];
        s /= 255.0, v /= 255.0;
        h = (h / 255.0) * 360.0;
        double croma = v * s;
        double prime = fmod(h / 60.0, 6.0);
        double x = croma * (1.0 - fabs(fmod(prime, 2.0) - 1.0));
        double m = v - croma;
        double r, g, b;
        if (0 <= prime && prime < 1) {
            r = croma;
            g = x;
            b = 0;
        } else if (1 <= prime && prime < 2) {
            r = x;
            g = croma;
            b = 0;
        } else if (2 <= prime && prime < 3) {
            r = 0;
            g = croma;
            b = x;
        } else if (3 <= prime && prime < 4) {
            r = 0;
            g = x;
            b = croma;
        } else if (4 <= prime && prime < 5) {
            r = x;
            g = 0;
            b = croma;
        } else if (5 <= prime && prime < 6) {
            r = croma;
            g = 0;
            b = x;
        } else {
            r = 0;
            g = 0;
            b = 0;
        }

        r += m;
        g += m;
        b += m;
        data[i].mas[0] = round(r * 255.0);
        data[i].mas[1] = round(g * 255.0);
        data[i].mas[2] = round(b * 255.0);
    }
}

void RGB_601() {
    for (int i = 0; i < width * height; i++) {
        double r = data[i].mas[0];
        double g = data[i].mas[1];
        double b = data[i].mas[2];

        double y = 0.257 * r + 0.504 * g + 0.098 * b + 16.0;
        double cb = -0.148 * r - 0.291 * g + 0.439 * b + 128.0;
        double cr = 0.439 * r - 0.368 * g - 0.071 * b + 128.0;


        /*y -= 16.0, cb -= 16.0, cr -= 16.0;
        y = (y / 219.0) * 255.0;
        cb = (cb / 224.0) * 255.0;
        cr = (cr / 224.0) * 255.0;*/

        data[i].mas[0] = round(y);
        data[i].mas[1] = round(cb);
        data[i].mas[2] = round(cr);
    }
}

void _601_RGB() {
    for (int i = 0; i < width * height; i++) {
        double y = data[i].mas[0];
        double cb = data[i].mas[1];
        double cr = data[i].mas[2];
        /*y /= 255.0, cb /= 255.0, cr /= 255.0;
        y *= 219.0, cb *= 224.0, cr *= 224.0;
        y += 16, cb += 16, cr += 16;*/

        double r = 1.164 * (y - 16.0) + 1.596 * (cr - 128.0);
        double g = 1.164 * (y - 16.0) - 0.813 * (cr - 128.0) - 0.391 * (cb - 128.0);
        double b = 1.164 * (y - 16.0) + 2.018 * (cb - 128.0);

        data[i].mas[0] = round(r);
        data[i].mas[1] = round(g);
        data[i].mas[2] = round(b);
    }
}

void RGB_709() {
    for (int i = 0; i < width * height; i++) {
        double r = data[i].mas[0];
        double g = data[i].mas[1];
        double b = data[i].mas[2];

        double y = 0.183 * r + 0.614 * g + 0.062 * b + 16.0;
        double cb = -0.101 * r - 0.338 * g + 0.439 * b + 128.0;
        double cr = 0.439 * r - 0.399 * g - 0.040 * b + 128.0;

        /*y -= 16.0, cb -= 16.0, cr -= 16.0;
        y = (y / 219.0) * 255.0;
        cb = (cb / 224.0) * 255.0;
        cr = (cr / 224.0) * 255.0;*/

        data[i].mas[0] = round(y);
        data[i].mas[1] = round(cb);
        data[i].mas[2] = round(cr);
    }
}

void _709_RGB() {
    for (int i = 0; i < width * height; i++) {
        double y = data[i].mas[0];
        double cb = data[i].mas[1];
        double cr = data[i].mas[2];

        /*y /= 255.0, cb /= 255.0, cr /= 255.0;
        y *= 219.0, cb *= 224.0, cr *= 224.0;
        y += 16, cb += 16, cr += 16;*/

        double r = 1.164 * (y - 16.0) + 1.793 * (cr - 128.0);
        double g = 1.164 * (y - 16.0) - 0.534 * (cr - 128.0) - 0.213 * (cb - 128.0);
        double b = 1.164 * (y - 16.0) + 2.115 * (cb - 128.0);

        data[i].mas[0] = round(r);
        data[i].mas[1] = round(g);
        data[i].mas[2] = round(b);
    }
}

void RGB_YCOCG() {

    double matr[3][3] = {{0.25,  0.5, 0.25},
                         {0.5,   0,   -0.5},
                         {-0.25, 0.5, -0.25}};

    for (int i = 0; i < width * height; i++) {
        double mas[3];
        mas[0] = data[i].mas[0] / 255.0;
        mas[1] = data[i].mas[1] / 255.0;
        mas[2] = data[i].mas[2] / 255.0;
        double mt[3] = {0, 0, 0};

        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                mt[j] += matr[j][k] * mas[k];

        double y = mt[0];
        double co = mt[1];
        double cg = mt[2];

        co += 0.5, cg += 0.5;
        data[i].mas[0] = round(y * 255.0);
        data[i].mas[1] = round(co * 255.0);
        data[i].mas[2] = round(cg * 255.0);
    }

}

void YCOCG_RGB() {
    for (int i = 0; i < width * height; i++) {
        double y = data[i].mas[0];
        double co = data[i].mas[1];
        double cg = data[i].mas[2];
        y /= 255.0, co /= 255.0, cg /= 255.0;
        co -= 0.5, cg -= 0.5;

        double tmp = y - cg;
        double r = tmp + co;
        double g = y + cg;
        double b = tmp - co;

        data[i].mas[0] = round(r * 255.0);
        data[i].mas[1] = round(g * 255.0);
        data[i].mas[2] = round(b * 255.0);
    }
}

void RGB_CMY() {
    for (int i = 0; i < width * height; i++) {
        double r = data[i].mas[0];
        double g = data[i].mas[1];
        double b = data[i].mas[2];

        double c = 1.0 - (r / 255.0);
        double m = 1.0 - (g / 255.0);
        double y = 1.0 - (b / 255.0);

        data[i].mas[0] = round(c * 255.0);
        data[i].mas[1] = round(m * 255.0);
        data[i].mas[2] = round(y * 255.0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 11) {
        std::cerr << "Wrong number of arguments\n";
        return 1;
    }

    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-f")) {
            fcs = argv[++i];
        } else if (!strcmp(argv[i], "-t")) {
            tcs = argv[++i];
        } else if (!strcmp(argv[i], "-i")) {
            cnt = atoi(argv[++i]);
            i++;
            if (cnt == 1)
                file_names.push_back(argv[i]);
            else {
                string tmp = argv[i];
                string tmp1 = "";
                int sz = tmp.size();
                for (int j = 0; j < sz - 4; j++)
                    tmp1 += tmp[j];
                tmp = tmp1 + "_1.pgm";
                file_names.push_back(tmp);
                tmp = tmp1 + "_2.pgm";
                file_names.push_back(tmp);
                tmp = tmp1 + "_3.pgm";
                file_names.push_back(tmp);
            }
        } else if (!strcmp(argv[i], "-o")) {
            cnt = atoi(argv[++i]);
            i++;
            if (cnt == 1)
                out_names.push_back(argv[i]);
            else {
                string tmp = argv[i];
                string tmp1 = "";
                int sz = tmp.size();
                for (int j = 0; j < sz - 4; j++)
                    tmp1 += tmp[j];
                tmp = tmp1 + "_1.pgm";
                out_names.push_back(tmp);
                tmp = tmp1 + "_2.pgm";
                out_names.push_back(tmp);
                tmp = tmp1 + "_3.pgm";
                out_names.push_back(tmp);
            }
        }

    int error = get_info();
    if (error) return 1;

    if (fcs != tcs) {
        if (fcs != "RGB") {
            if (fcs == "HSV") HSV_RGB();
            else if (fcs == "HSL") HSL_RGB();
            else if (fcs == "YCbCr.601") _601_RGB();
            else if (fcs == "YCbCr.709") _709_RGB();
            else if (fcs == "YCoCg") YCOCG_RGB();
            else if (fcs == "CMY") RGB_CMY();
        }
        if (tcs != "RGB") {
            if (tcs == "HSV") RGB_HSV();
            else if (tcs == "HSL") RGB_HSL();
            else if (tcs == "YCbCr.601") RGB_601();
            else if (tcs == "YCbCr.709") RGB_709();
            else if (tcs == "YCoCg") RGB_YCOCG();
            else if (tcs == "CMY") RGB_CMY();
        }
    }

    error = return_info();
    if (error) {
        delete[] data;
        return 1;
    }
    return 0;
}