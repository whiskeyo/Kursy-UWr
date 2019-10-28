#lang racket

; Procedury z ćwiczeń, wykładu oraz pomocnicze:
(define (average a b)
  (/ (+ a b) 2))

(define (compose f g)
  (lambda (x) (f (g x))))

(define (identity x) x)

(define (repeated p n)
  (if (= n 0)
      identity
      (compose p (repeated p (- n 1)))))

(define tolerance 0.00001)

(define (good-enough? a b)
  (> tolerance (abs (- a b))))

(define (fixed-point func guess)
  (define (check guess)
    (let ([next (func guess)])
      (if (good-enough? guess next)
          next
          (check next))))
  (check guess))

(define (average-damp f)
  (lambda (x) (average x (f x))))

; Procedury do rozwiązania zadania:

(define (nth-power x n)
  (if (= n 0)
      1
      (* x (nth-power x (- n 1)))))

(define (nth-root x n)
  (if (= n 1)
      x
      (let ([root (lambda (y) (/ x (nth-power y (- n 1))))]
            [composition (repeated average-damp (floor (log n 2)))])
        (fixed-point (composition root) 1.0))))

; Testowanie ilości wymaganych tłumień:

(define (composition n)
  (repeated average-damp n)) ; n-krotne tlumienie

(define (test-procedure x n comp)
  (fixed-point ((repeated average-damp comp) (lambda (y) (/ x (nth-power y (- n 1))))) 1.0))

(test-procedure 27 3 1)
(test-procedure 64 6 1)
(test-procedure 60 10 1)
(test-procedure 8 3 1)
(test-procedure 32 5 2)
(test-procedure 64 8 3)
(test-procedure 64 16 4)
(test-procedure 64 20 4)
(test-procedure 64 20 5)

; Z powyższych testów wynika, że wymagana liczba tłumień
; wynosi mniej niż log2(n), tzn. (floor (log n 2)).
"Działanie procedury nth-root dla przykładowych wartości:"
(nth-root 512 9)
(nth-root 216 3)
(nth-root 65536 16)
(nth-root 2401 4)
  
  