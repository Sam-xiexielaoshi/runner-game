#include "raylib.h"

struct AnimData{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight){
    return data.pos.y>=windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime,int maxFrame){
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime>=data.updateTime){
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame>maxFrame){
            data.frame =0;
        }
    }return data;
}

int main(){

    int windowDimensions[2];
    windowDimensions[0]=800;
    windowDimensions[1]=600;


    //intialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "Runner");

    //acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    //obstacle variables
    Texture2D obstacle = LoadTexture("textures/12_nebula_spritesheet.png");

    const int numberOfOb{6};
    AnimData ob[numberOfOb]{};

    for(int i=0;i<numberOfOb;i++){
        ob[i].rec.x = 0.0;
        ob[i].rec.y= 0.0;
        ob[i].rec.width = obstacle.width/8;
        ob[i].rec.height = obstacle.height/8;
        ob[i].pos.y = windowDimensions[1]-obstacle.height/8;
        ob[i].frame=0;
        ob[i].runningTime=0.0;
        ob[i].updateTime = 1.0/16.0;
        ob[i].pos.x=windowDimensions[0]+i*300;
    }

    float finishLine{ob[numberOfOb-1].pos.x};

    //obstacle x velocity pixel per second
    int obstacleVel{-600};

    //hero variables
    Texture2D hero = LoadTexture("textures/scarfy.png");
    AnimData heroData;
    heroData.rec.width = hero.width/6;
    heroData.rec.height = hero.height;
    heroData.rec.x = 0;
    heroData.rec.y = 0;
    heroData.pos.x = windowDimensions[0]/2 - heroData.rec.width/2;
    heroData.pos.y = windowDimensions[1] - heroData.rec.height;
    heroData.frame =0;
    heroData.updateTime = 1.0/12.0;
    heroData.runningTime = 0.0;

    bool isInAir{};

    //jump vel pixels/sec
    const int jump_vel{-600};
    
    // int ypos{windowDimensions[1] - rect_height}
    int vel{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/foreground.png");
    Texture2D foreground = LoadTexture("textures/back-buildings.png");
    float bgX{};
    float mgX{};
    float fgX{};

    bool collision_with_obstacle = false;
    //set fps
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        //delta time time since last frame (seconds)
        const float dt = GetFrameTime();
        //start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        bgX -= 20 * dt;
        if(bgX<=-background.width*3.2){
            bgX=0.0;
        }

        mgX -= 40 * dt;
        if(mgX<=-midground.width*3.2) mgX=0.0;

        fgX-=80*dt;
        if(fgX<=-foreground.width*3.2) fgX=0.0;

        //draw the background
        Vector2 bg1Pos{bgX,0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 3.2, WHITE);
        Vector2 bg2X{bgX+background.width*3.2,0.0};
        DrawTextureEx(background, bg2X, 0.0, 3.2, WHITE);

        //draw the midground
        Vector2 mg1Pos{mgX,0.0};
        DrawTextureEx(midground,mg1Pos,0.0,3.2,WHITE);
        Vector2 mg2Pos{mgX+midground.width*3.2,0.0};
        DrawTextureEx(midground, mg2Pos, 0.0,3.2,WHITE);

        //draw the foreground
        Vector2 fg1Pos{fgX,0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 3.2, WHITE);
        Vector2 fg2Pos{fgX+foreground.width*3.2,0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.2, WHITE);

        if (collision_with_obstacle){
            DrawText("Game Over", 400, 300, 20, RED); 
        }

        //perform ground check
        if(isOnGround(heroData,windowDimensions[1])){
            //rect is on ground
            vel=0;
            isInAir=false;
        }else{
            //apply gravity
            vel+=gravity*dt;
            isInAir=true;
        }
        
        if(IsKeyPressed(KEY_SPACE) && !isInAir){
            vel+=jump_vel;
        }

        //update hero positions
        heroData.pos.y += vel*dt;

        //update obstacle position
        for(int i=0;i<numberOfOb;i++){
            ob[i].pos.x+=obstacleVel*dt;
        }

        //update finish line
        finishLine+=obstacleVel*dt;

        //update hero animation
        if (!isInAir){
            heroData = updateAnimData(heroData, dt, 5);
        }

        for(int i=0; i<numberOfOb;i++){
            //update obstacle animation 1
            ob[i]=updateAnimData(ob[i], dt, 7);
        }

        //draw obstacle
        for(int i=0; i<numberOfOb;i++){
            DrawTextureRec(obstacle,ob[i].rec,ob[i].pos,WHITE);
        }

        //draw hero
        DrawTextureRec(hero, heroData.rec, heroData.pos, WHITE);

        //end drawing
        EndDrawing();
    }
    UnloadTexture(hero);
    UnloadTexture(obstacle);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}