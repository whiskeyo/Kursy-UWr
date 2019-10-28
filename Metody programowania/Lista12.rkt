#lang racket
;; Zadanie 1
(define (average z y)
  (/ (+ z y) 2))

(define (square x)
  (* x x))

(define (dist x y)
  (abs (- x y)))

(define (abs x)
  (if (< x 0)
      (- x)
      x))

(define/contract (sqrt x)
  (->i ([input (or/c zero? positive?)])
       [result (input) (and/c (or/c zero? positive?)
                              (lambda (x)
                                (< (abs (- input (square x))) 0.0001)))])
  ;; lokalne definicje
  ;; poprawianie przybliżenia
  (define (improve guess)
    (average guess (/ x guess)))
  ;; sprawdzanie czy przybliżenie jest wystarczająco dobre
  (define (good-enough? g)
    (< (dist x (square g))
       0.0001))
  ;; procedura iterująca poprawianie przybliżenia aż
  ;; osiągniemy wystarczająco dobre przybliżenie
  (define (iter guess)
    (if (good-enough? guess)
        guess
        (iter (improve guess))))
  
  (iter 1.0))


;; Zadanie 2
(define (check-list p? xs)
  (if (null? xs)
      #t
      (if (p? (car xs))
          (check-list p? (cdr xs))
          #f)))

(define/contract (filter p? xs)
  (and/c (parametric->/c [a] (-> (-> a boolean?) (listof a) (listof a)))
         (->i [(pred (-> any/c boolean?))
               (lst list?)]
              [result (pred lst) (and/c list? (lambda (x) (check-list pred x)))]))
  (if (null? xs)
      null
      (if (p? (car xs))
          (cons (car xs) (filter p? (cdr xs)))
          (filter p? (cdr xs)))))


;; Zadanie 3
(define-signature  monoid^
  ((contracted
    [elem?    (-> any/c boolean?)]
    [neutral  elem?]
    [oper     (-> elem? elem? elem?)])))

(define-unit ints@
  (import)
  (export monoid^)

  (define (elem? x)
    (integer? x))

  (define neutral 0)

  (define (oper e1 e2)
    (+ e1 e2)))

(define-unit lists@
  (import)
  (export monoid^)

  (define (elem? xs)
    (list? xs))

  (define neutral null)

  (define (oper e1 e2)
    (append e1 e2)))

;(define-values/invoke-unit/infer ints@)
;(define-values/invoke-unit/infer lists@)

;; Zadanie 4
(require quickcheck)
#;(quickcheck
 (property ([x arbitrary-integer])
           (==> (elem? x)
                (and (eq? x (oper neutral x))
                     (eq? x (oper x neutral))))))

#;(quickcheck
 (property ([x (arbitrary-list arbitrary-symbol)])
           (==> (elem? x)
                (and (equal? x (oper neutral x))
                     (equal? x (oper x neutral))))))

;; Zadanie 5
(define-signature set^
  ((contracted
    [elem?         (-> any/c boolean?)]
    [set?          (-> any/c boolean?)]
    [empty-set?    (-> set? boolean?)]
    [contains?     (-> any/c set? boolean?)]
    [empty-set     (and/c set? empty-set?)]
    [singleton-set (-> elem? set?)]
    [sum           (-> set? set? set?)]
    [intersection  (-> set? set? set?)]
    [set-len (-> set? integer?)])))

(define-unit sets@
  (import)
  (export set^)

  (define (elem? x)
    (integer? x))

  (define (set? xs)
  (if (null? xs)
      #t
      (if (integer? (car xs))
          (set? (cdr xs))
          #f)))

  (define (empty-set? x)
    (null? x))

  (define empty-set null)
  
  (define (contains? x s)
    (if (list? (member x s))
        #t
        #f))

  (define (singleton-set val)
    (list val))

  (define (sum s1 s2)
    (sort (remove-duplicates (append s1 s2)) >))

  (define (intersection s1 s2)
    (sort (remove-duplicates (filter (lambda (x) (contains? x s2)) s1)) >))

  (define set-len length))

(define-values/invoke-unit/infer sets@)

;; Zadanie 6
(quickcheck
 (property ([x arbitrary-integer])
           (not (contains? x empty-set))))

(quickcheck
 (property ([x arbitrary-integer])
           (and (= (set-len (singleton-set x)) 1)
                (contains? x (singleton-set x)))))

(quickcheck
 (property ([x (arbitrary-list arbitrary-integer)]
            [y (arbitrary-list arbitrary-integer)])
           (let
             ((z (sum x y)))
             (andmap (lambda (e) (or (contains? e x) (contains? e y))) z))))

(quickcheck
 (property ([x (arbitrary-list arbitrary-integer)]
            [y (arbitrary-list arbitrary-integer)])
           (let
             ((z (intersection x y)))
             (andmap (lambda (e) (and (contains? e x) (contains? e y))) z))))

(quickcheck
 (property ([x (arbitrary-list arbitrary-integer)]
            [y (arbitrary-list arbitrary-integer)]
            [z (arbitrary-list arbitrary-integer)])
           (equal? (sum x (intersection y z))
                   (intersection (sum x y) (sum x z)))))

(quickcheck
 (property ([x (arbitrary-list arbitrary-integer)]
            [y (arbitrary-list arbitrary-integer)]
            [z (arbitrary-list arbitrary-integer)])
           (equal? (intersection x (sum y z))
                   (sum (intersection x y) (intersection x z)))))