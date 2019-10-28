#lang racket

(struct const      (value)      #:transparent)
(struct variable   (x)          #:transparent)
(struct add        (left right) #:transparent)
(struct mul        (left right) #:transparent)
(struct derivative (expr)       #:transparent)

(define (deriv f)
  (match f
    [(const n)    (const 0)]
    [(variable x) (const 1)]
    [(add l r)    (add (deriv l) (deriv r))]
    [(mul l r)    (add (mul (deriv l) r)
                       (mul l (deriv r)))]))

(define (expr? e)
  (match e
    [(variable x)     (symbol? x)]
    [(const n)        (number? n)]
    [(add l r)        (and (expr? l)
                           (expr? r))]
    [(mul l r)        (and (expr? l)
                           (expr? r))]
    [(derivative exp) (expr? exp)]
    [_                false]))

(define (subst x v e)
  (match e
    [(const n)        (const n)]
    [(variable y)     (if (eq? x y)
                          (const v)
                          (variable y))]
    [(add l r)        (add (subst x v l)
                           (subst x v r))]
    [(mul l r)        (mul (subst x v l)
                           (subst x v r))]
    [(derivative exp) (derivative (subst x v (deriv exp)))]))

(define (eval e)
  (match e
    [(const n)        n]
    [(add l r)        (+ (eval l) (eval r))]
    [(mul l r)        (* (eval l) (eval r))]
    [(derivative exp) (eval (deriv exp))]
    [(variable n)     (error n "cannot reference an identifier before its definition!")]))

; Zestaw testów sprawdzających działanie powyższych procedur

(require rackunit)
(require rackunit/text-ui)

(define x (variable 'x))
(define e1 (add (const 2) x))
(define e2 (add (mul (const 2) x)
                (deriv (add (mul x x) x))))

(define abstract-syntax-tests
  (test-suite
   "Tests of given equations"
   (check-equal? (eval (subst 'x 5 e1))
                 7
                 "2 + x works fine!")
   (check-equal? (eval (subst 'x 3 e2))
                 13
                 "2x + d(x^2 + x) works fine!")))

(run-tests abstract-syntax-tests)

