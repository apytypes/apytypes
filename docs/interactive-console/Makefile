
console.js: console.ts
	@deno bundle 					\
		--platform browser 			\
		--packages=external 		\
		--external=@xterm/xterm 	\
		--external=@xterm/addon-fit \
		--external=pyodide 			\
		console.ts -o main.js
