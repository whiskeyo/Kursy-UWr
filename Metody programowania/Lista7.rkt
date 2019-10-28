#lang racket

; Zadanie 1 - rozszerz składnię abstrakcyjną o potęgowanie a^b,
; sumę wartości kolejnych liczb naturalnych, całkę oznaczoną,
; oblicanie minimum zbioru.

(struct const    (val)      #:transparent)
(struct op       (symb l r) #:transparent)
(struct let-expr (x e1 e2)  #:transparent)
(struct variable (x)        #:transparent)
(struct sum      (i b e f)  #:transparent) ; index, begin, end, func
(struct integral (a b f x)  #:transparent)
(struct minimum  (i f)      #:transparent)

(define (expr? e)
  (match e
    [(variable s)       (symbol? s)]
    [(const n)          (number? n)]
    [(op s l r)         (and (member s '(+ * ^))
                             (expr? l)
                             (expr? r))]
    [(let-expr x e1 e2) (and (symbol? x)
                             (expr? e1)
                             (expr? e2))]
    [(sum i b e f)      (and (symbol? i)
                             (natural? b)
                             (natural? e)
                             (expr? f))]
    [(integral a b f x) (and (symbol? x)
                             (expr? a)
                             (expr? b)
                             (expr? f))]
    [(minimum i f)      (and (symbol? i)
                             (expr? f))]
    [(if-st cond T F)   (and (expr? cond)
                                  (expr? T)
                                  (expr? F))]
    [(cond-st pairs)    (cond-pairs? pairs)]
  #;[(lambda-e arg exp) (and (symbol-list? args)
                             (expr? exp))]
    [_                  false]))

; Nie potrafimy przedstawić wyrażenia z zadania, ponieważ
; brakuje nam nieskończoności oraz dzielenia.

; Zadanie 2 - zaproponuj składnię abstrakcyjną następujących
; konstrukcji występujących w Rackecie: if, cond, lambda.
; Predykat expr? uzupełniony jest w 1. zadaniu.

(struct if-st       (condition if-true if-false) #:transparent)
(struct cond-st     (pairs)                      #:transparent)
(struct lambda-e    (arguments expression)       #:transparent)

(define (cond-pair? x)
  (match x
    [(cons cond expr) (and (expr? cond)
                           (expr? expr))]
    [_                false]))

(define (cond-pairs? pairs)
  (or (null? pairs)
      (and (cons? pairs)
           (cond-pair? (car pairs))
           (cond-pair? (cdr pairs)))))

; Zadanie 3 - procedura from-quote przetwarzająca zacytowane
; wyrażenie arytmetyczne na składnię abstrakcyjną wyrażeń
; arytmetycznych z wykładu.

(define (neutral op) ; element neutralny (+ 0, * 1)
  (if (eq? op '+)
      0
      1))

(define (from-quote expr)
  (cond [(number? expr)      (const expr)]
        [(symbol? expr)      (variable expr)]
        [(list? expr)        (let ([oper (car expr)]
                                   [args (cdr expr)])
                               (foldr (lambda (arg exp)
                                        (op oper
                                            (from-quote arg)
                                            exp))
                                      (const (neutral oper))
                                      args))]))

; Powyższa procedura za każdym razem dodaje element neutralny
; do wyniku, tzn. wywołanie (from-quote '(+ 2 (* 1 2) 4)) zwróci:
; (op
; '+
; (const 2)
; (op '+ (op '* (const 1) (op '* (const 2) (const 1))) (op '+ (const 4) (const 0))))

; Zadanie 7 - procedura make-cycle zmieniająca listę modyfikowalną
; podaną jako jedyny parametr na listę cykliczną, nadpisując mcdr
; ostatniej pary należącej do listy, aby wskazywała na pierwszą.
; Przydatne procedury: mcons, set-mcar!, set-mcdr!.

(define (append! xs ys) ; xs nie może być nullem
  (if (= (mcdr xs) '()) ; tu się wysypuje xd
      (set-mcdr! xs ys)
      (append! (mcdr xs) ys)))

(define (make-cycle xs)
  (begin
    (if (null? xs)
      '()
       (append! xs xs))
    xs))