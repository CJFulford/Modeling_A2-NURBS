#include "Header.h"

#define arcLength 0.08f

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

void bSpline(vector<vec3> &input, vector<vec3> &output, int order)
{
    float maxHeight = 0.f;
    int minNumOfKnots = input.size() + order;

    for (int i = 0; i < minNumOfKnots; i++)
    {
        if (i < order)
            knots.push_back(0.f);
        else if ((unsigned int)i > input.size())
            knots.push_back(1.f);
        else
            knots.push_back(knots[i - 1] + (1.f / (input.size() - order + 1)));
    }

    for (float u = 0.f; u <= 1.f; u += 0.000005f)
    {
        vec3 point = vec3(0.f, 0.f, 0.f);
        for (int i = 0; (unsigned int)i < input.size(); i++)
            point += (input[i] * deBoor_Cox(i, order, u));

        if (u == 0.f || distance(output.back(), point) > arcLength)
            output.push_back(point);
    }
}

void bSpline(vector<vec2> &input, vector<vec2> &output, int order)
{
    float maxHeight = 0.f;
    int minNumOfKnots = input.size() + order;

    for (int i = 0; i < minNumOfKnots; i++)
    {
        if (i < order)
            knots.push_back(0.f);
        else if ((unsigned int)i > input.size())
            knots.push_back(1.f);
        else
            knots.push_back(knots[i - 1] + (1.f / (input.size() - order + 1)));
    }

    for (float u = 0.f; u <= 1.f; u += 0.000005f)
    {
        vec2 point = vec2(0.f, 0.f);
        for (int i = 0; (unsigned int)i < input.size(); i++)
            point += (input[i] * deBoor_Cox(i, order, u));

        if (u == 0.f || distance(output.back(), point) > arcLength)
            output.push_back(point);
    }
}