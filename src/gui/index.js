const { app, BrowserWindow } = require('electron')
const path = require('path')
const fs = require('fs')

const config = './mtocptwm.cfg'

try {
	//require('electron-reloader')(module)
} catch (_) { }

const createWindow = () => {
	const win = new BrowserWindow({
		webPreferences: {
			preload: path.join(__dirname, 'preload.js')
		}
	});

	try {
		if (fs.existsSync(config)) {
			win.loadFile('index.html')
		}
		else {
			win.loadFile('setup.html')
		}
	} catch (err) {
		console.error(err)
	}

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