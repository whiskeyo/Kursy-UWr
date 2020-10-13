var account = document.getElementById('account');
var pesel = document.getElementById('pesel');
var birth = document.getElementById('birth');
var email = document.getElementById('email');

form.addEventListener("submit", function (event) {
    if (!validateForm())
        alert('Błędne dane');
    else return true;
    event.preventDefault();
});

account.addEventListener("blur", function () {
    if (!validateAccount())
        account.style.borderColor = "red";
    else
        account.style.borderColor = "green";
});

pesel.addEventListener("blur", function () {
    if (!validatePesel())
        pesel.style.borderColor = "red";
    else
        pesel.style.borderColor = "green";
});

birth.addEventListener("blur", function () {
    if (!validateBirth())
        birth.style.borderColor = "red";
    else
        birth.style.borderColor = "green";
});

email.addEventListener("blur", function () {
    if (!validateEmail())
        email.style.borderColor = "red";
    else
        email.style.borderColor = "green";
});

function validateForm() {
    return validateAccount() && validatePesel() && validateBirth() && validateEmail();
}

function validateAccount() {
    var regex = /[0-9]{26}/;
    return regex.test(account.value) && account.value.length == 26;
}

function validatePesel() {
    var regex = /[0-9]{11}/;
    return regex.test(pesel.value) && pesel.value.length == 11;
}

function validateBirth() {
    var regex = /^(0?[1-9]|[12][0-9]|3[01])[\/\-](0?[1-9]|1[012])[\/\-]\d{4}$/;
    return regex.test(birth.value);
}

function validateEmail() {
    var regex = /^[0-9a-zA-Z_.-]+@[0-9a-zA-Z.-]+\.[a-zA-Z]{2,3}$/;
    return regex.test(email.value);
}
