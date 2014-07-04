
a = factorial(41)
b = factorial(42)

print('a = fac(41) = ' .. tostring(a))
print('b = fac(42) = ' .. tostring(b))
print('b / a = ' .. tostring(b / a))

c = a * 42

d = sqrt(cl_R(10))

print('d = sqrt(10) = ' .. tostring(d))

if c == b then print('equality operator test: ok') else print('equality operator test: failed') end

-- prints
-- a = fac(41) = 33452526613163807108170062053440751665152000000000
-- b = fac(42) = 1405006117752879898543142606244511569936384000000000
-- b / a = 42
-- d = sqrt(10) = 3.1622777
