#lang racket

; Zadanie 1

; x - wolna

#;(let ([x 3])
    (+ x y))    ; x zwiazana, y wolna

#;(let ([x 1]
        [y (+ x 2)])
    (+ x y)) ; x zwiazany z [x 1], ale nie z [y (+ x 2)]
             ; y zwiazany z [y (+ x 2)]

#;(let ([x 1]
        (let [y (+ x 2)])
      (* x y))) ; x zwiazany z [x 1] i z [y (+ x 2)]
                ; y zwiazany z [y (+ x 2)]

#;(lambda (x y) 
    (* x y z)) ; x i y zwiazane, z wolne

#;(let ([x 1])
    (lambda (y z)
      (* x y z))) ; x, y, z zwiazane

; Zadanie 2 - zlozenie funkcji x-> f(g(x))

(define (square x)
  (* x x))

(define (inc x)
  (+ x 1))

(define (compose f g)
  (lambda (x)
    (f (g x))))

; Zadanie 3 - n-krotne zlozenie procedury p z sama soba
; (x->x) -> int -> (x->x)

(define (identity x) x)

(define (repeated p n)
  (if (= n 0)
      identity
      (compose p (repeated p (- n 1)))))
    
; Zadanie 4 - procedura product, analogiczna do sum
; (mnozenie, notacja duzego pi)

(define (product start end)
  (if (> start end)
      1.0
      (* (/ (* start (+ start 2))
            (square (+ start 1)))
         (product (+ start 2) end))))

(define (product-x val next start end)
  (if (> start end)
      1.0
      (* (val start)
         (product-x val next (next start) end))))

(define (count-pi x)
  (* 4
     (product-x (lambda (x)
                  (/ (* x (+ x 2))
                     (square (+ x 1))))
                (lambda (x)
                  (+ x 2))
                2
                x)))

(define (product-it val next start end result)
  (if (> start end)
      result
      (product-it val next (next start) end (* result start))))

; Zadanie 5 - procedura accumulate, combiner to procedura
; binarna, a null-value to element neutralny

(define (accumulate combiner null-value term a next b)
  (if (> a b)
      null-value
      (combiner (term a) (accumulate combiner null-value term (next a) next b))))

(define (accumulate-iter combiner null-value term a next b acc)
  (define (iter a acc)
    (if (> a b)
        acc
        (iter (next a) (combiner acc (term a)))))
  (iter a null-value))

; Zadanie 6 - procedura cont-frac obliczajaca k-ty wyraz
; ciagu skonczonych rozwiniec ulamka lancuchowego

(define (cont-frac num den k)
  (define (frac i)
    (if (< i k)
        (/ (num i) (+ (den i) (frac (+ i 1))))
        (/ (num i) (den i))))
  (frac 1))

(define (cont-frac-iter num den k)
  (define (iter k result)
    (if (= k 0)
        result
        (iter (- k 1) (/ (num k)
                         (+ (den k) result)))))
  (iter k 0))

; (cont-frac (lambda (i) 1.0) (lambda (i) 1.0) 100)
; wartosc 1/fi (fi - zloty podzial)

; Zadanie 7 - liczba pi jako ulamek lancuchowy,
; nalezy wywolac (count-pi-v2 14) dla dokladnosci
; 4 miejsc po przecinku

(define (count-pi-v2 approximation-level)
  (define (num k)
    (if (= k 1)
        1.0
        (square (- (* 2 k) 1))))
  (define (den k)
    6)
  (+ 3 (cont-frac num den approximation-level)))

; Zadanie 8 - obliczenie arctan x

(define (arctan x k)
  (define (num k)
    (if (= k 1)
        x
        (square (* (- k 1) x))))
  (define (den k)
    (- (* 2 k) 1))
  (cont-frac num den k))

; Zadanie 9 - N_i oraz D_i sa ciagami stalymi,
; tworzymy procedure obliczajaca N/(D+B)

(define (build n d b)
  (/ n (+ d b)))

(define (repeated-build k n d b)
    ((repeated (lambda (b) (build n d b)) k) b))


