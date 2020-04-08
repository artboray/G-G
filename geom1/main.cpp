#include <cstdio>
#include <iostream>

using namespace std;

typedef unsigned char uchar;
typedef long long ll;

const ll INF = (ll)1e8;

FILE *file;

int type;
int width, height;
int flag;

struct RGB {
    uchar r, g, b;
};

uchar *bytes;
RGB *data;

int cnt = 0;

void reader(int ind, int type) {
    if (type == 5)
        cnt += fread(&bytes[ind], sizeof(uchar), 1, file);
    else
        cnt += fread(&data[ind], sizeof(RGB), 1, file);
}

void writer(int ind, int type) {
    if (type == 5)
        fwrite(&bytes[ind], sizeof(uchar), 1, file);
    else
        fwrite(&data[ind], sizeof(RGB), 1, file);
}

void inverter(int ind, int type) {
    if (type == 5) {
        bytes[ind] = ~bytes[ind];
        fwrite(&bytes[ind], sizeof(uchar), 1, file);
    } else {
        data[ind].r = ~data[ind].r;
        data[ind].g = ~data[ind].g;
        data[ind].b = ~data[ind].b;
        fwrite(&data[ind], sizeof(RGB), 1, file);
    }
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    char tmp;
    tmp = argv[3][0];

    if (tmp < '0' || tmp > '4') {
        printf("Wrong operation asked\n");
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == nullptr) {
        printf("Input file can't be opened\n");
        return 1;
    }

    fscanf(file, "P%d%d%d%d\n", &type, &width, &height, &flag);

    if (type < 5 || type > 6) {
        printf("Input file has a wrong format\n");
        return 1;
    }
    if (width <= 0 || height <= 0 || (width * 1ll * height > INF)) {
        printf("Not acceptable amount of data\n");
        return 1;
    }

    if (type == 5) bytes = new uchar[height * width + 1];
    else data = new RGB[height * width + 1];

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            reader(i * width + j, type);
    reader(height * width, type);

    if (cnt != height * width) {
        printf("Not acceptable amount of data\n");

        if (type == 6) delete [] data;
        else delete [] bytes;

        fclose(file);
        return 1;
    }

    fclose(file);

    file = fopen(argv[2], "wb");

    if (file == nullptr) {
        printf("Output file can't be opened\n");

        if (type == 6) delete [] data;
        else delete [] bytes;

        return 1;
    }

    if (tmp == '0') {
        fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                inverter(i * width + j, type);
    }
    if (tmp == '1') {
        fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
        for (int i = 0; i < height; i++)
            for (int j = width - 1; j >= 0; j--)
                writer(i * width + j, type);
    }
    if (tmp == '2') {
        fprintf(file, "P%d\n%d %d\n%d\n", type, width, height, flag);
        for (int i = height - 1; i >= 0; i--)
            for (int j = 0; j < width; j++)
                writer(i * width + j, type);
    }
    if (tmp == '3') {
        fprintf(file, "P%d\n%d %d\n%d\n", type, height, width, flag);
        for (int i = 0; i < width; i++)
            for (int j = height - 1; j >= 0; j--)
                writer(j * width + i, type);
    }
    if (tmp == '4') {
        fprintf(file, "P%d\n%d %d\n%d\n", type, height, width, flag);
        for (int i = width - 1; i >= 0; i--)
            for (int j = 0; j < height; j++)
                writer(j * width + i, type);
    }

    if (type == 6) delete [] data;
    else delete [] bytes;

    fclose(file);
    return 0;
}