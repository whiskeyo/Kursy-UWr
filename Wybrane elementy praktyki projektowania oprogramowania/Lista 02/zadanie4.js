var color1 = new String("green"); //ciąg utworzony przy pomocy konstruktora
color1 instanceof String; // true
typeof color1; //object

var color2 = "coral"; //literał łańcuchowy
color2 instanceof String; // false
typeof color2; //string

console.log(color2 instanceof String)
console.log(color1 instanceof String)
console.log(typeof color1)
console.log(typeof color2)

// instanceof sprawdza czy dany obiekt był utworzony danym konstruktorem