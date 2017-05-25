#include "TXLib.h"
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

const int V=8;   // скорость кота
int N=10;   // кол-во мышей
int poloz=0; //положение кота
int ti=0;
int life=3; //кол-во жизней
int score=0;// счёт




int timeigra =0; //время в игре в минутах
int kolboigr =0; //кол-во игр
int winmsh=0;    //мышей отражено
int losemsg=0;   //пропущено мышей
int bestScore=0; //лучший счёт
int kolboydarov=0;// кол-во ударов битой

int timeBeginHour=0;   // это всё для вычисления времени игры
int timeBeginMin=0;
int timeEndHour=0;
int timeEndMin=0;
time_t t;
struct tm *t_m;

int ticktacmouse=0;   // это сложно объяснить для чего
int time2=0;


bool cheat = false;  // включение чита
bool infLife = false; // бесконечные жизни  (int это infinity)
bool infYdar = false; // кот постоянно бьёт
bool krob = false; //!   кровь
bool ydarS= false; //   удар по всей площади
bool fun = false; // нечто весёлое
bool tpKot = false; // кот телепортируется к мышам
bool clearigra= false;
bool exitigra=false;
int killMsh =0;
int iclearigra=0;

int iFun=0;



struct zhivodnoe_t // тип данных (мышей, котов ... вроде всё)
    {
    int x,y;
    HDC image;
    int h,w;

    int vx,vy;
    };

struct knopka_t   // для кнопки в меню
    {
    int x,y;
    int w,h;
    char text[25];
    COLORREF color;
    COLORREF colorBack;
    COLORREF colorForw;
    };

void pauseMain();
void cheats();   // функция реализации читов
void drawcheats(); //пишет состояние читов
void zaztavka ();// заставка в меню по нажатию кнопки автор
void standart ();// создаёт для меня красивое окно
void drawmsh(zhivodnoe_t pers, HDC immsh);  // рисует в основном мышей (ну я в программе в основном и рисую мышей)
void drawkot(zhivodnoe_t kot);  // пришлось сделать отдельно рисование кота
void dviganddrawkotnadydarmsh(zhivodnoe_t* kot,zhivodnoe_t* msh, int i); // двигает кота и использует функцию drawkot
void movemsh(zhivodnoe_t* msh);  //двигает мышей
void playsound(); // играет звуки :)
bool clickCircle(int x,int y, int r); // проверка нажатия кнопки в меню читов

void vivodtext(int n,int x,int y); // функция вывода типа данных int
void vivodbool(bool n,int x,int y); // функция вывода типа данных bool

void zaz();   // заставка загрузки игры (на самом деле загрузка игры не нужна)
void igra (); // сама игра

void drawKnopka(knopka_t knopka);// рисует кнопку
int gameMenu(); // меню
int overknopka (knopka_t* knopka); // нажата ли кнопка
void statys();// вывод статистики в меню

void igraBegin (); //это всё для вычисления времени игры
void igraEnd();

void igra () // int main  (игра)
    {
    if (fun)                             // чит <<нечто весёлое>>
        txPlaySound ("sound/begin.wav");
    txClear();   // чистка экрана
    N=10;     // <<обнулить>> все данные для корректной работы
    poloz=0;
    t=0;
    life=3;
    score=0;
    ticktacmouse=0;
    killMsh=0;
    // загружаю данные кота и мыши(ей)
    zhivodnoe_t  kot = {.x = 100, .y=350, txLoadImage("image/kot.bmp"), .h=105, .w=331};    // создаю кота
    zhivodnoe_t  msh[N];      // создаю мышей
    for (int i=0;i<N;i++)
        {
        msh[i].x = 900; msh[i].y=random(301) + 100; msh[i].h=153; msh[i].w=50; msh[i].vx=random(5) + 5; msh[i].vy=random(2) + 3;
        txSleep(1);
        }
    // загрузка картинок сердца и фона (fonst для трёхмерности, но это тоже фон)
    HDC doska= txLoadImage("image/board.bmp");
    HDC serd=  txLoadImage("image/serd.bmp");
    HDC fon=   txLoadImage("image/fon.bmp");
    HDC fonst= txLoadImage("image/fonst.bmp");
    HDC immsh= txLoadImage("image/mash.bmp");
    txSetColor(TX_BLACK); // делаю нормальный шрифт
    txSetDefaults ();
    txBegin();
    N=3;
    kolboigr++; // статистика
    if ((!fun) && (!krob))
        txPlaySound ("sound/fon.wav",SND_LOOP );
    if (fun)      // чит <<нечто весёлое>>
        {
        //txSetROP2 (R2_XORPEN);
        txSetColor(TX_WHITE,2);
        for (int i=0; i< 4; i++)
            {
            txSetFillColor(TX_BLACK);
            txClear();
            vivodtext(3 - i,390,290);
            txSleep(1000);
            }
        txPlaySound ("sound/end.wav",SND_LOOP);
        }
    exitigra=false;
    iclearigra=0;
    while (!exitigra)  // цикл :)
        {
        txSetROP2 ();
        txSetColor(TX_BLACK);
        if ((!clearigra) || (iclearigra==0))
            {
            txBitBlt (txDC(), 0,0 ,800,600,fon,0,0); // рисую фон
            iclearigra++;
            }
        if (life > 3 )      // не больше 3 жизней
            life=3;
        for (int i=0;i<N;i++) // цикл :)
            {
            movemsh(&msh[i]); // двигаю мышь
            drawmsh(msh[i],immsh); // рисую мышь
            }
        for (int i=0;i<N;i++)    // цикл :)
            {
            dviganddrawkotnadydarmsh(&kot,&msh[i],i); // двигаю и рисую кота
            playsound(); // чит <<кровь>>
            }
        ti=0; // отключает змейка за котом (можно увидеть включив чит на <<нечто весёлое>>
        txTransparentBlt(txDC(), 0,0,100,50,doska,0,0,RGB(34,177,76));       // доска для вывода очков и жизней
        txTransparentBlt(txDC(), 200,0,100,50,doska,0,0,RGB(34,177,76));
        vivodtext(score,205,16);
        vivodtext(life,5,16);
        txTextOut (15,16,"x");
        for (int i=1; i<=life ; i++)  // рисует сердца
            txTransparentBlt(txDC(), i * 47 , 4,46,40,serd,0,0,RGB(34,177,76));
        if ((score % 100==0) && (score != 0))    // каждые 100 очков добавляет жизнь
            {
            life++;
            score+=10;
            if (N < 10)
                N++;
            }
        txTransparentBlt(txDC(), 0,0,800,600,fonst,0,0,RGB(34,177,76)); // рисую псевдо трёхмерную стенку
        if (fun)
            {
            iFun++;
            iFun %=255;
            txSetROP2 (R2_XORPEN);
            txSetFillColor(txHSL2RGB (RGB(iFun,200,200)));
            txRectangle  (0,0,800,600);
            }
        if (GetAsyncKeyState(VK_ESCAPE))
            {
            pauseMain();
            }
        if ((life < 1) && (!infLife))
            break;
        txEnd();   // разрешаю обновить окно
        txSleep(10); // пауза 10 милисекунд для успевания рисования картинки
        txBegin(); // блокирую обновление окна
        }
    txPlaySound  ( );
    if (!cheat)
        {
        HDC go=txLoadImage("image/gameover.bmp");
        txSetColor(TX_WHITE);
        for (int i=0; i <= 600; i+=5)
            {
            txBitBlt(txDC(), 0,i - 600,800,600,go,0,0);
            txTextOut(400 - txGetTextExtentX("Ваш счёт: ") / 2, i - 200 ,"Ваш счёт: ");
            vivodtext(score , 400 + txGetTextExtentX("Ваш счёт: ") / 2, i -200);
            if (score >= bestScore)
                {
                bestScore = score;
                txTextOut(400 - txGetTextExtentX("Вы установили лучший рекорд в игре") / 2, i - 170 ,"Вы установили лучший рекорд в игре");
                }
            else
                {
                txTextOut(400 - txGetTextExtentX("Лучший счёт: ") / 2, i - 170 ,"Лучший счёт: ");
                vivodtext(bestScore , 400 + txGetTextExtentX("Лучший счёт: ") / 2, i - 170);
                }
            txSleep(1);
            }
        txDeleteDC(go);
        }
    // выгружаю оперативку
    txDeleteDC(kot.image);
    txDeleteDC(immsh);
    txDeleteDC(serd);
    txDeleteDC(fon);
    txDeleteDC(fonst);
    txDeleteDC(doska);
    txSleep(1000);
    }

void zaz()
    {
    HDC kot=txLoadImage("image/kot.bmp");
    HDC immsh= txLoadImage("image/mash.bmp");
    txSetFillColor(TX_BLACK);
    txBegin();
    for (int i=0; i <=100; i++)
        {
        txClear();
        txTransparentBlt(txDC(), i * 6 + 150 ,320,153,50,immsh,153,0,RGB(34,177,76));
        if (i <80)
            txTransparentBlt(txDC(), i * 6 + 40,260,124,105,  kot  , 0, 0,    RGB(34,177,76));
        else
            txTransparentBlt(txDC(), i * 6 + 63 ,260   ,207,105,  kot  ,0,105,    RGB(34,177,76));
        txSetColor(TX_WHITE);
        txLine (100,400,i * 6 + 100,400);
        txEnd();
        txSleep(1);
        txBegin();
        }
    txEnd();
    txDeleteDC(kot);
    txDeleteDC(immsh);
    }

void vivodtext(int n,int x,int y)
    {
    char text[255];
    sprintf(text,"%d",n);
    txTextOut (x,y,text);
    }


void dviganddrawkotnadydarmsh(zhivodnoe_t* kot,zhivodnoe_t* msh, int i)
    {
    // узнаю какая клавиша нажата и двигаю кота
    if ((GetAsyncKeyState(83)) && (i <3)) // вверх (W)
            kot->y+=V;
    if ((GetAsyncKeyState(87)) && (i <3)) //вниз (S)
            kot->y-=V;
    if ((GetAsyncKeyState(68)) && (i <3)) // вправо(D)
            {
            kot->x+=V;
            poloz = 0; // чтобы запомнить и просто нарисовать куда смотрит код
            }
    if ((GetAsyncKeyState(65)) && (i <3)) //влево (A)
            {
            kot->x-=V;
            poloz = 1; // то же самое запоминаю и рисую куда смотрит код
            }

    if (kot->x < 50)  kot->x+=V;
    if (kot->x > 550) kot->x-=V;
    if (kot->y < 50)  kot->y+=V;
    if (kot->y > 450) kot->y-=V;

    if ((msh->x < 700) && (tpKot) && ((poloz==1) || (poloz==3)))
        {
        kot->x = msh->x + 50;
        kot->y = msh->y - 40;
        }
     if ((msh->x < 700) && (tpKot) && ((poloz==0) || (poloz==2)))
        {
        kot->x = msh->x - 170;
        kot->y = msh->y - 40;
        }

    if ((GetAsyncKeyState(VK_LBUTTON)) || (infYdar))
        {
        if ((ticktacmouse < 10) || (infYdar))
            {
            if (ticktacmouse == 0)
                kolboydarov++;
            if (poloz == 0)
                poloz = 2;
            if (poloz == 1)
                poloz =3;
            //!                                             !
            if (msh->vx > 0)
                {
                if (poloz==3)// если ударил влево
                    {
                    if (((msh->x >= kot->x - 180) && (msh->x <= kot->x - 10) && (msh->y >=kot->y - 20) && (msh->y <= kot->y + 120)) || (ydarS))
                        {
                        msh->vx=-msh->vx;
                        score+=10;
                        winmsh++;
                        killMsh++;
                        }
                    }
                else if (poloz==2)  // если ударил вправо
                    {
                    if (((msh->x >= kot->x + 140) && (msh->x <= kot->x +300) && (msh->y >= kot->y) && (msh->y <= kot->y + 110)) || (ydarS))
                        {
                        msh->vx=-msh->vx;
                        score+=10;
                        winmsh++;
                        killMsh++;
                        }
                    }

                }
            }
        else
            {
            if (poloz == 3)
                poloz =1;
            if (poloz == 2)
                poloz = 0;
            }
        ticktacmouse++;
        //!                                             !
        }
    else
        {
        ticktacmouse=0;
        if (poloz == 3)
            poloz =1;
        if (poloz == 2)
            poloz = 0;
        }
    if ((ti==0) || (fun))
        drawkot(*kot); // посылаю запрос на рисование
    ti++;
    }

void drawkot(zhivodnoe_t kot)
    {
    // узнаю положение кота
    switch (poloz)
        {
        case 0:
            {
            txTransparentBlt(txDC(), kot.x           ,kot.y   ,124,105,  kot.image  , 0, 0,    RGB(34,177,76)); // вправо
            break;
            }
        case 1:
            {
            txTransparentBlt(txDC(), kot.x + kot.w/4,kot.y   ,124,105,  kot.image  ,124,0,    RGB(34,177,76)); // влево
            break;
            }
        case 2:
            {
            txTransparentBlt(txDC(), kot.x + kot.w/4 ,kot.y   ,207,105,  kot.image  ,0,105,    RGB(34,177,76)); // вправо с ударом
            break;
            }
        case 3:
            {
            txTransparentBlt(txDC(), kot.x - kot.w/4 ,kot.y   ,207,105,  kot.image  ,207,105,  RGB(34,177,76)); // влево с ударом
            break;
            }
        default:
            {}
        }
    }

void drawmsh(zhivodnoe_t pers, HDC immsh)
    {
    // ну тут думаю понятно что рисование
    if (pers.vx < 0)
        txTransparentBlt(txDC(), pers.x,pers.y ,pers.h,pers.w,immsh,153,0,RGB(34,177,76));
    else
        txTransparentBlt(txDC(), pers.x,pers.y ,pers.h,pers.w,immsh,0,0,RGB(34,177,76));
    }

void movemsh(zhivodnoe_t* msh)
    {
    msh->x -=msh->vx; // смещаю мышку влево
    int r= random(2);
    if ((r == 1) && (time2 < 10))
        msh->vy =-msh->vy;
    if ((r == 0) && (time2 < 10))
        msh->vy =msh->vy;
    time2++;
    msh->y += msh->vy;
    if ((msh->y > 500) || (msh->y < 50))
        msh->vy = -msh->vy;
    if (msh->x > 900)
        {
        msh->x= 900;
        msh->y=random(301) + 100; // то делаю рандомную высоту мыши
        msh->vx=random(5) + 2; //  делаю рандомную скорость
        msh->vy=random(2) + 3;
        }
    if (msh-> x < (-80 - msh->w))  // если вышла за левые пределы
        {
        msh->y=random(301) + 100; // то делаю рандомную высоту мыши
        msh->x= 800 + msh->w;// и тэпэшаю на правый край окна и она поедет
        msh->vx=random(5) + 2; //  делаю рандомную скорость
        msh->vy=random(2) + 3;
        life--;
        losemsg++;
        }
    }

void zaztavka ()
    {
    txSetFillColor(TX_WHITE);
    txClear();
    char text1[]="экстракшин-продакшен представляет";
    char text2[]="мега игру cat defence";
    char text3[]="автор и единственый человек в компании: Шараев Павел";

    txSelectFont ("Comic Sans MS", 40, 10);
    txSetColor(RGB(255,0,0));
    int n1= txGetTextExtentX (text1);

    txSelectFont ("Algerian", 40, 10);
    txSetColor(RGB(0,255,0));
    int n2= txGetTextExtentX (text2);
    int i=0;
    while ((!GetAsyncKeyState(VK_ESCAPE)) && ( i < 100))
        {
        txClear();
        txSelectFont ("Comic Sans MS", 40, 10);
        txSetColor(RGB(255,0,0));
        txTextOut (400 - n1/2,i - 10,text1);
        i++;
        txSleep(1);
        }

    i=0;

       while ((!GetAsyncKeyState(VK_ESCAPE)) && ( i < 400 - n2/2))
        {
        txClear();
            txSelectFont ("Comic Sans MS", 40, 10);
            txSetColor(RGB(255,0,0));
            txTextOut (400 - n1/2,90,text1);
        txSelectFont ("Algerian", 40, 10);
        txSetColor(RGB(0,255,0));
        txTextOut (i,140,text2);
        i++;
        txSleep(1);
        }
    i=0;
    txSetColor(RGB(0,0,255));
    int n3= txGetTextExtentX (text3);
    char a[1] = "" ;
    int sizen3 = sizeof(text3)/sizeof(text3[0]);
    while ((!GetAsyncKeyState(VK_ESCAPE)) && ( i < sizen3))
        {
        sprintf(a,"%c",text3[i]);
        for (int j=0 ; j < 5; j++ )
            {
            txSetColor(TX_WHITE);
            txRectangle(400-n3/2 + i * 11,400,800,600);
                                                                                                txSelectFont ("Comic Sans MS", 40, 10);
                                                                                                txSetColor(RGB(255,0,0));
                                                                                                txTextOut (400 - n1/2,90,text1);
                                                                                                txSelectFont ("Algerian", 40, 10);
                                                                                                txSetColor(RGB(0,255,0));
                                                                                                txTextOut (400 - n2/2,140,text2);
                                                                                                txSetColor(RGB(0,0,255));
            txTextOut (400-n3/2 + i * 11, 580 - j * 4, a);
            txSleep(1);
            }
        i++;
        }
    }


void pauseMain()
    {
    txSetROP2 (R2_MASKPEN  );
    txSetFillColor(RGB(240,240,240));
    txRectangle  (0,0,800,600);
    txSetROP2 ();
    bool again = false;
    exitigra = false;
        knopka_t knopka[2] = {
                             {random(600) + 100,random(400) + 100,120,50,"вернуться",RGB(255,0,0), RGB(0,255,0),RGB(255,0,0)},
                             {random(600) + 100,random(400) + 100,120,50,"выйти"      ,RGB(255,0,0), RGB(0,255,0),RGB(255,0,0)},
                            };
    while (!exitigra && !again)
        {
        for (int i=0; i<2; i++)
            {
            drawKnopka(knopka[i]);
            }
        for (int i=0; i<2; i++)
            {
            if (overknopka(&knopka[i])==1)
                {
                if (i==0) { again=true;   }
                if (i==1) { exitigra=true;}
                }
            }
        txSleep(10);
        txEnd();
        txSleep(10);
        txBegin();
        }
    txEnd();
    txSetDefaults ();
    }



void drawKnopka(knopka_t knopka)
    {
    txSetColor(knopka.color);
    txSetFillColor(TX_WHITE);
    txRectangle (knopka.x, knopka.y , knopka.x + knopka.w , knopka.y + knopka.h);
    txSelectFont ("Comic Sans MS", 40, 10);
    txTextOut(knopka.x + (knopka.w /2 - txGetTextExtentX(knopka.text) / 2) , knopka.y , knopka.text);
    }

int overknopka (knopka_t* knopka)
    {
    if ((txMouseX() > knopka->x) && (txMouseX() < knopka->x + knopka->w) && (txMouseY() > knopka->y) && (txMouseY() < knopka->y + knopka->h))
        {
        knopka->color= knopka->colorBack;
        if (txMouseButtons() & 1) {return 1;}
        }
        else
        {
        knopka->color= knopka->colorForw;
        }
    return 0;
    }

int gameMenu()
    {
    txPlaySound ("sound/fonmenu.wav",SND_LOOP   );
    HDC gamemain= txLoadImage("image/main.bmp");
    knopka_t knopka[3] = {
                         {20,300,100,50,"старт",RGB(255,0,0), RGB(0,255,0),RGB(255,0,0)},    //struct knopka_t {
                         {20,370,100,50,"автор",RGB(255,0,0), RGB(0,255,0),RGB(255,0,0)},    //int x,y;
                         {20,440,100,50,"выход",RGB(255,0,0), RGB(0,255,0),RGB(255,0,0)},    //int w,h;
                        };                                                                    //char text[25];
    txBegin();                                                                                //COLORREF color;
    txSetFillColor(TX_WHITE);                                                                 //COLORREF colorBack;
    txClear();                                                                                //COLORREF colorForw;
    while (true)                                                       //};
        {
        txBitBlt (txDC(), 0,0 ,800,600,gamemain,0,0);
            txSetDefaults ();
            txSetColor(TX_BLACK);
            txTextOut(200, 300 ,"время (в минутах) в игре: ");
            vivodtext (timeigra, 210 + txGetTextExtentX("время (в минутах) в игре: "),300);

                txTextOut(200, 320 ,"количество сыгранных игр: ");
                vivodtext (kolboigr, 210 + txGetTextExtentX("количество сыгранных игр: "),320);

            txTextOut(200, 340 ,"отбито мышей: ");
            vivodtext (winmsh,  210 + txGetTextExtentX("отбито мышей: "),340);

                txTextOut(200, 360 ,"пропущено мышей: ");
                vivodtext (losemsg,  210 + txGetTextExtentX("пропущено мышей: "),360);

            txTextOut(200, 380 ,"лучший счёт: ");
            vivodtext (bestScore ,  210 + txGetTextExtentX("лучший счёт: ") , 380);

                txTextOut(200, 400 ,"количество ударов: ");
                vivodtext (kolboydarov ,  210 + txGetTextExtentX("количество ударов: ") , 400);
        drawcheats();
        for (int i=0; i<3; i++)
            {
            drawKnopka(knopka[i]);
            }
        for (int i=0; i<3; i++)
            {
            if (overknopka(&knopka[i])==1)
            {
            if (i==0) { txDeleteDC(gamemain); txPlaySound (); return 1; }
            if (i==1) { txDeleteDC(gamemain); txPlaySound (); return 2; }
            if (i==2) { txDeleteDC(gamemain); txPlaySound (); return 3; }
            }
            }
        cheats();
        txSleep(10);
        txEnd();
        txSleep(10);
        txBegin();
        }
    txEnd();
    txPlaySound ();
    }

void igraBegin ()
    {
    freopen("save/save.lok",     "rt",      stdin);      //чтение из файла
    cin >> timeigra >> kolboigr >> winmsh >> losemsg >> bestScore >> kolboydarov;
    fclose (stdin);
    t=time(NULL);
    t_m=localtime(&t);
    timeBeginHour = t_m->tm_hour;
    timeBeginMin = t_m->tm_min;
    }

void igraEnd()
    {
    int timeHour;
    int timeMin;
    t=time(NULL);
    t_m=localtime(&t);
    timeEndHour = t_m->tm_hour;
    timeEndMin = t_m->tm_min;
    timeHour = timeEndHour - timeBeginHour ;
    timeMin = timeEndMin - timeBeginMin;
    if (timeMin < 0)
        timeMin += 60;
    if (timeHour < 0)
        timeHour += 23;
    timeMin = timeHour * 60 + timeMin;
    timeigra += timeMin;
    if (!cheat)
        {
        freopen("save/save.lok",     "w",      stdout);      //Вывод в файл
        cout << timeigra << " " << kolboigr << " " << winmsh << " " << losemsg << " " << bestScore << " " << kolboydarov;
        fclose (stdout);
        }
    }

void standart ()
    {
    srand(static_cast<unsigned int>(time(NULL))); // делаю рандом
    txTextCursor (false);      // создаю окно
    //_txWindowStyle &= ~WS_CAPTION;// убирает рамку окна
    txCreateWindow(800,600);  // ну это же понятно что окно создал
    SetWindowText (txWindow(), "cat defence");  // переименовываю окно
    txDisableAutoPause(); // шоб не было паузы после закрытя окна
    setlocale(LC_ALL, "Russian");
    }

void drawcheats()
    {
    txSetColor(RGB (255,0,0));
    if (cheat)
        {
        txTextOut(400 - txGetTextExtentX("ЧИТЫ ВКЛЮЧЕНЫ, СТАТИСТИКА НЕ СОХРАНЯЕТСЯ") / 2, 460 ,"ЧИТЫ ВКЛЮЧЕНЫ, СТАТИСТИКА НЕ СОХРАНЯЕТСЯ");
        txTextOut(400 - txGetTextExtentX("ДЛЯ ОТКЛЮЧЕНИЯ ЧИТОВ ПЕРЕЗАПУСТИТЕ ИГРУ") / 2, 480 ,"ДЛЯ ОТКЛЮЧЕНИЯ ЧИТОВ ПЕРЕЗАПУСТИТЕ ИГРУ");
        txSetColor(TX_BLACK);

            txTextOut(500, 300 ,"бесконечные жизни");
            vivodbool(infLife,680,300);

        txTextOut(500, 320 ,"злой кот");
        vivodbool(infYdar,680,320);

            txTextOut(500, 340 ,"другие звуки");
            vivodbool(krob,680,340);


            txTextOut(500, 360 ,"удар по всей карте");
            vivodbool(ydarS,680,360);

        txTextOut(500, 380 ,"нечто весёлое");
        vivodbool(fun,680,380);

            txTextOut(500, 400 ,"волшебный кот ");
            vivodbool(tpKot,680,400);

        txTextOut(500, 420 ,"лучше не включай ");
            vivodbool(clearigra,680,420);
        }

    }

void vivodbool(bool n,int x,int y)
    {
    if (n){
        txSetFillColor(RGB(0,255,0));
        txSetColor(RGB(0,255,0));
        txCircle(x,y + 9,5);}
    else{
        txSetFillColor(RGB(255,0,0));
        txSetColor(RGB(255,0,0));
        txCircle(x,y + 9,5);}
    txSetColor(TX_BLACK);
    }

void cheats()
    {
    if ((GetAsyncKeyState(VK_CONTROL)) && (GetAsyncKeyState(VK_MENU)) && ((GetAsyncKeyState('c')) || (GetAsyncKeyState('C'))))
        {
        cheat = true;      // включение читов
        }
    if (cheat)// набор читов
        {
        if ((GetAsyncKeyState(VK_OEM_COMMA)) || (GetAsyncKeyState('<')) || (clickCircle(680,300,5)))
            {
            infLife = !infLife;
            txSleep(100);
            }

        if ((GetAsyncKeyState('p')) || (GetAsyncKeyState('P')) || (clickCircle(680,320,5)))
            {
            infYdar = !infYdar;
            txSleep(100);
            }

        if ((GetAsyncKeyState('r')) || (GetAsyncKeyState('R')) || (clickCircle(680,340,5)))
            {
            krob = !krob;
            txSleep(100);
            }

        if ((GetAsyncKeyState('e')) || (GetAsyncKeyState('E')) || (clickCircle(680,360,5)))
            {
            ydarS  = !ydarS ;
            txSleep(100);
            }

        if ((GetAsyncKeyState('y')) || (GetAsyncKeyState('Y')) || (clickCircle(680,380,5)))
            {
            fun  = !fun ;
            txSleep(100);
            }

        if ((GetAsyncKeyState('d')) || (GetAsyncKeyState('D')) || (clickCircle(680,400,5)))
            {
            tpKot  = !tpKot ;
            txSleep(100);
            }
        if ((GetAsyncKeyState('k')) || (GetAsyncKeyState('K')) || (clickCircle(680,420,5)))
            {
            clearigra  = !clearigra ;
            txSleep(100);
            }
        }
    }

bool clickCircle(int x,int y, int r)
    {
    y +=9;
    int a = (txMouseX () - x) * (txMouseX () - x) ;
    int b = (txMouseY () - y) * (txMouseY () - y) ;
    if ( ((sqrt (a + b) ) <= r) && (GetAsyncKeyState(VK_LBUTTON))) {
        txSleep(100);
        return true; }
    else {
        return false;}
    }
void playsound()
    {
    DWORD dwr = SND_ASYNC;
            if ((killMsh == 1) && (krob)){
                 txPlaySound ("sound/First Blood.wav",dwr);
                 killMsh++;}
            if ((killMsh == 3) && (krob)){
                 txPlaySound ("sound/Double Kill.wav",dwr);
                 killMsh++;}
            if ((killMsh == 5) && (krob)) {
                 txPlaySound ("sound/Triple Kill.wav",dwr);
                 killMsh++;}
            if ((killMsh == 7) && (krob))  {
                 txPlaySound ("sound/Dominating.wav",dwr);
                 killMsh++;}
            if ((killMsh == 9) && (krob))   {
                 txPlaySound ("sound/Mega Kill.wav",dwr);
                 killMsh++;}
            if ((killMsh == 11) && (krob))    {
                 txPlaySound ("sound/Unstoppable.wav",dwr);
                 killMsh++;}
            if ((killMsh == 13) && (krob))     {
                 txPlaySound ("sound/Wicked Sick.wav",dwr);
                 killMsh++;}
            if ((killMsh == 15) && (krob))      {
                 txPlaySound ("sound/Monster Kill.wav",dwr);
                 killMsh++;}
            if ((killMsh == 18) && (krob))       {
                 txPlaySound ("sound/Godlike.wav",dwr);
                 killMsh++;}
            if ((killMsh > 18) && (killMsh % 3 == 0) && (krob))
                 {
                 int randomsound = random (6);
                 switch (randomsound)
                    {
                    case 0:
                        txPlaySound ("sound/Dominating.wav",dwr);
                        break;
                    case 1:
                        txPlaySound ("sound/Mega Kill.wav",dwr);
                        break;
                    case 2:
                        txPlaySound ("sound/Unstoppable.wav",dwr);
                        break;
                    case 3:
                        txPlaySound ("sound/Wicked Sick.wav",dwr);
                        break;
                    case 4:
                        txPlaySound ("sound/Monster Kill.wav",dwr);
                        break;
                    case 5:
                        txPlaySound ("sound/Godlike.wav",dwr);
                        break;
                    default :;
                    }
                 killMsh++;
                 }
    }


