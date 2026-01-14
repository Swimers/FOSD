#ifndef POCKETSCONFIG_H
#define POCKETSCONFIG_H


typedef union FLOATA{
    int all;
    char u8Byte[4];
}FLOATA;

typedef union FLOATA2{
    float all;
    char u8Byte[4];
}FLOATA2;

typedef union FLOATA3{
    int all;
    char u8Byte[2];
}FLOATA3;

#define MAX_POINTS 5000
#define MAX_CHANNELS  16
#define DataAddress 24
                    //7  28-29-30-31
                    //16 24-25-26-27

#endif // POCKETSCONFIG_H
