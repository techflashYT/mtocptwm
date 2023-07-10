const { app, BrowserWindow } = require('electron')
const path = require('path')


try {
	require('electron-reloader')(module)
} catch (_) { }
var win;
const createWindow = () => {
	win = new BrowserWindow({
		webPreferences: {
			nodeIntegration: true,
			contextIsolation: false,
			enableRemoteModule: true,
		}
	});

	win.loadFile('index.html')
}
app.on("quit", () => {
	ipcSock.write("exit\0");
	ipcSock.destroy();
})
app.on('window-all-closed', () => {
	if (process.platform !== 'darwin') app.quit()
})
app.whenReady().then(() => {
	createWindow()
	app.on('activate', () => {
		if (BrowserWindow.getAllWindows().length === 0) createWindow()
	})
})
