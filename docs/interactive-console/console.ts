/*
 * Interactive Pyodide console with APyTypes loaded. This is a TypeScript module that
 * should be compiled into JavaScript and then loaded into HTML.
 */

import { FitAddon } from "@xterm/addon-fit";
import { Terminal } from "@xterm/xterm";
import { loadPyodide } from "pyodide";

import {
    REPLConsole,
    refreshLine,
    handleTabCompletion,
    execLine,
    setBuffer,
} from "./util.ts";


/**
 * Load the APyTypes-in-Pyodide interactive console into an HTML `div` element
 */
export async function APyTypesConsole(
    webElement: HTMLDivElement,
    pyodideIndexURL: string = "https://cdn.jsdelivr.net/pyodide/v0.28.2/full",
) {
    const fitAddon = new FitAddon();
    const term = new Terminal({
        cursorBlink: true,
        cursorStyle: "block",
        convertEol: true,
        fontFamily: "monospace",
        lineHeight: 1.4,
        theme: { background: "#111" },
    });

    term.loadAddon(fitAddon);
    term.open(webElement);
    fitAddon.fit();
    term.focus();
    term.write('Xterm.js v6.0.0\r\n');
    term.write('Loading APyTypes in Pyodide...\r\n');

    // Load Pyodide. The version of Pyodide has to match exactly with that of the
    // APyTypes WASM32 wheel
    const pyodide = await loadPyodide({ indexURL: pyodideIndexURL });
    pyodide._api.on_fatal = (e: Error) => {
        if (e.name === "Exit") {
            term.write(`\x1b[31m${e}\x1b[0m\r\n`);
            term.write("\x1b[31mPyodide exited and can no longer be used.\x1b[0m\r\n");
        } else {
            term.write(
                ""
                + "\x1b[31mPyodide has suffered a fatal error. "
                + "Please report this to the Pyodide maintainers.\x1b[0m\r\n"
            );
            term.write("\x1b[31mThe cause of the fatal error was:\x1b[0m\r\n");
            term.write(`\x1b[31m${e.message || e}\x1b[0m\r\n`);
            term.write("\x1b[31mLook in browser console for more details.\x1b[0m\r\n");
        }
    };

    // Load the APyTypes wheel
    await pyodide.loadPackage("micropip")
    const micropip = pyodide.pyimport("micropip")
    await micropip.install(
        ""
        + "https://raw.githubusercontent.com/apytypes/WASM32-v0.5.0.dev84-g969f0dc47/"
        + "main/wheels/"
        + "apytypes-0.5.0.dev84+g969f0dc47-cp313-cp313-pyodide_2025_0_wasm32.whl"
    )

    // Print an initial banner
    const { repr_shorten, BANNER, PyodideConsole }
        = pyodide.pyimport("pyodide.console");
    term.clear();
    term.writeln(
        ""
        + `Welcome to APyTypes playground 🎉\r\n`
        + `This is an interactive Python console running in your browser using `
        + `Pyodide.\r\n`
        + `APyTypes is already imported as 'apy' and ready for use.\r\n\r\n`
        + `${BANNER}`
    )

    const pyconsole = PyodideConsole(pyodide.globals);

    const namespace = pyodide.globals.get("dict")();
    const await_fut = pyodide.runPython(
      `
        import builtins
        from pyodide.ffi import to_js
        async def await_fut(fut):
            res = await fut
            if res is not None:
                builtins._ = res
            return to_js([res], depth=1)
        await_fut
      `,
      { globals: namespace }
    );
    namespace.destroy();

    pyconsole.stdout_callback = (s: string) => term.write(s);
    pyconsole.stderr_callback = (s: string) => term.write(`\x1b[31m${s}\x1b[0m`);

    // Import APyTypes and print the version to verify it works.
    await pyconsole.push("import apytypes as apy");
    await pyconsole.push("print('APyTypes version:', apy.__version__)");


    // REPL implementation
    const ps1 = ">>> ";
    const repl: REPLConsole = {
        buffer: "", prompt: ps1, cursorIdx: 0, history: [], historyIdx: null,
    };

    // Handle some custom key events
    term.attachCustomKeyEventHandler((ev) => {

        // Only handle when the terminal has focus
        if (ev.type !== "keydown") return true;

        // Prevent page scroll / browser navigation behavior
        if (ev.key === "Home") {
            ev.preventDefault();
            ev.stopPropagation();

            // Move to start of input buffer (right after the prompt)
            repl.cursorIdx = 0;
            refreshLine(term, repl);
            return false; // don't let xterm do anything else with it
        }

        if (ev.key === "End") {
            ev.preventDefault();
            ev.stopPropagation();

            repl.cursorIdx = repl.buffer.length;
            refreshLine(term, repl);
            return false;
        }

        // (Optional) stop PageUp/PageDown from scrolling the whole webpage
        if (ev.key === "PageUp" || ev.key === "PageDown") {
            ev.preventDefault();
            ev.stopPropagation();
            return false;
        }

        return true;
    });


    term.write(repl.prompt);
    term.onData(async (data) => {
        switch (data) {
            case "\t": // TAB
                handleTabCompletion(term, repl, pyconsole);
                break;
            case "\r": // Enter
                term.write("\r\n");
                await execLine(
                    term, repl, pyconsole, await_fut, repr_shorten, pyodide, repl.buffer
                );
                repl.buffer = "";
                repl.cursorIdx = 0;
                term.write(repl.prompt);
                break;
            case "\u0003": // Ctrl-C
                pyconsole.buffer.clear();
                repl.buffer = "";
                repl.cursorIdx = 0;
                term.write("^C\r\nKeyboardInterrupt\r\n" + ps1);
                repl.prompt = ps1;
                repl.historyIdx = null;
                break;
            case "\u0016": { // Ctrl-V
                // paste from clipboard
                const clipboard = await navigator.clipboard.readText();
                const newBuf =
                    repl.buffer.slice(0, repl.cursorIdx) +
                    clipboard +
                    repl.buffer.slice(repl.cursorIdx);
                setBuffer(term, repl, newBuf, newBuf.length);
                break;
            }
            case "\u007F": // Backspace
                if (repl.cursorIdx > 0) {
                    const before = repl.buffer.slice(0, repl.cursorIdx - 1);
                    const after = repl.buffer.slice(repl.cursorIdx);
                    repl.cursorIdx -= 1;
                    setBuffer(term, repl, before + after, repl.cursorIdx);
                }
                break;
            case "\x1B[A": // Up arrow
                if (repl.prompt === ps1) {
                    if (repl.historyIdx === null) {
                        repl.historyIdx = history.length;
                    }
                    if (repl.historyIdx > 0) {
                        repl.historyIdx -= 1;
                        const newBuf = repl.history[repl.historyIdx] || "";
                        setBuffer(term, repl, newBuf, newBuf.length);
                    }
                }
                break;
            case "\x1B[B": // Down arrow
                if (repl.prompt === ps1 && repl.historyIdx !== null) {
                    if (repl.historyIdx < history.length - 1) {
                        repl.historyIdx += 1;
                        const newBuf = repl.history[repl.historyIdx] || "";
                        setBuffer(term, repl, newBuf, newBuf.length);
                    } else {
                        repl.historyIdx = null;
                        setBuffer(term, repl, "", 0);
                    }
                }
                break;
            case "\x1B[C": // Right arrow
                if (repl.cursorIdx < repl.buffer.length) {
                    repl.cursorIdx += 1;
                    refreshLine(term, repl);
                }
                break;
            case "\x1B[D": // Left arrow
                if (repl.cursorIdx > 0) {
                    repl.cursorIdx -= 1;
                    refreshLine(term, repl);
                }
                break;
            default:
                if (data) {
                    data = data.replace(/\u00a0/g, " ");
                    const before = repl.buffer.slice(0, repl.cursorIdx);
                    const after = repl.buffer.slice(repl.cursorIdx);
                    const newBuf = before + data + after;
                    const newCursor = repl.cursorIdx + data.length;
                    setBuffer(term, repl, newBuf, newCursor);
                }
        }
    });
}
