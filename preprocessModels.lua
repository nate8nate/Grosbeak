local bit = require 'bit'

local paths = {...}

local function writeTable(file, table, format)
  for i=1, #table do
    file:write(string.format(format, table[i]), ' ')
  end
  file:write('\n')
end

for i=1, #paths do
  local num
  local offset

  local meshName
  local vertices = {}
  local positions = {}
  local normals = {}
  local vertexIndices = {}

  local fbx = io.open(paths[i], 'r'):read('*a')

  local objectsLocation = string.match(fbx, 'Objects:()')

  local meshLocation
  meshName, meshLocation = string.match(fbx, 'Model:%s"Model::(%w+)",%s"Mesh()"', objectsLocation)

  local positionsLocation = string.match(fbx, 'Vertices:%s()', meshLocation)
  local endOfPositions = string.match(fbx, '()PolygonVertexIndex:', positionsLocation)
  offset = positionsLocation
  while true do
    num, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    if offset > endOfPositions then break end
    table.insert(positions, num)
  end

  local vertexIndicesLocation = string.match(fbx, 'PolygonVertexIndex:%s()', positionsLocation)
  local endOfVertexIndices = string.match(fbx, '()GeometryVersion:', vertexIndicesLocation)
  offset = vertexIndicesLocation
  while true do
    num, offset = string.match(fbx, '(%-?%d+),?()', offset)
    if offset > endOfVertexIndices then break end
    if string.match(num, '(-)') then num = tostring(tonumber(bit.bnot(num))) end
    table.insert(vertexIndices, num)
  end

  local normalsLocation = string.match(fbx, 'Normals:%s()', offset)
  -- local endOfNormals = string.match(fbx, '}()', normalsLocation)
  offset = normalsLocation
  for vi=1,#vertexIndices do
    -- local n = normals[vertexLocation[f]]
    local num1, num2, num3
    num1, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    num2, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    num3, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    local x,y,z = vertexIndices[vi]*3+1, vertexIndices[vi]*3+2, vertexIndices[vi]*3+3
    normals[x] = (normals[x] or 0) + num1
    normals[y] = (normals[y] or 0) + num2
    normals[z] = (normals[z] or 0) + num3
  end
  for n=1,#normals,3 do
    local x,y,z = normals[n],normals[n+1],normals[n+2]
    local m = math.sqrt(x*x + y*y + z*z)
    normals[n],normals[n+1],normals[n+2] = x/m, y/m, z/m
  end

  for v=1,#positions,3 do
    table.insert(vertices, positions[v])
    table.insert(vertices, positions[v+1])
    table.insert(vertices, positions[v+2])
    table.insert(vertices, normals[v])
    table.insert(vertices, normals[v+1])
    table.insert(vertices, normals[v+2])
  end

  local meshFile = io.open(string.format("%s.mesh", meshName), 'w')
  meshFile:write(#positions/3, ' ')
  writeTable(meshFile, vertices, "%f")
  meshFile:write(#vertexIndices, ' ')
  writeTable(meshFile, vertexIndices, "%d")
end
