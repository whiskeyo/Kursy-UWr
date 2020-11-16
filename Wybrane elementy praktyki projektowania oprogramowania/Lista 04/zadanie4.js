process.stdout.write("Witaj, podaj swoje imię: ");

process.stdout.on('data', function(data) {
    process.stdout.write("Wprowadziłeś dane: " + data);
    process.exit();
})

process.on('exit', function() {
    process.stdout.write("Do zobaczenia!\n");
})