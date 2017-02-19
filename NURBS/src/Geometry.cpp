#include "Header.h"

using namespace std;
using namespace glm;

vector<float> knots;

vec2 eDelta1(vector<vec2> &input, int order, float u)
{
    vector<vec2> c;
    // determine the delta needed for the specific value of u
    int delta = 0;
    while (u >= knots[delta + 1]) { delta++; }

    // get all the control points that are relavent to our u value
    for (int i = 0; i <= order - 1; i++)
            c.push_back(input[delta - i]);

    // use the efficient implentation to determine placement of the final point
    for (int r = order; r >= 2; r--)
    {
        int i = delta;
        for (int s = 0; s < r - 1; s++)
        {
            float   denominator = knots[i + r - 1] - knots[i],
                    omega = 0.f;
            if (denominator > FLOAT_ERR)
                omega = (u - knots[i]) / denominator;
            c[s] = omega * c[s] + (1 - omega) * c[s + 1];
            i--;
        }
    }
    return c[0];
}

void bSpline(vector<vec2> &input, vector<vec2> &output, int order, float uInc)
{
    unsigned int minNumOfKnots = input.size() + order;
    float uStep = (1.f / (input.size() - order + 1));

    knots.clear();

    for (int i = 0; i < minNumOfKnots; i++)
    {
        if (i < order)
            knots.push_back(0.f);
        else if (i > input.size())
            knots.push_back(1.f);
        else
            knots.push_back(knots[i - 1] + uStep);
    }

    // calculatate the bSpline efficiently
    for (float u = 0.f; u < 1.f; u += uInc)
        output.push_back(eDelta1(input, order, u));
    
    
    // due to floating point errors, if we made the previous for loop <= 1.f, it might not reach 1.f. so we restrict it to < 1.f to ensure that it never reaches 1.f and then discretly add in the last control point as the final point. We need to do this as we assume that the user always wants the standard knot sequence
    output.push_back(input.back());
}