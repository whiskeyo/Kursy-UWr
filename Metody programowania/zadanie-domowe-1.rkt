#lang racket
; Swoje rozwiązanie opierałem na kodzie z wykładu
; oraz tekście z pierwszego rozdziału SICP.

(define (cube-root x)

  (define (square x)
    (* x x))
  
  (define (cube x)
    (* x x x))

  (define (dist x y)
    (abs (- x y)))
  
  (define (average x y)
    (/ (+ x y) 2))
  
  (define (better-approx approx)
    (/ (+ (/ x
             (square approx))
          (* 2 approx))
       3))
  
  (define (good-enough? approx)
    (< (dist x (cube approx)) 0.0001))
  
  (define (iter approx)
    (if (good-enough? approx)
        approx
        (iter (better-approx approx))))
  
  (iter 1.0))

(cube-root 27)
(cube-root 125)
(cube-root 1000)
(cube-root 0.008)
(cube-root 0.343)
(cube-root 0)
(cube-root 1)
(cube-root -1)
(cube-root -343)
(cube-root -0.000125)