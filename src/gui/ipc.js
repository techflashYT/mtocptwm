const net = require("node:net")
const child_process = require("node:child_process");
ipcSock = null;



function readSockSync(timeout) {
	return new Promise((resolve, reject) => {
		let dataReceived = false;

		function onData(data) {
			dataReceived = true;
			ipcSock.off('data', onData);
			resolve(data);
		}

		ipcSock.on('data', onData);

		setTimeout(() => {
			if (!dataReceived) {
				ipcSock.off('data', onData);
				reject(new Error('Socket data wait timed out'));
			}
		}, timeout);
	});
}


/*
	attemptCallback must be in format:
	function callback(attemptNum [int], success [bool]) { ... }

	initFunc and failCallback take no args
*/
async function copyConfFromMcast(initFunc, attemptCallback, failCallback) {
	if (initFunc !== undefined && initFunc !== null) {
		initFunc();
	}

	for (var i = 0; i != 5; i++) {
		ipcSock.write("mcastCopy\0");
		const data = await readSockSync(5000);

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
	ipcSock.connect(40435, "127.0.0.1");
}
module.exports = {
	copyConfFromMcast,
	establishConnect
}