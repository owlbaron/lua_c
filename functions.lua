function soma(numb1, numb2)
    return numb1 + numb2
end

function subtracao(numb1, numb2)
    return numb1 - numb2
end

function multiplicacao(numb1, numb2)
    return numb1 * numb2
end

function divisao(numb1, numb2)
    return numb1 * numb2
end

function potenciacao(base, exponent)
  return base ^ exponent
end

function fatorial(n)
  if n == 0 then
		return 1
   end

    return n * factorial(n - 1)
end

function bhaskara(a, b, c)
  sqrt = ((b ^ 2) - (4 * a * c)) ^ (1/2)

  x1 = (-b + sqrt) / (2 * a)
  x2 = (-b - sqrt) / (2 * a)

  return x1, x2
end