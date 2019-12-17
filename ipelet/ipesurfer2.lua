-- ipe-surfer2 is an ipelet written in C++ and Lua. It is a wrapper
-- for surfer2, a weighted straight skeleton implementation based
-- exact arithmetic.
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

label = "Skeleton (Surfer2)"

about = [[ Create the (weighted) straight skeleton of a PSLG. ]]

ipelet = false

function run(model, num)
  if not ipelet then ipelet = assert(ipe.Ipelet("libipesurfer2")) end
  model:runIpelet(label, ipelet, num)
end

methods = {
  { label="Compute Skeleton" },
  { label="Compute Interior Skeleton" },
  { label="Compute Exterior Skeleton" },
  { label="Compute Offset" },
  { label="Compute Skeleton and Offset" },
  { label="Compute Skeleton and Offset (sep. layers)" },
}

-- define a shortcut for this function
shortcuts.ipelet_1_ipesurfer2 = "Shift+S"
--shortcuts.ipelet_2_ipesurfer2 = "Shift+E"
