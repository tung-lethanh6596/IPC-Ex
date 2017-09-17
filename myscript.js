const TASKS = document.querySelector("#tasks");

var myOption = document.createElement("div");
myOption.setAttribute("class", "task");
var optionText = document.createTextNode("My Option");
myOption.appendChild(optionText);

TASKS.appendChild(myOption);
