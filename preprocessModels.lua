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

  local location = {}
  local rotation = {}
  local scale = {}

  local vertices = {}
  local positions = {}
  local normals = {}
  local vertexIndices = {}

  local fbx = io.open(paths[i], 'r'):read('*a')

  local objectsLocation = string.match(fbx, 'Objects:()')

  local meshLocation
  meshName, meshLocation = string.match(fbx, 'Model:%s"Model::(%w+)",%s"Mesh()"', objectsLocation)

  local locationLocation = string.match(fbx, 'Property: "Lcl Translation", "Lcl Translation", "A%+",()', meshLocation)
  offset = locationLocation
  for i=1,3 do
    num, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    table.insert(location, num)
  end

  local euler = {}
  local rotationLocation = string.match(fbx, 'Property: "Lcl Rotation", "Lcl Rotation", "A%+",()', meshLocation)
  offset = rotationLocation
  for i=1,3 do
    num, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    num = num * math.pi/180
    table.insert(euler, num)
  end

  local c1 = math.cos(euler[1]/2)
  local c2 = math.cos(euler[2]/2)
  local c3 = math.cos(euler[3]/2)
  local s1 = math.sin(euler[1]/2)
  local s2 = math.sin(euler[2]/2)
  local s3 = math.sin(euler[3]/2)
  table.insert(rotation, c1*c2*c3 - s1*s2*s3)
  table.insert(rotation, s1*c2*c3 + c1*s2*s3)
  table.insert(rotation, c1*c2*s3 + s1*s2*c3)
  table.insert(rotation, c1*s2*c3 - s1*c2*s3)

  local scaleLocation = string.match(fbx, 'Property: "Lcl Scaling", "Lcl Scaling", "A%+",()', meshLocation)
  offset = scaleLocation
  for i=1,3 do
    num, offset = string.match(fbx, '(%-?%d+%.%d+),?()', offset)
    table.insert(scale, num)
  end

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
    local mag = math.sqrt(num1*num1 + num2*num2 + num3*num3)
    num1,num2,num3 = num1/mag, num2/mag, num3/mag
    table.insert(normals, num1)
    table.insert(normals, num2)
    table.insert(normals, num3)
    -- local x,y,z = vertexIndices[vi]*3+1, vertexIndices[vi]*3+2, vertexIndices[vi]*3+3
    -- normals[x] = (normals[x] or 0) + num1
    -- normals[y] = (normals[y] or 0) + num2
    -- normals[z] = (normals[z] or 0) + num3
  -- end
  -- for n=1,#normals,3 do
  --   local x,y,z = normals[n],normals[n+1],normals[n+2]
  --   local m = math.sqrt(x*x + y*y + z*z)
  --   normals[n],normals[n+1],normals[n+2] = x/m, y/m, z/m
  end

  for v=1,#positions,3 do
    table.insert(vertices, positions[v])
    table.insert(vertices, positions[v+1])
    table.insert(vertices, positions[v+2])
    table.insert(vertices, normals[v])
    table.insert(vertices, normals[v+1])
    table.insert(vertices, normals[v+2])
  end

  local meshFile = io.open(string.format("models/%s.model", meshName), 'w')
  writeTable(meshFile, location, "%f")
  writeTable(meshFile, rotation, "%f")
  writeTable(meshFile, scale, "%f")
  meshFile:write(#positions/3, ' ')
  writeTable(meshFile, vertices, "%f")
  meshFile:write(#vertexIndices, ' ')
  writeTable(meshFile, vertexIndices, "%d")
end
