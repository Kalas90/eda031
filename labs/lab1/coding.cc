#define C 5

static unsigned char encode(unsigned char c){
    return c + C;
}

static unsigned char decode(unsigned char c) {
    return c - C;
}