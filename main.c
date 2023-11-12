#include <stdio.h>
#include <conio.h> // Necessaire pour gérer l'affichage
#include <windows.h>
#include <time.h> //Necessaire pour gerer le temps

#define MAP_ROWS 10
#define MAP_COLS 20
#define MAP_BLANK_CHAR ' '

#define SNOOPY_SYMBOL 'S'
#define OISEAU_SYMBOL 'O'
#define BALL_SYMBOL 'B'

#define BILLIONTH 1.0e-9
#define SPEED_BALL_LEVEL1 0.3 //En secondes
#define SPEED_BALL_LEVEL2 0.1
#define SPEED_BALL_LEVEL3 0.04

#define GAME_MAX_TIME 120
#define GAME_LAST_LEVEL 3

char MAP[MAP_ROWS][MAP_COLS]; //Le tableau a 2 dimensions representant la MAP (Lignes x Colonnes)

typedef struct
{
    int row;
    int col;
    int lives;
    int score;
    int level; //Game level
    int oiseaux; // Nombre oiseaux captures dans le level
} SNOOPY;

typedef struct
{
    int row;
    int col;
} OISEAU;

typedef struct
{
    int row;
    int col;
    char direction_row;
    char direction_col;
    double time_last_move;
    double speed;
    char object_last_position; //Objet ecrase par la balle dans la MAP
} BALL;

void gotorowcol(int row, int col) //Positionne le curseur de la console en X,Y
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

void Regles_du_jeu()
{
    printf("Regles du jeu de Snoopy : \n");
    printf("A ecrire \n");
}

char Menu()
{
    char choix;
    printf("1. Regles du jeu\n");
    printf("2. Lancer un nouveau jeu\n");
    printf("6. Quitter\n");
    scanf("%c",&choix);
    return choix;
}

void Init_MAP()
{
    for(int row=0;row<MAP_ROWS;row++)
    {
        for(int col=0;col<MAP_COLS;col++)
        {
            MAP[row][col] = MAP_BLANK_CHAR;
        }
    }
}

void Draw_MAP(SNOOPY Snoopy, float duration, char status[10])
{
    gotorowcol(0,0); //Positionne le curseur en position 0,0 (en haut a gauche)

    printf("+-------------------------------------------------------------------------------+\n");
    printf("|      Score = %5d    |      Timer = %6.2f     |            Lives = %d        |\n",Snoopy.score,duration,Snoopy.lives);
    printf("+-------------------------------------------------------------------------------+\n");
    printf("|     L E V E L    %d    |      Status Game  =  %10s                       |\n",Snoopy.level, status);
    printf("+-------------------------------------------------------------------------------+\n");
    printf("+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    for(int row=0;row<MAP_ROWS;row++)
    {
        printf("|");
        for(int col=0;col<MAP_COLS;col++)
        {
            printf(" %c |",MAP[row][col]);
        }
        printf("\n");
        printf("+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }
}

SNOOPY Pos_Snoopy(SNOOPY Snoopy, int new_row, int new_col)
{
    if((new_row < 0) || (new_row > MAP_ROWS-1) || (new_col < 0) || (new_col > MAP_COLS-1)) //On touche un mur = On ne change pas la position
    {
        return Snoopy;
    }
    else
    {
        if(MAP[new_row][new_col] == OISEAU_SYMBOL) //Snoopy va attraper un oiseau...
        {
            Snoopy.oiseaux = Snoopy.oiseaux + 1;
        }

        MAP[Snoopy.row][Snoopy.col] = MAP_BLANK_CHAR; //On reinitialise la position précédente en effacant sa trace
        Snoopy.row = new_row;
        Snoopy.col = new_col;
        MAP[Snoopy.row][Snoopy.col] = SNOOPY_SYMBOL; //On repositionne Snoopy
        return Snoopy;
    }
}

SNOOPY Move_Snoopy(SNOOPY Snoopy, char key)
{
    if(key == 'd')
    {
        Snoopy = Pos_Snoopy(Snoopy,Snoopy.row,Snoopy.col+1);
    }
    if(key == 'q')
    {
        Snoopy = Pos_Snoopy(Snoopy,Snoopy.row,Snoopy.col-1);
    }
    if(key == 's')
    {
        Snoopy = Pos_Snoopy(Snoopy,Snoopy.row+1,Snoopy.col);
    }
    if(key == 'z')
    {
        Snoopy = Pos_Snoopy(Snoopy,Snoopy.row-1,Snoopy.col);
    }
    return Snoopy;
}

OISEAU Pos_Oiseau(OISEAU Oiseau, int row, int col)
{
    Oiseau.row = row;
    Oiseau.col = col;
    MAP[Oiseau.row][Oiseau.col] = OISEAU_SYMBOL;
    return Oiseau;
}

BALL Move_Ball(BALL Ball, double duration) //Deplace la balle
{
    if(duration - Ball.time_last_move >= Ball.speed)
    {
        MAP[Ball.row][Ball.col] = Ball.object_last_position; //Permet de recuperer le dernier objet ecrase par la balle
        if(Ball.direction_row == 'S') // La balle descend au Sud = Risque de collision avec le mur Sud
        {
            if(Ball.row+1 > MAP_ROWS-1) //On touche le mur Sud
            {
                Ball.row = Ball.row-1;
                Ball.direction_row = 'N';
            }
            else
            {
                Ball.row = Ball.row+1;
            }
        }

        if(Ball.direction_row == 'N') // La balle monte au Nord = Risque de collision avec le mur Nord
        {
            if(Ball.row-1 < 0) //On touche le mur Nord
            {
                Ball.row = Ball.row+1;
                Ball.direction_row = 'S';
            }
            else
            {
                Ball.row = Ball.row-1;
            }
        }
        if(Ball.direction_col == 'E')
        {
            if(Ball.col+1 > MAP_COLS-1) //On touche le mur Est
            {
                Ball.col = Ball.col-1;
                Ball.direction_col = 'W';
            }
            else
            {
                Ball.col = Ball.col+1;
            }
        }

        if(Ball.direction_col == 'W')
        {
            if(Ball.col-1 < 0) //On touche le mur West
            {
                Ball.col = Ball.col+1;
                Ball.direction_col = 'E';
            }
            else
            {
                Ball.col = Ball.col-1;
            }
        }
        Ball.object_last_position = MAP[Ball.row][Ball.col];
        MAP[Ball.row][Ball.col] = BALL_SYMBOL;
        Ball.time_last_move = duration;
    }
    return Ball;
}

SNOOPY Game_Lost(SNOOPY Snoopy)
{
    Snoopy.lives = Snoopy.lives - 1;
    Snoopy.oiseaux = 0;
    return Snoopy;
}

SNOOPY Game_Win(SNOOPY Snoopy, int duration)
{
    if(Snoopy.level < GAME_LAST_LEVEL)
    {
        Snoopy.level = Snoopy.level + 1;
    }
    Snoopy.score = Snoopy.score + (GAME_MAX_TIME-duration)*100;
    Snoopy.oiseaux = 0;
    return Snoopy;
}

SNOOPY Game(SNOOPY Snoopy)
{
    //On initialise les objets
    OISEAU Oiseau1 = {0,0};
    OISEAU Oiseau2 = {0,0};
    OISEAU Oiseau3 = {0,0};
    OISEAU Oiseau4 = {0,0};

    BALL Ball = {0,1,'S','E',0,SPEED_BALL_LEVEL1,MAP_BLANK_CHAR}; //Coordonnee (row,col), direction, time last move,speed

    char key, key2;

    struct timespec Game_begin, Game_time, Game_paused, Game_unpaused;
    float Game_duration = 0;
    float Pause_duration = 0;

    clock_gettime(CLOCK_MONOTONIC, &Game_begin);

    system("cls"); //Efface l'ecran de la console

    Init_MAP(); // Initialise les valeurs de la map

    //On positionne les Oiseaux et snoopy dans la MAP
    Snoopy = Pos_Snoopy(Snoopy, MAP_ROWS/2, MAP_COLS/2);
    Oiseau1 = Pos_Oiseau(Oiseau1,0,0);
    Oiseau2 = Pos_Oiseau(Oiseau2,0,MAP_COLS-1);
    Oiseau3 = Pos_Oiseau(Oiseau3,MAP_ROWS-1,0);
    Oiseau4 = Pos_Oiseau(Oiseau4,MAP_ROWS-1,MAP_COLS-1);

    while (Game_duration < 120) // Conditions de sortie = timer > 120s
    {
        if(kbhit()) //Detecte si une touche clavier a ete saisie
        {
            key = getch();  // Recupere la touche du clavier avec getch de la librairie conio.h pour eviter d'attendre le retour chariot

            if(key == 'k')  // Exemple : Sortir de la boucle si 'k' est pressée
            {
                exit(0);
            }

            if(key == 'p') // Enter Password
            {
            }

            if(key == 'l') // Save Game
            {
            }

            if(key == ' ') // Pause
            {
                clock_gettime(CLOCK_MONOTONIC, &Game_paused);
                Draw_MAP(Snoopy, Game_duration, "PAUSED   !");
                while(1)
                {
                    key2 = getch();  // Recupere une touche du clavier
                    if(key2 == ' ')
                    {
                        break;
                    }
                    if(key == 'k')  // Exemple : Sortir de la boucle si 'k' est pressée
                    {
                        exit(0);
                    }
                }
                clock_gettime(CLOCK_MONOTONIC, &Game_unpaused);
                Pause_duration = Pause_duration + (Game_unpaused.tv_sec - Game_paused.tv_sec )+(Game_unpaused.tv_nsec - Game_paused.tv_nsec)*BILLIONTH; //La duree de la pause peut etre cumulee entre plusieurs pauses... Dans le calcul d'une duree, il faut ajouter les valeurs des temps en secondes et en nanosecondes
            }

            Snoopy = Move_Snoopy(Snoopy,key);
            if(Snoopy.oiseaux == 4)
            {
                Snoopy = Game_Win(Snoopy, Game_duration);
                Draw_MAP(Snoopy, Game_duration, "WIN  !!!!!");
                Sleep(3000);
                return Snoopy;
            }
        }

        // Gestion du temps de jeu
        clock_gettime(CLOCK_MONOTONIC, &Game_time);
        Game_duration = (Game_time.tv_sec - Game_begin.tv_sec )+(Game_time.tv_nsec - Game_begin.tv_nsec)*BILLIONTH - Pause_duration; //tv_sec est en secondes et tv.nsec en nanosecondes

        //On fait avancer la balle
        Ball = Move_Ball(Ball,Game_duration);

        if(Ball.object_last_position == SNOOPY_SYMBOL) // La balle touche Snoopy -> C'est perdu !!
        {
            Snoopy = Game_Lost(Snoopy);
            Draw_MAP(Snoopy, Game_duration, "LOST !!!!!");
            Sleep(3000);
            return Snoopy;
        }

        //Sleep(100);
        //Affichage de la map
        Draw_MAP(Snoopy,Game_duration,"Running   ");
    }
    return Snoopy;
}

int main()
{
    char choix = '0';

    while(choix != '6')
    {
        //printf("je suis ici et choix = %d\n",choix);
        if(choix == '0')
        {
            choix = Menu();
            //printf("jai saisi = %d\n",choix);
        }
        else if (choix == '1')
        {
            Regles_du_jeu();
            choix = '0';
        }
        else if (choix == '2')
        {
            SNOOPY Snoopy = {MAP_ROWS/2, MAP_COLS/2, 3, 0, 1, 0}; //Coordonnee (row,col), Nombre de vies, score, level, oiseaux
            while ((Snoopy.level <= 3) && (Snoopy.lives > 0)) {
                Snoopy = Game(Snoopy);
            }
            choix = '0';
        }
        else {
            choix = '0';
        }
    }
    return 0;
}
