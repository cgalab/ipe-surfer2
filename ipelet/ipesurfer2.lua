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

function collect_input(model)
   local p = model:page()
   local newPage  = ipe.Page()
   local newLayer = newPage:addLayer("input")
   for i, obj, sel, layer in p:objects() do
      if sel then
         newPage:insert(nil,obj,nil,newLayer)
	   end	    
   end
   newPage:removeLayer("alpha")
   newPage:setVisible(1, "input", true)
   return newPage
end

function create_skeleton(model)
   page = collect_input(model)
   doc = ipe.Document()
   doc:insert(1,page)
   doc:save(".surfer2-ipelet.ipe", nil, nil)
end

-- parameters for the C++ code
parameters = { n = "7" }


function c_test(model)
  if not ipelet then ipelet = assert(ipe.Ipelet("libipesurfer2")) end
  model:runIpelet(label, ipelet, 1, parameters)
end

methods = {
  { label="Create Skeleton", run = create_skeleton },
  { label="Run C", run = c_test},
}

-- define a shortcut for this function
--shortcuts.ipelet_1_skeleton = "Shift+H"
