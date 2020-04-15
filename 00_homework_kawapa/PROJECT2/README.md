# Description

*Multithreaded raytracer* is one of the two final projects which conclude the course "Multithreading in C++" from Coders School (https://coders.school/).

## Objective

Extend a ray tracing algorithm which generates a static scene and saves it as a picture (preferably, use the scene with a chessboard).

Use [this](http://cosinekitty.com/raytrace/chapter05_cpp_code.html) code for starters, if you want.<br>
What is ray tracing? See [this](https://www.youtube.com/watch?v=JdfV2sB-Qcw) youtube video.

## Detailed requirements

* Implement a multithreading to the existing algorithm
* Divide the picture and work-to-do between several asynchronous tasks
    * Use `std::async`, `std::future`, `std::promise` and/or `std::packaged_task`
* Measure the execution time if you apply:
    * Division into rows
    * Division into columns
    * Division into rectangles
* Measure the execution time depending on the number of asynchronous tasks