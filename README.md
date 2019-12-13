# ipe-surfer2

An ipelet that uses surfer2 and computes the (weighted) straight skeleton of a PSLG.

# Requirements 

- libipe-dev
- all that is required by surfer2

# Compiling

	git clone --recurse-submodules https://github.com/guenthereder/ipe-surfer2
	mkdir -p monos/build && cd monos/build
	cmake .. 
	make -j 6

In case the repository is already checked out without the submodules enter use:

	git submodule update --init --recursive

# Usage

Copy or link the 

.lua-file to `~/.ipe/ipelets`
	
.so-file to a path that is in `LD_LIBRARY`

# License

Monos is licenced under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html).

