#lang racket

; Zadanie 2
(/ (+ 5 4 (- 2 (- 3 (+ 6 (/ 4 5)))))
   (* 3 (- 6 2) (- 2 7)))

; Zadanie 4
(define (square x)
  (* x x))

(define (square-sum x y)
  (+ (square x) (square y)))

(define (find-min a b c)
  (cond
    [(and (>= a b) (>= b c)) c]   ; c min
    [(and (>= a b) (>= c b)) b]   ; b min
    [(and (>= b a) (>= c a)) a])) ; a min  

(define (find-biggest a b c)
  (cond
    [(and (>= a b) (>= b c))  (square-sum a b)]   ; c min
    [(and (>= a b) (>= c b))  (square-sum a c)]   ; b min
    [(and (>= b a) (>= c a))  (square-sum b c)])) ; a min

(define (find-biggest-v2 a b c)
  (- (+ (square a)
        (square b)
        (square c))
     (square (find-min a b c))))

; Zadanie 5
; Formuła oblicza wartość działania a+|b|.

; Zadanie 6
; (and #f (/ 2 0))
; (or #t (/ 2 0))

; Zadanie 7
(define (p) (p))

(define (test x y)
  (if (= x 0)
      0
      y))

; Dla wywołania procedury (test 0 (p)):
; Dla kolejności stosowanej (gorliwej) program zapętli się, będzie wywoływane (test 0 (p)) w nieskończoność.
; Dla kolejności normalnej (leniwej) program najpierw wywoła (test 0 (p)), a następnie
; sprawdzi warunek (if (= 0 0)), zwróci wartość true i skończy swoje działanie.

; Zadanie 8
(define (power-close-to b n)
  (define (temp e)
    (if (> (expt b e) n)
        e
        (temp (+ 1 e))))
  (temp 0))