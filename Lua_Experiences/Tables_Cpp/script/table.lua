-- table.lua

-- Data structure
local records = {
--  Name = "",
--  Links = {}, -- favorite links
--  Date = ""   -- dd-mm-yyyy format
}

--[[
Adds a new record to the 'records' table.

Parameters:
  name: A string containing the name of the record. (Required)
  links: A table (array) of strings containing the favorite links. (Required)
  date: A string containing the date in dd-mm-yyyy format. (Required)

Returns:
  true if the record was added successfully, false otherwise.
--]]
function AddRecord(name, links, date)
  -- Check 'Name'
  if type(name) ~= "string" then
    return false, "Error: Inválid type for 'Name'!"
  end
  if name == "" then
    return false, "Error: 'Name' is empty!"
  end

  -- Check 'Links'
  if type(links) ~= "table" then
    return false, "Error: Inválid type for 'Links'!"
  else
    for _, link in ipairs(links) do
      if type(link) ~= "string" then
        return false, "Error: all items in 'Links' must be strings!"
      end      
    end
  end

  -- Check 'Date'
  if type(date) ~= "string" then
    return false, "Error: Inválid type for 'Date'!"
  end

  local day, month, year = date:match("^(%d%d)%-(%d%d)%-(%d%d%d%d)$")
  day, month, year = tonumber(day), tonumber(month), tonumber(year)
  if not (day and month and year) then
    return false, "Error: 'Date' format is invalid."
  end

  local timestamp = os.time{year = year, month = month, day = day}
  local resolved = os.date("*t", timestamp)
  if resolved.day ~= day or resolved.month ~= month or resolved.year ~= year then
    return false, "Error: 'Date' is not a valid calendar date."
  end

  -- Data Structure
  local record = {
    Name = name,
    Links = links,
    Date = date
  }

  -- Insert
  table.insert(records, record)

  return true, "Inserted successfully!"
end

--[[
Returns a specific record from the 'records' table based on the index.

Parameters:
  index: An integer representing the index of the record to be returned.

Returns:
  The record table at the specified index, or nil if the index is invalid.
--]]
function GetRecord(index)
  if index < 1 or index > #records then
    return nil
  end
  return records[index]
end

--[[
Generates a string representation of all records in the 'records' table.

Returns:
  A formatted string containing the details of each record.
--]]
function Summary()
  local output = "{\n"
  for i = 1, #records do
    local rec = GetRecord(i)
    if rec then
      output = output .. "[" .. i .. "]\n"
      output = output .. "  Name: " .. rec.Name .. "\n"
      output = output .. "  Links: {\n    " .. table.concat(rec.Links, "\n    ") .. "\n  }\n"
      output = output .. "  Date: " .. rec.Date .. "\n"
    end
  end
  output = output .. "}"
  return output
end

-- Test in Lua interpreter.
local function test()
  print("Test ...")
  print(AddRecord("01", {"link", "link"}, "01-05-2025")) -- OK
  print(AddRecord("02", {"link", "link"}, "01-01-2025")) -- OK
  print(AddRecord("03", "is not table", "02-05-2025"))   -- Error 'Links'
  print(AddRecord("04", {"link", 10}, "01-01-2025"))     -- Error 'Links'   
  print(AddRecord("05", {}, "2025-05-03"))               -- Error 'Date'
  print(AddRecord("06", {"link"}, "05/05/2025"))         -- Error 'Date'
  print(AddRecord("07", {"link"}, "05-05-25"))           -- Error 'Date'
  print(AddRecord("08", {"link"}, "01-13-2025"))         -- Error 'Date'
  print(AddRecord("09", {"link"}, "31-02-2025"))         -- Error 'Date'
  print(Summary()) -- Call the sumary function to print its output
end

-- Leave commented if used in C++ integration.
-- test()