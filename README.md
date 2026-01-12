# Boids

## A Note on Realism
The first versions of the code started from a somewhat brutal reimplementatoin in c of [ben eater's boids program](https://github.com/beneater/boids)
and the current versions of the program do keep the same update logic as what is found in ben eater's code, which is a somewhat simplified take on the more traditional boids update logic, where instead of modifying the boid's speed and angle separately to keep its motion realistic, we act directly on the boid's speed vector with a bunch of forces going hither and thither.

Reading the orignal paper in which boids were presented one might see that the questionable realism of this approach is the ratio essendi itself of the boids algorithm, one may therefore conclude that this repository defeats its own purpose.

This is, at a first glance, true, but it is to be noted that the ratio essendi of this repo lies less in the realism of the behaviour implemented, and more in the parallel programming aspects of implementing it, which don't change much between this version and the one presented in the paper.

I apologize for the confusion this may have brought

# Repo Structure
