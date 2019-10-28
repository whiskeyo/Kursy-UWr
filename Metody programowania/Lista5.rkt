#lang racket

; Formuły rachunku zdań
; Zadanie 1 - konstruktory i selektory dla neg, conj, disj

(define (var x)
  x)

(define (var-name x)
  x)

(define (var? t)
  (symbol? t))

(define (neg t)
  (list 'neg t))

(define (neg-subf t)
  (second t))

(define (neg? t)
  (and (list? t)
       (= 2 (length t))
       (eq? 'neg (car t))))

(define (conj t1 t2)
  (list 'conj t1 t2))

(define (conj-left t)
  (second t))

(define (conj-right t)
  (third t))

(define (conj? t)
  (and (list? t)
       (= 3 (length t))
       (eq? 'conj (car t))))

(define (disj t1 t2)
  (list 'disj t1 t2))

(define (disj-left t)
  (second t))

(define (disj-right t)
  (third t))

(define (disj? t)
  (and (list? t)
       (= 3 (length t))
       (eq? 'disj (car t))))

(define (prop? f)
  (or (var? f)
      (and (neg? f)
           (prop? (neg-subf f)))
      (and (disj? f)
           (prop? (disj-left f))
           (prop? (disj-right f)))
      (and (conj? f)
           (prop? (conj-left f))
           (prop? (conj-right f)))))

; Twierdzenia o indukcji:
; Dla dow. f: (prop? f) => (prop? (neg f))
; Dla dow. f1, f2: (prop? f1) & (prop? f2) => (prop? (disj f1 f2))
; Dla dow. f1, f2: (prop? f1) & (prop? f2) => (prop? (conj f1 f2))

; Zadanie 2 - procedura znajdująca zbiór zmiennych wolnych formuły
; reprezentowany za pomocą listy.

(define (free-vars f)
  (define (iter f xs)
    (cond [(neg? f)   (iter (neg-subf f) xs)]
          [(disj? f)  (iter (disj-right f) (iter (disj-left f) xs))]
          [(conj? f)  (iter (conj-right f) (iter (conj-left f) xs))]
          [else (remove-duplicates (append xs (list f)))]))
  (if (prop? f)
      (iter f null)
      (error "Something went wrong.")))

; Zadanie 3 - procedura gen-vals gerenuje zbiór wszystkich wartościowań
; dla danego zbioru zmiennych. Definiujemy procedurę eval-formula
; przyjmującą formułę i wartościowanie, która obliczy wartość logiczną
; formuły dla tego wartościowania.
; Następnie definiujemy procedurę falsifiable-eval? przyjmującą formułę
; logiczną, która zwraca wartościowanie przy którym jest ona fałszywa
; lub false jeśli formuła jest tautologią.

(define (gen-vals xs)
  (if (null? xs)
      (list null)
      (let* ([vss (gen-vals (cdr xs))]
             [x   (car xs)]
             [vst (map (lambda (vs) (cons (list x true)  vs)) vss)]
             [vsf (map (lambda (vs) (cons (list x false) vs)) vss)])
        (append vst vsf))))

(define (lookup x sigma) ; funkcja sprawdzajaca wartosciowanie zmiennej
  (if (member (list x #t) sigma)
      #t
      (if (member (list x #f) sigma)
          #f
          (void))))

(define (eval-formula f sigma)
  (cond [(var? f)  (let ([var-sigma (lookup f sigma)])
                     (if (void? var-sigma)
                         (error "Value not found")
                         var-sigma))]
        [(conj? f) (and (eval-formula (conj-left f)  sigma)
                        (eval-formula (conj-right f) sigma))]
        [(disj? f) (or  (eval-formula (disj-left f)  sigma)
                        (eval-formula (disj-right f) sigma))]
        [(neg? f)  (not (eval-formula (neg-subf f)   sigma))]))

#;(define (falsifiable-eval? f)
  (let ([sigma (gen-vals (free-vars f))]))) ; zbiór wszystkich wartościowań

(define formula-one
  (conj 'p (disj (neg 'p) 'q))) ; p & (!p + q)

(define sigma-one
  (gen-vals '(p q)))

; Zadanie 4 - predykat nnf? mówiący czy formuła zdaniowa jest w
; negacyjnej postaci normalnej. Warto zdefiniować dodatkowy typ danych
; reprezentujący literały (tj. potencjalne zanegowane zmienne zdaniowe).
; Następnie rozszerzamy procedury free-vars i eval-formula tak, żeby
; działały dla zwykłych formuł zdaniowych, jak i tych w NNF.

(define (literal? l) ; literał to zmienna lub zanegowana zmienna
  (or (var? l)
      (and (neg? l)
           (var? (neg-subf l)))))

(define (nnf? f)
  (or (literal? f)
      (and (disj? f)
           (nnf? (disj-left f))
           (nnf? (disj-right f)))
      (and (conj? f)
           (nnf? (conj-left f))
           (nnf? (conj-right f)))))

; Zadanie 5 - procedura convert-to-nnf przekształcająca formułę do
; równoważnej jej formuły w NNF. Translacja powinna być strukturalna,
; tzn. rekurencyjnie wywoływać będzie się wyłącznie na podformułach
; argumentu. Warto utworzyć dodatkową procedurę, która będzie
; wzajemnie rekurencyjna z convert-to-nnf. Następnie udowodnimy
; że dla procedury zachodzi:
; Dla dowolnego p: (prop? p) => (nnf? (convert-to-nnf p)).

; Wykorzystujemy do tego zadania prawa de Morgan'a:
; !(p&q) = !p + !q
; !(p+q) = !p & !q
; !p = p

(define (convert-to-nnf f)
  (cond [(var? f)  f]
        [(neg? f)  (neg-convert (neg-subf f))]
        [(conj? f) (conj (convert-to-nnf (conj-left f))
                         (convert-to-nnf (conj-right f)))]
        [(disj? f) (disj (convert-to-nnf (disj-left f))
                         (convert-to-nnf (disj-right f)))]))
(define (neg-convert f)
  (cond [(var? f)  (neg f)]
        [(neg? f)  (convert-to-nnf (neg-subf f))]
        [(conj? f) (disj (neg-convert (conj-left f))
                         (neg-convert (conj-right f)))]
        [(disj? f) (conj (neg-convert (disj-left f))
                         (neg-convert (disj-right f)))]))

; Dla dow. f: (prop? f) => f  EQUAL (convert-to-nnf f)
;                        & !f EQUAL (neg-convert f)
; Indukcja po budowie f:
; (1) Baza (zmienna): f   EQUAL (convert-to-nnf f)  - z definicji
;                     !f  EQUAL (neg-convert f)     - z definicji
; (2) Krok (negacja): !f  EQUAL (convert-to-nnf !f)
;                         EQUAL (neg-convert f)
;                     f   EQUAL (convert-to-nnf f)
;                   & !f  EQUAL (neg-convert !f)
;                CEL: !f  EQUAL (convert-to-nnf !f)
;                   & !!f EQUAL (neg-convert !f) (?)
; (3)
  
; Zadanie 6 - reprezentacja formuł w koniunkcyjnej postaci normalnej
; oraz predykat cnf?, a także potrzebne konstruktory i selektory.
; Następnie definiujemy procedurę convert-to-cnf przekształcającą
; formułę NNF do CNF. Tutaj translacja też powinna być strukturalna
; (trudność polega na zrozumieniu co zrobić z formułami w CNFie
; otrzymanymi z translacji podformuł).

; Procedury pomocnicze

(define (tagged-tuple? tag len x)
  (and (list? x)
       (=   len (length x))
       (eq? tag (car x))))

(define (tagged-list? tag x)
  (and (pair? x)
       (eq? tag car x)
       (list? cdr x)))

; Definicje klauzul 

(define (clause? c)
  (and (tagged-list? 'clause c)
       (andmap lit? (cdr c))))

(define (clause . lits)
  (cons 'clause lits))

(define (clause-lits c)
  (cdr c))

(define (cnf . clauses)
  (cons 'cnf clauses))

(define (cnf-clauses f)
  (cdr f))

(define (cnf? f)
  (and (tagged-list? 'cnf f)
       (andmap clause? (cdr f))))

; Definicje literałów

(define (lit pol var)
  (list 'lit pol var))

(define (pos-l x)
  (lit true (var x)))

(define (neg-l x)
  (lit false (var x)))

(define (lit? x)
  (and (tagged-tuple? 'lit 3 x)
       (boolean? (second x))
       (var? (third x))))

(define (lit-pol l)
  (second l))

(define (lit-var l)
  (third l))

; Procedura tworzy wszystkie pary z list, np. dla (a b c) oraz (d e)
; otrzymamy (a d) (b d) (c d) (a e) (b e) (c e)

(define (cartesian xs ys) 
  (apply append (map (lambda (y)         
                       (map (lambda (x) (cons x y)) xs) ys))))

(define (pack-clauses xsys)
  (apply clause (append (clause-lits (car xsys))
                        (clause-lits (cdr xsys)))))

(define (convert-to-cnf f)
  (cond [(lit? f)      (cnf (clause f))] 
        [(conj? f)     (let ([l (convert-to-cnf (conj-left f))]
                             [r (convert-to-cnf (conj-right f))])
                         (apply cnf (append (cnf-clauses l) (cnf-clauses r))))]
        [(disj? f)     (let ([l (convert-to-cnf (disj-left f))]
                             [r (convert-to-cnf (disj-right f))])
                         (apply cnf (map pack-clauses
                                         (cartesian (cnf-clauses l)
                                                    (cnf-clauses r)))))]))

; Zadanie 7 - procedura falsifiable-cnf? przyjmująca formułę zdaniową,
; która tłumaczy ją do CNFu i na tej podstawie znajduje fragment
; wartościowania który falsyfikuje wejściową formułę (lub zwraca false
; gdy formuła jest tautologią). Porównaj działanie dwóch procedur
; sprawdzających czy formuła jest tautologią.