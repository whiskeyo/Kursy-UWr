#lang typed/racket

;; definicja wyrażeń z let-wyrażeniami i if-wyrażeniami

(struct if-expr  ([b : expr]
                  [t : expr]
                  [e : expr])    #:transparent)

(struct const ([val : Value])      #:transparent)
(struct op    ([symb : Symbol]
               [l : expr]
               [r : expr]) #:transparent)
(struct let-expr ([x : Symbol]
                  [e1 : expr]
                  [e2 : expr])  #:transparent)
(struct variable ([x : Symbol])        #:transparent)

(define-type expr (U const op let-expr variable if-expr))

(define-type (Env a) (Listof (List Symbol a)))

(define-type Value (U Real Boolean))  
;; wyszukiwanie wartości dla klucza na liście asocjacyjnej
;; dwuelementowych list

(: lookup (All (a) (-> Symbol (Env a) a)))
(define (lookup x xs)
  (cond
    [(null? xs)
     (error x "unknown identifier :(")]
    [(eq? (caar xs) x) (cadar xs)]
    [else (lookup x (cdr xs))]))
#|
(: lookup2 (-> Symbol (Listof (List Symbol Procedure)) Procedure))
(define (lookup2 x xs)
  (cond
    [(null? xs)
     (error x "unknown identifier :(")]
    [(eq? (caar xs) x) (cadar xs)]
    [else (lookup2 x (cdr xs))]))
;; kilka operatorów do wykorzystania w interpreterze
|#

(: op-to-proc (-> Symbol (-> Real Real Value)))
(define (op-to-proc x)
  (lookup x `(
              (+ ,+)
              (* ,*)
              (- ,-)
              (/ ,/)
              (> ,>)
              (>= ,>=)
              (< ,<)
              (<= ,<=)
              (= ,=)
              )))

;; interfejs do obsługi środowisk

(define (env-empty) null)
(define env-lookup lookup)

(: env-add (All (a) (-> Symbol a (Env a) (Env a) )))
(define (env-add x v env) (cons (list x v) env))

;; interpretacja wyrażeń

(: trans (-> Value Real))
(define (trans x)
  (if (real? x)
      x
      (error "error")))

(: eval (-> expr (Env Value) Value))
(define (eval e env)
  (match e
    [(const n) n]
    [(op s l r)
     ((op-to-proc s) (trans (eval l env))
                     (trans (eval r env)))]
    [(let-expr x e1 e2)
     (let ((v1 (eval e1 env)))
       (eval e2 (env-add x v1 env)))]
    [(variable x) (env-lookup x env)]
    [(if-expr b t e) (if (eval b env)
                         (eval t env)
                         (eval e env))]))

(: run (-> expr Value))
(define (run e)
  (eval e (env-empty)))

;; przykładowe wyrażenie

(define ex1
  (op '+ (const 1)
         (let-expr 'x (const 3)
                   (if-expr (op '< (variable 'x) (const 4))
                            (op '+ (const 4) (variable 'x))
                            (const true)))))