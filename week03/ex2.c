#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Point{
    double x, y;
};

struct Point *createPoint(double x, double y){
    struct Point *point = (struct Point *)malloc(sizeof(struct Point));
    point->x = x;
    point->y = y;

    return point;
}

double distance(struct Point *pointA, struct Point *pointB){
    double x_diff = pointA->x - pointB->x;
    double y_diff = pointA->y - pointB->y;


    double dist = sqrt((x_diff * x_diff) + (y_diff * y_diff));

    //printf("%f\n%f\n%f\n", (x_diff * x_diff), (y_diff * y_diff), (x_diff * x_diff) + (y_diff * y_diff));

    return dist;
}

double area(struct Point *A, struct Point *B, struct Point *C){
    double triangleArea = ((A->x * B->y) - (B->x * A->y) + (B->x * B->y) - (C->x * B->y) + (C->x * A->y) - (A->x * C->y)) / 2;

    return triangleArea;
}

int main(){
    /*
        During compilation on my machine, it would not run, and I was getting some error, until I added
        the '-lm' flag to the command. I read a little about it and it has something to do with the 
        sqrt function
    */
   
    struct Point *A = createPoint(2.5, 6);
    struct Point *B = createPoint(1, 2.2);
    struct Point *C = createPoint(10, 6);


    printf("The distance betwene Point A and B is %f\n", distance(A, B));

    printf("The area of the triangle is %f\n", area(A, B, C));

    return 0;
}