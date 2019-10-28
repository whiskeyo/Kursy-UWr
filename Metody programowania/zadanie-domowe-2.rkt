#lang racket

; Procedury pomocnicze:

(define tolerance 0.00001)

(define (inc number)
  (+ number 1))

(define (good-enough? a b)
  (> tolerance (abs (- a b))))

(define (square a)
  (* a a))

; Procedura do wyznaczenia wartosci A_n lub B_n
; (ich wzory sa takie same, roznia sie wyrazeniami
; poczatkowymi, ktore procedura bedzie przyjmowac).

(define (calculate-nth-value
         num den x-1 x-2 k)
  (+ (* (den k) x-1) (* (num k) x-2)))

; Procedura glowna wykonujaca polecenie z zadania:

(define (ex-10 num den)
  (define (calculate-f_k
           A B next-A next-B k)
    (if (good-enough? (/ A B) (/ next-A next-B))
        (/ next-A next-B)
        (calculate-f_k next-A
                       next-B
                       (calculate-nth-value num den next-A A (inc k))
                       (calculate-nth-value num den next-B B (inc k))
                       (inc k))))
    (calculate-f_k 0    ; A_0
                   1.0  ; B_0
                   (calculate-nth-value num den 0 1 1) ; A_1
                   (calculate-nth-value num den 1 0 1) ; B_1
                   1))

; Procedura wyznaczajaca arctan(x):

(define (arctan x)
  (define (num k)
    (if (= k 1)
        x
        (square (* (- k 1) x))))
  (define (den k)
    (- (* 2 k) 1))
  (ex-10 num den))

; Testy sprawdzajace dzialanie:

(arctan 0)
(atan 0)
(arctan 0.07)
(atan 0.07)
(arctan 0.5)
(atan 0.5)
(arctan 1)
(atan 1)
(arctan 5)
(atan 5)
(arctan 13)
(atan 13)
(arctan 17)
(atan 17)
(arctan 18)
(atan 18)
(arctan 40)
(atan 40)

; Metoda z zadania sprawdza się do obliczania arctan dla argumentów
; nie większych niż około 17 (dla wartości wyższych niż 17 z rozwinięciem
; dziesiętnym program wpada w nieskończoną pętlę), dla wartości całkowitych,
; zaczynając od 18, B_n rośnie bardzo szybko, dążąc do nieskończoności,
; przez co otrzymany wynik jest równy 0. Można jednak zaimplementować
; procedurę, która zatrzymywałaby działanie, gdy B osiągnie bardzo dużą wartość.

(define (in-range? a b B)
  (cond [(> tolerance (abs (- a b))) #t]
        [(> B 10E100) #t]
        [else #f]))

(define (ex-10-v2 num den)
  (define (calculate-f_k-v2
           A B next-A next-B k)
    (if (in-range? (/ A B) (/ next-A next-B) next-B)
        (/ next-A next-B)
        (calculate-f_k-v2 next-A
                          next-B
                          (calculate-nth-value num den next-A A (inc k))
                          (calculate-nth-value num den next-B B (inc k))
                          (inc k))))
  (calculate-f_k-v2 0    ; A_0
                    1.0  ; B_0
                    (calculate-nth-value num den 0 1 1) ; A_1
                    (calculate-nth-value num den 1 0 1) ; B_1
                    1))

; Zmieniona definicja arctan(x), aby korzystała z procedury (ex-10-v2)

(define (arctan-v2 x)
  (define (num k)
    (if (= k 1)
        x
        (square (* (- k 1) x))))
  (define (den k)
    (- (* 2 k) 1))
  (ex-10-v2 num den))

; Testy sprawdzające działanie przedefiniowanej procedury:

"Wartości dla przedefiniowanej procedury obliczajacej arctan x:"
(arctan-v2 0)
(atan 0)
(arctan-v2 0.07)
(atan 0.07)
(arctan-v2 0.5)
(atan 0.5)
(arctan-v2 1)
(atan 1)
(arctan-v2 5)
(atan 5)
(arctan-v2 13)
(atan 13)
(arctan-v2 17)
(atan 17)
(arctan-v2 18)
(atan 18)
(arctan-v2 40)
(atan 40)

; Procedura z ograniczeniem wielkości wyrażenia B wylicza już wartość
; dla kolejnych argumentów arctan, jednak są one zbyt odległe od
; poprawnych wartości.