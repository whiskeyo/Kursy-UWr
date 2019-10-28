#lang racket

; Zadanie 1 - zamiana zwykłej listy na cykl złożony z mconsów,
; czyli ostatni element wskazuje na pierwszy.

(define (make-cycle xs)
  (define (last-pair xs)
    (if (null? (mcdr xs))
        xs
        (last-pair (mcdr xs))))
  
  (define (change-ptr xs)
    (set-mcdr! (last-pair xs) xs)
    xs)
  
  (define (helper xs)
    (if (null? xs)
        null
        (mcons (car xs) (helper (cdr xs)))))
  
  (change-ptr (helper xs)))

; Zadanie 2 - procedura set-nth! zmieniająca n-ty element
; modyfikowalnej listy xs na wartość wyrażenia v.

(define (set-nth! n xs v) ; lista indeksowana od 0 do n-1
  (define (get-nth n xs)
    (if (= n 0)
        xs
        (get-nth (- n 1) (mcdr xs))))
  (set-mcar! (get-nth n xs) v))

; Zadanie 3 - program w składni abstrakcyjnej języka WHILE
; obliczający n-tą liczbę Fibbonaciego.

(struct variable (x)         #:transparent)
(struct const    (val)       #:transparent)
(struct op       (symb l r)  #:transparent)
(struct let-expr (x e1 e2)   #:transparent)
(struct if-expr  (b t e)     #:transparent)

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
    [_                  false]))

(struct skip      ()       #:transparent) ; skip
(struct comp      (s1 s2)  #:transparent) ; s1; s2
(struct assign    (x e)    #:transparent) ; x := e
(struct while     (b s)    #:transparent) ; while (b) s
(struct if-stm    (b t e)  #:transparent) ; if (b) t else e
(struct var-block (x e s)  #:transparent) ; var x := e in s
(struct ++        (x)      #:transparent) ; x++
(struct for       (x e1 e2 s1 s2) #:transparent) ; for (x := e1, e2, s1) s2
(struct just-for  (x e1 e2 s1 s2) #:transparent) ; for (x := e1, e2, s1) s2

(define (stm? e)
  (match e
    [(skip) true]
    [(comp s1 s2)   (and (stm? s1) (stm? s2))]
    [(assign x e)   (and (symbol? x) (expr? e))]
    [(while b s)    (and (expr? b) (stm? s))]
    [(if-stm b t e) (and (expr? b) (stm? t) (stm? e))]
    [(++ x)         (symbol? x)]
    [(for x a b c d)  (and (symbol? x) (expr? a) (expr? b) (stm? c) (stm? d))]
    [(just-for x a b c d) (and (symbol? x) (expr? a) (expr? b) (stm? c) (stm? d))]
    [_ false]))
  
;; wyszukiwanie wartości dla klucza na liście asocjacyjnej
;; dwuelementowych list

(define (lookup x xs)
  (cond
    [(null? xs)
     (error x "unknown identifier :(")]
    [(eq? (caar xs) x) (cadar xs)]
    [else (lookup x (cdr xs))]))

;; aktualizacja środowiska dla danej zmiennej (koniecznie już
;; istniejącej w środowisku!)

(define (update x v xs)
  (cond
    [(null? xs)
     (error x "unknown identifier :(")]
    [(eq? (caar xs) x)
     (cons (list (caar xs) v) (cdr xs))]
    [else
     (cons (car xs) (update x v (cdr xs)))]))

;; kilka operatorów do wykorzystania w interpreterze

(define (op-to-proc x)
  (lookup x `((+ ,+)
              (* ,*)
              (- ,-)
              (/ ,/)
              (%, modulo)
              (> ,>)
              (>= ,>=)
              (< ,<)
              (<= ,<=)
              (= ,=)
              (!= ,(lambda (x y) (not (= x y)))) 
              (&& ,(lambda (x y) (and x y)))
              (|| ,(lambda (x y) (or x y)))
              )))

;; interfejs do obsługi środowisk

(define (env-empty) null)
(define env-lookup lookup)
(define (env-add x v env) (cons (list x v) env))
(define env-update update)
(define env-discard cdr)
(define (env-from-assoc-list xs) xs)

;; ewaluacja wyrażeń ze środowiskiem

(define (eval e env)
  (match e
    [(const n) n]
    [(op s l r) ((op-to-proc s) (eval l env)
                                (eval r env))]
    [(let-expr x e1 e2)
     (let ((v1 (eval e1 env)))
       (eval e2 (env-add x v1 env)))]
    [(variable x) (env-lookup x env)]
    [(if-expr b t e) (if (eval b env)
                         (eval t env)
                         (eval e env))]))

;; interpretacja programów w języku WHILE, gdzie środowisko m to stan
;; pamięci. Interpreter to procedura, która dostaje program i początkowy
;; stan pamięci, a której wynikiem jest końcowy stan pamięci. Pamięć to
;; aktualne środowisko zawierające wartości zmiennych

(define (interp p m)
  (match p
    [(skip) m]
    [(comp s1 s2) (interp s2 (interp s1 m))]
    [(assign x e) (env-update x (eval e m) m)]
    [(while b s) (if (eval b m)
                     (interp p (interp s m))
                     m)]
    [(var-block x e s) (env-discard (interp s (env-add x (eval e m) m)))]
    [(if-stm b t e) (if (eval b m)
                        (interp t m)
                        (interp e m))]
    ;[(++ x) (env-update x (+ 1 (eval (variable x) m)) m)]
    [(++ x) (interp (assign x (op '+ (const 1) (variable x))) m)]
    [(for x e1 e2 s1 s2) (interp (var-block x e1 (while e2 (comp s2 s1))) m)]))

;; silnia zmiennej i

(define fact-in-WHILE
  (var-block 'x (const 0)                                           ; var x := 0 in
     (comp (assign 'x (const 1))                                    ;   x := 1
     (comp (while (op '> (variable 'i) (const 0))                   ;   while (i > 0)
              (comp (assign 'x (op '* (variable 'x) (variable 'i))) ;     x := x * i
                    (assign 'i (op '- (variable 'i) (const 1)))))   ;     i := i - 1
           (assign 'i (variable 'x))))))                            ;   i := x

(define (factorial n)
  (env-lookup 'i (interp fact-in-WHILE
                         (env-from-assoc-list `((i ,n))))))

(define fib-in-WHILE ;
  (var-block 'temp (const 0)
             (while (op '> (variable 'n) (const 1))
                    (comp (assign 'temp (op '+ (variable 'i) (variable 'j)))
                          (comp (assign 'i (variable 'j))
                                (comp (assign 'j (variable 'temp))
                                      (assign 'n (op '- (variable 'n) (const 1)))))))))

(define (fib n)
  (env-lookup 'j (interp fib-in-WHILE
                         (env-from-assoc-list `((i 1)
                                                (j 1)
                                                (n ,n))))))

; Zadanie 4 - koniunkcja jest gorliwa, bo najpierw liczy lambdę, a następnie sprawdza and'a.

; Zadanie 5 - rozszerz składnię abstrakcyjną języka WHILE i interpreter z wykładu o instrukcję ++.
; Składnia konkretna tej instrukcji mogłaby być dana jako x++, gdzie x to zmienna.

(define ++-in-WHILE
  (++ 'x))

(define (++proc n)
  (env-lookup 'x (interp ++-in-WHILE
                         (env-from-assoc-list `((x ,n))))))

; Zadanie 6 - rozszerzenie składni o for

(define for-in-WHILE
  (for 'x (const 0) (op '<= (variable 'x) (const 10)) (++ 'x) (assign 'y (op '+ (variable 'x) (variable 'y)))))

(define (forloop y)
  (env-lookup 'y (interp for-in-WHILE
                         (env-from-assoc-list `((y ,y))))))

; Zadanie 7 - jakies makro instrukcje czy cos

(define (comp* . xs)
  (cond [(null? xs) (skip)]
        [else (comp (car xs) (apply comp* (cdr xs)))]))

(define (for* x e1 e2 s1 s2)
 (var-block x e1 (while e2 (comp s2 s1))))

(define for*-in-WHILE
  (for* 'x (const 0) (op '<= (variable 'x) (const 10)) (++ 'x) (assign 'y (op '+ (variable 'x) (variable 'y)))))

(define (for*loop y)
  (env-lookup 'y (interp for*-in-WHILE
                         (env-from-assoc-list `((y ,y))))))