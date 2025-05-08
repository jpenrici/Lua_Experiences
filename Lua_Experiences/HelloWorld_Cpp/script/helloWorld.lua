-- helloWorld.lua

-- Simple function for testing.
function Calc(num1, num2, operador)
  if operador == "+" then
    return num1 + num2
  elseif operador == "-" then
    return num1 - num2
  elseif operador == "*" then
    return num1 * num2
  elseif operador == "/" then
    if num2 == 0 then
      return "Error! Division by zero!"
    else
      return num1 / num2
    end
  else
    return "Invalid operator!"
  end
end
