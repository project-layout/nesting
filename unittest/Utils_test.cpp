#include <stdio.h>

#include "Utils.h"
#include "UnitTest.h"


void TestGetCircleCenter()
{
    Point p1, p2, pc;

    p1.x = 3; p1.y = 3;
    p2.x = 5; p2.y = 5;
    pc = GetCircleCenter(p1, p2, PI/6, -1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);

    p1.x = 0; p1.y = 0;
    p2.x = 10; p2.y = 10;
    pc = GetCircleCenter(p1, p2, PI/2, -1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);

    p1.x = 0; p1.y = 0;
    p2.x = 10; p2.y = 10;
    pc = GetCircleCenter(p1, p2, PI/2, 1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);

    p1.x = -5; p1.y = -5;
    p2.x = -5; p2.y = 5;
    pc = GetCircleCenter(p1, p2, PI/2, -1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);

    p1.x = -5; p1.y = -5;
    p2.x = -5; p2.y = 5;
    pc = GetCircleCenter(p1, p2, PI/2, 1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);

    p1.x = 5; p1.y = 5;
    p2.x = 5; p2.y = -5;
    pc = GetCircleCenter(p1, p2, PI/2, 1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);

    p1.x = 5; p1.y = 5;
    p2.x = 5; p2.y = -5;
    pc = GetCircleCenter(p1, p2, PI/2, -1);
    printf("p1:(%lf,%lf), p2:(%lf,%lf) -> center:(%lf,%lf)\n", p1.x, p1.y, p2.x, p2.y, pc.x, pc.y);
}
