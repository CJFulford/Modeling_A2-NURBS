#include "Header.h"

#define pointStep 0.001f

using namespace std;
using namespace glm;

vector<float> knots;

float deBoor_Cox(int i, int order, float u)
{
    if (order == 1)
        if (knots[i] <= u && u <= knots[i + 1])
            return 1.f;
        else
            return 0.f;
    else
    {
        float ratio1 = 0.f, ratio2 = 0.f;

        // if denominator of the ratios is zero, leave that side of the equation as 0
        if (knots[i + order - 1] - knots[i] != 0.f)
        {
            ratio1 = (u - knots[i]) / (knots[i + order - 1] - knots[i]);
            ratio1 *= deBoor_Cox(i, order - 1, u);
        }

        if (knots[i + order] - knots[i + 1] >= 0.0001f)
        {
            ratio2 = (knots[i + order] - u) / (knots[i + order] - knots[i + 1]);
            ratio2 *= deBoor_Cox(i + 1, order - 1, u);
        }
        return ratio1 + ratio2;
    }
}

// different parameters for bspline function if 2d or 3d
#ifdef ThreeD
    void bSpline(vector<vec3> &input, vector<vec3> &output, int order)
#else
    void bSpline(vector<vec2> &input, vector<vec2> &output, int order)
#endif

{
    unsigned int minNumOfKnots = input.size() + order;

    for (unsigned int i = 0; i < minNumOfKnots; i++)
    {
        if ((int)i < order)
            knots.push_back(0.f);
        else if (i > input.size())
            knots.push_back(1.f);
        else
            knots.push_back(knots[i - 1] + (1.f / (input.size() - order + 1)));
    }

    for (float u = 0.f; u <= 1.f; u += pointStep)
    {
        #ifdef ThreeD
            vec3 point = vec3(0.f, 0.f);
        #else
            vec2 point = vec2(0.f, 0.f);
        #endif
        for (unsigned int i = 0; i < input.size(); i++)
            point += (input[i] * deBoor_Cox(i, order, u));

        output.push_back(point);
    }
    output.push_back(input.back());
}