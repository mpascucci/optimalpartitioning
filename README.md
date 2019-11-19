# Optimal Partitioning
Implementation of the OP algorithm for C++ and Python.

## functions:

op1D : OP for piece-wise constant signal

![](doc/img/op1d.png)

op2D : OP for piece-wise linear signals

![](doc/img/op2d.png)

op2D : OP for piece-wise linear signals with a fast but non optimal linear constraint.

![](doc/img/op2dcc.png)

## clone the repo

This repo uses Pybind as a submodule. This requires attention when cloning. Please do:

    git clone --recursive git://github.com/foo/bar.git  

## install Python
Execute one of the following commands in the base folder of the repo.

Install in the current python environment:

    pip install .

Create an importable python module in the package folder:

    pip install -e .
