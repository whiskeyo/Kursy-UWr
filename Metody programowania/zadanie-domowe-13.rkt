#lang racket

;; definicja wyrażeń

(struct variable     (x)        #:transparent)
(struct const        (val)      #:transparent)
(struct op           (symb l r) #:transparent)
(struct let-expr     (x e1 e2)  #:transparent)
(struct if-expr      (b t e)    #:transparent)
(struct cons-expr    (l r)      #:transparent)
(struct car-expr     (p)        #:transparent)
(struct cdr-expr     (p)        #:transparent)
(struct pair?-expr   (p)        #:transparent)
(struct null-expr    ()         #:transparent)
(struct null?-expr   (e)        #:transparent)
(struct symbol-expr  (v)        #:transparent)
(struct symbol?-expr (e)        #:transparent)
(struct lambda-expr  (xs b)     #:transparent)
(struct app-expr     (f es)     #:transparent)
(struct apply-expr   (f e)      #:transparent)

(define (expr? e)
  (match e
    [(variable s)       (symbol? s)]
    [(const n)          (or (number? n)
                            (boolean? n))]
    [(op s l r)         (and (member s '(+ *))
                             (expr? l)
                             (expr? r))]
    [(let-expr x e1 e2) (and (symbol? x)
                             (expr? e1)
                             (expr? e2))]
    [(if-expr b t e)    (andmap expr? (list b t e))]
    [(cons-expr l r)    (andmap expr? (list l r))]
    [(car-expr p)       (expr? p)]
    [(cdr-expr p)       (expr? p)]
    [(pair?-expr p)     (expr? p)]
    [(null-expr)        true]
    [(null?-expr p)     (expr? p)]
    [(symbol-expr v)    (symbol? v)]
    [(symbol?-expr p)   (expr? p)]
    [(lambda-expr xs b) (and (list? xs)
                             (andmap symbol? xs)
                             (expr? b)
                             (not (check-duplicates xs)))]
    [(app-expr f es)    (and (expr? f)
                             (list? es)
                             (andmap expr? es))]
    [(apply-expr f e)   (and (expr? f)
                             (expr? e))]
    [_                  false]))

;; wartości zwracane przez interpreter

(struct val-symbol (s)   #:transparent)

(define (my-value? v)
  (or (number? v)
      (boolean? v)
      (and (pair? v)
           (my-value? (car v))
           (my-value? (cdr v)))
      (and (symbol? v) (eq? v 'null)) ; null reprezentowany symbolem (bo tak chcemy)
      (and ((val-symbol? v) (symbol? (val-symbol-s v))))
      (lambda-expr? v)))

;; wyszukiwanie wartości dla klucza na liście asocjacyjnej
;; dwuelementowych list

(define (lookup x xs)
  (cond
    [(null? xs)
     (error x "lookup: unknown identifier :(")]
    [(eq? (caar xs) x) (cadar xs)]
    [else (lookup x (cdr xs))]))

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
              (eq? ,(lambda (x y) (eq? (val-symbol-s x)
                                       (val-symbol-s y))))
              )))

;; interfejs do obsługi środowisk

(define (env-empty) null)
(define env-lookup lookup)
(define (env-add x v env) (cons (list x v) env))

(define (env? e)
  (and (list? e)
       (andmap (lambda (xs) (and (list? e)
                                 (= (length e) 2)
                                 (symbol? (first e)))))))

;; interpretacja wyrażeń

(define (eval e env)
  (match e
    [(const n)          n]
    [(op s l r)         ((op-to-proc s) (eval l env) (eval r env))]
    [(let-expr x e1 e2) (let ((v1 (eval e1 env)))
                          (eval e2 (env-add x v1 env)))]
    [(variable x)       (env-lookup x env)]
    [(if-expr b t e)    (if (eval b env)
                            (eval t env)
                            (eval e env))]
    [(cons-expr l r)    (let ((vl (eval l env))
                              (vr (eval r env)))
                          (cons vl vr))]
    [(car-expr p)       (car (eval p env))]
    [(cdr-expr p)       (cdr (eval p env))]
    [(pair?-expr p)     (pair? (eval p env))]
    [(null-expr)        'null]
    [(null?-expr e)     (eq? (eval e env) 'null)]
    [(symbol-expr v)    (val-symbol v)]
    [(lambda-expr xs b) (define (checker xs)
                          (if (eq? 'null xs)
                              true
                              (if (val-symbol? (car xs))
                                  (checker (cdr xs))
                                  false)))
                        (let ([xs-helper (eval xs env)])
                          (if (checker xs-helper)
                              (lambda-expr xs b)
                              (error "lambda-expr: there is something more than val-symbols in arguments list")))]
    [(app-expr f es)    (define (app-lambda-expr xs es new-env body)
                          (cond [(and (eq? 'null xs)
                                      (eq? 'null es)) (eval body new-env)]
                                [(eq? 'null xs)           (error "app-expr: too much arguments in lambda")]
                                [(eq? 'null es)       (error "app-expr: not enough arguments in lambda")]
                                [else                     (app-lambda-expr (cdr xs)
                                                                           (cdr es)
                                                                           (env-add (val-symbol-s (car xs))
                                                                                    (car es)
                                                                                    new-env)
                                                                           body)]))
                        (let ([lambda-checker (eval f env)])
                          (if (lambda-expr? lambda-checker)
                              (app-lambda-expr (eval (lambda-expr-xs lambda-checker)
                                                     env)
                                               (eval es env)
                                               env
                                               (lambda-expr-b lambda-checker))
                              (error "app-expr: no lambda in its arguments")))]
    [(apply-expr f e)   (define (apply-helper xs)
                          (define (apply-res xs res)
                            (if (eq? 'null xs)
                                res
                                (apply-res (cdr xs)
                                           ((op-to-proc f) (car xs) res))))
                          (define (apply-res-rev xs res)
                            (if (eq? 'null xs)
                                res
                                (apply-res-rev (cdr xs)
                                               ((op-to-proc f) res (car xs)))))
                          (cond [(eq? '+ f) (apply-res xs 0)]
                                [(eq? '* f) (apply-res xs 1)]
                                [(eq? '- f) (if (eq? 'null xs)
                                                (error "apply-expr: no arguments in substitution")
                                                (if (eq? 'null (cdr xs))
                                                         (- (car xs))
                                                         (apply-res-rev (cdr xs) (car xs))))]
                                [(eq? '/ f) (if (eq? 'null xs)
                                                (error "apply-expr: no arguments in division")
                                                (if (eq? 'null (cdr xs))
                                                         (/ 1 (car xs))
                                                         (apply-res-rev (cdr xs) (car xs))))]))
                        (if (eq? false (member f '(+ * - /)))
                            (eval (app-expr f e) env)
                            (apply-helper (eval e env)))]))

(define (run e)
  (eval e (env-empty)))

; Testy do zadania 12 (od add-the-same-variable do add-no-args) oraz 13 (wszystkie pozostale)
(define add-the-same-variable
  (app-expr (lambda-expr (cons-expr (symbol-expr 'var) (null-expr))
                         (op '+ (variable 'var) (variable 'var)))
            (cons-expr (const 5) (null-expr))))

(define add-different-variables
  (app-expr (lambda-expr (cons-expr (symbol-expr 'var1)
                                    (cons-expr (symbol-expr 'var2) (null-expr)))
                         (op '+ (variable 'var1) (variable 'var2)))
            (cons-expr (const 5)
                       (cons-expr (const 10) (null-expr)))))

(define add-from-env
  (let-expr 'already-in-env (const 5)
    (app-expr (lambda-expr (cons-expr (symbol-expr 'var) (null-expr))
                           (op '+ (variable 'var) (variable 'already-in-env)))
              (cons-expr (const 25) (null-expr)))))

(define add-no-args
  (let-expr 'already-in-env (const 5)
    (app-expr (lambda-expr (null-expr)
                           (op '+ (const 5) (variable 'already-in-env)))
              (null-expr))))

(define apply-expr-add ; (apply + '(-5 100 25))
   (apply-expr '+ (cons-expr (const -5)
                             (cons-expr (const 100)
                                        (cons-expr (const 25) (null-expr))))))

(define apply-expr-add-no-args ; (apply + '())
   (apply-expr '+ (null-expr)))

(define apply-expr-sub ; (apply - '(150 50 20 10))
   (apply-expr '- (cons-expr (const 150)
                             (cons-expr (const 50)
                                        (cons-expr (const 20)
                                                   (cons-expr (const 10) (null-expr)))))))

(define apply-expr-mul ; (apply * '(10 5 2 3))
   (apply-expr '* (cons-expr (const 10)
                             (cons-expr (const 5)
                                        (cons-expr (const 2)
                                                   (cons-expr (const 3) (null-expr)))))))

(define apply-expr-mul-no-args ; (apply * '())
   (apply-expr '* (null-expr)))

(define apply-expr-div ; (apply / '(150 2 5 5))
   (apply-expr '/ (cons-expr (const 150)
                             (cons-expr (const 2)
                                        (cons-expr (const 5)
                                                   (cons-expr (const 5) (null-expr)))))))

(define apply-lambda-cube ; (apply (lambda (var) (* var var var)) '(5))
   (apply-expr (lambda-expr (cons-expr (symbol-expr 'var) (null-expr))
                            (op '* (variable 'var) (op '* (variable 'var) (variable 'var))))
               (cons-expr (const 5) (null-expr))))

(require rackunit)
(require rackunit/text-ui)

(define app-expr-test
  (test-suite
   "Tests of app-expr written in the language from a lecture."
   (check-equal? (run add-the-same-variable)
                 10
                 "app-expr-test-1: lambda adds the same variable (var = 5)")
   (check-equal? (run add-different-variables)
                 15
                 "app-expr-test-2: lambda adds two variables (var1 = 5, var2 = 10)")
   (check-equal? (run add-from-env)
                 30
                 "app-expr-test-3: lambda takes one argument and returns its sum
                  with a number 5 added before to environment")
   (check-equal? (run add-no-args)
                 10
                 "app-expr-test-4: lambda does not take any arguments, returns 
                  sum of 5 (added to environment before) and 5")))

(define apply-expr-test
  (test-suite
   "Tests of apply-expr using +, *, -, / operations and my own function (cubing a number)"
   (check-equal? (run apply-expr-add)
                 120
                 "apply-expr-test-1: equivalent to (apply + '(-5 100 25))")
   (check-equal? (run apply-expr-add-no-args)
                 0
                 "apply-expr-test-2: equivalent to (apply + '())")
   (check-equal? (run apply-expr-sub)
                 70
                 "apply-expr-test-3: equivalent to (apply - '(150 50 20 10))")
   (check-equal? (run apply-expr-mul)
                 300
                 "apply-expr-test-4: equivalent to (apply * '(10 5 2 3))")
   (check-equal? (run apply-expr-mul-no-args)
                 1
                 "apply-expr-test-5: equivalent to (apply * '())")
   (check-equal? (run apply-expr-div)
                 3
                 "apply-expr-test-6: equivalent to (apply / '(150 2 5 5))")
   (check-equal? (run apply-lambda-cube)
                 125
                 "apply-expr-test-7: equivalent to (apply (lambda (var) (* var var var)) '(5))")))

(run-tests app-expr-test)
(run-tests apply-expr-test)

"This test checks if a function without arguments throw right error: "
(define expr-no-args
  (app-expr (lambda-expr (cons-expr (symbol-expr 'v) (null-expr))
                         (op '+ (variable 'v) (variable 'v)))
            (null-expr)))

(run expr-no-args)
