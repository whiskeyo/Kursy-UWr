#lang racket
;;task 1
(define/contract (suffixes xs)
  (parametric->/c [a] (-> (listof a) (listof (listof a))))
  (if (null? xs)
      xs
      (cons xs (suffixes (cdr xs)))))

;;task 2
(define/contract (sublists  xs)
  (parametric->/c [a] (-> (listof a) (listof (listof a))))
  (if (null? xs)
     (list null)
     (append-map (lambda (ys)
                   (list ys (cons (car xs) ys)))
                 (sublists (cdr xs)))))

;;task 3

(define (id x)
  x)

(define/contract (foo1 x y)
  (parametric->/c [a b c] (-> a b a))
  x)

(define/contract (foo2 f g x)
  (parametric->/c [a b c] (-> (-> a b c) (-> a b) a c))
  (f x (g x)))

(define/contract (foo3 f g)
  (parametric->/c [a b c] (-> (-> b c) (-> a b) (-> a c)))
  (lambda (x)
    (f (g x))))

(define/contract (foo4 f)
  (parametric->/c [a] (-> (-> (-> a a) a) a))
  (f f))

;;task 4

(define/contract (foo x)
  (parametric->/c [a b] (-> a b))
  (foo x))

;;task 5
(define/contract (foldl-map f a xs)
  (parametric->/c [a acc c] (-> (-> a acc (cons/c c acc)) acc (listof a)
                              (cons/c (listof c) acc)))
  (define (it a xs ys)
    (if (null? xs)
        (cons (reverse  ys) a)
        (let [(p (f (car xs) a))]
          (it (cdr p)
              (cdr xs)
              (cons (car p) ys)))))
  (it a xs null))

;;task 6

;; definicja wyrażeń z let-wyrażeniami

(struct const    (val)      #:transparent)
(struct op       (symb l r) #:transparent)
(struct let-expr (x e1 e2)  #:transparent)
(struct variable (x)        #:transparent)

(define (expr? e)
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

(define (expr/c e)
  (flat-rec-contract expr
                     (struct/c variable symbol?)
                     (struct/c const number?)
                     (struct/c let-expr symbol? expr expr)
                     (struct/c op symbol? expr expr)))

;; podstawienie wartości (= wyniku ewaluacji wyrażenia) jako stałej w wyrażeniu

(define/contract (subst x v e)
  (-> symbol? number? expr/c expr/c)
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

(define/contract (eval e) (-> expr/c number?)
  (match e
    [(const n)    n]
    [(op '+ l r)  (+ (eval l) (eval r))]
    [(op '* l r)  (* (eval l) (eval r))]
    [(let-expr x e1 e2)
     (eval (subst x (eval e1) e2))]
    [(variable n) (error n "cannot reference an identifier before its definition ;)")]))

;; przykładowe programy

(define/contract p1 expr/c 
  (let-expr 'x (op '+ (const 'x) (const 2))
     (op '+ (const 1000) (let-expr 'y (op '+ (const 5) (const 5))
                            (op '* (variable 'x) (variable 'y))))))

(define/contract p2 expr/c
  (let-expr 'x (op '+ (const 2) (const 2))
     (op '+ (const 1000) (let-expr 'x (op '+ (const 5) (const 5))
                            (op '* (variable 'x) (variable 'x))))))

(define/contract p3 expr/c
  (let-expr 'x (op '+ (const 2) (const 2))
     (op '+ (const 1000) (let-expr 'y (op '+ (const 5) (const 5))
                            (op '* (variable 'x) (variable 'z))))))

(define prog (eval p1))