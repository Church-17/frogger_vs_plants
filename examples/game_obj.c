#define FROGGER_BULLET 0
#define PLANT_BULLET 1


struct Frog
{
    pid_t pid;
    int x;
    int y;
    int speed;
    int add_speed;
    int lifes;
};

struct Crocodile
{
    pid_t pid;
    int speed;
    bool kind;
};

struct Plant
{
    pid_t pid;
    int x;
    int y; // o tutte sulla stessa riga...?
    int lifes;
};

struct Bullet
{
    pid_t pid;
    int x;
    int y;
    int speed;
    bool shooter;
};


