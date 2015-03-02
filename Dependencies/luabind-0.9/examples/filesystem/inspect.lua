-- base class for all inspectors
-- every derived class must implement inspect()

class 'inspector'

  function inspector:__init(name)
    self.name = name
    self.warnings = {}
  end

  function inspector:warning(path, str)
    table.insert( self.warnings, { path, str } )
  end

  function inspector:report()
    local output = function(_,x)
        local name = x[1]:string()
        print(name .. ": " .. x[2])
    end

    local cmp = function(a,b)
        return a[1]:string() < b[1]:string()
    end

    local violations

    if table.getn(self.warnings) ~= 0 then
      violations = table.getn(self.warnings) .. " violations"
    else
      violations = "no violations"
    end

    print("\n-- " .. self.name .. " [" .. violations .. "]\n")

    table.sort(self.warnings, cmp)
    table.foreach(self.warnings, output)
  end

  inspector.inspect = nil

-- checks filename length
class 'filename_length' (inspector)

  function filename_length:__init(n)
    super("filename length (" .. n .. " characters)")
    self.maxlen = n
  end

  function filename_length:inspect(path)
    local n = string.len(path:leaf())
      if n > self.maxlen then
        self:warning(path, n .. " characters in filename")
      end
  end

-- checks that the filename is all lowercase
class 'filename_case' (inspector)

  function filename_case:__init()
    super("filename case")
  end

  function filename_case:inspect(path)
    if string.lower(path:leaf()) ~= path:leaf() then
      self:warning(path, "uppercase letters")
    end
  end

-- checks that the file doesn't contain tabs
class 'tab_inspector' (inspector)

  function tab_inspector:__init()
    super("tab inspector")
  end

  function tab_inspector:inspect(path)
    if has_endings(path:leaf(), ".hpp", ".cpp") then
      for line in io.lines(path:string()) do
        if string.find(line, '\t') ~= nil then
          self:warning(path, "tabs in file")
          return
        end
      end
    end
  end

-- checks that the file doesn't contain too long lines
class 'line_length_inspector' (inspector)

  function line_length_inspector:__init(n)
    super("line length inspector (" .. n .. " characters)")
    self.maxlen = n
  end

  function line_length_inspector:inspect(path)
    if has_endings(path:leaf(), ".hpp", ".cpp") then
      for line in io.lines(path:string()) do
        if string.len(line) > self.maxlen then
          self:warning(path, "lines too long " .. string.len(line))
          return
        end
      end
    end
  end

-- checks for unmatched #define/#undef pairs
class 'define_inspector' (inspector)

  function define_inspector:__init()
    super("define inspector")
  end

  function define_inspector:inspect(path)
    if has_endings(path:leaf(), ".hpp") then

      local defs = {}

      for line in io.lines(path:string()) do
        local pos, _, def = string.find(line, "#%s*define%s+([%w_]+)")
        if pos ~= nil then defs[def] = true end
        local pos, _, def = string.find(line, "#%s*undef%s+([%w_]+)")
        if pos ~= nil then defs[def] = nil end
      end

      table.foreach(defs, function(def)
              self:warning(path, def)
            end)
    end
  end

-- helper functions

function file_ending(name)
  local pos = string.find(name, "%.")
  if pos == nil then return ""
  else
    return string.sub(name, pos)
  end
end

function has_endings(name, ...)
  local ending = file_ending(name)
  for _,i in arg do
    if ending == i then return true end
  end
  return false
end

function recurse_dir(path)
  for i in path.contents do
    if i:is_directory() then recurse_dir(i)
    else 
      table.foreach(inspectors, function(_,x)
              x:inspect(i)
            end)
      number_of_files = number_of_files + 1
    end
  end
end

-- main

inspectors = { filename_length(31), filename_case(), 
               tab_inspector(), line_length_inspector(79),
               define_inspector() }
number_of_files = 0

if args.n >= 3 then root = filesystem.path(args[3])
else root = filesystem.initial_path() end

print("inspecting '" .. root:string() .. "' ...")

recurse_dir(root)

print(" ** " .. number_of_files .. " files was inspected")

table.foreach(inspectors, function(_,i)
        i:report()
      end)

