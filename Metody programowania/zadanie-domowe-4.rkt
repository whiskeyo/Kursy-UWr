#lang racket

; Rozwiazanie na podstawie materialow z http://gauss.ececs.uc.edu/
; oraz kilku topiców ze StackOverflow.

; Implementacja merge sort:
; - dwuargumentowa procedura merge scalajaca dwie listy
; - procedura split dzielaca liste na dwie polowy
; - zestaw testow z wykorzystaniem rackunit

(define (merge left right)
  (if (null? left)
      right
      (if (null? right)
          left
          (if (< (car left) (car right))
              (cons (car left) (merge (cdr left) right))
              (cons (car right) (merge (cdr right) left))))))

(define (split ls)
  (if (null? ls)
      (list '() '())
      (if (null? (cdr ls))
          (list ls '())
          (let ([other (split (cdr (cdr ls)))])
            (list (cons (car ls) (car other))
                  (cons (car (cdr ls)) (car (cdr other))))))))

(define (merge-sort ls)
  (if (null? ls)
      null
      (if (null? (cdr ls))
          ls
          (merge (merge-sort (car (split ls)))
                 (merge-sort (car (cdr (split ls))))))))

; Zestaw testow powyzszych procedur z wykorzystaniem
; biblioteki rackunit

(require rackunit)
(require rackunit/text-ui)

(define merge-sort-tests
  (test-suite
   "Tests of procedures used in merge sort"
   (check-equal? (merge '(5) '(7))
                 (list 5 7)
                 "Merges two sorted values properly")
   (check-equal? (merge '(3) '(1))
                 (list 1 3)
                 "Merges two unsorted values properly")
   "Test of splitting procedure"
   (check-equal? (split '(1 2 3 4))
                 (list (list 1 3) (list 2 4))
                 "Splits list into two properly")
   (check-equal? (merge-sort '(17 3 1 14 2 9 10 11 0))
                 (list 0 1 2 3 9 10 11 14 17)
                 "Merge sort works fine")))

(run-tests merge-sort-tests)