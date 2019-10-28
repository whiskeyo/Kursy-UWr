#lang racket

; Zadanie 1 - wektory na płaszczyźnie

; Zdefiniujmy najpierw punkt (konstruktor, predykat oraz selektory):
(define (make-point x y)
  (cons x y))

(define (point? x)
  (and (cons? x)
       (number? (car x))
       (number? (cdr x))))

(define (point-x x)
  (car x))

(define (point-y x)
  (cdr x))

; A następnie wektor:
(define (make-vect p1 p2)
  (cons p1 p2))

(define (vect? x)
  (and (cons? x)
       (point? (car x))
       (point? (cdr x))))

(define (vect-begin x)
  (car x))

(define (vect-end x)
  (cdr x))

; Wyświetlanie punktu i wektora:
(define (display-point p)
  (display "(")
  (display (point-x p))
  (display ", ")
  (display (point-y p))
  (display ")") )

(define (display-vect v)
  (display "[")
  (display-point (vect-begin v))
  (display ", ")
  (display-point (vect-end v))
  (display "]"))

; Oraz procedury (powinno być ich więcej ale to waste of time):
(define (vect-length x)
  (let ([x1 (point-x (vect-begin x))]
        [x2 (point-x (vect-end x))]
        [y1 (point-y (vect-begin x))]
        [y2 (point-y (vect-end x))])
    (sqrt (+ (sqr (+ x1 x2)) (sqr (+ y1 y2))))))

; Zadanie 2 - bardzo podobne do 1. dlatego pomijamy

; Zadanie 3 - funkcja reverse odwracajaca kolejnosc listy
; (0 1 2 3) -> (1 2 3 0) -> (2 3 1 0) -> (3 2 1 0)

(define (reverse xs)
  (if (null? xs)
      null
      (append (reverse (rest xs)) (list (first xs)))))

(define (reverse-iter xs)
  (define (iter xs acc)
    (if (null? xs)
        acc
        (iter (cdr xs) (cons (car xs) acc))))
  (iter xs null))

; Zadanie 4 - procedura insert wstawiajaca do posortowanej
; (rosnaco) listy element n tak, aby lista pozostala posortowana

(define (insert xs n)
  (if (> n (car xs))
      (append (list (car xs)) (insert (cdr xs) n))
      (append (list n) xs)))

; Zadanie 5 - dowod
; Z definicji append wynikaja dwa "lematy":
; (1) (append null ys) = ys
; (2) (append (cons x xs) ys) = (cons x (append xs ys))

; Zadanie 6 - zbior potegowy P(A) (zbiorem A jest lista)
; wiec wypisujemy wszystkie podzbiory zbioru A

(define (powerset A)
  (if (null? A)
      '(())
      (let ([elems (powerset (rest A))])
        (append (map (lambda (x) (cons (first A) x)) elems)
                elems))))

; Zadanie 7 - append przyjmujacy dowolnie duzo list jako argumenty 