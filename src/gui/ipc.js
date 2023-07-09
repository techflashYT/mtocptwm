const net = require("node:net")
ipcSock = null;
/*
	attemptCallback must be in format:
	function callback(attemptNum [int], success [bool]) { ... }

	initFunc and failCallback take no args
*/
function copyConfFromMcast(initFunc, attemptCallback, failCallback) {
	if (initFunc !== undefined && initFunc !== null) {
		initFunc();
	}

	for (var i = 0; i != 5; i++) {
		ipcSock.write("mcastCopy\0");

		var success = false;
		if (attemptCallback !== undefined && attemptCallback !== null) {
			attemptCallback(i, success);
		}
	}
	

}
function establishConnect() {
	ipcSock = new net.Socket();
	ipcSock.on("ready", () => {
		console.log("connected to server");
		ipcSock.write("hello ipc\0");
	})
	ipcSock.on("data", (data) => {
		console.log(`data: ${data}`)
	})
	ipcSock.connect(40435, "127.0.0.1");
}
module.exports = {
	copyConfFromMcast,
	establishConnect
}