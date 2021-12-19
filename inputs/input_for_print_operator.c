int square(x) {
    return x * 2;
};

int main() {
    int a = 1;
    int b = 2;
    b += a;
    b = square(b);
    return b;
}
