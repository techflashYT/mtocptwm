const { app, BrowserWindow } = require('electron')
const path = require('path')
try {
	require('electron-reloader')(module)
} catch (_) {}

const createWindow = () => {
	const win = new BrowserWindow({
		webPreferences: {
			preload: path.join(__dirname, 'preload.js')
		}
	});

	win.loadFile('index.html')
}
app.on('window-all-closed', () => {
	if (process.platform !== 'darwin') app.quit()
})
app.whenReady().then(() => {
	createWindow()

	app.on('activate', () => {
		if (BrowserWindow.getAllWindows().length === 0) createWindow()
	})
})