local bit = require 'bit'

local paths = {...}

local function writeTable(file, table)
  file:write(#table, ' ')
  for i=1, #table do
    file:write(table[i], ' ')
  end
  file:write('\n')
end

for i=1, #paths do
  local num
  local offset

  local meshName
  local vertices = {}
  local faces = {}
  local normals = {}

  local fbx = io.open(paths[i], 'r'):read('*a')

  local objectsIndex = string.match(fbx, 'Objects:()')

  local meshIndex
  meshName, meshIndex = string.match(fbx, 'Model:%s"Model::(%w+)",%s"Mesh()"', objectsIndex)

  local verticesIndex = string.match(fbx, 'Vertices:%s()', meshIndex)
  local endOfVertices, facesIndex = string.match(fbx, '()PolygonVertexIndex:%s()', verticesIndex)
  offset = verticesIndex
  while true do
    num, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    if offset > endOfVertices then break end
    table.insert(vertices, num)
  end

  local endOfFaces = string.match(fbx, '()GeometryVersion:', facesIndex)
  offset = facesIndex
  while true do
    num, offset = string.match(fbx, '(%-?%d+),?()', offset)
    if offset > endOfFaces then break end
    if string.match(num, '(-)') then num = tostring(tonumber(bit.bnot(num))) end
    table.insert(faces, num)
  end

  local normalsIndex = string.match(fbx, 'Normals:%s()', offset)
  local endOfNormals = string.match(fbx, '}()', normalsIndex)
  while true do
    num, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    if offset > endOfNormals then break end
    table.insert(normals, num)
  end

  local meshFile = io.open(string.format("%s.mesh", meshName), 'w')

  writeTable(meshFile, vertices)
  writeTable(meshFile, faces)
  writeTable(meshFile, normals)
end
