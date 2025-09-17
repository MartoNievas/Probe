

--Ejercicio 2 

--Definir la funcion Curry 

curry :: ((a,b) -> c) -> a -> b -> c
curry fUncurry x y = fUncurry (x,y)

--Definir uncurry 
uncurry':: (a -> b -> c) -> (a,b) -> c
uncurry' fCurry (x,y) = fCurry x y

--Ejercicio 3

--definir sum, elem (++), filter, map 

sum' :: (Num a) => [a] -> a
sum' = foldr (+) 0 

elem' :: (Eq a) => a -> [a] -> Bool
elem' value = foldr (\x rec -> x == value || rec) False

filter' :: (a -> Bool) -> [a] -> [a]
filter' p = foldr (\x rec -> if p x then x : rec else rec) []

map' :: (a -> b) -> [a] -> [b]
map' func = foldr (\x rec -> func x : rec) []

--definir mejor segun

mejorSegun :: (a -> a -> Bool) -> [a] -> a
mejorSegun comp = foldr1 (\x y -> if comp x y then x else y)

--definir sumaParcial

sumasParcialesFoldr :: (Num a) => [a] -> [a]
sumasParcialesFoldr xs = foldr (\x acc -> 
    case acc of
        [] -> [x]
        (y:ys) -> (x+y) : (y:ys)
    ) [] xs

--definir sumaAlt 

sumaAlt :: [Int] -> Int
sumaAlt = foldr (\x rec -> x - rec) 0

-- sumaAltInv
--conviene usar foldl porque es plegado hacia la izquierda
--Si invierto la lista sumaAlt = sumaAltInv
sumaAltInv :: [Int] -> Int
sumaAltInv = foldl (\rec x -> x - rec) 0 



--Ejercicio 5

--definir partes

partes :: [a] -> [[a]]
partes = foldr (\x rec ->  map (x:) rec ++ rec) [[]] 

--definir prefijos 

prefijos :: [a] -> [[a]]
prefijos = foldr (\x rec -> [x] : map(x:) rec) [[]] 

--definir sublistas
sublistas :: (Eq a) => [a] -> [[a]]
sublistas [] = [[]]
sublistas (x:xs) =
    quitasRepetidos(
   sublistas xs ++ prefijos (x:xs))


quitasRepetidos :: (Eq a) => [a] -> [a]
quitasRepetidos [] = []
quitasRepetidos (x:xs) = if elem x xs then quitasRepetidos xs else x : quitasRepetidos xs

--ejercicio 6 

recr :: (a -> [a] -> b -> b) -> b -> [a] -> b
recr _ z [] = z
recr f z (x : xs) = f x xs (recr f z xs)

sacarUna :: Eq a => a -> [a] -> [a]
sacarUna v = recr (\x xs rec -> if v == x then xs else x :rec) []

insertarOrdenado :: Ord a => a -> [a] -> [a]
insertarOrdenado v = foldr (\x rec -> if v <= x then v : x : rec else x : rec) []

mapPares :: (a -> b -> c) -> [(a,b)] -> [c]
mapPares fcurry pares = map (uncurry fcurry) pares

--Ejercicio 11 

data Polinomio a = X | Cte a | Suma (Polinomio a) (Polinomio a)| Prod (Polinomio a) (Polinomio a)

foldPol :: b -> (a -> b) -> (b -> b -> b) -> (b -> b -> b) -> Polinomio a -> b
foldPol z fCte fSum fProd pol = case pol of
    X -> z
    Cte x -> fCte x
    Suma x y -> fSum (rec x) (rec y)
    Prod x y -> fProd (rec x) (rec y)
    where 
        rec = foldPol z fCte fSum fProd

evaluar :: Num a => a -> Polinomio a -> a
evaluar num = foldPol num (\x -> x) (+) (*)

--Ejercicio 12

data AB a = Nil | Bin (AB a) a (AB a)

foldAB :: b -> (b -> a -> b -> b) -> AB a -> b
foldAB z fBin a = case a of
    Nil -> z
    Bin i r d -> fBin (rec i) r (rec d)
    where 
        rec = foldAB z fBin 

recAB :: b -> (AB a -> a -> AB a -> b -> b -> b) -> AB a -> b
recAB z fBin a = case a of
    Nil -> z
    Bin i r d -> fBin i r d (rec i) (rec d)
    where
        rec = recAB z fBin


--definir funciones 

esNil :: AB a -> Bool
esNil = foldAB True (\_ _ _ -> False)

altura :: AB a -> Int
altura = foldAB 0 (\reci _ recd -> 1 + max reci recd)

cantNodos :: AB a -> Int
cantNodos = foldAB 0 (\reci _ recd -> 1 + reci + recd)

mejorSegun2 :: (a -> a -> Bool) -> AB a -> a
mejorSegun2 f (Bin izq r der) = foldAB r (\recIzq r recDer -> if f r recIzq && f r recDer then r else if f recIzq recDer then recIzq else recDer) (Bin izq r der)


estaOrdenada :: (Ord a) => [a] -> Bool
estaOrdenada = recr (\x xs rec -> case xs of 
    [] -> True 
    (y:_) -> x <= y && rec) True

inorder :: AB a -> [a]
inorder = foldAB [] (\reci r recd -> reci ++ [r] ++ recd) 


esABB :: (Ord a) => AB a -> Bool
esABB tree = estaOrdenada(inorder tree)

mismaEstructura :: AB a -> AB b -> Bool
mismaEstructura = foldAB (\treeb -> case treeb of 
    Nil -> True
    _ -> False ) 
    (\reci _ recd b -> case b of 
        Nil -> False
        Bin i _ d -> reci i && recd d)


ramas :: AB a -> [[a]]
ramas = foldAB [] (\reci x recd -> if (null reci) && (null recd) then [[x]] else map (x :) (reci ++ recd))
--Ejercicio 14

data AIH a = Hoja a | Bin1 (AIH a ) (AIH a)

foldAIH :: (a -> b) -> (b -> b -> b) -> AIH a -> b
foldAIH fHoja fBin tree = case tree of 
    Hoja x -> fHoja x
    Bin1 i d -> fBin (rec i) (rec d)
    where
        rec = foldAIH fHoja fBin

alturaAIH :: AIH a -> Integer
alturaAIH = foldAIH (const 1) (\recI recD -> max recI recD)

tamano :: AIH a -> Integer
tamano = foldAIH (const 1) (\recI recD -> recI + recD)

--Ejercicio 15 rose tree

data RoseTree a= Rose a [RoseTree a]

foldRoseTree :: (a -> [b] -> b) -> RoseTree a -> b
foldRoseTree fRose (Rose v hijos) = fRose v (map (foldRoseTree fRose) hijos)

hojas :: RoseTree a -> [a]
hojas = foldRoseTree (\v recHijos -> v : concat recHijos)

distancia :: RoseTree a -> [(a,Int)]
distancia = foldRoseTree (\_ recHijos -> map(\(tree,alt) -> (tree,alt + 1)) (concat recHijos))


altura1 :: RoseTree a -> Int
altura1= foldRoseTree (\_ recHijos -> if null recHijos then 0 else 1 + maximum recHijos)

--Ejercicio 16

data HashSet a = Hash (a -> Integer) (Integer -> [a])

vacio :: (a -> Integer) -> HashSet a
vacio f = Hash f (\_ -> [])

perteneceHash :: Eq a => a -> HashSet a -> Bool
perteneceHash x (Hash f g) = elem x (g (f x)) 

agregar :: Eq a => a -> HashSet a -> HashSet a
agregar x (Hash hashFunc conjFunc) = 
    let h = hashFunc x
        currentList = conjFunc h
        newList = if elem x currentList 
            then currentList 
            else x : currentList
        newConjFunc = \key -> if key == h then newList else conjFunc key
    in Hash hashFunc newConjFunc


--Generacion infinita 

paresDeNat::[(Int,Int)] = [(x,y) | aux <- [0..], x <- [0..aux], let y = aux - x]


--No funciona porque hay 3 generadores infinitos, y ademas como c es la solucion este debe ser el generador infinito
pitagóricas :: [(Integer, Integer, Integer)]
pitagóricas = [(a, b, c) | a <- [1..], b <-[1..], c <- [1..], a^2 + b^2 == c^2]

pitagóricas1 :: [(Integer, Integer, Integer)]
pitagóricas1 = [(a, b, c) | c <- [1..], b <-[1..c], a <- [1..b], a*a + b*b == c*c]

listasQueSuman :: Int -> [[Int]]
listasQueSuman 0 = [[]]
listasQueSuman n = [x:xs | x <- [1..n],xs <- listasQueSuman (n-x) ]


recrConFoldr :: (a -> [a] -> b -> b) -> b -> [a] -> b 
recrConFoldr f z xs = 
    snd (foldr step (\_ -> ([],z)) xs xs) 
    where 
        step x r (_:ys) = 
            let (cola, rec) = r ys 
            in (x:cola, f x ys rec)
        step _ _ [] = error "no deberia pasar"

recrConFoldr2 :: (a -> [a] -> b -> b) -> b -> [a] -> b 
recrConFoldr2 f z xs = foldr (\(x:xs) r -> f x xs r ) z (init (tails xs))

tails :: [a] -> [[a]]
tails = foldr step [[]]
  where
    step x acc = (x : head acc) : acc
