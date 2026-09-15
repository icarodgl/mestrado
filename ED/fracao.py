from dataclasses import dataclass

def mdc(n, m):
    resto = n % m
    while resto != 0:
        n = m
        m = resto
        resto = n % m
    return m


@dataclass
class Fracao:
    

    def __init__(self, topo = 0, base = 1):
        
        
        if base == 0:
            raise ValueError("Denominador igual a zero!\n")
        self.num = topo
        self.den = base

    def __add__(self, f2 :'Fracao') -> 'Fracao':
        
        xnum = self.num * f2.den + self.den * f2.num
        xden = self.den * f2.den
        xmdc = mdc(xnum, xden)
        return Fracao(xnum // xmdc, xden // xmdc)
    
    def __sub__(self, f2 :'Fracao') -> 'Fracao':
        if self.den == f2.den:
            return Fracao(self.num - f2.num, self.den)
        else:
            return Fracao(self.num - f2.num, mdc(self.den, f2.den))

    def __mul__(self, f2 :'Fracao') -> 'Fracao':
        n = self.num * f2.num
        d = self.den * f2.den
        m = mdc(n,d)
        return Fracao(n//m, d//n)

    
    def __truediv__(self, f2 :'Fracao') -> 'Fracao':
        n = self.num * f2.den
        d = self.den * f2.num
        m = mdc(n,d)
        return Fracao(n//m, d//m)
    
    def __eq__(self, f2 :'Fracao') -> bool:
        n = self.num * f2.den
        d = self.den * f2.num
        return n == d
    
    def __ne__(self, f2 :'Fracao') -> bool:
        n = self.num * f2.den
        d = self.den * f2.num
        return n != d
    
    def __lt__(self, f2 :'Fracao') -> bool:
        # >
        s = self.num * f2.den
        f = self.den * f2.num
        return s > f

    def __gt__(self, f2 :'Fracao') -> bool:
        # <
        s = self.num * f2.den
        f = self.den * f2.num
        return s < f
    
    def __le__(self, f2 :'Fracao') -> bool:
        # <=
        s = self.num * f2.den
        f = self.den * f2.num
        return s <= f
    def __ge__(self, f2 :'Fracao') -> bool:
        # >=
        s = self.num * f2.den
        f = self.den * f2.num
        return s >= f

    def __str__(self):
        return str(self.num) + "/" + str(self.den)
    
x = Fracao(3, 4)
y = Fracao(5, 12)

print('+' , x + y ) 
print('-' , x - y ) 
print('/' , x / y ) 
print('*' , x * y ) 

print('==' , x   ==  y )
print('!=' , x   !=  y )
print('>' , x > y ) 
print('<' , x < y ) 
print('>=' , x >= y )
print('<=' , x <= y )



