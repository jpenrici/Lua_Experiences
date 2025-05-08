-- matrix.lua

-- Data Structure
local Matrix2D = {
  Rows = 0,
  Columns = 0,
  Values = {}
}

local function split(str, delimiter)
  local result = {}
  for match in (str:gmatch("([^" .. delimiter .. "]+)")) do
    table.insert(result, match)
  end
  return result
end

local function validate(variable, expected_type)
  return type(variable) == expected_type
end

local function format(matrix2D)
  if not validate(matrix2D, 'table') then
    return false, "Invalid type for 'Matrix2D'!"
  end
  if not validate(matrix2D.Values, 'table') then
    return false, "Invalid type for 'matrix2D.Values'!"
  end  
  local output = "Matrix (" .. matrix2D.Rows .. " x " .. matrix2D.Columns .. "):\n"
  for i = 1, matrix2D.Rows do
    for j = 1, matrix2D.Columns do
      local index = (i - 1) * matrix2D.Columns + j
      output = output .. matrix2D.Values[index]
      if j == matrix2D.Columns then
        output = output .. "\n"
      else
        output = output .. " "
      end
    end
  end
  -- Return String
  return output
end

local function str_to_matrix2D(str, delimiter, rows, columns)
  if not validate(str, 'string') or not validate(delimiter, 'string') then
    return nil, "Invalid type for 'str' or 'delimiter'!"
  end

  if str == "" or delimiter == "" then
    return nil, "Empty 'str' or 'delimiter' parameters!"
  end

  if not validate(rows, 'number') or not validate(columns, 'number') then
    return nil, "Invalid type for 'rows' or 'columns'!"
  end

  if rows < 1 or columns < 1 then
    return nil, "Invalid array dimensions!"
  end
  
  local values = split(str, delimiter)
  local size = #values

  if size == 1 then
    return nil, "Check if the delimiter is correct!"
  end

  if (rows * columns) - size ~= 0 then
    return nil, "Error setting array size! Expected " .. (rows * columns) .. " (" .. rows .. "x" .. columns .. ") elements and found " .. size .. "."
  end

  -- Store data
  Matrix2D.Rows = rows
  Matrix2D.Columns = columns
  Matrix2D.Values = values

  -- Returns Matrix2D table and message
  return Matrix2D, "Conversion was successful!"
end

-- For integration with C++
function Convert(str, delimiter, rows, columns)
  local matrix, message = str_to_matrix2D(str, delimiter, rows, columns)
  if matrix == nil then
    -- Return String
    return message
  end
  -- Return String
  return format(matrix)
end

-- Test in Lua interpreter. 
local function test()
  print("Test ...")
  local str = "0;1;2;3;4;5;6;7;8;9;10;11"
  print(Convert(str, ";", 1, 12))
  print(Convert(str, ";", 2, 6))
  print(Convert(str, ";", 3, 4))
  print(Convert(str, ";", 4, 3))
  print(Convert(str, ",", 4, 3))    -- Check if the delimiter is correct!
  print(Convert(str, ";", -1, -1))  -- Invalid array dimensions!
  print(Convert("", ";", 4, 3))     -- Empty 'str' or 'delimiter' parameters!
end

-- Leave commented if used in C++ integration.
-- test()
