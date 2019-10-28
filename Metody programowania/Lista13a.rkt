#lang typed/racket

;; zadanko 1
(: prefixes (All (a) (-> (Listof a) (Listof (Listof a)))))
(define (prefixes xs)
  (if (null? xs)
      null
      (cons xs (prefixes (reverse (cdr (reverse xs)))))))

;; zadanko 2
(struct vector2 ([x : Real] [y : Real])            #:transparent)
(struct vector3 ([x : Real] [y : Real] [z : Real]) #:transparent)

(: vector-length (-> (U vector2 vector3) Real))
(define (vector-length v)
  (match v
    [(vector2 x y) (sqrt (+ (sqr x) (sqr y)))]
    [(vector3 x y z) (sqrt (+ (sqr x) (sqr y) (sqr z)))]))

(: vector-length2 (-> (U vector2 vector3) Real))
(define (vector-length2 v)
  (cond
    [(vector2? v) (sqrt (+ (sqr (vector2-x v)) (sqr (vector2-y v))))]
    [(vector3? v) (sqrt (+ (sqr (vector3-x v)) (sqr (vector3-y v)) (sqr (vector3-z v))))]))

;; zadanko 3
(: inc (-> Real Real))
(define (inc x)
  (+ x 1))

(: map2 (All [a b] (-> (-> a b) (Listof a) (Listof b))))
(define (map2 f xs)
  (if (null? xs)
      null
      (cons (f (car xs)) (map2 f (cdr xs)))))

(: map3 (All [a] (-> (-> a Any) (Listof a) (Listof Any))))
(define (map3 f xs)
  (if (null? xs)
      null
      (cons (car xs) (map3 f (cdr xs)))))

;; zadanko 4
(struct (a b) node ([v : a] [xs : b]) #:transparent)
(struct leaf ()                       #:transparent)

(define-type (Tree a) (U leaf (node a (Listof (Tree a)))))

(define-predicate tree? (Tree Any))

(: puts (All (a) (-> (Tree a) (Listof a))))
(define (puts t)
  (match t
    [(leaf) null]
    [(node w xs) (apply append (cons (list w) (map (inst puts a) xs)))]))

(define test-tree1 (node 1 (list (node 2 (list (leaf))) (node 3 (list (leaf))))))

(define test-tree2 (node 1
                         (list
                          (node 2 (list (node 4 (list (leaf) (leaf)))
                                        (node 5 (list (leaf)))))
                          (node 3 (list (leaf))))))

;; zadanko 5
(struct const   ([val : Real])      #:transparent)
(struct op      ([symb : Symbol]
                 [l : expr]
                 [r : expr])        #:transparent)
(struct let-expr ([x : Symbol]
                  [e1 : expr]
                  [e2 : expr])      #:transparent)
(struct variable ([x : Symbol])     #:transparent)

(define-type expr (U const op let-expr variable))

#;(define (expr? e)
  (match e
    [(variable s)       (symbol? s)]
    [(const n)          (number? n)]
    [(op s l r)         (and (member s '(+ *))
                             (expr? l)
                             (expr? r))]
    [(let-expr x e1 e2) (and (symbol? x)
                             (expr? e1)
                             (expr? e2))]
    [_                  false]))

;; podstawienie wartości (= wyniku ewaluacji wyrażenia) jako stałej w wyrażeniu
(: subst (-> Symbol Real expr expr))
(define (subst x v e)
  (match e
    [(op s l r)   (op s (subst x v l)
                        (subst x v r))]
    [(const n)    (const n)]
    [(variable y) (if (eq? x y)
                      (const v)
                      (variable y))]
    [(let-expr y e1 e2)
     (if (eq? x y)
         (let-expr y
                   (subst x v e1)
                   e2)
         (let-expr y
                   (subst x v e1)
                   (subst x v e2)))]))

;; (gorliwa) ewaluacja wyrażenia w modelu podstawieniowym

(: eval (-> expr Real))
(define (eval e)
  (match e
    [(const n)    n]
    [(op '+ l r)  (+ (eval l) (eval r))]
    [(op '* l r)  (* (eval l) (eval r))]
    [(let-expr x e1 e2)
     (eval (subst x (eval e1) e2))]
    [(variable n) (error n "cannot reference an identifier before its definition ;)")]))

(define p1
  (let-expr 'x (op '+ (const 2) (const 2))
     (op '+ (const 1000) (let-expr 'y (op '+ (const 5) (const 5))
                            (op '* (variable 'x) (variable 'y))))))