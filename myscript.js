const TASKS = document.getElementById('tasks');

var marioTask = document.createElement("div");
marioTask.setAttribute("class", "task");

var marioLink = document.createElement("a");
marioLink.setAttribute("class", "task-icon-link");
marioLink.setAttribute("href", "/plugin/global-build-stats/");

var marioIMG = document.createElement("img");
marioIMG.setAttribute("class", "icon-mario");
marioIMG.setAttribute("src", "https://cdn.rawgit.com/tung-lethanh6596/IPC-Ex/f2da783a/mario.ico");
marioIMG.setAttribute("style", "width: 24px; height: 24px; margin: 2px;");

var marioText = document.createElement("a");
marioText.setAttribute("class", "task-link");
marioText.setAttribute("href", "/plugin/global-build-stats/");

var captionText = document.createTextNode("Play Mario");
marioText.appendChild(captionText);

marioLink.appendChild(marioIMG);
marioTask.appendChild(marioLink);
marioTask.appendChild(marioText);

TASKS.appendChild(marioTask);
