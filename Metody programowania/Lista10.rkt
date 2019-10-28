#lang racket

(provide (all-defined-out))

;; definicja wyrażeń

(struct variable     (x)        #:transparent)
(struct const        (val)      #:transparent)
(struct op           (symb l r) #:transparent)
(struct let-expr     (x e1 e2)  #:transparent)
(struct letrec-expr  (x e1 e2)  #:transparent)
(struct if-expr      (b t e)    #:transparent)
(struct cons-expr    (l r)      #:transparent)
(struct car-expr     (p)        #:transparent)
(struct cdr-expr     (p)        #:transparent)
(struct pair?-expr   (p)        #:transparent)
(struct null-expr    ()         #:transparent)
(struct null?-expr   (e)        #:transparent)
(struct symbol-expr  (v)        #:transparent)
(struct symbol?-expr (e)        #:transparent)
(struct lambda-expr  (x b)      #:transparent)
(struct app-expr     (f e)      #:transparent)
(struct set!-expr    (x v)      #:transparent)

;; wartości zwracane przez interpreter

(struct val-symbol (s)   #:transparent)
(struct closure (x b e)) ; Racket nie jest transparentny w tym miejscu,
                         ; to my też nie będziemy
(struct blackhole ()) ; lepiej tzrymać się z daleka!

;; wyszukiwanie wartości dla klucza na liście asocjacyjnej
;; dwuelementowych list

(define (lookup x xs)
  (cond
    [(null? xs)
     (error x "unknown identifier :(")]
    [(eq? (caar xs) x) (cadar xs)]
    [else (lookup x (cdr xs))]))

(define (mlookup x xs)
  (cond
    [(null? xs)
     (error x "unknown identifier :(")]
    [(eq? (mcar (mcar xs)) x)
     (match (mcar (mcdr (mcar xs)))
       [(blackhole) (error "Stuck in a black hole :(")]
       [x x])]
    [else (mlookup x (mcdr xs))]))

(define (mupdate! x v xs)
  (define (update! ys)
    (cond
      [(null? ys) (error x "unknown identifier :(")]
      [(eq? x (mcar (mcar ys)))
       (set-mcar! (mcdr (mcar ys)) v)]
      [else (update! (mcdr ys))]))
  (begin (update! xs) xs))

;; kilka operatorów do wykorzystania w interpreterze

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
              (% ,modulo)
              (!= ,(lambda (x y) (not (= x y)))) 
              (&& ,(lambda (x y) (and x y)))
              (|| ,(lambda (x y) (or x y)))
              (eq? ,(lambda (x y) (eq? (val-symbol-s x)
                                       (val-symbol-s y))))
              )))

;; interfejs do obsługi środowisk

(define (env-empty) null)
(define env-lookup mlookup)
(define (env-add x v env)
  (mcons (mcons x (mcons v null)) env))
(define env-update! mupdate!)

;; interpretacja wyrażeń

(define (eval e env)
  (match e
    [(const n) n]
    [(op s l r)
     ((op-to-proc s) (eval l env)
                     (eval r env))]
    [(let-expr x e1 e2)
     (let ((v1 (eval e1 env)))
       (eval e2 (env-add x v1 env)))]
    [(letrec-expr x e1 e2)
     (let* ((new-env (env-add x (blackhole) env))
            (v1 (eval e1 new-env)))
       (eval e2 (env-update! x v1 new-env)))]
    [(variable x) (env-lookup x env)]
    [(if-expr b t e) (if (eval b env)
                         (eval t env)
                         (eval e env))]
    [(cons-expr l r)
     (let ((vl (eval l env))
           (vr (eval r env)))
       (cons vl vr))]
    [(car-expr p)      (car (eval p env))]
    [(cdr-expr p)      (cdr (eval p env))]
    [(pair?-expr p)    (pair? (eval p env))]
    [(null-expr)       'null]
    [(null?-expr e)    (eq? (eval e env) 'null)]
    [(symbol-expr v)   (val-symbol v)]
    [(lambda-expr x b) (closure x b (opt-env b env))]
    [(app-expr f e)    (let ((vf (eval f env))
                             (ve (eval e env)))
                         (match vf
                           [(closure x b c-env)
                            (eval b (env-add x ve c-env))]
                           [_ (error "application: not a function :(")]))]
    [(set!-expr x e)
     (env-update! x (eval e env) env)]
    ))

(define (run e)
  (eval e (env-empty)))

;; przykład

(define fact-in-expr
  (letrec-expr 'fact (lambda-expr 'n
     (if-expr (op '= (const 0) (variable 'n))
              (const 1)
              (op '* (variable 'n)
                  (app-expr (variable 'fact)
                            (op '- (variable 'n)
                                   (const 1))))))
     (app-expr (variable 'fact)
               (const 5))))

(define (reverse-racket xs)
  (if (null? xs)
      null
      (append (reverse-racket (cdr xs)) (list (car xs)))))

#;(define reverse
  (letrec-expr
   'rev
   (lambda-expr
    'xs
    (if-expr (null?-expr (variable 'xs))
             (null-expr)
             (cons-expr (app-expr (variable 'rev)
                                  (cdr-expr (variable 'xs)))
                        (car-expr (variable 'xs)))))
   (app-expr (variable 'rev) (cons-expr (const 1) (cons-expr (const 2) (null-expr))))))

(define rev-while
  (letrec-expr
   'rev
   (lambda-expr
    'xs
    (lambda-expr
     'acc
     (if-expr (null?-expr (variable 'xs))
              (variable 'acc)
              (app-expr (app-expr (variable 'rev) (cdr-expr (variable 'xs)))
                        (cons-expr (car-expr (variable 'xs)) (variable 'acc))))))
   (app-expr (app-expr (variable 'rev) (cons-expr (const 1) (cons-expr (const 2) (null-expr))))
                        (null-expr))))

(define append-while
  (letrec-expr
   'app
   (lambda-expr
    'xs
    (lambda-expr
     'ys
     (if-expr (null?-expr (variable 'xs))
              (variable 'ys)
              (cons-expr (car-expr (variable 'xs))
                         (app-expr (app-expr (variable 'app)
                                   (cdr-expr (variable 'xs))) (variable 'ys))))))
   (app-expr (app-expr (variable 'app)
                       (cons-expr (const 1) (cons-expr (const 2) (null-expr))))
             (cons-expr (const 3) (cons-expr (const 4) (null-expr))))))

(define map-while
  (letrec-expr
   'app
   (lambda-expr
    'xs
    (lambda-expr
     'f
     (if-expr (null?-expr (variable 'xs))
              (null-expr)
              (cons-expr (app-expr (variable 'f) (car-expr (variable 'xs)))
                         (app-expr (app-expr (variable 'app)
                                   (cdr-expr (variable 'xs))) (variable 'f))))))
   (app-expr (app-expr (variable 'app)
                       (cons-expr (const 1) (cons-expr (const 2) (null-expr))))
             (lambda-expr 'x (op '+ (const 1) (variable 'x))))))

(define (garbage-collector x xs)
  (cond
    [(null? xs) null]
    [(eq? x (car xs)) (garbage-collector x (cdr xs))]
    [else (cons (car xs) (garbage-collector x (cdr xs)))]))

(define (opt-env program env)
  (define (help program)
    (match program
      [(op s l r)
       (append (opt-env l)
               (opt-env r))]
      [(let-expr x e1 e2)
       (append (opt-env e1) (garbage-collector x (opt-env e2)))]
      [(letrec-expr x e1 e2)
       (append (garbage-collector x (opt-env e1)) (garbage-collector x (opt-env e2)))]
      [(variable x) (list x)]
      [(if-expr b t e) (append (opt-env b)
                               (opt-env t)
                               (opt-env e))]
      [(cons-expr l r)
       (append (opt-env l) (opt-env r))]
      [(car-expr p)      (opt-env p)]
      [(cdr-expr p)      (opt-env p)]
      [(pair?-expr p)    (opt-env p)]
      [(null-expr)       null]
      [(null?-expr e)    (opt-env e)]
      [(lambda-expr x b) (garbage-collector x (opt-env b))]
      [(app-expr f e)    (opt-env e)]
      [(set!-expr x e) (opt-env e)]
      [_ null]))
  
  (define (godlike-env xs env)
    (cond
      [(null? xs) null]
      [else (mcons (mcons (car xs) (mlookup (car xs) env)) (godlike-env (cdr xs) env))]))
  
  (godlike-env (remove-duplicates (help program)) env))