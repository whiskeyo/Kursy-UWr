from datetime import date, timedelta

start = date(1601, 1, 1)
end   = date(2000,12,31)
weekdays = [0, 0, 0, 0, 0, 0, 0]

currentdate = start
currentday  = 0

while currentdate != end:
    if currentdate.day == 13:
        weekdays[currentday] += 1

    currentday = (currentday + 1) % 7
    currentdate += timedelta(days=1)

print("Poniedzialki: ", weekdays[0])
print("Wtorki:       ", weekdays[1])
print("Srody:        ", weekdays[2])
print("Czwartki:     ", weekdays[3])
print("Piatki:       ", weekdays[4])
print("Soboty:       ", weekdays[5])
print("Niedziele:    ", weekdays[6])
print("Średnio:      ", sum(weekdays)/len(weekdays))