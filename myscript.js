const TASKS = document.getElementById('tasks');

var marioTask = document.createElement("div");
marioTask.setAttribute("class", "task");

var marioLink = document.createElement("a");
marioLink.setAttribute("class", "task-icon-link");
marioLink.setAttribute("href", "_blank");

var marioIMG = document.createElement("img");
marioIMG.setAttribute("class", "icon-mario");
marioIMG.setAttribute("src", "mario.ico");
marioIMG.setAttribute("style", "width: 24px; height: 24px; margin: 2px;");

marioLink.appendChild(marioIMG);

marioTask.appendChild(marioTask);

TASKS.appendChild(marioTask);
