#lang racket

; Moduł cons, czyli WHILE na trwałych strukturach danych.
(module cons racket
  (provide (all-defined-out))

;; definicja wyrażeń z let-wyrażeniami i if-wyrażeniami

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

;; definicja instrukcji w języku WHILE

(struct skip      ()       #:transparent) ; skip
(struct comp      (s1 s2)  #:transparent) ; s1; s2
(struct assign    (x e)    #:transparent) ; x := e
(struct while     (b s)    #:transparent) ; while (b) s
(struct if-stm    (b t e)  #:transparent) ; if (b) t else e
(struct var-block (x e s)  #:transparent) ; var x := e in s

(define (stm? e)
  (match e
    [(skip) true]
    [(comp s1 s2)   (and (stm? s1) (stm? s2))]
    [(assign x e)   (and (symbol? x) (expr? e))]
    [(while b s)    (and (expr? b) (stm? s))]
    [(if-stm b t e) (and (expr? b) (stm? t) (stm? e))]
    [_ false]))
  
;; wyszukiwanie wartości dla klucza na liście asocjacyjnej
;; dwuelementowych list

(define (lookup x xs)
  (cond
    [(null? xs)        (error x "unknown identifier :(")]
    [(eq? (caar xs) x) (cadar xs)]
    [else              (lookup x (cdr xs))]))

;; aktualizacja środowiska dla danej zmiennej (koniecznie już
;; istniejącej w środowisku!)

(define (update x v xs)
  (cond
    [(null? xs)         (error x "unknown identifier :(")]
    [(eq? (caar xs) x)  (cons (list (caar xs) v) (cdr xs))]
    [else               (cons (car xs) (update x v (cdr xs)))]))

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
    [(const n)           n]
    [(op s l r)          ((op-to-proc s) (eval l env)
                                       (eval r env))]
    [(let-expr x e1 e2)  (let ([v1 (eval e1 env)])
                           (eval e2 (env-add x v1 env)))]
    [(variable x)        (env-lookup x env)]
    [(if-expr b t e)     (if (eval b env)
                             (eval t env)
                             (eval e env))]))

;; interpretacja programów w języku WHILE, gdzie środowisko m to stan
;; pamięci. Interpreter to procedura, która dostaje program i początkowy
;; stan pamięci, a której wynikiem jest końcowy stan pamięci. Pamięć to
;; aktualne środowisko zawierające wartości zmiennych

(define (interp p m)
  (match p
    [(skip)             m]
    [(comp s1 s2)       (interp s2 (interp s1 m))]
    [(assign x e)       (env-update x (eval e m) m)]
    [(while b s)        (if (eval b m)
                            (interp p (interp s m))
                            m)]
    [(var-block x e s)  (env-discard (interp s (env-add x (eval e m) m)))]
    [(if-stm b t e)     (if (eval b m)
                            (interp t m)
                            (interp e m))]))

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

;; najmniejsza liczba pierwsza nie mniejsza niż i

(define find-prime-in-WHILE
 (var-block 'c (variable 'i)                                         ; var c := i in
 (var-block 'continue (const true)                                   ; var continue := true in
 (comp
  (while (variable 'continue)                                        ; while (continue)
   (var-block 'is-prime (const true)                                 ;   var is-prime := true in
   (var-block 'x (const 2)                                           ;   var x := 2 in
    (comp
     (while (op '&& (variable 'is-prime)                             ;   while (is-prime &&
                    (op '< (variable 'x) (variable 'c)))             ;            x < c)
        (comp (if-stm (op '= (op '% (variable 'c) (variable 'x))     ;     if (c % x =
                             (const 0))                              ;                 0)
                      (assign 'is-prime (const false))               ;       is-prime := false
                      (skip))                                        ;     else skip
              (assign 'x (op '+ (variable 'x) (const 1)))))          ;     x := x + 1 
     (if-stm (variable 'is-prime)                                    ;   if (is-prime)
             (assign 'continue (const false))                        ;     continue := false
             (comp (assign 'continue (const true))                   ;   else continue := true
                   (assign 'c (op '+ (variable 'c) (const 1))))))))) ;        c := c + 1
  (assign 'i (variable 'c))))))                                      ; i := c

(define (find-prime-using-WHILE n)
  (env-lookup 'i (interp find-prime-in-WHILE
                         (env-from-assoc-list `((i ,n) (is-prime ,true))))))          


(define fib-in-WHILE 
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
  
;; porownajmy wydajnosc!

;; ten sam algorytm do wyszukiwania liczby pierwszej nie mniejszej niż n
;; zapisany funkcyjnie jest dosc brzydki, ale odpowiada temu zapisanemu w WHILE

(define (find-prime-native n)
  (define (is-prime c isp x)
    (if (and isp (< x c))
      (if (= (modulo c x) 0)
          (is-prime c false (+ x 1))
          (is-prime c isp   (+ x 1)))
      isp))
  (if (is-prime n true 2)
      n
      (find-prime-native (+ n 1))))

;; testujemy, żeby dowiedzieć się, jak dużo wydajności tracimy przez
;; uruchamianie programu w naszym interpreterze

(define (test)
  (begin
    (display "wait...\n")
    (flush-output (current-output-port))
    (test-performance)))

(define (test-performance)
  (let-values
      (((r1 cpu1 real1 gc1) (time-apply find-prime-using-WHILE (list 1111111)))
       ((r2 cpu2 real2 gc2) (time-apply find-prime-native      (list 1111111))))
    (begin
      (display "WHILE  time (cpu, real, gc): ")
      (display cpu1)  (display ", ")
      (display real1) (display ", ")
      (display gc1)   (display "\n")
      (display "native time (cpu, real, gc): ")
      (display cpu2)  (display ", ")
      (display real2) (display ", ")
      (display gc2)   (display "\n"))))
) ; module close

; Moduł mcons, czyli WHILE na zmiennych strukturach danych.
; Zmienia się niewiele, jedynie consy zamieniamy na mconsy,
; car i cdr na mcar i mcdr oraz w inny sposób tworzymy listy.
(module mcons racket
  (provide (all-defined-out))

;; definicja wyrażeń z let-wyrażeniami i if-wyrażeniami

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

;; definicja instrukcji w języku WHILE

(struct skip      ()       #:transparent) ; skip
(struct comp      (s1 s2)  #:transparent) ; s1; s2
(struct assign    (x e)    #:transparent) ; x := e
(struct while     (b s)    #:transparent) ; while (b) s
(struct if-stm    (b t e)  #:transparent) ; if (b) t else e
(struct var-block (x e s)  #:transparent) ; var x := e in s

(define (stm? e)
  (match e
    [(skip) true]
    [(comp s1 s2)   (and (stm? s1) (stm? s2))]
    [(assign x e)   (and (symbol? x) (expr? e))]
    [(while b s)    (and (expr? b) (stm? s))]
    [(if-stm b t e) (and (expr? b) (stm? t) (stm? e))]
    [_ false]))
  
;; wyszukiwanie wartości dla klucza na liście asocjacyjnej
;; dwuelementowych list

(define (lookup x xs)
  (cond
    [(null? xs)               (error x "unknown identifier :(")]
    [(eq? (mcar (mcar xs)) x) (mcar (mcdr (mcar xs)))]
    [else                     (lookup x (mcdr xs))]))

;; aktualizacja środowiska dla danej zmiennej (koniecznie już
;; istniejącej w środowisku!)

(define (update x v xs)
  (define (update-aux x v xs)
    (cond
      [(null? xs)                (error x "unknown identifier :(")]
      [(eq? (mcar (mcar xs)) x)  (set-mcar! (mcdr (mcar xs)) v)]
      [else                      (update-aux x v (mcdr xs))]))
  (begin (update-aux x v xs)
         xs))

;; kilka operatorów do wykorzystania w interpreterze

(define (op-to-proc x)
  (lookup x (mcons (mcons '+ (mcons + null))
            (mcons (mcons '* (mcons * null))
            (mcons (mcons '- (mcons - null))
            (mcons (mcons '/ (mcons / null))
            (mcons (mcons '% (mcons modulo null))
            (mcons (mcons '> (mcons > null))
            (mcons (mcons '>= (mcons >= null))
            (mcons (mcons '< (mcons < null))
            (mcons (mcons '<= (mcons <= null))
            (mcons (mcons '= (mcons = null))
            (mcons (mcons '!= (mcons (lambda (x y) (not (= x y))) null))
            (mcons (mcons '&& (mcons (lambda (x y) (and x y)) null))
            (mcons (mcons '|| (mcons (lambda (x y) (or x y)) null))
                   null)))))))))))))))

;; interfejs do obsługi środowisk

(define (list->mlist xs)
  (if (null? xs) null
      (mcons (mcons (caar xs) (mcons (cadar xs) null))
             (list->mlist (cdr xs)))))

(define (env-empty) null)
(define env-lookup lookup)
(define (env-add x v env) (mcons (mcons x (mcons v null)) env))
(define env-update update)
(define env-discard mcdr)
(define (env-from-assoc-list xs) (list->mlist xs))

;; ewaluacja wyrażeń ze środowiskiem

(define (eval e env)
  (match e
    [(const n)           n]
    [(op s l r)          ((op-to-proc s) (eval l env)
                                       (eval r env))]
    [(let-expr x e1 e2)  (let ([v1 (eval e1 env)])
                           (eval e2 (env-add x v1 env)))]
    [(variable x)        (env-lookup x env)]
    [(if-expr b t e)     (if (eval b env)
                             (eval t env)
                             (eval e env))]))

;; interpretacja programów w języku WHILE, gdzie środowisko m to stan
;; pamięci. Interpreter to procedura, która dostaje program i początkowy
;; stan pamięci, a której wynikiem jest końcowy stan pamięci. Pamięć to
;; aktualne środowisko zawierające wartości zmiennych

(define (interp p m)
  (match p
    [(skip)             m]
    [(comp s1 s2)       (interp s2 (interp s1 m))]
    [(assign x e)       (env-update x (eval e m) m)]
    [(while b s)        (if (eval b m)
                            (interp p (interp s m))
                            m)]
    [(var-block x e s)  (env-discard (interp s (env-add x (eval e m) m)))]
    [(if-stm b t e)     (if (eval b m)
                            (interp t m)
                            (interp e m))]))

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

;; najmniejsza liczba pierwsza nie mniejsza niż i

(define find-prime-in-WHILE
 (var-block 'c (variable 'i)                                         ; var c := i in
 (var-block 'continue (const true)                                   ; var continue := true in
 (comp
  (while (variable 'continue)                                        ; while (continue)
   (var-block 'is-prime (const true)                                 ;   var is-prime := true in
   (var-block 'x (const 2)                                           ;   var x := 2 in
    (comp
     (while (op '&& (variable 'is-prime)                             ;   while (is-prime &&
                    (op '< (variable 'x) (variable 'c)))             ;            x < c)
        (comp (if-stm (op '= (op '% (variable 'c) (variable 'x))     ;     if (c % x =
                             (const 0))                              ;                 0)
                      (assign 'is-prime (const false))               ;       is-prime := false
                      (skip))                                        ;     else skip
              (assign 'x (op '+ (variable 'x) (const 1)))))          ;     x := x + 1 
     (if-stm (variable 'is-prime)                                    ;   if (is-prime)
             (assign 'continue (const false))                        ;     continue := false
             (comp (assign 'continue (const true))                   ;   else continue := true
                   (assign 'c (op '+ (variable 'c) (const 1))))))))) ;        c := c + 1
  (assign 'i (variable 'c))))))                                      ; i := c

(define (find-prime-using-WHILE n)
  (env-lookup 'i (interp find-prime-in-WHILE
                         (env-from-assoc-list `((i ,n) (is-prime ,true))))))          

(define fib-in-WHILE 
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
  
;; porownajmy wydajnosc!

;; ten sam algorytm do wyszukiwania liczby pierwszej nie mniejszej niż n
;; zapisany funkcyjnie jest dosc brzydki, ale odpowiada temu zapisanemu w WHILE

(define (find-prime-native n)
  (define (is-prime c isp x)
    (if (and isp (< x c))
      (if (= (modulo c x) 0)
          (is-prime c false (+ x 1))
          (is-prime c isp   (+ x 1)))
      isp))
  (if (is-prime n true 2)
      n
      (find-prime-native (+ n 1))))

;; testujemy, żeby dowiedzieć się, jak dużo wydajności tracimy przez
;; uruchamianie programu w naszym interpreterze

(define (test)
  (begin
    (display "wait...\n")
    (flush-output (current-output-port))
    (test-performance)))

(define (test-performance)
  (let-values
      (((r1 cpu1 real1 gc1) (time-apply find-prime-using-WHILE (list 1111111)))
       ((r2 cpu2 real2 gc2) (time-apply find-prime-native      (list 1111111))))
    (begin
      (display "WHILE  time (cpu, real, gc): ")
      (display cpu1)  (display ", ")
      (display real1) (display ", ")
      (display gc1)   (display "\n")
      (display "native time (cpu, real, gc): ")
      (display cpu2)  (display ", ")
      (display real2) (display ", ")
      (display gc2)   (display "\n"))))
) ; module close

(require (prefix-in mcons: 'mcons))
(require (prefix-in cons: 'cons))

; Robiąc kilkukrotne testy wydajności otrzymałem następujące wyniki:
; > (cons:test-performance)
; WHILE  time (cpu, real, gc): 6593, 6595, 1454
; native time (cpu, real, gc): 49, 48, 0
; > (cons:test-performance)
; WHILE  time (cpu, real, gc): 8354, 8354, 2923
; native time (cpu, real, gc): 50, 50, 0
; > (cons:test-performance)
; WHILE  time (cpu, real, gc): 7442, 7438, 1998
; native time (cpu, real, gc): 48, 48, 0
; > (cons:test-performance)
; WHILE  time (cpu, real, gc): 6767, 6763, 1339
; native time (cpu, real, gc): 48, 48, 0
; > (cons:test-performance)
; WHILE  time (cpu, real, gc): 6804, 6808, 1600
; native time (cpu, real, gc): 47, 46, 0
; > (cons:test-performance)
; WHILE  time (cpu, real, gc): 7191, 7187, 1876
; native time (cpu, real, gc): 49, 48, 0

; > (mcons:test-performance)
; WHILE  time (cpu, real, gc): 7047, 7046, 1363
; native time (cpu, real, gc): 51, 50, 0
; > (mcons:test-performance)
; WHILE  time (cpu, real, gc): 7157, 7153, 1555
; native time (cpu, real, gc): 51, 51, 0
; > (mcons:test-performance)
; WHILE  time (cpu, real, gc): 7903, 7906, 2098
; native time (cpu, real, gc): 50, 50, 0
; > (mcons:test-performance)
; WHILE  time (cpu, real, gc): 6856, 6854, 1199
; native time (cpu, real, gc): 49, 50, 0
; > (mcons:test-performance)
; WHILE  time (cpu, real, gc): 6818, 6819, 1163
; native time (cpu, real, gc): 49, 49, 0
; > (mcons:test-performance)
; WHILE  time (cpu, real, gc): 6680, 6676, 1227
; native time (cpu, real, gc): 46, 47, 0

; Średnie czasy interpretacji programu:
; cons:  WHILE   7192 ms
; cons:  native  48.5 ms
; mcons: WHILE   7077 ms
; mcons: native  49.3 ms

; Stąd możemy wywnioskować, że zamiana z trwałych struktur
; na struktury zmienne może przynieść niewielkie korzyści
; w przypadku własnego interpretera dla języka WHILE,
; ponieważ średni czas działania programu testowego jest krótszy,
; jednak zmiana ta będzie minimalna. Sprawdzimy jeszcze wydajność
; przeprowadzając kolejne testy dla tych samych programów działających
; na interpreterze opartym na consach oraz mconsach, a następnie
; porównamy czas ich działania. Dzięki temu będziemy mogli stwierdzić
; czy opłaca się zmieniać strukturę programów na tą opartą na mconsach.

(require rackunit)
(require rackunit/text-ui)

(define equality-test
  (test-suite "Do mcons and cons versions return the same values?"
              (check-equal? (cons:factorial 5)
                            (mcons:factorial 5)
                            "Test of factorial 5")
              (check-equal? (cons:factorial 15)
                            (mcons:factorial 15)
                            "Test of factorial 15")
              (check-equal? (cons:factorial 25)
                            (mcons:factorial 25)
                            "Test of factorial 25")
              (check-equal? (cons:find-prime-using-WHILE 5)
                            (mcons:find-prime-using-WHILE 5)
                            "Test of finding lowest prime bigger than 5")
              (check-equal? (cons:find-prime-using-WHILE 50)
                            (mcons:find-prime-using-WHILE 50)
                            "Test of finding lowest prime bigger than 50")
              (check-equal? (cons:find-prime-using-WHILE 50000)
                            (mcons:find-prime-using-WHILE 50000)
                            "Test of finding lowest prime bigger than 50000")))

; Testy dla obliczania silni danej liczby.

(define (fact_test1)
  (begin (display "\n1st test of factorial, n = 20000\n")
         (flush-output (current-output-port))
         (fact_test-performance1)))

(define (fact_test-performance1)
  (let-values (((r1 cpu1 real1 gc1) (time-apply cons:factorial  (list 20000)))
               ((r2 cpu2 real2 gc2) (time-apply mcons:factorial (list 20000))))
    (begin (display "cons  factorial (cpu, real, gc): ")
           (display cpu1)  (display ", ")
           (display real1) (display ", ")
           (display gc1)   (display "\n")
           (display "mcons factorial (cpu, real, gc): ")
           (display cpu2)  (display ", ")
           (display real2) (display ", ")
           (display gc2)   (display "\n"))))

(define (fact_test2)
  (begin (display "\n2nd test of factorial, n = 35000\n")
         (flush-output (current-output-port))
         (fact_test-performance2)))

(define (fact_test-performance2)
  (let-values (((r1 cpu1 real1 gc1) (time-apply cons:factorial  (list 35000)))
               ((r2 cpu2 real2 gc2) (time-apply mcons:factorial (list 35000))))
    (begin (display "cons  factorial (cpu, real, gc): ")
           (display cpu1)  (display ", ")
           (display real1) (display ", ")
           (display gc1)   (display "\n")
           (display "mcons factorial (cpu, real, gc): ")
           (display cpu2)  (display ", ")
           (display real2) (display ", ")
           (display gc2)   (display "\n"))))

(define (fact_test3)
  (begin (display "\n3rd test of factorial, n = 50000\n")
         (flush-output (current-output-port))
         (fact_test-performance3)))

(define (fact_test-performance3)
  (let-values (((r1 cpu1 real1 gc1) (time-apply cons:factorial  (list 50000)))
               ((r2 cpu2 real2 gc2) (time-apply mcons:factorial (list 50000))))
    (begin (display "cons  factorial (cpu, real, gc): ")
           (display cpu1)  (display ", ")
           (display real1) (display ", ")
           (display gc1)   (display "\n")
           (display "mcons factorial (cpu, real, gc): ")
           (display cpu2)  (display ", ")
           (display real2) (display ", ")
           (display gc2)   (display "\n"))))

(fact_test1)
(fact_test2)
(fact_test3)

; Z wyników testów możemy wywnioskować, że czas pracy na obu wersjach
; nieznacznie się różni, jednak nie możemy powiedzieć czy zamiana na mcons
; przynosi jakiekolwiek korzyści, ponieważ po wykonaniu kilkukrotnych testów
; raz wyniki są korzystne dla środowiska opartego na trwałych strukturach,
; a innym dla środowiska opartego na zmiennych strukturach.

; Poniżej testy dla obliczania n-tej liczby Fibonacciego:
(define (fib_test1)
  (begin (display "\n1st test of fib, n = 100000\n")
         (flush-output (current-output-port))
         (fib_test-performance1)))

(define (fib_test-performance1)
  (let-values (((r1 cpu1 real1 gc1) (time-apply cons:fib  (list 100000)))
               ((r2 cpu2 real2 gc2) (time-apply mcons:fib (list 100000))))
    (begin (display "cons  fib (cpu, real, gc): ")
           (display cpu1)  (display ", ")
           (display real1) (display ", ")
           (display gc1)   (display "\n")
           (display "mcons fib (cpu, real, gc): ")
           (display cpu2)  (display ", ")
           (display real2) (display ", ")
           (display gc2)   (display "\n"))))

(define (fib_test2)
  (begin (display "\n2nd test of fib, n = 250000\n")
         (flush-output (current-output-port))
         (fib_test-performance2)))

(define (fib_test-performance2)
  (let-values (((r1 cpu1 real1 gc1) (time-apply cons:fib  (list 250000)))
               ((r2 cpu2 real2 gc2) (time-apply mcons:fib (list 250000))))
    (begin (display "cons  fib (cpu, real, gc): ")
           (display cpu1)  (display ", ")
           (display real1) (display ", ")
           (display gc1)   (display "\n")
           (display "mcons fib (cpu, real, gc): ")
           (display cpu2)  (display ", ")
           (display real2) (display ", ")
           (display gc2)   (display "\n"))))

(define (fib_test3)
  (begin (display "\n3rd test of fib, n = 500000\n")
         (flush-output (current-output-port))
         (fib_test-performance3)))

(define (fib_test-performance3)
  (let-values (((r1 cpu1 real1 gc1) (time-apply cons:fib  (list 500000)))
               ((r2 cpu2 real2 gc2) (time-apply mcons:fib (list 500000))))
    (begin (display "cons  fib (cpu, real, gc): ")
           (display cpu1)  (display ", ")
           (display real1) (display ", ")
           (display gc1)   (display "\n")
           (display "mcons fib (cpu, real, gc): ")
           (display cpu2)  (display ", ")
           (display real2) (display ", ")
           (display gc2)   (display "\n"))))

(fib_test1)
(fib_test2)
(fib_test3)

; Z tych testów również nie możemy wywnioskować czy zmiana przyniosła
; jakąkolwiek poprawę. Czasy wykonania programu po wielu wykonaniach
; testów są bardzo różne - czasami szybciej działa wersja oparta na
; consach, innym razem na mconsach, a jeszcze innym czasy są prawie
; takie same.

; Analizując powyższe wyniki testów możemy stwierdzić, że "optymalizacja"
; z zadania 10 nie przyspieszyła interpretera, jest ona zbędna.