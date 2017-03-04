# Modeling_A2-NURBS

Author: Cody Fulford

This code is written for the second assignment of CPSC 589 - Modeling at the University of Calgary, Winter 2017

This code uses GLFW x32, GLAD V4.5, and GLM
This code was created for use with OpenGL V4.5

Program to draw and edit NURBS curves

The basic code for this project has been converted from assignment 1 from the same course.
The beginnings of the b-spline code has been taken from my assignment 1 for CPSC 587 - Animation at the University of Calgary, Winter 2017.


to compile the code type 'make' in the terminal.
to run the code, then type './NURBS'


CONTROLS:
left click on a location to add a point to the curve at that location
right click on an existing point to remove it from the curve

left click and hold on a point to select it. you can now scroll the wheel to increase and decrease the weight of the point

press g to enable geometric mode
press u and scroll to change the point to geometrically view
press i and scroll to change the u increment used in the gernation of the curve

press w to increase the order of the curve
press s to decrease the order of the curve
