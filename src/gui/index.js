const fs = require("fs");
const os = require("os");
const { copyConfFromMcast, establishConnect } = require("./pipeio.js");
establishConnect();

var configLocation;
if (process.platform == "win32") {
	configLocation = `C:\\Users\\${os.userInfo().username}\\AppData\\Roaming\\mtocptwm\\config.cfg`
}
else if (process.platform == "darwin") {
	configLocation = `/Users/${os.userInfo().username}/Library/Preferences/mtocptwm/config.cfg`
}
else {
	// probably unix-esque
	configLocation = `/home/${os.userInfo().username}/.config/mtocptwm/config.cfg`
}
if (!fs.existsSync(configLocation)) {
	console.log("config doesn't exist, do setup")
	document.addEventListener("DOMContentLoaded", doSetup);
}
function doSetup() {
	var title = document.getElementById('title');
	title.textContent = title.textContent.replace("mtocptwm", "mtocptwm setup")
	document.body.insertAdjacentHTML("beforeend",
`
<p>It looks like this is your first time using mtocptwm. Please choose an option to begin setup</p>
<button onclick="copyConfFromMcast()">Copy config from another instance</button>
<button onclick="newConfig()">Create new config</button>
`
	)
}