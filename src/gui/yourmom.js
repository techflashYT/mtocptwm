function loadInterface() {

	const path = './mtocptwm.cfg'

	try {
		if (fs.existsSync(path)) {
			test = document.createElement("h1")
			test.innerText = "YES CONFIG"
			document.appendChild(test)
		}
		else {
			test = document.createElement("h1")
			test.innerText = "NO CONFIG"
			document.appendChild(test)
		}
	} catch (err) {
		console.error(err)
	}
}

