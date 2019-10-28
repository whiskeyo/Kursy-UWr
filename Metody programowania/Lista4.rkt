#lang racket

; Zadanie 1 - uogolnienie append dla wielu list

#;(define (append . xs)
  (if (null? xs)
      null
      (if (null? (cdr xs))
          xs
          (apply append (cdr xs)))))

; Zadanie 2 - funkcja mirror zwracajaca lustrzane odbicie drzewa
; (mirror '(node a (node b (node c leaf leaf) leaf) (node d leaf leaf))
; => '(node a (node d leaf leaf) (node b leaf (node c leaf leaf)))

(define leaf 'leaf)

(define (leaf? x)
  (eq? x 'leaf))

(define (node e l r)
  (list 'node e l r))

(define (node-elem x)
  (second x))

(define (node-left x)
  (third x))

(define (node-right x)
  (fourth x))

(define (tagged-list? len sym x)
  (and (list? x)
       (= (length x) len)
       (eq? (first x) sym)))

(define (btree? t)
  (or (eq? t 'leaf)
      (and (tagged-list? 4 'node t)
           (btree? (node-left t))
           (btree? (node-right t)))))

(define (mirror t)
  (if (leaf? t)
      'leaf
      (node (node-elem t)
            (mirror (node-right t))
            (mirror (node-left t)))))

(define t1 (node 5 (node 3 leaf leaf) (node 6 (node 7 leaf leaf) leaf)))

; Zadanie 3 - procedura flatten do wypisania infiksowego (LVR)
; niestety nie mozna uzyc append'a :(

(define (flatten-append t)
  (if (leaf? t)
      null
      (append (flatten-append (node-left t))
              (cons (node-elem t)
                    (flatten-append (node-right t))))))

(define (flatten t)
  (define (flatten-aux t xs)
    (if (leaf? t)
        xs
        (flatten-aux (node-left t) 
                     (cons (node-elem t)
                           (flatten-aux (node-right t) xs)))))
  (flatten-aux t null))

; Zadanie 4 - treesort - sortowanie listy poprzez wstawienie
; jej elementow do BST, a nastepnie splaszczenie drzewa do listy

(define (insert x t)
  (cond
    [(leaf? t)            (node x leaf leaf)]
    [(= (node-elem t) x)  t]
    [(> (node-elem t) x)  (node (node-elem t)
                                (insert x (node-left t))
                                (node-right t))]
    [(< (node-elem t) x)  (node (node-elem t)
                                (node-left t)
                                (insert x (node-right t)))]))