function BMI()
{
    let mass = parseInt(document.querySelector("#mass").value);
    let h = parseInt(document.querySelector("#h").value);

    let meters = h/100;
    let bmi = (mass/(meters*meters)).toFixed(2);

    let outp = document.querySelector("#out");

    outp.style.visibility = "visible";

    if ((bmi < 18.5) || (bmi > 28))
    {
        outp.style.color= "red";
        outp.innerHTML = "Your BMI: " + bmi;
    }
    else
    {
        outp.style.color= "green";
        outp.innerHTML = "Your BMI: " + bmi;
    }
}