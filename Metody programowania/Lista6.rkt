#lang racket

; Zadanie 1 - cytowanie wyłącznie przed identyfikatorami
; (tj. nie cytujemy nic poza symbolami). Sprawdzamy czy
; poszczególne wyrażenia są równe za pomocą (equal? x y).

(define e1 '(+ 4 3 (* 5 8)))
(define e2 '(+ 4 3 '(* 5 8)))
(define e3 `(+ 4 3 (5 . 8)))
(define e4 `(+ 4 3 ,(* 5 8)))

(define a1 (list '+ 4 3 (list '* 5 8)))
(define a2 (list '+ 4 3 (list 'quote (list '* 5 8))))
(define a3 (list '+ 4 3 (cons 5 8)))
(define a4 (list '+ 4 3 40))

; Zadanie 2 - procedura paths zwracająca listę ścieżek
; w drzewie binarnym. Ścieżka powinna być listą etykiet
; w kolejności od korzenia do liścia, list kończący
; ścieżkę powinien znaleźć się w odpowiedzi jako symbol '*.

(struct node (v l r) #:transparent)
(struct leaf () #:transparent)

(define (tree? t)
  (match t
    [(leaf) true]
    [(node _ l r) (and (tree? l) (tree? r))]
    [_ false]))

; Zadanie 3 - procedura zwracająca czy w danym wyrażeniu
; arytmetycznym jest więcej mnożeń czy dodawań.

(define expr1 (list '+ 1 2 (list '* 3 4)))

(define (count-operations expr)
  (if (null? expr)
      0
      (let ([x (car expr)]
            [y (cdr expr)])
        (cond [(list? x)  (+ (count-operations x) (count-operations y))]
              [(eq? x '+) (- (count-operations y) 1)]
              [(eq? x '*) (+ (count-operations y) 1)]
              [else (count-operations y)]))))

; Zadanie dodatkowe - zamień wszystkie operacje dodawania
; w wyrażeniu arytmetycznym na mnożenie.

(define expr2 (list '+ 1 2 (list '+ 3 4)))

(define (change-operator expr)
  (if (list? expr)
      (let ([head (if (eq? '+ (car expr))
                      '*
                      (car expr))]
            [tail (map change-operator (cdr expr))])
        (cons head tail))
      expr))
          