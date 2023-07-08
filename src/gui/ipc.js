debugger;
const net = require("node:net")
ipcSock = null;
function copyConfFromMcast() {
	
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