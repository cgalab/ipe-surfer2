# ipe-surfer2

ipe-surfer2 is an ipelet that uses surfer2 and computes the (weighted) straight skeleton of a PSLG.
Proved the weight for a segment by selecting a specific 'pen-width'.

# Requirements 

- libipe-dev
- Requirements of surfer2

# Compiling

	git clone --recurse-submodules https://github.com/cgalab/ipe-surfer2
	mkdir -p ipe-surfer2/build && cd ipe-surfer2/build
	cmake .. 
	make -j 6

In case the repository is already checked out without the submodules enter use:

	git submodule update --init --recursive

# Usage

Copy or link:
(i) the `.lua-file` to `~/.ipe/ipelets` and 
(ii) the `.so-file` to a path in `LD_LIBRARY_PATH`. 

Possible setup:

	mkdir -p ~/.local/lib
	export LD_LIBRARY_PATH="$HOME/.local/lib:$LIBRARY_PATH"
	
	mkdir -p ~/.ipe/ipelets

Then, from the `build` directory:

	cp ipelet/libipesurfer2.so ~/.local/lib
	cp ../ipelet/ipesurfer2.lua ~/.ipe/ipelets
	
Ipe shows under `ipelets` a menu entry `Skeleton (Surfer2)` with various options.

# License

ipe-surfer2 is licenced under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html).
