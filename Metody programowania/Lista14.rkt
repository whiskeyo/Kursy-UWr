#lang racket

(require racklog)

(define %rodzic ; (%rodzic x y) oznacza, że iks jest rodzicem igreka
  (%rel ()
        [('elżbieta2 'karol)]
        [('elżbieta2 'anna)]
        [('elżbieta2 'andrzej)]
        [('elżbieta2 'edward)]
        [('karol     'william)]
        [('karol     'harry)]
        [('anna      'piotr)]
        [('anna      'zara)]
        [('andrzej   'beatrycze)]
        [('andrzej   'eugenia)]
        [('edward    'james)]
        [('edward    'louise)]
        [('william   'george)]
        [('william   'charlotte)]
        [('william   'louis)]
        [('harry     'archie)]
        [('piotr     'savannah)]
        [('piotr     'isla)]
        [('zara      'mia)]
        [('zara      'lena)]))

(define %rok-urodzenia
  (%rel ()
        [('elżbieta2 1926)]
        [('karol     1948)]
        [('anna      1950)]
        [('andrzej   1960)]
        [('edward    1964)]
        [('william   1982)]
        [('harry     1984)]
        [('piotr     1977)]
        [('zara      1981)]
        [('beatrycze 1988)]
        [('euagenia  1990)]
        [('james     2007)]
        [('louise    2003)]
        [('george    2013)]
        [('charlotte 2015)]
        [('louis     2018)]
        [('archie    2019)]
        [('savannah  2010)]
        [('isla      2012)]
        [('mia       2014)]
        [('lena      2018)]))

(define %plec
  (%rel ()
        [('elżbieta2 'k)]
        [('karol     'm)]
        [('anna      'k)]
        [('andrzej   'm)]
        [('edward    'm)]
        [('william   'm)]
        [('harry     'm)]
        [('piotr     'm)]
        [('zara      'k)]
        [('beatrycze 'k)]
        [('euagenia  'k)]
        [('james     'm)]
        [('louise    'k)]
        [('george    'm)]
        [('charlotte 'k)]
        [('louis     'm)]
        [('archie    'm)]
        [('savannah  'k)]
        [('isla      'k)]
        [('mia       'k)]
        [('lena      'k)]))

(define %spadl-z-konia
  (%rel ()
        [('anna)]))

; zadanie 1
(define %prababcia
  (%rel (x y q z)
        [(x z)
         (%rodzic x y) (%rodzic y q) (%rodzic q z)]))

(define %praprababcia
  (%rel (x y z)
        [(x z)
         (%rodzic x y) (%prababcia y z)]))

(define %kuzyn
  (%rel (x y z rx ry)
        [(x y)
         (%rodzic rx x)
         (%rodzic z rx)
         (%rodzic ry y)
         (%rodzic z ry)
         (%not (%= rx ry))]))

(define %prababcia-wiek-wnukow
  (%which (x) (%rok-urodzenia (cdar (%which (z) (%prababcia 'elżbieta2 z))) x)))

(define %pww
  (%rel (x y z)
        [(x y z)
         (%prababcia x y)
         (%rok-urodzenia y z)]))

(define %ww
  (%rel (x y z w)
        [(x y w)
         (%prababcia x y)
         (%rok-urodzenia y z)
         (%is w (- 2019 z))]))

;; zadanie 2

(define %rodzenstwo
  (%rel (x y z)
        [(x z)
         (%rodzic y x)
         (%rodzic y z)
         (%not (%= x z))]))

#;(define %starszy-wiek
  (%rel (x y wx wy)
        [(x y)
         (%is)]))

(define %starsze-rodzenstwo
  (%rel (x z px pz wz wx)
        [(x z)
         (%rodzenstwo x z)
         (%plec x px)
         (%plec z pz)
         (%rok-urodzenia x wx)
         (%rok-urodzenia z wz)
         (%if-then-else (%= px pz)
                        (%< wz wx)
                        (%= pz 'm))]))

(define %przodek
  (%rel (x y z)
        [(x y)
         (%rodzic x y)]
        [(x x)]
        [(x y)
         (%rodzic x z)
         (%przodek z y)]))

(define %wyprzedza
  (%rel (x y px py)
        [(x y) (%przodek x y)]
        ;[(x y) (%starsze-rodzenstwo x y)]
        [(x y) (%przodek py y)
               (%starsze-rodzenstwo py x)]))

(define %rev
  (%rel (x xs acc)
        [(null acc acc)]
        [((cons x xs) acc (cons x acc))
         (%rev xs acc)]))

(define (reverse xs)
  (define (iter xs acc)
    (if (null? xs)
        acc
        (iter (cdr xs) (cons (car xs) acc))))
  (iter xs null))

(define %append
  (%rel (x xs ys zs)
        [(null ys ys)]
        [((cons x xs) ys (cons x zs))
         (%append xs ys zs)]))