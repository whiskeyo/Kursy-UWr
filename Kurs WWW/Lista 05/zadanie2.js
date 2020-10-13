let marka = ["Ford", "Fiat"];
modeldict = {
    "Ford": ["Focus", "Escort"],
    "Fiat": ["126p", "Seicento"]
};


let marka_element = document.getElementsByName("marka")[0];
let model_element = document.getElementsByName("model")[0];
let input = document.getElementsByName("nowy")[0];
let radio_marka = document.getElementsByName("rodzaj")[0];
let radio_model = document.getElementsByName("rodzaj")[1];
let button = document.getElementsByName("dodaj")[0];

function updateMarka() 
{
    let options = "";
    for (element of marka) 
        options += '<option value="' + element + '">' + element + '</option>';
    marka_element.innerHTML = options;
}

function updateModel() 
{
    options = "";
    for (model of modeldict[marka_element.value]) 
        options += '<option value="' + model + '">' + model + '</option>';
    model_element.innerHTML = options;
}

function buttonClicked() {
    if (radio_marka.checked) 
    {
        marka.push(input.value);
        modeldict[input.value] = [];
        updateMarka();
    } 
    else if (radio_model.checked) 
    {
        modeldict[marka_element.value].push(input.value);
        updateModel();
    }
}

updateMarka();
updateModel();

marka_element.addEventListener('change', updateModel);
