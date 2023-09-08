
# Introduction

This is an experiment in extracting the network bound part of NNCL initialization in pytorch so we can explore
measuring it in isolation and optimize it.

# How to build

You need pytorch build/installed.
The author is incredibly lazy so edit TORCH_HOME in Makefile.
Run `make ws_2` to build and test running with 2 ranks.
