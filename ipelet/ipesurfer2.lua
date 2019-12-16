-- surfer2.lua is a wrapper for surfer2 to be used in Ipe.
--
-- Copyright 2019 Günther Eder - geder@cs.sbg.ac.at
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

label = "Surfer2"

about = [[ Create the (weighted) straight skeleton of a PSLG. ]]

ipelet = false

-- parameters for the C++ code
parameters = { kind = "0"}

function create_skeleton(model)
   run_ipelet(model,0)
end

function create_offset(model)
   run_ipelet(model,1)
end

function create_both(model)
   run_ipelet(model,2)
end

function create_both_sep(model)
   run_ipelet(model,3)
end


function run_ipelet(model, kind)
  if not ipelet then ipelet = assert(ipe.Ipelet("libipesurfer2")) end
  parameters.kind = tostring(kind)
  model:runIpelet(label, ipelet, 1, parameters)
end

methods = {
  --{ label="Create Skeleton", run = create_skeleton },
  { label="Compute Skeleton", run = create_skeleton},
  { label="Compute Offset", run = create_offset},
  { label="Compute Skeleton and Offset", run = create_both},
  { label="Compute Skeleton and Offset (sep. layers)", run = create_both_sep},
}

-- define a shortcut for this function
shortcuts.ipelet_1_ipesurfer2 = "Shift+S"
