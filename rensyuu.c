#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

//暗号化用のSbox
int S_box[16][16]={
        {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76},
        {0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0},
        {0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15},
        {0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75},
        {0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84},
        {0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf},
        {0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8},
        {0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2},
        {0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73},
        {0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb},
        {0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79},
        {0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08},
        {0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a},
        {0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e},
        {0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf},
        {0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16}
    };

//復号化用のSbox
int Inv_S_box[16][16]={
        {0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},
        {0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},
        {0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},
        {0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},
        {0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},
        {0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},
        {0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},
        {0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},
        {0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},
        {0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},
        {0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},
        {0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},
        {0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},
        {0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},
        {0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},
        {0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}
        };

//色々使う配列temp
int temp[4] = {};

//rotword
void rotword(int w[4][4]){
    int p = w[3][0];
    for(int i = 0;i < 4;i++){
        if(i == 3){
            w[3][i] = p;
        }else{
            w[3][i] = w[3][i + 1];
        }
    }
}

//サブ鍵作成の時に使用するsubword
void subword(int w[4][4]){
    int x,y;
    //printf("******************\nSubWord\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            x = w[i][j] / 0x10;
            y = w[i][j] % 0x10;
            if(i == 3){
                w[i][j] = S_box[x][y];
            }
            //printf("%x ",w[i][j]);
        }
        //printf("\n");
    }
}

//p168 4a-3 排他的論理和をとる関数
void XOR_4a3(int w[4][4],int rcon[4]){
    //printf("**************\nXOR_4a3\n");
    for(int i = 0;i < 4;i++){
        temp[i] = w[3][i] ^ rcon[i];
        //printf("temp[%d] = %d %x\n",i,temp[i],temp[i]);
    }
}

//rcon
void rcon(int w[4][4],int count){
    int forrcon[10] = {
        0b00000001,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b10000000,
        0b00011011,
        0b00110110
    };

    int ans = forrcon[count - 1];

    int rcon[4] = {ans,0,0,0};

    XOR_4a3(w,rcon);

}

void XOR_4c(int w[4][4],int k[11][4][4],int count){
    //printf("***************\nXOR_4c\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            if(i == 0){
                w[i][j] = w[i][j] ^ temp[j];
                k[count][i][j] = w[i][j];
                temp[j] = w[i][j];
                //printf("temp[%d] = %d %x\n",j,temp[j],temp[j]);
                continue;
            }
            w[i][j] = k[count - 1][i][j] ^ temp[j];
            k[count][i][j] = w[i][j];
            temp[j] = w[i][j];
            //printf("temp[%d] = %d %x\n",j,temp[j],temp[j]);
        }
    }
}

void addroundkey(int m[4][4],int k[11][4][4],int count){
    //printf("**********\naddroundkey\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            //printf("m[%d][%d] = m[%d][%d] ^ subkey[%d][%d][%d]\n",i,j,i,j,count,j,i);
            m[i][j] = m[i][j] ^ k[count][j][i];
            //printf("%x ",m[i][j]);
        }
        //printf("\n");
    }
}

//暗号化の時に使用するsubword
void subword_2(int m[4][4]){
    int x,y;
    //printf("******************\nSubWord_2\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            x = m[i][j] / 0x10;
            y = m[i][j] % 0x10;
            m[i][j] = S_box[x][y];
            //printf("%x ",m[i][j]);
        }
        //printf("\n");
    }
}

void shiftrows(int m[4][4]){
    int p;

    p = m[1][0];
    m[1][0] = m[1][1];
    m[1][1] = m[1][2];
    m[1][2] = m[1][3];
    m[1][3] = p;

    p = m[2][0];
    m[2][0] = m[2][2];
    m[2][2] = p;
    p = m[2][1];
    m[2][1] = m[2][3];
    m[2][3] = p;

    p = m[3][0];
    m[3][0] = m[3][3];
    m[3][3] = m[3][2];
    m[3][2] = m[3][1];
    m[3][1] = p;

    //printf("***********\nshiftrows\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            //printf("%x ",m[i][j]);
        }
        //printf("\n");
    }
}

void mixcolumns(int m[4][4]){
    int mix[4][4] = {
        {0x02,0x03,0x01,0x01},
        {0x01,0x02,0x03,0x01},
        {0x01,0x01,0x02,0x03},
        {0x03,0x01,0x01,0x02}
    };

    int mixcolumn[4][4] = {0};

    int a,b;

    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            for(int k = 0;k < 4;k++){
                b = 0;
                for(int l = 0;l < 8;l++){
                    a = mix[i][k] & (1 << l);
                    b = b ^ (a * m[k][j]);
                }
                mixcolumn[i][j] = mixcolumn[i][j] ^ b;
            }
            for(int q = 7;q >- 1;q--){
                if(mixcolumn[i][j] >= (0x100 << q)){
                    mixcolumn[i][j] = mixcolumn[i][j] ^ (0x11b << q);
                }
            }
        }
    }

    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            m[i][j] = mixcolumn[i][j];
        }
    }
}

//復号で使用する関数
void invmixcolumns(int m[4][4]){
    int invmix[4][4] = {
        {0x0e,0x0b,0x0d,0x09},
        {0x09,0x0e,0x0b,0x0d},
        {0x0d,0x09,0x0e,0x0b},
        {0x0b,0x0d,0x09,0x0e}
    };

    int invmixcolumn[4][4] = {0};

    int a,b;

    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            for(int k = 0;k < 4;k++){
                b = 0;
                for(int l = 0;l < 8;l++){
                    a = invmix[i][k] & (1 << l);
                    b = b ^ (a * m[k][j]);
                }
                invmixcolumn[i][j] = invmixcolumn[i][j] ^ b;
            }
            for(int q = 7;q >- 1;q--){
                if(invmixcolumn[i][j] >= (0x100 << q)){
                    invmixcolumn[i][j] = invmixcolumn[i][j] ^ (0x11b << q);
                }
            }
        }
    }

    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            m[i][j] = invmixcolumn[i][j];
        }
    }
}

void invshiftrows(int m[4][4]){
    int p;

    p = m[1][3];
    m[1][3] = m[1][2];
    m[1][2] = m[1][1];
    m[1][1] = m[1][0];
    m[1][0] = p;

    p = m[2][0];
    m[2][0] = m[2][2];
    m[2][2] = p;
    p = m[1][3];
    m[1][3] = m[1][1];
    m[1][1] = p;

    p = m[3][0];
    m[3][0] = m[3][1];
    m[3][1] = m[3][2];
    m[3][2] = m[3][3];
    m[3][3] = p;

    //printff("****************\ninvshiftrows\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            //printf("%x ",m[i][j]);
        }
        //printf("\n");
    }
}

void invsubword(int m[4][4]){
    int x,y;
    //printf("**********\ninvsubword\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            x = m[i][j] / 0x10;
            y = m[i][j] % 0x10;
            m[i][j] = Inv_S_box[x][y];
            //printf("%x ",m[i][j]);
        }
        //printf("\n");
    }
}

int main(){
//秘密鍵の作成 秘密鍵を替える場合はここをいじる
    int w[4][4] = {
        {0x00,0x01,0x02,0x03},//w0
        {0x04,0x05,0x06,0x07},//w1
        {0x08,0x09,0x0a,0x0b},//w2
        {0x0c,0x0d,0x0e,0x0f} //w3
    };

//サブ鍵 k
    int k[11][4][4] = {};
//サブ鍵生成
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            k[0][i][j] = w[i][j];
            //printf("%x ",k[0][i][j]);
        }
        //printf("\n");
    }
    //printf("*****\n");

    for(int t = 1;t <= 10;t++){
        rotword(w);
        subword(w);
        rcon(w,t);

        XOR_4c(w,k,t);

        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                //printf("%x ",k[t][i][j]);
            }
            //printf("\n");
        }
        //printf("*****\n");
    }
//暗号処理
//平文?
    int m[4][4] = {
        {0x00,0x04,0x08,0x0c},
        {0x01,0x05,0x09,0x0d},
        {0x02,0x06,0x0a,0x0e},
        {0x03,0x07,0x0b,0x0f}
    };

    printf("***Plain text***\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("%3x ",m[i][j]);
        }
        printf("\n");
    }

    int i = 0;
//p172-3 最初のAddRoundKey
    addroundkey(m,k,i);

    /*printf("***1staddroundkey***\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("%3x ",m[i][j]);
        }
        printf("\n");
    }*/

    i++;

    for(;i <= 10;i++){
        subword_2(m);
        shiftrows(m);
        if(i != 10){
            mixcolumns(m);
        }
        addroundkey(m,k,i);
    }

    printf("***Cipher text***\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("%3x ",m[i][j]);
        }
        printf("\n");
    }

//復号処理

    int j = 10;

    for(;j >= 1;j--){
        addroundkey(m,k,j);
        if(j != 10){
            invmixcolumns(m);
        if(j == 9){
            printf("***1st invmixcolumns***\n");
            for(int k = 0;k < 4;k++){
                for(int g = 0;g < 4;g++){
                    printf("%3x",m[k][g]);
                }
                printf("\n");
        }
        invshiftrows(m);
        invsubword(m);
            }
        }
    }

    addroundkey(m,k,j);

    printf("***Decrypted text***\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("%3x ",m[i][j]);
        }
        printf("\n");
    }
    return 0;
}