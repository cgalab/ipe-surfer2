# ipe-surfer2

ipe-surfer2 is an ipelet that uses surfer2 and computes the (weighted) straight skeleton of a PSLG.
Proved the weight for a segment by selecting a specific 'pen-width'.

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

Copy or link (i) the `.lua-file` to `~/.ipe/ipelets` and (ii) the `.so-file` to a path in `LD_LIBRARY`

Ipe shows under `ipelets` a menu entry `Skeleton (Surfer2)` with various options.

# License

ipe-surfer2 is licenced under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html).


