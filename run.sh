#!/bin/bash

cd build/
google-chrome http://localhost:8000/mandelbrot.html &
python -m http.server
